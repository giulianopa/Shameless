/*
* Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "eeprom.h"

#define TEST_AND_CHECK(function) printf("%s ", #function); \
	if (!function()) { printf("FAILED\n"); return false; } printf("SUCCEEDED\n");
#define TEST_FILE_NAME  "eeprom.txt"

// Test cases
bool test_erase_eeprom(void);
bool test_read_write_words(void);
bool test_read_write_pages(void);
bool test_erase_page(void);

// Test entry point
bool test_eeprom(void) {
	TEST_AND_CHECK(test_erase_eeprom);
	TEST_AND_CHECK(test_read_write_pages);
	TEST_AND_CHECK(test_read_write_words);
	return true;
}

int main(int argc, char *argv[]) {
	printf("Reading file %s\n", TEST_FILE_NAME);
	if (eeprom_from_file(TEST_FILE_NAME) != EEPROM_SZ)
		fprintf(stderr, "Cannot read file, starting from blank EEPROM\n");

	// REPL
	char option = 0;
	printf("\n------------------------------------------\n");
	printf(" EEPROM Test \n");
	do {
		printf("------------------------------------------\n");
		printf(" e. Erase EEPROM or specific page (`e [0|page_no]`)\n");
		printf(" l. Load EEPROM from file.\n");
		printf(" p. Print entire EEPROM content\n");
		printf(" t. Run unit tests (will erase EEPROM).\n");
		printf(" q. Save and quit.\n");
		printf(" s. Save EEPROM content to file.\n");
		printf(" r. Read page or word from a given offset (`r [p|w] <off>`).\n");
		printf(" w. Write page or word to a given offset (`w [p|w] <off> <hex>`).\n");
		printf("Enter an option: ");
		scanf(" %c", &option);
		printf("------------------------------------------\n");

		switch(option){
		case 'e':
			{
				uint32_t offset = 0;
				scanf(" %u", &offset);
				if (offset == 0) {
					if (eeprom_erase() != EEPROM_SZ)
						fprintf(stderr, "Could not erase\n");
				}
				else {
						if (eeprom_erase_page(offset) != EEPROM_PAGE_SZ)
							fprintf(stderr, "Could not erase page %u\n", offset);
				}
			}
			break;

		case 'p':
			{
				uint32_t p;
				for (p = 0; p < EEPROM_N_PAGES; p++) {
					uint8_t page[EEPROM_PAGE_SZ];
					if (eeprom_read_page(p, page) != EEPROM_PAGE_SZ)
						break;
					printf("[p:%u|w:%u] ", p, page_to_addr(p));
					uint32_t s = 0;
					for (s = 0; s < EEPROM_PAGE_SZ; s++)
						printf("%02x,", page[s]);
					printf("\n");
				}
			}
			break;

		case 'l':
			if (eeprom_from_file(TEST_FILE_NAME) != EEPROM_SZ)
				fprintf(stderr, "Cannot read EEPROM file %s", TEST_FILE_NAME);
			break;

		case 'w':
			{
				char type = 0;
				uint32_t offset = 0;
				scanf(" %c %u", &type, &offset);
				if (type == 'w') {
					uint8_t word[EEPROM_WORD_SZ];
					for (uint32_t i = 0; i < EEPROM_WORD_SZ; i++)
						scanf("%02hhx", &word[i]);
					if (eeprom_write_word(offset, (uint8_t *)&word) != EEPROM_WORD_SZ)
						fprintf(stderr, "Cannot write word %u\n", offset);
				}
				else if  (type == 'p') {
					uint8_t page[EEPROM_PAGE_SZ];
					for (uint32_t i = 0; i < EEPROM_PAGE_SZ; i++)
						scanf("%02hhx", &page[i]);
					if (eeprom_write_page(offset, (uint8_t *)&page) != EEPROM_PAGE_SZ)
						fprintf(stderr, "Cannot write page %u\n", offset);
				}
			}
			break;

		case 'r':
			{
				char type = 0;
				uint32_t offset = 0;
				scanf(" %c %u", &type, &offset);
				if (type == 'w') {
					uint8_t word[EEPROM_WORD_SZ];
					if (eeprom_read_word(offset, (uint8_t *)&word) != EEPROM_WORD_SZ)
						fprintf(stderr, "Cannot write word %u\n", offset);
					printf("[p:%u+%u w:%u] ", addr_to_page(offset), addr_to_page_off(offset), offset);
					for (uint32_t s = 0; s < EEPROM_WORD_SZ; s++)
						printf("%02x,", word[s]);
					printf("\n");
				}
				else if  (type == 'p') {
					uint8_t page[EEPROM_PAGE_SZ];
					if (eeprom_read_page(offset, (uint8_t *)&page) != EEPROM_PAGE_SZ)
						fprintf(stderr, "Cannot write page %u\n", offset);
					printf("[p:%u|w:%u] ", offset, page_to_addr(offset));
					for (uint32_t s = 0; s < EEPROM_PAGE_SZ; s++)
						printf("%02x,", page[s]);
					printf("\n");
				}
			}
			break;

		case 's':
			if (eeprom_to_file(TEST_FILE_NAME) != EEPROM_SZ)
				fprintf(stderr, "Cannot write EEPROM file %s", TEST_FILE_NAME);
			break;

		case 't':
			if (!test_eeprom())
				fprintf(stderr, "Test failed!\n");
			break;

		case 'q':
			break;

		default:
			printf("Invalid option %x\n", option);
			break;
		}
	} while (option != 'q');

	// Save before closing
	printf("Writing file %s\n", TEST_FILE_NAME);
	if (eeprom_to_file(TEST_FILE_NAME) != EEPROM_SZ) {
		fprintf(stderr, "Cannot write EEPROM file!");
		return -1;
	}
	return 0;
}


//------------
// Test cases
//------------

// Erase EEPROM and read back word by word and pag by page
bool test_erase_eeprom(void) {
	uint8_t erased_word[EEPROM_WORD_SZ];
	uint8_t erased_page[EEPROM_PAGE_SZ];
	memset(erased_word, EEPROM_ERASE_STATE, EEPROM_WORD_SZ);
	memset(erased_page, EEPROM_ERASE_STATE, EEPROM_PAGE_SZ);

	// Erase and Check
	if (eeprom_erase() != EEPROM_SZ)
		return false;

	// Read back words
	uint32_t w;
	for (w = 0; w < EEPROM_N_WORDS; w++) {
		uint8_t word[EEPROM_WORD_SZ];
		if (eeprom_read_word(w, word) != EEPROM_WORD_SZ)
			return false;
		if (memcmp(erased_word, word, EEPROM_WORD_SZ) != 0)
			return false;
	}
#if (EEPROM_WORD_SZ == 1)
	for (w = 0; w < EEPROM_N_WORDS; w++) {
		uint8_t word = 0x00;
		if (!eeprom_read_byte(w, &word))
			return false;
		if (word != (uint8_t)EEPROM_ERASE_STATE)
			return false;
	}
#endif

	// Read back pages
	uint32_t p;
	for (p = 0; p < EEPROM_N_PAGES; p++) {
		uint8_t page[EEPROM_PAGE_SZ];
		if (eeprom_read_page(p, page) != EEPROM_PAGE_SZ)
			return false;
		if (memcmp(erased_page, page, EEPROM_PAGE_SZ) != 0)
			return false;
	}
#if (EEPROM_PAGE_SZ == 4)
	for (p = 0; p < EEPROM_N_PAGES; p++) {
		uint32_t page = 0;
		if (!eeprom_read_uint(p, &page))
			return false;
		if (memcmp(erased_page, &page, EEPROM_PAGE_SZ) != 0)
			return false;
	}
#endif

	// Success
	return true;
}

// Write word by word and read back.
bool test_read_write_words(void) {
	if (eeprom_erase() != EEPROM_SZ)
		return false;
	uint32_t w;
	for (w = 0; w < EEPROM_N_WORDS; w++) {
		uint8_t in[EEPROM_WORD_SZ], out[EEPROM_WORD_SZ];
		uint32_t i = 0;
		for (i = 0; i < EEPROM_WORD_SZ; i++)
			in[i] = rand() % 0xff;
		if (eeprom_write_word(w, in) != EEPROM_WORD_SZ)
			return false;
		if (eeprom_read_word(w, out) != EEPROM_WORD_SZ)
			return false;
		if (memcmp(in, out, EEPROM_WORD_SZ) != 0)
			return false;
	}

#if (EEPROM_WORD_SZ == 1)
	if (eeprom_erase() != EEPROM_SZ)
		return false;
	uint8_t word = 0x00;
	for (w = 0; w < EEPROM_N_WORDS; w++) {
		if (!eeprom_write_byte(w, (uint8_t)(w % 0xff)))
			return false;
		if (!eeprom_read_byte(w, &word))
			return false;
		if ((uint8_t)(w % 0xff) != word)
			return false;
	}
#endif

	// Success
	return true;
}

// Write page by page and read back.
bool test_read_write_pages(void) {
	if (eeprom_erase() != EEPROM_SZ)
		return false;
	for (uint32_t p = 0; p < EEPROM_N_PAGES; p++) {
		uint8_t page[EEPROM_PAGE_SZ];
		if (eeprom_write_page(p, (uint8_t *)&p) != EEPROM_PAGE_SZ)
			return false;
		if (eeprom_read_page(p, page) != EEPROM_PAGE_SZ)
			return false;
		if (memcmp(&p, page, EEPROM_PAGE_SZ) != 0)
			return false;
	}

#if (EEPROM_PAGE_SZ == 4)
	if (eeprom_erase() != EEPROM_SZ)
		return false;
	uint32_t page = 0;
	for (uint32_t p = 0; p < EEPROM_N_PAGES; p++) {
		if (!eeprom_write_uint(p, p))
			return false;
		if (!eeprom_read_uint(p, &page))
			return false;
		if (p != page)
			return false;
	}
#endif

	// Success
	return true;
}

// Erase a single page, with random offset
bool test_erase_page(void) {
	// Write something on all pages
	uint32_t p;
	for (p = 0; p < EEPROM_N_PAGES; p++)
		if (eeprom_write_page(p, (uint8_t *)&p) != EEPROM_PAGE_SZ)
			return false;

	// Get a random page id and erase it
	uint32_t page_id = rand() % EEPROM_N_PAGES;
	if (eeprom_erase_page(page_id) != EEPROM_SZ)
		return false;

	// Make sure all pages but one were erased
	uint8_t erased_page[EEPROM_PAGE_SZ];
	memset(erased_page, EEPROM_ERASE_STATE, EEPROM_PAGE_SZ);
	for (p = 0; p < EEPROM_N_PAGES; p++) {
		uint8_t page[EEPROM_PAGE_SZ];
		if (eeprom_read_page(p, page) != EEPROM_PAGE_SZ)
			return false;
		if (memcmp(erased_page, page, EEPROM_PAGE_SZ) == 0 && p != page_id)
			return false;
	}

	// Success
	return true;
}
