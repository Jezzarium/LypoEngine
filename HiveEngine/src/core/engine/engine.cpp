//
// Created by Justin Abourjaili-Bilodeau on 8/19/24.
//
#include <filesystem>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "engine.h"

#include "core/logging/Logger.h"
#include "core/logging/LoggingFactory.h"
#include "core/engine/argument_parser.h"

namespace hive {

    Engine::Engine(const std::string &title, const uint32_t &width, const uint32_t &height,
                   const int& argc, char *argv[], const WindowFlags &flag) : argc(argc), argv(argv) {
        window = hive::Window::create(title, width, height, flag);

        try {
            init();
        } catch (const std::logic_error& e) {
            std::cerr << "Failed to initialize engine: " <<  e.what() << std::endl;
//            std::cerr.flush();
            std::exit(EXIT_FAILURE);
        }

        Logger::log("Engine has successfully initialized", LogLevel::Info);
        Logger::log("Currently running: " + std::filesystem::path(argv[0]).filename().string(), LogLevel::Info);
    }

    Engine::~Engine() {

    }

    void Engine::init() {

        ArgumentParser parser = ArgumentParser(argc, argv);
        auto debugArg = parser.addArgument("debug", 1, "d", "debug", "Enable debug mode");
        auto testArg = parser.addArgument("u", 0);
        parser.parseArguments();

        //Checking --debug or -d arguments
        if (parser.checkArgument(debugArg)) {
            Logger::setLogger(LoggingFactory::createLogger(LogOutputType::Console, LogLevel::Debug));
        } else {
            Logger::setLogger(LoggingFactory::createLogger(LogOutputType::Console, LogLevel::Info));
        }

        if (parser.checkArgument("u")) {
            Logger::log("Test argument 2 found", LogLevel::Debug);
        }
    }

    void Engine::mainLoop() {
        while (!window->shouldClose())
        {
            /* Render here TEMP */
            glClear(GL_COLOR_BUFFER_BIT);
            /* Poll for and process events */
            window->onUpdate();
        }
    }

    void Engine::run() {
        mainLoop();
    }

} // hive