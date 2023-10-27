#include "game.h"
#include "event.h"

Game::Game() {
  this->sc = s7_init();
  this->logger = spdlog::basic_logger_mt("schemod", "schemod.log");
  // TODO: set log level to debug only in debug builds
  this->logger->set_level(spdlog::level::debug);
  this->logger->flush_on(spdlog::level::warn);
}


Game::~Game() {
    s7_free(sc);
}

Event* Game::find_event(std::string name) {
  for (Event &event : events) {
    if (event.name == name) {
      return &event;
    }
  }
  logger->error("Event {} not found", name);
  return nullptr;
}

bool Game::run_event(std::string name, sf::Event* sfml_event, bool silent) {
  Event* e = find_event(name);
  if(e == nullptr) {
    logger->error("failed to run {}", name);
    return false;
  } else {
    e->run(this->sc, sfml_event);
    if(!silent) logger->debug("event {} fired", name);
    return true;
  }
}

void Game::add_event(Event event) { 
  for (Event &e : events) {
    if (e.name == event.name) {
      logger->error("Event {} already exists", event.name);
      return;
    }
  }
  events.push_back(event);
}

bool Game::add_hook(std::string event, void *func, bool is_scheme) {
  Event* e = find_event(event);
  if(e == nullptr) {
    logger->error("failed to add hook to {}", event);
    return false;
  } else {
    e->add_hook(Hook(func, is_scheme));
    return true;
  }
}

void Game::check_if_should_run_events(void) {
  for (Event event : events) {
    if (event.should_run_callback != nullptr) {
      if (event.should_run_callback()) {
        event.run(sc);
      }
    } else {
      event.run(sc);
    }
  }
}