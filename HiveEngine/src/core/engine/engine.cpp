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

#include "core/window/window_configuration.h"
#include "core/window/window_factory.h"
#include "core/inputs/input.h"
#include "core/window/window.h"

namespace hive {
    Engine::Engine(const int& argc, char *argv[]) : argc(argc), argv(argv) {
        try {
            init();
        } catch (const std::logic_error& e) {
            std::cerr << "Failed to initialize engine: " <<  e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        Logger::log("Engine has successfully initialized", LogLevel::Info);
        Logger::log("Currently running: " + std::filesystem::path(argv[0]).filename().string(), LogLevel::Info);
    }

    Engine::~Engine() {
        // Proper cleanup logic if necessary
    }

    void Engine::init() {
        // Window configuration logic moved to init()
        WindowConfiguration configuration;
        configuration.set(WindowConfigurationOptions::CURSOR_DISABLED, true);
        window = std::unique_ptr<Window>(WindowFactory::Create("Hive Engine", 800, 600, configuration));

        ArgumentParser parser = ArgumentParser(argc, argv);
        auto debugArg = parser.addArgument("debug", 1, "d", "debug", "Enable debug mode");
        auto testArg = parser.addArgument("u", 0);
        parser.parseArguments();

        // Checking --debug or -d arguments
        if (parser.checkArgument(debugArg)) {
            Logger::setLogger(LoggingFactory::createLogger(LogOutputType::Console, LogLevel::Debug));
        } else {
            Logger::setLogger(LoggingFactory::createLogger(LogOutputType::Console, LogLevel::Info));
        }

        if (parser.checkArgument("u")) {
            Logger::log("Test argument 2 found", LogLevel::Debug);
        }

        // Init Input
        hive::Input::init(window->getNativeWindow());
    }

    void Engine::mainLoop() {
        while (!window->shouldClose()) {
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
