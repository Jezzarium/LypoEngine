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
        ArgumentParser(int argc, char **argv, std::optional<std::string> prog = std::nullopt,
                       std::string_view usage = "", std::string_view description = "",
                       std::string_view epilog = "",
                       const std::vector<std::shared_ptr<ArgumentParser>> &parents = {},
                       std::string_view prefixChars = "-",
                       bool addHelp = true, bool allowAbbrev = true);
        void addArgument();
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
    };

} // hive

#endif // SANDBOX_ARGUMENT_PARSER_H
