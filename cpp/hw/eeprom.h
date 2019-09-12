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
  Read one byte from EEPROM.
  \param[in] offset Zero-based EEPROM offset (word number), in [0, EEPROM_N_WORDS - 1].
  \param[out] data Output buffer to write to (on success, only).
  \return Number of bytes read (EEPROM_WORD_SZ on success, 0 otherwise).
 */
uint32_t eeprom_read_word(const uint32_t offset, uint8_t *data);

/**
  Write one byte to EEPROM.
  \param[in] offset Zero-based EEPROM offset (word number), in [0, EEPROM_N_WORDS - 1].
  \param[out] data Input buffer to write.
  \return Number of bytes written (EEPROM_WORD_SZ on success, 0 otherwise).
 */
uint32_t eeprom_write_word(const uint32_t offset, const uint8_t *data);

/**
  Read one page from EEPROM.
  \param[in] offset Zero-based offset (page number), in [0, EEPROM_N_PAGES - 1].
  \param[out] data Output buffer to write to (on success, only).
  \return Number of bytes read (EEPROM_PAGE_SZ on success, 0 otherwise).
 */
uint32_t eeprom_read_page(const uint32_t offset, uint8_t *data);

/**
  Write one byte to EEPROM.
  \param[in] offset Zero-based offset (page number), in [0, EEPROM_N_PAGES - 1].
  \param[out] data Input buffer to write.
  \return Number of bytes written (EEPROM_PAGE_SZ on success, 0 otherwise).
 */
uint32_t eeprom_write_page(const uint32_t offset, const uint8_t *data);

/**
  Erase entire EEPROM.
  \return Number of erased bytes (EEPROM_SZ on success, 0 otherwise).
 */
uint32_t eeprom_erase(void);

/**
  Erase one EEPROM page.
  \param[in] offset Zero-based offset (page number), in [0, EEPROM_N_PAGES - 1].
  \return Number of erased bytes (EEPROM_PAGE_SZ on success, 0 otherwise).
 */
uint32_t eeprom_erase_page(const uint32_t offset);

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
  Read single byte from EEPROM (same as reading a single byte word).
  \param[in] offset EEPROM byte number, in [0, EEPROM_SZ - 1].
  \param[out] data Output byte.
  \return True on success, false otherwise.
 */
static inline bool eeprom_read_uint(const uint32_t offset, uint32_t *data) {
  return (eeprom_read_page(offset, (uint8_t *)data) == EEPROM_PAGE_SZ);
}

/**
  Write single byte from EEPROM (same as reading a single byte word).
  \param[in] offset EEPROM byte number, in [0, EEPROM_SZ - 1].
  \param[out] data Input byte.
  \return True on success, false otherwise.
 */
static inline bool eeprom_write_uint(const uint32_t offset, const uint32_t data) {
  return (eeprom_write_page(offset, (uint8_t *)&data) == EEPROM_PAGE_SZ);
}
#endif

#endif // _EEPROM_H
