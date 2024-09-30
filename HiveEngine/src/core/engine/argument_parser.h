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

    struct Argument {
        std::string name;
        std::optional<std::string> short_arg;
        std::optional<std::string> long_arg;
        std::string help;
        bool required;
        std::optional<std::string> default_value;
        bool is_flag;
        int nargs;
    };

    class ArgumentParser {
    public:
        ArgumentParser(int argc, char **argv, std::optional<std::string> prog = std::nullopt,
                       std::string_view usage = "", std::string_view description = "",
                       std::string_view epilog = "",
                       const std::vector<std::shared_ptr<ArgumentParser>> &parents = {},
                       std::string_view prefixChars = "-",
                       bool addHelp = true, bool allowAbbrev = true);
        void addArgument(const std::string &name,
                         std::optional<std::string> short_arg = std::nullopt,
                         std::optional<std::string> long_arg = std::nullopt,
                         std::string help = "",
                         bool required = false,
                         std::optional<std::string> default_value = std::nullopt,
                         bool is_flag = false,
                         int nargs = 1);
        void parseArguments();
        void printArguments();

    private:
        int argc_;
        char **argv_;
        std::optional<std::string> prog_;
        std::string usage_;
        std::string description_;
        std::string epilog_;
        std::vector<std::shared_ptr<ArgumentParser>> parents_;
        std::string prefix_chars_;
        bool add_help_;
        bool allow_abbrev_;
        std::vector<Argument> arguments_;
    };

} // hive

#endif // SANDBOX_ARGUMENT_PARSER_H
