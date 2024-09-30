//
// Created by justi on 2024-09-08.
//

#include "argument_parser.h"
#include <filesystem>

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
              parents_(parents), prefix_chars_(prefixChars), add_help_(addHelp), allow_abbrev_(allowAbbrev) {}

    // DEBUG TEMP
    void ArgumentParser::printArguments() {
        std::cout << *prog_ << std::endl;

        std::cout << "There are " << argc_ << " command line arguments:" << std::endl;

        for (int i = 0; i < argc_; ++i) {
            std::cout << "Argument " << i + 1 << ": " << argv_[i] << std::endl;
        }
    }
} // hive
