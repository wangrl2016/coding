//
// Created by wangrl on 2021/3/15.
//

#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <iostream>

/**
 * Including this file (and compiling CommandLineFlags.cpp) provides command line
 * parsing. In order to use it, use the following macros in global namespace.
 *
 * DEFINE_bool(name, defaultValue, helpString);
 * DEFINE_string(name, defaultValue, helpString);
 * DEFINE_int(name, defaultValue, helpString);
 * DEFINE_double(name, defaultValue, helpString);
 *
 * Then, in main, call CommandLineFlags::SetUsage() to describe usage and call
 * CommandLineFlags::Parse() to parse the flags. Henceforth, each flags can be
 * referenced using
 *
 * FLAGS_name
 *
 * For example, the line
 *
 * DEFINE_bool(boolean, false, "The variable boolean does such and such");
 *
 * will create the following variable:
 *
 * bool FLAGS_boolean;
 *
 * which will initially beset to false, and can be set to true by using the
 * flag "--boolean" on the command line. "--noboolean" will set FLAGS_boolean
 * to false. FLAGS_boolean can also be set using "--boolean=true"or
 * "--boolean true" (where "true" can be replaced by "false", "TRUE", "FALSE",
 * "1" or "0").
 *
 * The helpString will be printed if he help flag (-h or -help) is used.
 *
 * Similarly, the line
 *
 * DEFINE_int(integer, .., ..);
 *
 * will create
 *
 * int FLAGS_integer;
 *
 * and
 *
 * DEFINE_double(real, .., ..);
 *
 * will create
 *
 * double FLAGS_real;
 *
 * These flags can be set by specify, for example," "--integer 7" and
 * "--real 3.14" on the command line. Unsigned integers are parsed from the
 * command line using strtoul() so will detect the base (0 for octal, and
 * 0x or 0X for hex, otherwise assumes decimal).
 *
 * Unlike the others, the line
 *
 * DEFINE_string(args, .., ..)
 *
 * create an array:
 *
 * CommandLineFalse::StringArray FLAGS_args;
 *
 * If the default value is the empty string, FLAGS_args will default to a size
 * of zero. Otherwise it will default to a size of 1 with the default string
 * as its value. All strings that follow the flag on the command line (until
 * a string that begins with '-') will be entries in the array.
 *
 * DEFINE_extended_string(args, .., .., extendedHelpString);
 *
 * create a similar string array flags as DEFINE_string. The flag will have extended
 * help text (extendedHelpString) that can the user can see with '--help <args>' flag.
 *
 * Any flag can be referenced from another file after using the following:
 *
 * DECLARE_x(name);
 *
 * (where 'x' is the type specified in the DEFINE).
 */

class FlagInfo;

class CommandLineFlags {
public:
    // Call to set the help message to be displayed.
    // Should be called before Parse.
    static void SetUsage(const char* usage);

    // Call this to display the help message.
    // Should be called after SetUsage.
    static void PrintUsage();

    // Call at the beginning of main to parse flags created by DEFINE_x, above.
    // Must only be called once.
    static void Parse(int argc, const char* const* argv);

    // Custom class for holding the arguments for a string flag.
    // Publicly only has accessors so the strings cannot be modified.
    class StringArray {
    public:
        StringArray() {}

        explicit StringArray(const std::vector<std::string>& strings) : fStrings(strings) {}

        const char* operator[](int i) const {
            // 防止数组越界
            assert(i >= 0 && i < fStrings.size());
            return fStrings[i].c_str();
        }

        int count() const {
            return fStrings.size();
        }

        bool isEmpty() const {
            return this->count() == 0;
        }

        bool contains(const char* string) const {
            for (int i = 0; i < fStrings.size(); i++) {
                if (fStrings[i] == string)
                    return true;
            }
            return false;
        }

        void set(int i, const char* str) {
            if (i >= fStrings.size()) {
                this->append(str);
                return;
            }
            fStrings[i] = str;
        }

        std::string begin() const {
            return fStrings[0];
        }

        std::string end() const {
            return fStrings[fStrings.size() - 1];
        }

    private:
        void append(const char* string) {
            fStrings.push_back(string);
        }

        void append(const char* string, size_t length) {
            if (0 == length)
                return;
            std::string temp(string, length);
            fStrings.push_back(temp);
        }

        void reset() {
            while (!fStrings.empty())
                fStrings.pop_back();
        }

        // SkTArray<T> implements a typical, mostly std::vector-like array.
        // Each T will be default-initialized on allocation, and ~T will be
        // called on destruction.
        // MEM_MOVE controls the behavior when a T needs to be moved
        // (e.g. when the array is resized)
        //    - true: T will be bit-copied via memcpy
        //    - false: T will be moved via move-constructors.
        // Modern implementations of std::vector<T> will generally provide
        // similar performance characteristics when used with appropriate care.
        // Consider using std::vector<T> in new code.
        // SkTArray<SkString> fStrings;
        std::vector<std::string> fStrings;

        // 让FlagInfo类能够访问
        friend class FlagInfo;
    };

    static FlagInfo* gHead;
    static std::string gUsage;

    // For access to gHead.
    friend class FlagInfo;
};

#define TO_STRING(s) #s

#define DEFINE_bool(name, defaultValue, helpString)                   \
    bool                  FLAGS_##name;                               \
    static bool unused_##name = FlagInfo::CreateBoolFlag(             \
            TO_STRING(name), nullptr, &FLAGS_##name, defaultValue, helpString)

// bool_s allows specifying a short name. No check is done to ensure that shortName
// is actually shorter than name.
#define DEFINE_bool_s(name, shortName, defaultValue, helpString)        \
    bool FLAGS_##name;                                                  \
    static bool unused_##name = FlagInfo::CreateBoolFlag(               \
        TO_STRING(name), TO_STRING(shortName), &FLAGS_##name, defaultValue, helpString)

#define DECLARE_bool(name) extern bool FLAGS_##name;

#define DEFINE_string(name, defaultValue, helpString)                   \
    CommandLineFlags::StringArray FLAGS_##name;                         \
    static bool unused_##name = FlagInfo::CreateStringFlag(             \
        TO_STRING(name), nullptr, &FLAGS_##name, defaultValue, helpString, nullptr)

#define DEFINE_string_s(name, shortName, defaultValue, helpString)      \
    CommandLineFlags::StringArray FLAGS_##name;                         \
    static bool unused_##name = FlagInfo::CreateStringFlag(             \
        TO_STRING(name), TO_STRING(shortName), &FLAGS_##name, defaultValue, helpString, nullptr)

#define DEFINE_extended_string(name, defaultValue, helpString, extendedHelpString)  \
    CommandLineFlags::StringArray FLAGS_##name;                                     \
    static bool unused_##name = FlagInfo::CreateStringFlag(                         \
        TO_STRING(name),nullptr, &FLAGS_##name, defaultValue,helpString,extendedHelpString)

#define DECLARE_string(name) extern CommandLineFlags::StringArray FLAGS_##name;

#define DEFINE_int(name, defaultValue, helpString)                      \
    int FLAGS_##name;                                                   \
    static bool unused_##name = FlagInfo::CreateIntFlag(                \
        TO_STRING(name), &FLAGS_##name, defaultValue, helpString)

#define DEFINE_int_s(name, shortName, defaultValue, helpString)         \
    int FLAGS_##name;                                                   \
    static bool unused_##name = FlagInfo::CreateIntFlag(                \
        TO_STRING(name), TO_STRING(shortName), &FLAGS_##name, defaultValue, helpString)

#define DECLARE_int(name) extern int FLAGS_##name;

#define DEFINE_double(name, defaultValue, helpString)                   \
    double FLAGS_##name;                                                \
    static bool unused_##name = FlagInfo::CreateDoubleFlag(             \
        TO_STRING(name), &FLAGS_##name, defaultValue, helpString)

#define DECLARE_double(name) extern double FLAGS_##name;

class FlagInfo {
public:
    enum FlagTypes {
        kBool_FlagType,
        kString_FlagType,
        kInt_FlagType,
        kDouble_FlagType,
    };


    /**
     * Each Create<Type>Flag function create an FlagInfo of the specified type. The FlagInfo
     * object is appended to a list, which is deleted when CommandLineFlags::Parse is called.
     * Therefore, each call should be made before the call to ::Parse. They are not intended
     * to be called directly. Instead, use the macros described above.
     *
     * @param name          Long version (at least 2 characters) of the name of the flag. This name can
     *                      be referenced on the command line as "--name" to set the value of the flag.
     * @param shortName     Short version (1 character) of the name of the flag. This name can
     *                      be referenced on the command line as "-shortName" to set the value of this flag.
     * @param pBool   [out] Pointer to a global variable which holds the value set by CommandLineFlags.
     * @param defaultValue  The default value of this flag. The variable pointed to by p<Type> will
     *                      be set to this value initially. This is also displayed as part of the help output.
     * @param helpString    Explanation of what this flag changes in the program.
     */
    static bool CreateBoolFlag(const char* name,
                               const char* shortName,
                               bool* pBool,
                               bool defaultValue,
                               const char* helpString) {
        FlagInfo* info = new FlagInfo(name, shortName, kBool_FlagType, helpString, nullptr);
        info->fBoolValue = pBool;
        info->fBoolValue = pBool;
        *info->fBoolValue = info->fDefaultBool = defaultValue;
        return true;
    }

    /**
     * @param pStrings          Unlike the others, this is a pointer to an array of values.
     * @param defaultValue      These default will be parsed so that strings separated by spaces
     *                          will be added to pStrings.
     */
    static bool CreateStringFlag(const char* name,
                                 const char* shortName,
                                 CommandLineFlags::StringArray* pStrings,
                                 const char* defaultValue,
                                 const char* helpString,
                                 const char* extendedHelpString);

    static bool CreateIntFlag(const char* name,
                              int* pInt,
                              int defaultValue,
                              const char* helpString) {
        FlagInfo* info = new FlagInfo(name, nullptr, kInt_FlagType, helpString, nullptr);
        info->fIntValue = pInt;
        *info->fIntValue = info->fDefaultInt = defaultValue;
        return true;
    }

    static bool CreateIntFlag(const char* name,
                              const char* shortName,
                              int* pInt,
                              int defaultValue,
                              const char* helpString) {
        FlagInfo* info = new FlagInfo(name, shortName, kInt_FlagType, helpString, nullptr);
        info->fIntValue = pInt;
        *info->fIntValue = info->fDefaultInt = defaultValue;
        return true;
    }

    static bool CreateDoubleFlag(const char* name,
                                 double* pDouble,
                                 double defaultValue,
                                 const char* helpString) {
        FlagInfo* info = new FlagInfo(name, nullptr, kDouble_FlagType, helpString, nullptr);
        info->fDoubleValue = pDouble;
        *info->fDoubleValue = info->fDefaultDouble = defaultValue;
        return true;
    }

    void resetStrings() {
        if (kString_FlagType == fFlagType) {
            fStrings->reset();
        } else {
            std::cout << "Can only call resetStrings on kString_FlagType\n";
        }
    }

    void append(const char* string) {
        if (kString_FlagType == fFlagType) {
            fStrings->append(string);
        } else {
            std::cout << "Can only append to kString_FlagType\n";
        }
    }

    // 改变全局变量
    void setBool(bool value) {
        if (kBool_FlagType == fFlagType) {
            *fBoolValue = value;
        } else {
            std::cout << "Can only call setBool on kBool_FlagType\n";
        }
    }

    void setDouble(double value) {
        if (kDouble_FlagType == fFlagType) {
            *fDoubleValue = value;
        } else {
            std::cout << "Can only call setDouble on kDouble_FlagType\n";
        }
    }

    void setInt(int value) {
        if (kInt_FlagType == fFlagType) {
            *fIntValue = value;
        } else {
            std::cout << "Can only call setInt on kInt_FlagType\n";
        }
    }

    /**
     * Returns true if the string matches this flag.
     * For a boolean flag, also sets the value, since a boolean flag can be set in a number of
     * ways without looking at the following string:
     *      --name
     *      --noname
     *      --name=true
     *      --name=false
     *      --name=1
     *      --name=0
     *      --name=TRUE
     *      --name=FALSE
     */
    bool match(const char* string);

    FlagTypes getFlagType() const {
        return fFlagType;
    }

    FlagInfo* next() {
        return fNext;
    }

    const std::string& name() const {
        return fName;
    }

    const std::string& shortName() const {
        return fShortName;
    }

    const std::string& help() const {
        return fHelpString;
    }

    const std::string& extendedHelp() const {
        return fExtendedHelpString;
    }

    std::string defaultValue() const {
        std::string result;
        switch (fFlagType) {
            case FlagInfo::kBool_FlagType:
                result = fDefaultBool ? "true" : "false";
                break;
            case FlagInfo::kString_FlagType:
                return fDefaultString;
            case FlagInfo::kInt_FlagType:
                result = std::to_string(fDefaultInt);
                break;
            case FlagInfo::kDouble_FlagType:
                result = std::to_string(fDefaultDouble);
                break;
            default:
                std::cout << "Invalid flag type\n";
        }
        return result;
    }

    std::string typeAsString() const {
        switch (fFlagType) {
            case FlagInfo::kBool_FlagType:
                return std::string("bool");
            case FlagInfo::kString_FlagType:
                return std::string("string");
            case FlagInfo::kInt_FlagType:
                return std::string("int");
            case FlagInfo::kDouble_FlagType:
                return std::string("double");
            default:
                std::cout << "Invalid flag type\n";
                return std::string();
        }
    }

private:

    FlagInfo(const char* name,
             const char* shortName,
             FlagTypes type,
             const char* helpString,
             const char* extendedHelpString)
            : fName(name),
              fShortName(shortName == nullptr ? std::string() : shortName),
              fFlagType(type),
              fHelpString(helpString),
              fExtendedHelpString(extendedHelpString == nullptr ? std::string() : extendedHelpString),
              fBoolValue(nullptr),
              fDefaultBool(false),
              fIntValue(nullptr),
              fDefaultInt(0),
              fDoubleValue(nullptr),
              fDefaultDouble(0.0),
              fStrings(nullptr) {
        // 插入到链表最前面
        fNext = CommandLineFlags::gHead;
        CommandLineFlags::gHead = this;
        assert(name && strlen(name) > 1);
        assert(nullptr == shortName || 1 == strlen(shortName));
    }

    static void SetDefaultStrings(CommandLineFlags::StringArray* array, const char* defaultStrings);

    // Name of the flag, without initial dashes.
    std::string fName;
    std::string fShortName;
    FlagTypes fFlagType;
    std::string fHelpString;
    std::string fExtendedHelpString;
    bool* fBoolValue;
    bool fDefaultBool;
    int* fIntValue;
    int fDefaultInt;
    double* fDoubleValue;
    double fDefaultDouble;
    CommandLineFlags::StringArray* fStrings;

    // Both for the help string and in case fStrings is empty.
    std::string fDefaultString;

    // In order to keep a linked list.
    FlagInfo* fNext;
};
