#ifndef _GAME_H
#define _GAME_H
#include "event.h"
#include "schemod_inc.h"
#include <string>
#include <map>
#include "hook.h"

class Game {
    private:
        std::vector<Event> events;
    public:
        s7_scheme* sc;
        std::shared_ptr<spdlog::logger> logger;
        Event* find_event(std::string name);
        bool run_event(std::string name, sf::Event* sfml_event = nullptr, bool silent = false);
        void add_event(Event event);
        bool add_hook(std::string event, void* func, bool is_scheme = false);
        void check_if_should_run_events(void);
        Game();
        ~Game();
};

#endif