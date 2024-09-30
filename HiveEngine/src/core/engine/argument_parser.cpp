//
// Created by justi on 2024-09-08.
//

#include "argument_parser.h"
#include <filesystem>
#include "core/logging/logger.h"

namespace hive {
    std::string get_program_name(char *argv0) {
        std::filesystem::path path = argv0;
        return path.filename().string();
    }

    ArgumentParser::ArgumentParser(int argc, char **argv, std::optional<std::string> prog,
                                   std::string_view usage, std::string_view description,
                                   std::string_view epilog,
                                   const std::vector<std::shared_ptr<ArgumentParser>> &parents,
                                   std::string_view prefixChars, bool addHelp, bool allowAbbrev)
            : argc_(argc), argv_(argv), prog_(prog ? *prog : get_program_name(argv[0])),
              usage_(usage), description_(description), epilog_(epilog),
              parents_(parents), prefix_chars_(prefixChars), add_help_(addHelp), allow_abbrev_(allowAbbrev){

        Logger::log("Program name: " + *prog_, LogLevel::Debug);
        Logger::log("There are " + std::to_string(argc_) + " command line arguments:", LogLevel::Debug);

        for (int i = 0; i < argc_; ++i) {
            Logger::log("Argument " + std::to_string(i + 1) + ": " + argv_[i], LogLevel::Debug);
        }
    }

    void ArgumentParser::addArgument(const std::string &name,
                                     std::optional<std::string> short_arg,
                                     std::optional<std::string> long_arg,
                                     std::string help,
                                     bool required,
                                     std::optional<std::string> default_value,
                                     bool is_flag,
                                     int nargs) {
        Argument arg{name, short_arg, long_arg, help, required, default_value, is_flag, nargs};
        arguments_.push_back(arg);
    }

    void ArgumentParser::parseArguments() {

    }

    bool ArgumentParser::checkArgument(const std::string &name) {
        if (parsed_arguments_.find(name) != parsed_arguments_.end()) {
            return true;
        }
    }
} // hive
