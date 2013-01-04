/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
#include "ul_strlib_x.h"
#include "chartypes_utf8.h"
#include "chartypes_utf8_ext.h"

std::string chomp(std::string str) {
    int len = str.size();

    if (len == 0) {
        return "";
    }

    while (str[len - 1] == ' ' ||
            str[len - 1] == '\r' ||
            str[len - 1] == '\n' ) {
        str = str.substr(0, -- len);
    }
    while (str[0] == ' ' ||
            str[0] == '\r' ||
            str[0] == '\n') {
        str = str.substr(1, -- len);
    }
    return str;
}

std::string cutoff(std::string str, std::string mark) {
    size_t pos = str.find(mark);

    if (pos == std::string::npos) {
        return str;
    } else {
        return str.substr(0, pos);
    }
}

std::vector<std::string> split(std::string str) {
    std::vector<std::string> sep;

    while (str.size() > 0) {
        int i;
        for (i = 0; i < str.size(); ++ i) {
            if (str[i] == ' ' || str[i] == '\t') {
                break;
            }
        }
        sep.push_back( str.substr(0, i) );

        for (; i < str.size() && (str[i] == ' ' || str[i] == '\t'); ++ i);
        if (i == str.size()) {
           str = "";
        } else {
            str = str.substr(i);
        }
    }

    return sep;
}

std::vector<std::string> split(std::string str, std::string separator) {
    std::vector<std::string> sep;

    while (str.size() > 0) {
        size_t pos = str.find(separator);
        sep.push_back(str.substr(0, pos));
        if (pos == std::string::npos) {
            str = "";
        } else {
            str = str.substr(pos + separator.size());
        }
    }

    return sep;
}

std::string join(std::vector<std::string> sep) {
    std::string ret = "";
    for (int i = 0; i < sep.size(); ++ i) {
        ret = ret + sep[i];
    }
    return ret;
}

std::string join(std::vector<std::string> sep, std::string separator) {
    if (sep.size() == 0) return "";
    std::string ret = sep[0];
    for (int i = 1; i < sep.size(); ++ i) {
        ret = ret + separator + sep[i];
    }
    return ret;
}

bool startswith(const std::string &str, const std::string &head) {
    int len = head.size();

    return (str.substr(0, len) == head);
}

bool isInt(const std::string &str) {
    int i = 0;
    if (str[0] == '-')
        i = 1;

    for (; i < str.size(); ++ i) {
        if (false == (str[i] >= '0' && str[i] <= '9')) {
            return false;
        }
    }
    return true;
}

bool isDouble(const std::string &str) {
    int i = 0;
    int state = 0;
    if (str[0] == '-')
        i = 1;

    for (; i < str.size(); ++ i) {
        if (str[i] == '.') {
            ++ state;
            if (state > 1) return false;
        } else if (false == (str[i] >= '0' && str[i] <= '9')) {
            return false;
        }

    }
    return true;
}

int toInt(const std::string &str) {
    int ret = 0;
    int sign = 1;
    int i = 0;
    if ('-' == str[0]) {
        sign = -1;
        i = 1;
    }
    for (; i < str.size(); ++ i) {
        ret *= 10;
        ret += str[i] - '0';
    }
    return sign * ret;
}

double toDouble(const std::string &str) {
    double x = 0.0, y = 1.0;
    double sign = 1.0;
    int i = 0;
    if ('-' == str[0]) {
        sign = -1.0;
        i = 1;
    }
    for (; i < str.size() && str[i] != '.'; ++ i) {
        x *= 10.0;
        x += (str[i] - '0');
    }

    for (++ i; i < str.size(); ++ i) {
        y /= 10.0;
        x += (str[i] - '0') * y;
    }

    return x * sign;

}

int charType(const std::string &str) {
    if (str.size() == 1 && str[0] > 0 && str[0] < 0x7f) {
        if (str[0] >= '0' && str[0] < '9') {
            return CHAR_DIGIT_TYPE;
        }

        if ((str[0] >= 'a' && str[0] < 'z') || (str[0] >= 'A' && str[0] < 'Z')) {
            return CHAR_LETTER_TYPE;
        }

        for (int i = SBC_PUNC_GROUP_OFFSET; i < SBC_TABLE_SIZE; ++ i) {
            if (str[0] == SBCTable[i])
                return CHAR_PUNC_TYPE;
        }

        return CHAR_OTHER_TYPE;

    } else {
        for (int i = DBC_DIGIT_GROUP_OFFSET; i < DBC_UP_ALPHA_GROUP_OFFSET; ++ i) {
            if (std::string(DBCTable[i]) == str) {
                return CHAR_DIGIT_TYPE;
            }
        }

        for (int i = DBC_UP_ALPHA_GROUP_OFFSET; i < DBC_PUNC_GROUP_OFFSET; ++ i) {
            if (std::string(DBCTable[i]) == str) {
                return CHAR_LETTER_TYPE;
            }
        }

        for (int i = DBC_PUNC_GROUP_OFFSET; i < DBC_TABLE_SIZE; ++ i) {
            if (std::string(DBCTable[i]) == str) {
                return CHAR_PUNC_TYPE;
            }
        }

        return CHAR_OTHER_TYPE;
    }
}

int charType_ext(const std::string &str) {
    if (str.size() == 1 && str[0] > 0 && str[0] < 0x7f) {
        if (str[0] >= '0' && str[0] < '9') {
            return CHAR_DIGIT_TYPE;
        }

        if ((str[0] >= 'a' && str[0] < 'z') || (str[0] >= 'A' && str[0] < 'Z')) {
            return CHAR_LETTER_TYPE;
        }

        for (int i = SBC_PUNC_GROUP_OFFSET; i < SBC_TABLE_SIZE; ++ i) {
            if (str[0] == SBCTable[i])
                return CHAR_PUNC_TYPE;
        }

        return CHAR_OTHER_TYPE;

    } else {
        for (int i = DBC_DIGIT_GROUP_OFFSET; i < DBC_UP_ALPHA_GROUP_OFFSET; ++ i) {
            if (std::string(DBCTable[i]) == str) {
                return CHAR_DIGIT_TYPE;
            }
        }

        for (int i = DBC_UP_ALPHA_GROUP_OFFSET; i < DBC_PUNC_GROUP_OFFSET; ++ i) {
            if (std::string(DBCTable[i]) == str) {
                return CHAR_LETTER_TYPE;
            }
        }

        for (int i = DBC_EXT_CHINESE_OFFSET; i < DBC_EXT_TABLE_SIZE; ++ i) {
            if (std::string(DBCTable_ext[i]) == str) {
                return CHAR_PUNC_TYPE;
            }
        }

        return CHAR_OTHER_TYPE;

    }
}

bool isSBC(const std::string &str) {
    if (str.size() == 1 && str[0] <= 0x7f)
        return true;
    return false;
}

bool isDBC(const std::string &str) {
    return true;
}

bool isLetter(const std::string &str) {
    int type = charType(str);
    return (type == CHAR_LETTER_TYPE);
}

bool isPunc(const std::string &str) {
    int type = charType(str);
    return (type == CHAR_PUNC_TYPE);
}

bool isDigit(const std::string &str) {
    int type = charType(str);
    return (type == CHAR_DIGIT_TYPE);
}

std::string SBC2DBC(const std::string &str) {
    int offset;
    if (isSBC(str) == true) {
        for (int i = 0; i < SBC_TABLE_SIZE; ++ i) {
            if (str[0] == SBCTable[i]) {
                if (i < SBC_UP_ALPHA_GROUP_OFFSET) { // SBC digit
                    offset = i - SBC_DIGIT_GROUP_OFFSET;
                    return std::string(DBCTable[DBC_DIGIT_GROUP_OFFSET + offset]);
                } else if (i < SBC_LOW_ALPHA_GROUP_OFFSET) { // SBC uppercase
                    offset = i - SBC_UP_ALPHA_GROUP_OFFSET;
                    return std::string(DBCTable[DBC_UP_ALPHA_GROUP_OFFSET + offset]);
                } else if (i < SBC_PUNC_GROUP_OFFSET) { // SBC lowercase
                    offset = i - SBC_LOW_ALPHA_GROUP_OFFSET;
                    return std::string(DBCTable[DBC_LOW_ALPHA_GROUP_OFFSET + offset]);
                } else { // SBC punc
                    offset = i - SBC_PUNC_GROUP_OFFSET;
                    return std::string(DBCTable[DBC_PUNC_GROUP_OFFSET + offset]);
                }
            }
        }
    }
    return std::string(str);
}

std::string DBC2SBC(const std::string &str) {
    return std::string(str);
}

