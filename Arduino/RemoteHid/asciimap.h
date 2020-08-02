#ifndef _ASCIIMAP_H_
#define _ASCIIMAP_H_

#include <HIDKeyboardTypes.h>

#define KEYID_SIZE_JP  (134)
extern const unsigned char keyid_jp[KEYID_SIZE_JP];

enum KEY_MODIFIER_MASK {
  KEY_MASK_CTRL = 0x01,
  KEY_MASK_SHIFT = 0x02,
  KEY_MASK_ALT = 0x04,
  KEY_MASK_WIN = 0x08
};

#define ASCIIMAP_SIZE_JP (127)
extern const KEYMAP asciimap_jp[ASCIIMAP_SIZE_JP];

#endif
