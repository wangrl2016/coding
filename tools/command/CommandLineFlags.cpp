//
// Created by wangrl on 2021/3/15.
//

#include <algorithm>
#include "CommandLineFlags.h"

FlagInfo* CommandLineFlags::gHead;
std::string CommandLineFlags::gUsage;

// Some helper function for C strings.
static inline bool StrStartsWith(const char string[], const char prefixStr[]) { // NOLINT
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
    return (int) (first - &string[0]);
}


// Maximum line length for the help message
#define LINE_LENGTH 72

static void printIndented(const std::string& text) {
    size_t length = text.size();
    const char* currLine = text.c_str();
    const char* stop = currLine + length;
    while (currLine < stop) {
        int lineBreak = StrFind(currLine, "\n");
        if (lineBreak < 0) {
            lineBreak = static_cast<int>(strlen(currLine));
        }
        if (lineBreak > LINE_LENGTH) {
            // No line break within line length. Will need to insert one.
            // Find a space before the line break.
            int spaceIndex = LINE_LENGTH - 1;
            while (spaceIndex > 0 && currLine[spaceIndex] != ' ') {
                spaceIndex--;
            }
            int gap;
            if (0 == spaceIndex) {
                // No spaces on the entire line. Go ahead and break mid word.
                spaceIndex = LINE_LENGTH;
                gap = 0;
            } else {
                // Skip the space on the next line
                gap = 1;
            }
            printf("        %.*s\n", spaceIndex, currLine);
            currLine += spaceIndex + gap;
        } else {
            // The line break is within the limit.
            // Break there.
            lineBreak++;
            printf("        %.*s", lineBreak, currLine);
            currLine += lineBreak;
        }
    }
}

static void printHelpForFlag(const FlagInfo* flag) {
    printf("    --%s", flag->name().c_str());
    const std::string& shortName = flag->shortName();
    if (!shortName.empty()) {
        printf(" or -%s", shortName.c_str());
    }
    printf(":\ttype: %s", flag->typeAsString().c_str());
    if (!flag->defaultValue().empty()) {
        printf("\tdefault: %s", flag->defaultValue().c_str());
    }
    printf("\n");
    const std::string& help = flag->help();
    printIndented(help);
    printf("\n");
}

static void printExtendedHelpForFlag(const FlagInfo* flag) {
    printHelpForFlag(flag);
    printIndented(flag->extendedHelp());
    printf("\n");
}

/**
 * @return the number of entries in an array (not a pointer)
 */
template<typename T, size_t N>
char (& ArrayCountHelper(T (& array)[N]))[N];

#define ARRAY_COUNT(array) (sizeof(ArrayCountHelper(array)))

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

void CommandLineFlags::SetUsage(const char* usage) {
    gUsage = std::string(usage);
}

void CommandLineFlags::PrintUsage() {
    std::cout << gUsage.c_str();
}

template<typename T>
static void ignoreResult(const T&) {}

void CommandLineFlags::Parse(int argc, const char* const* argv) {
    // Only allow calling this function once.
    static bool gOnce;
    if (gOnce) {
        std::cout << "Parse should only be called once at the beginning of main!\n";
        assert(false);
        return;
    }
    gOnce = true;
    bool helpPrinted = false;
    // 用于首次打印"Flags:"字符串
    bool flagsPrinted = false;

    // Loop over argv, starting with 1, since the first is just the name of the program.
    for (int i = 1; i < argc; i++) {
        if (0 == strcmp("-h", argv[i]) || 0 == strcmp("--help", argv[i])) {
            // Print help message.
            helpPrinted = true;
            std::vector<const char*> helpFlags;

            for (int j = i + 1; j < argc; j++) {
                // 碰到下一个以-开始的符号结束
                if (StrStartsWith(argv[j], "-")) {
                    break;
                }
                helpFlags.push_back(argv[j]);
            }

            if (helpFlags.empty()) {
                // Only print general help message if help for specific flags is not requested.
                PrintUsage();
            }

            std::sort(helpFlags.begin(), helpFlags.end());

            if (!flagsPrinted) {
                printf("Flags:\n");
                flagsPrinted = true;
            }
            if (helpFlags.empty()) {
                // If no flags followed --help, print them all.
                std::vector<FlagInfo*> allFlags;
                for (FlagInfo* flag = CommandLineFlags::gHead; flag; flag = flag->next()) {
                    allFlags.push_back(flag);
                }
                std::sort(allFlags.begin(), allFlags.end());
                for (FlagInfo* flag: allFlags) {
                    printHelpForFlag(flag);
                    if (flag->extendedHelp().size() > 0) {
                        printf("        Use '--help %s' for more information.\n",
                               flag->name().c_str());
                    }
                }
            } else {
                for (FlagInfo* flag = CommandLineFlags::gHead; flag; flag = flag->next()) {
                    for (int k = 0; k < helpFlags.size(); k++) {
                        if (flag->name() == helpFlags[k] ||
                            flag->shortName() == helpFlags[k]) {
                            printExtendedHelpForFlag(flag);
                            helpFlags.erase(helpFlags.begin() + k);
                            break;
                        }
                    }
                }
            }
            if (!helpFlags.empty()) {
                printf("Requested help for unrecognized flags:\n");
                for (auto& helpFlag : helpFlags) {
                    printf("    --%s\n", helpFlag);
                }
            }
            helpPrinted = true;
        }
        if (!helpPrinted) {
            FlagInfo* matchedFlag = nullptr;
            FlagInfo* flag = gHead;
            int start = i;
            // 遍历flag和命令行的字符串匹配
            while (flag != nullptr) {
                if (flag->match(argv[start])) {
                    i = start;
                    if (matchedFlag) {
                        // Don't redefine the same flag with different types.
                        assert(matchedFlag->getFlagType() == flag->getFlagType());
                    } else {
                        matchedFlag = flag;
                    }
                    switch (flag->getFlagType()) {
                        case FlagInfo::kBool_FlagType: {
                            // Can be handled by match, above, but can also be set by the next string.
                            if (i + 1 < argc && !StrStartsWith(argv[i + 1], "-")) {
                                i++;
                                bool value;
                                if (parseBoolArg(argv[i], &value)) {
                                    flag->setBool(value);
                                }
                            }
                            break;
                        }
                        case FlagInfo::kString_FlagType: {
                            flag->resetStrings();
                            // Add all arguments until another flag is reached.
                            while (i + 1 < argc) {
                                char* end = nullptr;
                                // Negative number aren't flags.
                                ignoreResult(strtod(argv[i], &end));
                                if (end == argv[i + 1] &&
                                    StrStartsWith(argv[i + 1], "-")) {
                                    break;
                                }
                                i++;
                                flag->append(argv[i]);
                            }
                            break;
                        }
                        case FlagInfo::kInt_FlagType: {
                            i++;
                            char* end;
                            flag->setInt((int) strtol(argv[i], &end, 10));
                            break;
                        }
                        case FlagInfo::kDouble_FlagType: {
                            i++;
                            char* end;
                            flag->setDouble(strtod(argv[i], &end));
                            break;
                        }
                        default:
                            printf("Invalid flag type\n");
                    }
                }
                flag = flag->next();
            }
            if (!matchedFlag) {
                printf("Got unknown flag %s. Exiting\n", argv[i]);
                exit(EXIT_FAILURE);
            }
        }
    }
    // Since all of the flags have been set, release the memory used by each flag.
    // FLAGS_x can still be used after this.
    FlagInfo* flag = gHead;
    gHead = nullptr;
    while (flag != nullptr) {
        FlagInfo* next = flag->next();
        delete flag;
        flag = next;
    }
    if (helpPrinted) {
        exit(EXIT_SUCCESS);
    }
}


bool FlagInfo::match(const char* string) {
    const char* pre = "-";
    if (StrStartsWith(string, pre) && strlen(string) > 1) {
        string++;
        std::string compareName;
        if (StrStartsWith(string, "-") && strlen(string) > 1) {
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
    }
    return false;
}

bool FlagInfo::CreateStringFlag(const char* name, const char* shortName, CommandLineFlags::StringArray* pStrings,
                                const char* defaultValue, const char* helpString, const char* extendedHelpString) {
    auto* info =
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

