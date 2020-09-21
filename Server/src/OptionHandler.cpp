#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

#include <OptionHandler.h>

using namespace std;

/**
 * Parse the command line. Returns true if clean, false if bad args or help was provided.
 */
bool
OptionHandler::handleOptions(int argc, const char * const *argv, Argument *arguments, HelpFunction helpFunction) {
    ArgumentVector vec;
    vec.addAll(arguments);
    return handleOptions(argc, argv, vec, helpFunction);
}

/**
 * Parse the command line. Returns true if clean, false if bad args or help was provided.
 */
bool
OptionHandler::handleOptions(int argc, const char * const *argv, ArgumentVector & vec, HelpFunction helpFunction) {
    bool retVal = true;

    if (!helpFunction && vec.anyHaveHelpText() && (vec.searchForLongName("help") == nullptr) ) {
        char helpShortCode = (vec.searchForShortCode('h') == nullptr) ? '?' : 0;
        vec.push_back( std::make_shared<Argument> ("help", no_argument, helpShortCode, [&](const char *) { vec.autoHelp(); retVal = false; }, "", "Provide this help" ) );
    }

    unsigned int count = static_cast<unsigned int>(vec.size());
    string shortArgs;
    option * options = vec.getOptions(shortArgs);
    int optionIndex = -1;
    int opt;
    const char *shortArgsPtr = shortArgs.c_str();

    while ( (opt = getopt_long (argc, const_cast<char **>(argv), shortArgsPtr, options, &optionIndex)) != -1) {
        // This happens with short options.
        if (optionIndex < 0) {
            for (unsigned int index = 0; index < count; ++index) {
                if (vec.at(index)->val == opt) {
                    optionIndex = static_cast<int>(index);
                    break;
                }
            }
        }

        if (optionIndex < 0) {
            if (helpFunction != nullptr) {
                helpFunction();
            }
            retVal = false;
            break;
        }

        vec.at(static_cast<unsigned int>(optionIndex))->callbackFunction(optarg);
        optionIndex = -1;
    }

    return retVal;
}

//======================================================================
// Argument.
//======================================================================

/**
 * Constructor.
 */
OptionHandler::Argument::Argument() {
}

/**
 * Complicated constructor.
 */
OptionHandler::Argument::Argument(
    const char *_name,
    int _has_arg, char _val,
    CallbackFunction cb,
    const string & argText,
    const string & help )
: has_arg(_has_arg), val(_val), callbackFunction(cb), argumentText(argText), helpText(help)
{
    if (_name != nullptr) {
        name = _name;
    }
}

/**
 * Copy constructor.
 */
OptionHandler::Argument::Argument(const Argument &arg)
:	name ( arg.name ),
    has_arg ( arg.has_arg ),
    val ( arg.val ),
    callbackFunction ( arg.callbackFunction ),
    argumentText ( arg.argumentText ),
    helpText ( arg.helpText )
{
}


string
OptionHandler::Argument::argumentForHelp() const {
    string retVal;

    if (name.length() > 0) {
        retVal = string("--") + name;
        if (isprint(val)) {
            retVal += string(" (-") + val + string(")");
        }
    }
    else if (isprint(val)) {
        retVal = string("-") + val;
    }

    if (argumentText.length() > 0) {
        retVal += string(" ") + argumentText;
    }

    return retVal;
}

/**
 * For auto-generated help, we're going to write something like:
 *
 *    --foo (-f) fooValue
 *
 * Return the string length.
 */
int
OptionHandler::Argument::argumentLength() const {
    return static_cast<int>(argumentForHelp().length());
}

string
OptionHandler::Argument::paddedArgument(int length) const {
    string str = argumentForHelp();

    if (static_cast<int>(str.length()) < length) {
        str += string(length - str.length(), ' ');
    }

    return str;
}

//======================================================================
// Argument Vector.
//======================================================================

/**
 * Vector constructor.
 */
OptionHandler::ArgumentVector::ArgumentVector()
    : options(nullptr), optionsCount(0)
{
}

OptionHandler::ArgumentVector::~ArgumentVector() {
    if (options != nullptr) {
        delete[] options;
        options = nullptr;
        optionsCount = 0;
    }
}

/**
 * Add one option.
 *
 * @param name    This becomes --name as the command line argument.
 * @param has_arg This is a getopt flag such as required_argument or no_argument
 * @param val     If you want a short form such as -b. If no short form, pass in 0.
 * @param cb      This is the lambda expression to execute.
 * @param argText If required_argument, this text becomes the argument name used in the help.
 * @param help    This is the text of the help.
 */
OptionHandler::ArgPointer
OptionHandler::ArgumentVector::add(
    const char *name,
    int has_arg,
    char val,
    CallbackFunction cb,
    const std::string & argText,
    const std::string & help)
{
    OptionHandler::ArgPointer arg = std::make_shared<OptionHandler::Argument>( name, has_arg, val, cb, argText, help );
    push_back(arg);
    return arg;
}

/**
 * A convenience method to use when you don't want a short form. This is just
 * a wrapper to the longer form of add with the val argument passed as 0 for you.
 *
 * For parameters, see the description for the longer form of add.
 */
OptionHandler::ArgPointer
OptionHandler::ArgumentVector::add(
    const char *name,
    int has_arg,
    CallbackFunction cb,
    const std::string & argText,
    const std::string & help)
{
    return add(name, has_arg, 0, cb, argText, help);
}

/**
 * This adder is for when there's a required_argument. The method exists just to
 * remove a little boilerplate.
 *
 * For parameters, see the description for the longer form of add.
 */
OptionHandler::ArgPointer
OptionHandler::ArgumentVector::addArg(
    const char *name,
    CallbackFunction cb,
    const std::string & argText,
    const std::string & help)
{
    return add(name, required_argument, 0, cb, argText, help);
}

/**
 * This adder is for when there's no argument to the option -- boolean flags and what not.
 * This method exists simply as a shortcut to remove the "no_argument" boilerplate all over
 * the place. Because it's no_argument, we can also eliminate the argText, as it should always
 * be empty. Thus, we have a nice, tight signature with only the three arguments that make sense
 * in this situation.
 *
 * For parameters, see the description for the longer form of add.
 */
OptionHandler::ArgPointer
OptionHandler::ArgumentVector::addNoArg( const char *name, CallbackFunction cb, const std::string & help) {
    return add(name, no_argument, 0, cb, "", help);
}


/**
 * Append these.
 */
void
OptionHandler::ArgumentVector::addAll(Argument *arguments) {
    unsigned long count = 0;
    while(arguments[count].name.length() > 0 || arguments[count].val != 0) {
        ++count;
    }

    for (unsigned long index = 0; index < count; ++index) {
        Argument &arg = arguments[index];
        ArgPointer newArg = std::make_shared<Argument>(arg);
        push_back(newArg);
    }
}

/**
 * Append.
 */
void
OptionHandler::ArgumentVector::addAll(ArgumentVector &vec) {
    for (ArgPointer arg: vec) {
        ArgPointer  newArg = std::make_shared<Argument>(*arg);
        push_back(newArg);
    }
}

/**
 * We use getopt_long to actually do the parsing. He assumes an array of struct option. We produce one.
 * Note that if you fiddle with our contents without changing our size, we sort of screw up, so if you call
 * this method more than once, and you make changes between, it might be a problem.
 */
struct option *
OptionHandler::ArgumentVector::getOptions(std::string &shortArgsReturned) {
    if (options == nullptr || optionsCount != size()) {
        optionsCount = size() + 1;
        options = new option[optionsCount];
        shortArgs = "";

        int index = 0;
        for (ArgPointer arg: *this) {
            option &opt = options[index++];

            opt.name = arg->name.c_str();
            opt.has_arg = arg->has_arg;
            opt.flag = nullptr;
            opt.val = arg->val;

            if (arg->val != 0) {
                shortArgs += arg->val;
                if (opt.has_arg != no_argument) {
                    shortArgs += ":";
                }
            }
        }
        {
            // Do the last one.
            option &opt = options[index];
            opt.name = nullptr;
            opt.has_arg = 0;
            opt.flag = nullptr;
            opt.val = 0;
        }

    }

    shortArgsReturned = shortArgs;
    return options;
}

OptionHandler::ArgPointer
OptionHandler::ArgumentVector::searchForLongName(const std::string &value) {
    ArgPointer retVal = nullptr;
    for (auto it = begin(); it != end(); ++it) {
        ArgPointer arg = *it;
        if (arg->name == value) {
            retVal = arg;
            break;
        }
    }
    return retVal;
}


OptionHandler::ArgPointer
OptionHandler::ArgumentVector::searchForShortCode(char code) {
    ArgPointer retVal = nullptr;
    for (auto it = begin(); it != end(); ++it) {
        ArgPointer arg = *it;
        if (arg->val == code) {
            retVal = arg;
            break;
        }
    }
    return retVal;
}

bool
OptionHandler::ArgumentVector::anyHaveHelpText() const {
    bool retVal = false;
    for (auto it = begin(); it != end(); ++it) {
        ArgPointer arg = *it;
        if (arg->argumentText.length() > 0 || arg->helpText.length() > 0) {
            retVal = true;
            break;
        }
    }
    return retVal;
}

void
OptionHandler::ArgumentVector::autoHelp() const {
    int longest = 0;
    for (auto it = cbegin(); it != cend(); ++it) {
        ArgPointer arg = *it;
        longest = max(longest, arg->argumentLength());
    }

    for (auto it = cbegin(); it != cend(); ++it) {
        ArgPointer arg = *it;
        cout << arg->paddedArgument(longest) << " -- " << arg->helpText << endl;
    }
}
