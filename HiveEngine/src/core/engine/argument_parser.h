//
// Created by justi on 2024-09-08.
//

#ifndef SANDBOX_ARGUMENT_PARSER_H
#define SANDBOX_ARGUMENT_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace hive {

    class ArgumentParser {
    public:
        struct Argument {
            std::string name;
            int nArgs;
            std::string short_arg;
            std::string long_arg;
            std::string help;
        };

        ArgumentParser(int argc, char **argv, std::string prefixChar = "-",
                       bool addHelp = true, bool allowAbbrev = true,
                       std::string_view usage = "", std::string_view description = "",
                       std::string_view epilog = "");
        Argument addArgument(const std::string &name,
                             int nargs = 0,
                            const std::string& short_arg = "",
                            const std::string& long_arg = "",
                            std::string help = "");
        void parseArguments();
        bool checkArgument(const std::string &name);
        bool checkArgument(const Argument &arg);

    private:
        std::string helpMessage();

        int argc_;
        char **argv_;
        std::string prog_;
        std::string usage_;
        std::string description_;
        std::string epilog_;
        std::string prefix_char_;
        bool add_help_;
        bool allow_abbrev_;
        std::vector<Argument> arguments_;
        std::unordered_map<std::string, std::vector<std::string>> parsed_arguments_;
    };

} // hive

#endif // SANDBOX_ARGUMENT_PARSER_H
