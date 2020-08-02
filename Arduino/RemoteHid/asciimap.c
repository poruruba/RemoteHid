#include "asciimap.h"

const unsigned char keyid_jp[KEYID_SIZE_JP] = {
	0x00, // dummy
	0x35, // 漢字
	0x1e, // 1
	0x1f, // 2
	0x20, // 3
	0x21, // 4
	0x22, // 5
	0x23, // 6
	0x24, // 7
	0x25, // 8
	0x26, // 9
	0x27, // 0
	0x2d, // -
	0x2e, // ^
	0x89, // |
	0x2a, // BS
	0x2b, // Tab
	0x14, // Q
	0x1a, // W
	0x08, // E
	0x15, // R
	0x17, // T
	0x1c, // Y
	0x18, // U
	0x0c, // I
	0x12, // O
	0x13, // P
	0x2f, // @
	0x30, // [
	0x31, //
	0x39, // CapsLock 
	0x04, // A
	0x16, // S
	0x07, // D
	0x09, // F
	0x0a, // G
	0x0b, // H
	0x0d, // J
	0x0e, // K
	0x0f, // L
	0x33, // ;
	0x34, // :
	0x32, // ]
	0x28, // Enter(right)
	0x21, // Shift(left)
	0x64,
	0x1d, // Z
	0x1b, // X
	0x06, // C
	0x19, // V
	0x05, // B
	0x11, // N
	0x10, // M
	0x36, // ,
	0x37, // .
	0x38, // /
	0x87, // \
	0xe5, // Shift(right)
	0xe0, // Ctrl(left)
	0x00, // dummy
	0xe2, // Alt(left)
	0x2c, // Space
	0xe6, // Alt(right)
	0x00, // dummy
	0xe4, // Ctrl(right)
	0x00, // dummy
	0x00, // dummy
	0x00, // dummy
	0x00, // dummy
	0x00, // dummy
	0x00, // dummy
	0x00, // dummy
	0x00, // dummy
	0x00, // dummy
	0x00, // dummy
	0x49, // Insert
	0x4c, // Delete
	0x00, // dummy
	0x00, // dummy
	0x50, // LeftArrow
	0x4a, // Home
	0x4d, // End
	0x00, // dummy
	0x52, // UpAllow
	0x51, // DownAllow
	0x4b, // PageUp
	0x4e, // PageDown
	0x00, // dummy
	0x00, // dummy
	0x4f, // RightArrow
	0x53, // NumLock
	0x5f, // 7(10key)
	0x5c, // 4(10key)
	0x59, // 1(10key)
	0x00, // dummy
	0x54, // /(10key)
	0x60, // 8(10key)
	0x5d, // 5(10key)
	0x5a, // 2(10key)
	0x62, // 0(10key)
	0x55, // *(10key)
	0x61, // 9(10key)
	0x5e, // 6(10key)
	0x5b, // 3(10key)
	0x63, // Delete(10key)
	0x56, // -
	0x57, // +
	0x00, // dummy
	0x58, // Enter(left)
	0x00, // dummy
	0x29, // Esc
	0x00, // dummy
	0x3a, // F1
	0x3b, // F2
	0x3c, // F3
	0x3d, // F4
	0x3e, // F5
	0x3f, // F6
	0x40, // F7
	0x41, // F8
	0x42, // F9
	0x43, // F10
	0x44, // F11
	0x45, // F12
	0x46, // PrintScreen
	0x47, // ScrollLock
	0x48, // Pause
	0xe3,
	0xe7,
	0x65, 
	0x00, // dummy
	0x8b, // 無変換
	0x8a, // 変換
	0x88, // ひらがな
};

const KEYMAP asciimap_jp[ASCIIMAP_SIZE_JP] = {
    {0, 0},             /* NUL */
    {0, 0},             /* SOH */
    {0, 0},             /* STX */
    {0, 0},             /* ETX */
    {0, 0},             /* EOT */
    {0, 0},             /* ENQ */
    {0, 0},             /* ACK */
    {0, 0},             /* BEL */
    {0x2a, 0},          /* BS  */  /* Keyboard Delete (Backspace) */
    {0x2b, 0},          /* TAB */  /* Keyboard Tab */
    {0x28, 0},          /* LF  */  /* Keyboard Return (Enter) */
    {0, 0},             /* VT  */
    {0, 0},             /* FF  */
    {0, 0},             /* CR  */
    {0, 0},             /* SO  */
    {0, 0},             /* SI  */
    {0, 0},             /* DLE */
    {0, 0},             /* DC1 */
    {0, 0},             /* DC2 */
    {0, 0},             /* DC3 */
    {0, 0},             /* DC4 */
    {0, 0},             /* NAK */
    {0, 0},             /* SYN */
    {0, 0},             /* ETB */
    {0, 0},             /* CAN */
    {0, 0},             /* EM  */
    {0, 0},             /* SUB */
    {0, 0},             /* ESC */
    {0, 0},             /* FS  */
    {0, 0},             /* GS  */
    {0, 0},             /* RS  */
    {0, 0},             /* US  */
    {0x2c, 0},          /*   */
    {0x1e, KEY_SHIFT},      /* ! */
    {0x34, KEY_SHIFT},      /* " */
    {0x20, KEY_SHIFT},      /* # */
    {0x21, KEY_SHIFT},      /* $ */
    {0x22, KEY_SHIFT},      /* % */
    {0x24, KEY_SHIFT},      /* & */
    {0x34, 0},          /* ' */
    {0x26, KEY_SHIFT},      /* ( */
    {0x27, KEY_SHIFT},      /* ) */
    {0x25, KEY_SHIFT},      /* * */
    {0x2e, KEY_SHIFT},      /* + */
    {0x36, 0},          /* , */
    {0x2d, 0},          /* - */
    {0x37, 0},          /* . */
    {0x38, 0},          /* / */
    {0x27, 0},          /* 0 */
    {0x1e, 0},          /* 1 */
    {0x1f, 0},          /* 2 */
    {0x20, 0},          /* 3 */
    {0x21, 0},          /* 4 */
    {0x22, 0},          /* 5 */
    {0x23, 0},          /* 6 */
    {0x24, 0},          /* 7 */
    {0x25, 0},          /* 8 */
    {0x26, 0},          /* 9 */
    {0x33, KEY_SHIFT},      /* : */
    {0x33, 0},          /* ; */
    {0x36, KEY_SHIFT},      /* < */
    {0x2e, 0},          /* = */
    {0x37, KEY_SHIFT},      /* > */
    {0x38, KEY_SHIFT},      /* ? */
    {0x1f, KEY_SHIFT},      /* @ */
    {0x04, KEY_SHIFT},      /* A */
    {0x05, KEY_SHIFT},      /* B */
    {0x06, KEY_SHIFT},      /* C */
    {0x07, KEY_SHIFT},      /* D */
    {0x08, KEY_SHIFT},      /* E */
    {0x09, KEY_SHIFT},      /* F */
    {0x0a, KEY_SHIFT},      /* G */
    {0x0b, KEY_SHIFT},      /* H */
    {0x0c, KEY_SHIFT},      /* I */
    {0x0d, KEY_SHIFT},      /* J */
    {0x0e, KEY_SHIFT},      /* K */
    {0x0f, KEY_SHIFT},      /* L */
    {0x10, KEY_SHIFT},      /* M */
    {0x11, KEY_SHIFT},      /* N */
    {0x12, KEY_SHIFT},      /* O */
    {0x13, KEY_SHIFT},      /* P */
    {0x14, KEY_SHIFT},      /* Q */
    {0x15, KEY_SHIFT},      /* R */
    {0x16, KEY_SHIFT},      /* S */
    {0x17, KEY_SHIFT},      /* T */
    {0x18, KEY_SHIFT},      /* U */
    {0x19, KEY_SHIFT},      /* V */
    {0x1a, KEY_SHIFT},      /* W */
    {0x1b, KEY_SHIFT},      /* X */
    {0x1c, KEY_SHIFT},      /* Y */
    {0x1d, KEY_SHIFT},      /* Z */
    {0x2f, 0},          /* [ */
    {0x31, 0},          /* \ */
    {0x30, 0},          /* ] */
    {0x23, KEY_SHIFT},      /* ^ */
    {0x2d, KEY_SHIFT},      /* _ */
    {0x35, 0},          /* ` */
    {0x04, 0},          /* a */
    {0x05, 0},          /* b */
    {0x06, 0},          /* c */
    {0x07, 0},          /* d */
    {0x08, 0},          /* e */
    {0x09, 0},          /* f */
    {0x0a, 0},          /* g */
    {0x0b, 0},          /* h */
    {0x0c, 0},          /* i */
    {0x0d, 0},          /* j */
    {0x0e, 0},          /* k */
    {0x0f, 0},          /* l */
    {0x10, 0},          /* m */
    {0x11, 0},          /* n */
    {0x12, 0},          /* o */
    {0x13, 0},          /* p */
    {0x14, 0},          /* q */
    {0x15, 0},          /* r */
    {0x16, 0},          /* s */
    {0x17, 0},          /* t */
    {0x18, 0},          /* u */
    {0x19, 0},          /* v */
    {0x1a, 0},          /* w */
    {0x1b, 0},          /* x */
    {0x1c, 0},          /* y */
    {0x1d, 0},          /* z */
    {0x2f, KEY_SHIFT},      /* { */
    {0x31, KEY_SHIFT},      /* | */
    {0x30, KEY_SHIFT},      /* } */
    {0x35, KEY_SHIFT},      /* ~ */
    {0,0},              /* DEL */
};
