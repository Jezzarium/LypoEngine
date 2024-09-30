//
// Created by Justin Abourjaili-Bilodeau on 8/19/24.
//

#ifndef LYPO_ENGINE_ENGINE_H
#define LYPO_ENGINE_ENGINE_H

#include "core/window.h"
#include "core/events/event_bus.h"
#include "argument_parser.h"


namespace hive {

    class Engine {
    public:
        Engine(const std::string &title, const uint32_t &width, const uint32_t &height,
               const WindowFlags &flag = WindowFlags::DEFAULT);
        ~Engine();

        void run();

    private:
        void init();
        void mainLoop();

        std::unique_ptr<Window> window;
        const int argc = 0;
        const char *argv[];
    };

} // hive

#endif //LYPO_ENGINE_ENGINE_H
