//
// Created by justi on 2024-09-08.
//

#include "argument_parser.h"
#include <utility>
#include <filesystem>

namespace hive {
    ArgumentParser::ArgumentParser(int argc, char **argv,
                                   std::string prefixChar,
                                   bool addHelp,
                                   bool allowAbbrev,
                                   std::string_view usage,
                                   std::string_view description,
                                   std::string_view epilog)
            : argc_(argc), argv_(argv), prefix_char_(prefixChar), add_help_(addHelp),
              allow_abbrev_(allowAbbrev), usage_(usage), description_(description), epilog_(epilog) {

        if(prefixChar.length() != 1) {
            throw std::invalid_argument("Prefix character must be a single character (" + prog_ + ").");
        }

        prog_ = std::filesystem::path(argv_[0]).filename().string();

        //debug (cannot use Logger because it is not initialized yet)
//        std::cout << "There are " + std::to_string(argc_) + " command line arguments:" << std::endl;
//        for (int i = 0; i < argc_; ++i) {
//            std::cout << ("Argument " + std::to_string(i + 1) + ": " + argv_[i]) << std::endl;
//        }
    }

    ArgumentParser::Argument ArgumentParser::addArgument(const std::string &name, int nargs, const std::string& short_arg,
                                                         const std::string& long_arg, std::string help) {
        std::string actual_short_arg = short_arg.empty() ? std::string(1, name[0]) : short_arg;
        std::string actual_long_arg = long_arg.empty() ? name : long_arg;

        for (const auto& arg : arguments_) {
            if (arg.name == name) {
                throw std::invalid_argument("Cannot add duplicate argument '" + name + "' (parser of " + prog_ + ").");
            }
            if (arg.short_arg == actual_short_arg) {
                throw std::invalid_argument("Cannot add duplicate short argument '" + actual_short_arg + "' to parser of " + prog_ + ".");
            }
            if (arg.long_arg == actual_long_arg) {
                throw std::invalid_argument("Cannot add duplicate long argument '" + actual_long_arg + "' to parser of " + prog_ + ".");
            }
        }
        Argument arg{name,nargs,actual_short_arg,actual_long_arg,std::move(help)};
        arguments_.push_back(arg);

        return arg;
    }

    void ArgumentParser::parseArguments() {
        enum class ParseState {
            Start,
            Argument,
            Value
        };

        ParseState currentState = ParseState::Start;
        int argIndex = 1;
        Argument previousArg;
        bool foundArg = false;


        while (argIndex < argc_) {
            switch (currentState) {
                case ParseState::Start:
                    if (argv_[argIndex][0] == prefix_char_[0]) {
                        currentState = ParseState::Argument;
                    } else {
                        currentState = ParseState::Value;
                    }
                    break;
                case ParseState::Argument:
                    for(const Argument& arg : arguments_) {
                        if (std::string(argv_[argIndex]) == "-" + arg.short_arg or std::string(argv_[argIndex]) == "--" + arg.long_arg) {
                            parsed_arguments_.emplace(arg.name, std::vector<std::string>{});
                            previousArg = arg;
                            foundArg = true;
                        }
                    }
                    if (!foundArg) {
                        std::string currentArg = std::string(argv_[argIndex]);
                        if (currentArg[0] == '-' && currentArg[1] != '-' && currentArg.length() > 2) {
                            throw std::invalid_argument("Invalid short argument: " + currentArg + " (" + prog_ + "). Short arguments must be a single character after a single '-'.");
                        }
                        throw std::invalid_argument("Unrecognized argument: " + currentArg + " (" + prog_ + ").");
                    }
                    foundArg = false;
                    currentState = ParseState::Start;
                    ++argIndex;
                    break;
                case ParseState::Value:
                    if (previousArg.name.empty()) {
                        throw std::invalid_argument("Value provided without an argument (" + prog_ + ")." );
                    }

                    if (previousArg.nArgs == 0) {
                        throw std::out_of_range("Argument -" + previousArg.name + " does not take any values (" + prog_ + ").");
                    }

                    if (parsed_arguments_[previousArg.name].size() >= previousArg.nArgs) {
                        throw std::out_of_range("Too many arguments provided for -" + previousArg.name + " (" + prog_ + ").");
                    }

                    parsed_arguments_[previousArg.name].emplace_back(argv_[argIndex]);
                    currentState = ParseState::Start;
                    ++argIndex;
                    break;
            }
        }
    }

    bool ArgumentParser::checkArgument(const std::string &name) {
        if (parsed_arguments_.find(name) != parsed_arguments_.end()) {
            return true;
        }
        return false;
    }

    bool ArgumentParser::checkArgument(const ArgumentParser::Argument &arg) {
        if (parsed_arguments_.find(arg.name) != parsed_arguments_.end()) {
            return true;
        }
        return false;
    }

    std::string ArgumentParser::helpMessage() {
        std::string help = "Usage: " + prog_ + " " + usage_ + "\n";
        help += description_ + "\n";
        help += "Arguments:\n";
        for (const auto& arg : arguments_) {
            help += "  " + arg.short_arg + ", " + arg.long_arg + " " + arg.help + "\n";
        }
        help += epilog_;
        return help;
    }

} // hive
