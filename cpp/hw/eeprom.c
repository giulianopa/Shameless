/*
* Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include "eeprom.h"

// Check EEPROM offset
#define CHECK_OFFSET(offset) if (offset < 0 || offset >= EEPROM_SZ) return 0;

// Check word address
#define CHECK_WORD_ADDR(addr) CHECK_OFFSET(addr * EEPROM_WORD_SZ)

// Check page address
#define CHECK_PAGE_ADDR(addr) CHECK_OFFSET(addr * EEPROM_PAGE_SZ)

// EEPROM memory buffer
static uint8_t eeprom[EEPROM_SZ];

// Lock to protect the buffer
static pthread_mutex_t buf_mutex;

/**
 Lock the entire file.
 Blocker version.
 \param[in] fd File descriptor, previously opened.
 \param[in] type l_type value for flock (e.g., F_RDLCK or F_WRLCK).
 \return True on success, false otherwise.
 */
static bool lock_fd(FILE *fd, const unsigned short type) {
	if (!fd)
		return false;
	int fno = fileno(fd);
	struct flock fl;
	memset(&fl, 0, sizeof(fl));
	fl.l_type = type;
	fl.l_whence = SEEK_SET;
	return (fcntl(fno, F_SETLKW, &fl) != -1);
}

// Format: <page_id> <byte_0><byte_1>...<byte_$(EEPROM_PAGE_SZ - 1)>
uint32_t eeprom_from_file(const char *file_name) {
	if (!eeprom_erase())
		return 0;

	// Parse file, line by line.
	FILE *fp = NULL;
	fp = fopen(file_name, "r");
	if (!fp)
		return 0;
	if (!lock_fd(fp, F_RDLCK)) {
		fclose(fp);
		return 0;
	}
	char * line = NULL;
	size_t len = 0;
	ssize_t read = 0;
	pthread_mutex_lock(&buf_mutex);
	while (getline(&line, &len, fp) != -1) {
		char *token = strtok(line, " ,");
		if (!token)
			break;

		// Parse page number
		uint32_t page_addr = atoi(token);
		CHECK_PAGE_ADDR(page_addr);
		token = strtok(NULL, " ,");
		if (!token)
			break;
		uint32_t s = 0;
		for (s = 0; s < EEPROM_PAGE_SZ && token != NULL; s++, token = strtok(NULL, " ,")) {
			sscanf(token, "%2hhx", &eeprom[page_addr * EEPROM_PAGE_SZ + s])	;
			read++;
		}
	}
	pthread_mutex_unlock(&buf_mutex);
	fclose(fp);
	return read;
}
// Format: <page_id> <byte_0><byte_1>...<byte_$(EEPROM_PAGE_SZ - 1)>
uint32_t eeprom_to_file(const char *file_name) {
	FILE *fp = NULL;
	fp = fopen(file_name, "w+");
	if (!fp)
		return 0;
	if (!lock_fd(fp, F_WRLCK)) {
		fclose(fp);
		return 0;
	}

	// Write one page per line
	uint32_t i = 0, s = 0, written = 0;
	uint8_t page[EEPROM_PAGE_SZ];
	for (i = 0; i < EEPROM_N_PAGES; i++) {
		if (eeprom_read_page(i, page) != EEPROM_PAGE_SZ)
			break;
		fprintf(fp, "%d ", i);
		for (s = 0; s < EEPROM_PAGE_SZ; s++) {
			fprintf(fp, "%02x,", page[s]);
			written++;
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return written;
}

uint32_t eeprom_read_word(const uint32_t offset, uint8_t *data) {
	CHECK_WORD_ADDR(offset);
	pthread_mutex_lock(&buf_mutex);
	memcpy(data, eeprom + (EEPROM_WORD_SZ * offset), EEPROM_WORD_SZ);
	pthread_mutex_unlock(&buf_mutex);
	return EEPROM_WORD_SZ;
}

uint32_t eeprom_write_word(const uint32_t offset, const uint8_t *data) {
	CHECK_WORD_ADDR(offset);
	pthread_mutex_lock(&buf_mutex);
	memcpy(eeprom + (EEPROM_WORD_SZ * offset), data, EEPROM_WORD_SZ);
	pthread_mutex_unlock(&buf_mutex);
	return EEPROM_WORD_SZ;
}

uint32_t eeprom_read_page(const uint32_t page, uint8_t *data) {
	CHECK_PAGE_ADDR(page);
	pthread_mutex_lock(&buf_mutex);
	memcpy(data, eeprom + (EEPROM_PAGE_SZ * page), EEPROM_PAGE_SZ);
	pthread_mutex_unlock(&buf_mutex);
	return EEPROM_PAGE_SZ;
}

uint32_t eeprom_write_page(const uint32_t page, const uint8_t *data) {
	CHECK_PAGE_ADDR(page);
	pthread_mutex_lock(&buf_mutex);
	memcpy(eeprom + (EEPROM_PAGE_SZ * page), data, EEPROM_PAGE_SZ);
	pthread_mutex_unlock(&buf_mutex);
	return EEPROM_PAGE_SZ;
}

uint32_t eeprom_erase(void) {
	pthread_mutex_lock(&buf_mutex);
	memset(eeprom, EEPROM_ERASE_STATE, EEPROM_SZ);
	pthread_mutex_unlock(&buf_mutex);
	return EEPROM_SZ;
}

uint32_t eeprom_erase_page(const uint32_t page) {
	CHECK_PAGE_ADDR(page);
	pthread_mutex_lock(&buf_mutex);
	memset(eeprom + (EEPROM_PAGE_SZ * page), EEPROM_ERASE_STATE, EEPROM_PAGE_SZ);
	pthread_mutex_unlock(&buf_mutex);
	return EEPROM_PAGE_SZ;
}

uint32_t eeprom_read(const uint32_t addr, uint8_t *data, const uint32_t len) {
	CHECK_OFFSET(addr + len - 1);

	// Read first words, getting to the beginning of the next page
	uint32_t bytes_left = len, tot_read = 0, curr_addr = addr;
	uint32_t next_page = eeprom_addr_to_page(addr) + 1;
	const uint32_t next_page_addr = eeprom_page_to_addr(next_page);
	while (bytes_left > 0 && curr_addr < next_page_addr) {
		uint32_t bytes_read = eeprom_read_word(curr_addr++, data + tot_read);
		if (bytes_read != EEPROM_WORD_SZ)
			return tot_read + bytes_read;
		tot_read += bytes_read;
		bytes_left -= bytes_read;
	}

	// Read entire pages
	while (bytes_left >= EEPROM_PAGE_SZ) {
		uint32_t bytes_read = eeprom_read_page(next_page++, data + tot_read);
		tot_read += bytes_read;
		if (bytes_read != EEPROM_PAGE_SZ)
			return tot_read;
		bytes_left -= bytes_read;
	}

	// Read reamaining words
	curr_addr = eeprom_page_to_addr(next_page);
	while (bytes_left > 0) {
		uint32_t bytes_read = eeprom_read_word(curr_addr++, data + tot_read);
		tot_read += bytes_read;
		if (bytes_read != EEPROM_WORD_SZ)
			return tot_read;
		bytes_left -= bytes_read;
	}
	return tot_read;
}

uint32_t eeprom_write(const uint32_t addr, const uint8_t *data, const uint32_t len) {
	CHECK_OFFSET(addr + len - 1);

	// Read first words, getting to the beginning of the next page
	uint32_t bytes_left = len, tot_written = 0, curr_addr = addr;
	uint32_t next_page = eeprom_addr_to_page(addr) + 1;
	const uint32_t next_page_addr = eeprom_page_to_addr(next_page);
	while (bytes_left > 0 && curr_addr < next_page_addr) {
		uint32_t bytes_read = eeprom_write_word(curr_addr++, data + tot_written);
		if (bytes_read != EEPROM_WORD_SZ)
			return tot_written + bytes_read;
		tot_written += bytes_read;
		bytes_left -= bytes_read;
	}

	// Read entire pages
	while (bytes_left >= EEPROM_PAGE_SZ) {
		uint32_t bytes_read = eeprom_write_page(next_page++, data + tot_written);
		tot_written += bytes_read;
		if (bytes_read != EEPROM_PAGE_SZ)
			return tot_written;
		bytes_left -= bytes_read;
	}

	// Read reamaining words
	curr_addr = eeprom_page_to_addr(next_page);
	while (bytes_left > 0) {
		uint32_t bytes_read = eeprom_write_word(curr_addr++, data + tot_written);
		tot_written += bytes_read;
		if (bytes_read != EEPROM_WORD_SZ)
			return tot_written;
		bytes_left -= bytes_read;
	}
	return tot_written;
}

#if (EEPROM_WORD_SZ == 1)
bool eeprom_read_byte(const uint32_t offset, uint8_t *data) {
	if (offset >= EEPROM_SZ)
		return false;
	pthread_mutex_lock(&buf_mutex);
	*data = eeprom[offset];
	pthread_mutex_unlock(&buf_mutex);
	return true;
}

bool eeprom_write_byte(const uint32_t offset, const uint8_t data) {
	if (offset >= EEPROM_SZ)
		return false;
	pthread_mutex_lock(&buf_mutex);
	eeprom[offset] = data;
	pthread_mutex_unlock(&buf_mutex);
	return true;
}
#endif
