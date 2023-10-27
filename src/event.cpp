#include "event.h"

Event::Event(std::string name, bool (*should_run_callback)(void), s7_pointer (*make_scheme_args)(s7_scheme*, sf::Event*)) {
    this->name = name;
    this->should_run_callback = should_run_callback;
    this->make_scheme_args = make_scheme_args;
}

void Event::add_hook(Hook hook) {
    hooks.push_back(hook);
}

void Event::run(s7_scheme* sc, sf::Event* sfml_event) {
    for (Hook hook : hooks) {
      if(hook.scheme) {
        if(this->make_scheme_args == nullptr) {
          s7_call(sc, (s7_pointer)hook.func, s7_list(sc, 0));
        } else {
          s7_call(sc, (s7_pointer)hook.func, make_scheme_args(sc, sfml_event));
        }
      } else {
        void (*callback)(sf::Event*) =
            reinterpret_cast<void (*)(sf::Event*)>(hook.func);
        callback(sfml_event);
      }
    }
}