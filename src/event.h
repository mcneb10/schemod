#ifndef _EVENT_H
#define _EVENT_H

#include <string>
#include "hook.h"
#include "s7/s7.h"
#include "SFML/Window/Event.hpp"

class Event {
    public:
        std::vector<Hook> hooks;
        std::string name;
        bool (*should_run_callback)(void);
        s7_pointer (*make_scheme_args)(s7_scheme*, sf::Event*);
        Event(std::string name, bool (*should_run_callback)(void) = nullptr, s7_pointer (*make_scheme_args)(s7_scheme*, sf::Event*) = nullptr);
        void add_hook(Hook hook);
        void run(s7_scheme* sc, sf::Event* sfml_event = nullptr);
};

#endif