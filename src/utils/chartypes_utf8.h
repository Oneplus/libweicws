#ifndef __UL_CHARTYPE_H__
#define __UL_CHARTYPE_H_

#define DBC_TABLE_SIZE              132
#define DBC_DIGIT_GROUP_OFFSET      0
#define DBC_DIGIT_GROUP_SIZE        30
#define DBC_UP_ALPHA_GROUP_OFFSET   30
#define DBC_UP_ALPHA_GROUP_SIZE     26
#define DBC_LOW_ALPHA_GROUP_OFFSET  56
#define DBC_LOW_ALPHA_GROUP_SIZE    26
#define DBC_PUNC_GROUP_OFFSET       82
#define DBC_PUNC_GROUP_SIZE         49

#define SBC_TABLE_SIZE              95
#define SBC_DIGIT_GROUP_OFFSET      0
#define SBC_DIGIT_GROUP_SIZE        10
#define SBC_UP_ALPHA_GROUP_OFFSET   10
#define SBC_UP_ALPHA_GROUP_SIZE     26
#define SBC_LOW_ALPHA_GROUP_OFFSET  36
#define SBC_LOW_ALPHA_GROUP_SIZE    26
#define SBC_PUNC_GROUP_OFFSET       62
#define SBC_PUNC_GROUP_SIZE         32

const char *DBCTable[] = {
                // digit group
/* 0-9 */       "０", "１", "２", "３", "４", "５", "６", "７", "８", "９",
/* 10-19 */     "〇", "一", "二", "三", "四", "五", "六", "七", "八", "九",
/* 20-29 */     "零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖",

                // upper-case alphabet
/* 30-39 */     "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ", "Ｈ", "Ｉ", "Ｊ",
/* 40-49 */     "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ", "Ｐ", "Ｑ", "Ｒ", "Ｓ", "Ｔ",
/* 50-55 */     "Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ", "Ｚ",

                // lower-case alphabet
/* 56-65 */     "ａ", "ｂ", "ｃ", "ｄ", "ｅ", "ｆ", "ｇ", "ｈ", "ｉ", "ｊ",
/* 66-75 */     "ｋ", "ｌ", "ｍ", "ｎ", "ｏ", "ｐ", "ｑ", "ｒ", "ｓ", "ｔ",
/* 76-81 */     "ｕ", "ｖ", "ｗ", "ｘ", "ｙ", "ｚ",

                // dbc punctuation group
/* 82-91 */     "　", "！", "＂", "＃", "＄", "％", "＆", "＇", "（", "）",
/* 92-101 */    "＊", "＋", "，", "－", "．", "／", "：", "；", "＜", "＝",
/* 102-111 */   "＞", "？", "＠", "［", "＼", "］", "＾", "＿", "｀", "｛",
/* 112-114 */   "｜", "｝", "～",

                // chinese punctutaion group
/* 114-123 */   "。", "、", "“",  "”",  "﹃", "﹄", "‘",  "’",  "﹁", "﹂",
/* 124-131 */   "…", "【", "】", "《", "》", "〈", "〉", "·"
};

const char SBCTable[] = {
                // digit group
/* 0-9 */       '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

                // upper-case group
/* 10-19 */     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
/* 20-29 */     'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
/* 30-35 */     'U', 'V', 'W', 'X', 'Y', 'Z',

                // lower-case group
/* 36-45 */     'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
/* 46-55 */     'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
/* 56-61 */     'u', 'v', 'w', 'x', 'y', 'z',

                // punctuation group
/* 62-71 */     ' ', '!', '"', '#', '$', '%', '&', '\'','(', ')',
/* 72-81 */     '*', '+', ',', '-', '.', '/', ':', ';', '<', '=',
/* 82-91 */     '>', '?', '@', '[', '\\',']', '^', '_', '`', '{',
/* 92-94 */     '|', '}', '~',
};

enum {CHAR_DIGIT_TYPE, CHAR_LETTER_TYPE, CHAR_PUNC_TYPE, CHAR_OTHER_TYPE} CHAR_TYPES;

#endif  // end for __UL_CHARTYPE_H__
