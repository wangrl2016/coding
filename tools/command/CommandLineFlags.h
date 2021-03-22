//
// Created by wangrl on 2021/3/15.
//

#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <cassert>

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

    // For access to gHead.
    friend class FlagInfo;
};

#define TO_STRING(s) #s

#define DEFINE_bool(name, defaultValue, helpString)                   \
    bool                  FLAGS_##name;                               \
    static bool unused_##name = FlagInfo::CreateBoolFlag(             \
            TO_STRING(name), nullptr, &FLAGS_##name, defaultValue, helpString)

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
     * @param pBool         Pointer to a global variable which holds the value set by CommandLineFlags.
     * @param defaultValue  The default value of this flag. The variable pointed to by pBool will
     *                      be set to this value initially. This is also displayed as part of the help output.
     * @param helpString    Explanation of what this flag changes in the program.
     * @return
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


