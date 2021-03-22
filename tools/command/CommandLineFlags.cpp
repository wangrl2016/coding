//
// Created by wangrl on 2021/3/15.
//

#include "CommandLineFlags.h"

FlagInfo* CommandLineFlags::gHead;
std::string CommandLineFlags::gUsage;

// Some helper function for C strings.
static inline bool StrStartsWith(const char string[], const char prefixStr[]) {
    assert(string);
    assert(prefixStr);
    return !strncmp(string, prefixStr, strlen(prefixStr));
}

static inline int StrFind(const char string[], const char substring[]) {
    // char *strstr(const char *haystack, const char *needle);
    // Finds the first occurrence of the substring needle in the string haystack.
    const char* first = strstr(string, substring);
    if (nullptr == first)
        return -1;
    return first - &string[0];
}

/**
 * @return the number of entries in an array (not a pointer)
 */
template<typename T, size_t N>
char (& ArrayCountHelper(T (& array)[N]))[N];

#define ARRAY_COUNT(array) (sizeof(ArrayCountHelper(array)))

void CommandLineFlags::SetUsage(const char* usage) {
    gUsage = std::string(usage);
}

void CommandLineFlags::PrintUsage() {
    std::cout << gUsage.c_str();
}

void CommandLineFlags::Parse(int argc, const char* const* argv) {
    // Only allow calling this function once.
    static bool gOnce;
    if (gOnce) {
        std::cout << "Parse should only be called once at the beginning of main!\n";
        return;
    }
    gOnce = true;
    bool helpPrinted = false;
    bool flagsPrinted = false;

    // Loop over argv, starting with 1, since the first is just the name of the program.
    for (int i = 1; i < argc; i++) {
        if (0 == strcmp("-h", argv[i]) || 0 == strcmp("--help", argv[i])) {
            // Print help message.
            helpPrinted = true;
        }
        if (!helpPrinted) {
            FlagInfo* matchFlags = nullptr;
            FlagInfo* flag = gHead;
            int start = i;
            // 遍历flag和命令行的字符串匹配
            while (flag != nullptr) {

                flag = flag->next();
            }
        }
    }
}

/**
 * 检查target是否在长度为len的set集合中。
 *
 * @param target　   被检查的字符串
 * @param set       检查集合
 * @param len       集合长度
 * @return          ture如何字符串在集合中
 */
static bool stringIsIn(const char* target, const char* set[], size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (0 == strcmp(target, set[i]))
            return true;
    }
    return false;
}

static bool parseBoolArg(const char* string, bool* result) {
    static const char* trueValues[] = {"1", "TRUE", "true"};
    if (stringIsIn(string, trueValues, ARRAY_COUNT(trueValues))) {
        *result = true;
        return true;
    }
    static const char* falseValues[] = {"0", "FALSE", "false"};
    if (stringIsIn(string, falseValues, ARRAY_COUNT(falseValues))) {
        *result = false;
        return true;
    }
    std::cout << "Parameter %s not supported\n" << string;
    return false;
}

bool FlagInfo::match(const char* string) {
    if (StrStartsWith(string, reinterpret_cast<const char*>('-')) && strlen(string) > 1) {
        string++;
        std::string compareName;
        if (StrStartsWith(string, reinterpret_cast<const char*>('-')) && strlen(string) > 1) {
            string++;
            // There are two dashes. Compare against full name.
            compareName = fName;
        } else {
            // On dash. Compare against the short name.
            compareName = fShortName;
        }

        if (kBool_FlagType == fFlagType) {
            // In this cas, go ahead and set the value.
            if (compareName == string) {
                *fBoolValue = true;
                return true;
            }

            if (StrStartsWith(string, "no") && strlen(string) > 2) {
                string += 2;
                // Only allow "no" to be prepended to the full name.
                if (fName == string) {
                    *fBoolValue = false;
                    return true;
                }
                return false;
            }
            int equalIndex = StrFind(string, "=");
            if (equalIndex > 0) {
                // The string has an equal sign. Check to see if the string matches.
                std::string flag(string, equalIndex);
                if (flag == compareName) {
                    // Check to see if the remainder beyond the equal sign is true or false.
                    string += equalIndex + 1;
                    parseBoolArg(string, fBoolValue);
                    return true;
                } else {
                    return false;
                }
            }
        }
        return compareName == string;
    } else {
        // Has no dash
        return false;
    }
    return false;
}

bool FlagInfo::CreateStringFlag(const char* name, const char* shortName, CommandLineFlags::StringArray* pStrings,
                                const char* defaultValue, const char* helpString, const char* extendedHelpString) {
    FlagInfo* info =
            new FlagInfo(name, shortName, kString_FlagType, helpString, extendedHelpString);
    info->fDefaultString = defaultValue;
    info->fStrings = pStrings;
    SetDefaultStrings(pStrings, defaultValue);
    return true;
}

// defaultValue以空格分开
void FlagInfo::SetDefaultStrings(CommandLineFlags::StringArray* pStrings, const char* defaultValue) {
    pStrings->reset();
    if (nullptr == defaultValue)
        return;
    // If default is "", leave the array empty.
    size_t defaultLength = strlen(defaultValue);
    if (defaultLength > 0) {
        const char* const defaultEnd = defaultValue + defaultLength;
        const char* begin = defaultValue;
        while (true) {
            while (begin < defaultEnd && ' ' == *begin) {
                begin++;
            }
            if (begin < defaultEnd) {
                const char* end = begin + 1;
                while (end < defaultEnd && ' ' != *end) {
                    end++;
                }
                size_t length = end - begin;
                pStrings->append(begin, length);
                begin = end + 1;
            } else {
                break;
            }
        }
    }
}
