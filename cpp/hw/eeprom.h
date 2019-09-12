/*
* Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
* This code is licensed under MIT license (see LICENSE.txt for details)
*/
#ifndef _EEPROM_H
#define _EEPROM_H

#include <stdint.h>
#include <stdbool.h>

#define EEPROM_N_WORDS      8192
#define EEPROM_WORD_SZ      1
#define EEPROM_PAGE_SZ      4
#define EEPROM_SZ           (EEPROM_N_WORDS * EEPROM_WORD_SZ)
#define EEPROM_N_PAGES      (EEPROM_SZ / EEPROM_PAGE_SZ)
#define EEPROM_ERASE_STATE  0xff


/**
	Load EEPROM content from file to memory.
	\param[in] file_name Input EEPROM file name.
	\return Number of bytes read (EEPROM_SZ on success).
 */
uint32_t eeprom_from_file(const char *file_name);

/**
	Write memory content to EEPROM file.
	\param[in] file_name Output EEPROM file name.
	\return Number of bytes written (EEPROM_SZ on success).
 */
uint32_t eeprom_to_file(const char *file_name);

/**
	Read one byte from EEPROM (memory buffer).
	\param[in] offset Zero-based EEPROM offset (word number), in [0, EEPROM_N_WORDS - 1].
	\param[out] data Output buffer to write to (on success, only).
	\return Number of bytes read (EEPROM_WORD_SZ on success, 0 otherwise).
 */
uint32_t eeprom_read_word(const uint32_t offset, uint8_t *data);

/**
	Write one byte to EEPROM (memory buffer).
	\param[in] offset Zero-based EEPROM offset (word number), in [0, EEPROM_N_WORDS - 1].
	\param[out] data Input buffer to write.
	\return Number of bytes written (EEPROM_WORD_SZ on success, 0 otherwise).
 */
uint32_t eeprom_write_word(const uint32_t offset, const uint8_t *data);

/**
	Read one page from EEPROM (memory buffer).
	\param[in] page Zero-based offset (page number), in [0, EEPROM_N_PAGES - 1].
	\param[out] data Output buffer to write to (on success, only).
	\return Number of bytes read (EEPROM_PAGE_SZ on success, 0 otherwise).
 */
uint32_t eeprom_read_page(const uint32_t page, uint8_t *data);

/**
	Write one byte to EEPROM (memory buffer).
	\param[in] page Zero-based offset (page number), in [0, EEPROM_N_PAGES - 1].
	\param[out] data Input buffer to write.
	\return Number of bytes written (EEPROM_PAGE_SZ on success, 0 otherwise).
 */
uint32_t eeprom_write_page(const uint32_t page, const uint8_t *data);

/**
	Erase entire EEPROM, from memory buffer.
	\return Number of erased bytes (EEPROM_SZ on success, 0 otherwise).
 */
uint32_t eeprom_erase(void);

/**
	Erase one EEPROM page, from memory buffer.
	\param[in] offset Zero-based offset (page number), in [0, EEPROM_N_PAGES - 1].
	\return Number of erased bytes (EEPROM_PAGE_SZ on success, 0 otherwise).
 */
uint32_t eeprom_erase_page(const uint32_t offset);

/**
	Convert address (word number) to page number.
 */
static inline uint32_t eeprom_addr_to_page(const uint32_t addr) {
	const uint32_t byte_addr = addr * EEPROM_WORD_SZ;
	return (byte_addr - (byte_addr % EEPROM_PAGE_SZ)) / EEPROM_PAGE_SZ;
}

/**
	Convert address (word number) to page offset.
 */
static inline uint32_t eeprom_addr_to_page_off(const uint32_t addr) {
	return (addr * EEPROM_WORD_SZ) % EEPROM_PAGE_SZ;
}

/**
	Convert page number to address (word number).
 */
static inline uint32_t eeprom_page_to_addr(const uint32_t page) {
	return (page * EEPROM_PAGE_SZ);
}

/**
	Read one byte from EEPROM (memory buffer).
	\param[in] page Zero-based offset (page number), in [0, EEPROM_N_PAGES - 1].
	\param[in] page_off Zero-based offset within the page, in [0, EEPROM_PAGE_SZ - 1].
	\param[out] data Output buffer to write to (on success, only).
	\return Number of bytes read (EEPROM_WORD_SZ on success, 0 otherwise).
 */
static inline uint32_t eeprom_read_word_from_page(
			const uint32_t page, const uint32_t page_off, uint8_t *data) {
	return eeprom_read_word(eeprom_page_to_addr(page) + page_off, data);
}

/**
	Write one byte to EEPROM (memory buffer).
	\param[in] page Zero-based offset (page number), in [0, EEPROM_N_PAGES - 1].
	\param[in] page_off Zero-based offset within the page, in [0, EEPROM_PAGE_SZ - 1].
	\param[out] data Input buffer to write.
	\return Number of bytes written (EEPROM_WORD_SZ on success, 0 otherwise).
 */
static inline uint32_t eeprom_write_word_from_page(
			const uint32_t page, const uint32_t page_off, const uint8_t *data) {
	return eeprom_write_word(eeprom_page_to_addr(page) + page_off, data);
}

#if (EEPROM_WORD_SZ == 1)
/**
	Read single byte from EEPROM (same as reading a single byte word).
	\param[in] offset EEPROM byte number, in [0, EEPROM_SZ - 1].
	\param[out] data Output byte.
	\return True on success, false otherwise.
 */
bool eeprom_read_byte(const uint32_t offset, uint8_t *data);

/**
	Write single byte from EEPROM (same as reading a single byte word).
	\param[in] offset EEPROM byte number, in [0, EEPROM_SZ - 1].
	\param[out] data Input byte.
	\return True on success, false otherwise.
 */
bool eeprom_write_byte(const uint32_t offset, const uint8_t data);
#endif

#if (EEPROM_PAGE_SZ == 4)
/**
	Read 4 bytes from EEPROM (same as reading a 4 bytes page).
	\param[in] offset EEPROM byte number, in [0, EEPROM_SZ - 1].
	\param[out] data Output unsigned integer.
	\return True on success, false otherwise.
 */
static inline bool eeprom_read_uint(const uint32_t offset, uint32_t *data) {
	return (eeprom_read_page(offset, (uint8_t *)data) == EEPROM_PAGE_SZ);
}

/**
	Write 4 bytes from EEPROM (same as reading a 4 bytes page).
	\param[in] offset EEPROM byte number, in [0, EEPROM_SZ - 1].
	\param[out] data Input unsigned integer.
	\return True on success, false otherwise.
 */
static inline bool eeprom_write_uint(const uint32_t offset, const uint32_t data) {
	return (eeprom_write_page(offset, (uint8_t *)&data) == EEPROM_PAGE_SZ);
}
#endif

#endif // _EEPROM_H
