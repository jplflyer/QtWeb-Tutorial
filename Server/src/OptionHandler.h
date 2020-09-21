#ifndef SRC_VITAC_OPTIONHANDLER_H_
#define SRC_VITAC_OPTIONHANDLER_H_

#include <string>
#include <getopt.h>
#include <functional>

#include "PointerVector.h"

/**
 * This makes command line option parsing easy.
 *
 * Example:
 *
 *    OptionHandler::ArgumentVector vec;
 *    vec.addNoArg("docker",     [](const char *)     { environment = "docker"; }, "", "(default) Docker instance on your Desktop");
 *    vec.addArg  ("program-id", [&](const char *val) { skipCreate = true; program->id = atoi(val); }, "program-id", "ID of an existing program.");
 *
 *    OptionHandler::handleOptions(argc, argv, vec);
 *
 * We'll automatically add a --help argument and out-generate help text from the
 * last two arguments to the various add methods.
 */
class OptionHandler {
public:
    /** Each option that's being parsed requires a callback/lambda. This is the signature. */
	typedef std::function<void(const char *)> CallbackFunction;

    /** You can specify a help function with this signature. */
	typedef std::function<void()> HelpFunction;

	/**
	 * This is equivalent to struct option from getopt.h, but allows
	 * passing in the lambda.
	 */
    class Argument {
    public:
		/** name of long option */
        std::string name;

		/**
		 * one of no_argument, required_argument, and optional_argument:
		 * whether option takes an argument
		 */
		int has_arg;

		/** if flag not NULL, value to set *flag to; else return value */
		char val;

		/** What to call. */
		CallbackFunction callbackFunction;

        /** This is for automated help. It is the text to display for the 'arguments' portion of the help. */
        std::string argumentText;

        /** This is for automated help. This is an explanation of the argument. */
        std::string helpText;

        // Methods.
        Argument();
        Argument(const char *_name, int _has_arg, char _val, CallbackFunction cb, const std::string & argText = "", const std::string & help = "");
        Argument(const Argument &arg);

        int argumentLength() const;
        std::string argumentForHelp() const;
        std::string paddedArgument(int) const;
    };

    /** Type of pointer. */
    typedef std::shared_ptr<Argument> ArgPointer;

    /**
     * A vector of these guys. We always own this data.
     */
    class ArgumentVector: public std::vector<ArgPointer> {
    private:
        struct option * options;
        unsigned long optionsCount;
        std::string shortArgs;

    public:
        ArgumentVector();
        ArgumentVector(Argument *arguments);
        ArgumentVector(ArgumentVector &vec);
        virtual ~ArgumentVector();

        ArgPointer add     (const char *_name, int _has_arg, char _val, CallbackFunction cb, const std::string & argText = "", const std::string & help = "");
        ArgPointer add     (const char *_name, int _has_arg, CallbackFunction cb, const std::string & argText = "", const std::string & help = "");
        ArgPointer addArg  (const char *_name, CallbackFunction cb, const std::string & argText = "", const std::string & help = "");
        ArgPointer addNoArg(const char *_name, CallbackFunction cb, const std::string & help = "");

        void addAll(Argument *arguments);
        void addAll(ArgumentVector &vec);

        struct option * getOptions(std::string &shortArgsReturned);

        ArgPointer searchForShortCode(char);
        ArgPointer searchForLongName(const std::string &);
        bool anyHaveHelpText() const;

        void autoHelp() const;
    };


public:
    static bool handleOptions(int argc, const char * const *argv, Argument * arguments, HelpFunction helpFunction = nullptr);
    static bool handleOptions(int argc, const char * const *argv, ArgumentVector & arguments, HelpFunction helpFunction = nullptr);

};

#endif /* SRC_VITAC_OPTIONHANDLER_H_ */
