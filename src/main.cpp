#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "event.h"
#include "game.h"
#include "s7/s7.h"
#include "schemod_inc.h"
#include <string>
#include <iostream>

static Game game;
static sf::RenderWindow window(sf::VideoMode(640, 480), "Schemod");

static s7_pointer scm_gettext(s7_scheme *sc, s7_pointer args) {
  if (s7_is_string(s7_car(args))) {
    return s7_make_string(sc, gettext(s7_string(s7_car(args))));
  } // else {
  return s7_wrong_type_arg_error(sc, __func__, 1, s7_car(args), "a string");
  //}
}

static s7_pointer scm_textdomain(s7_scheme *sc, s7_pointer args) {
  if (s7_is_string(s7_car(args))) {
    return s7_make_string(sc, textdomain(s7_string(s7_car(args))));
  } // else {
  return s7_wrong_type_arg_error(sc, __func__, 1, s7_car(args), "a string");
  //}
}

static s7_pointer call_event(s7_scheme *sc, s7_pointer args) {
  if (s7_is_string(s7_car(args))) {
    return s7_make_integer(
        sc, game.run_event(std::string(s7_string(s7_car(args)))));
  }
  return s7_wrong_type_arg_error(sc, __func__, 1, s7_car(args), "event name");
}

static s7_pointer add_event_hook(s7_scheme *sc, s7_pointer args) {
  // s7_display(sc, s7_cadr(args), s7_current_output_port(sc));
  // printf("\nis pair: %i\n", s7_is_function(s7_cadr(args)));
  if (s7_is_string(s7_car(args))) {
    if (s7_is_pair(s7_cdr(args))) {
      s7_pointer hook = s7_cadr(args);
      s7_gc_protect(sc, hook);
      const char *name = s7_string(s7_car(args));
      return s7_make_integer(sc, game.add_hook(std::string(name), hook, true));
    }
    return s7_wrong_type_arg_error(sc, __func__, 2, s7_cdr(args), "a function");
  } // else {
  return s7_wrong_type_arg_error(sc, __func__, 2, s7_car(args), "a string");
  //}
}

static s7_pointer log_info(s7_scheme *sc, s7_pointer args) {
  if (s7_is_string(s7_car(args))) {
    // because of variadic stuff, we cant use the formatting facilities built
    // into spdlog we have to use the (slightly inferior) scheme format function
    // instead
    game.logger->info("[scheme] "+std::string(
        s7_format(sc, s7_cons(sc, s7_make_boolean(sc, false), args))));
    return s7_list(sc, 0);
  } else {
    return s7_wrong_type_arg_error(sc, __func__, 1, s7_car(args),
                                   "first argument must be format");
  }
}

static s7_pointer log_warn(s7_scheme *sc, s7_pointer args) {
  if (s7_is_string(s7_car(args))) {
    // because of variadic stuff, we cant use the formatting facilities built
    // into spdlog we have to use the (slightly inferior) scheme format function
    // instead
    game.logger->warn("[scheme] "+std::string(
        s7_format(sc, s7_cons(sc, s7_make_boolean(sc, false), args))));
    return s7_list(sc, 0);
  } else {
    return s7_wrong_type_arg_error(sc, __func__, 1, s7_car(args),
                                   "first argument must be format");
  }
}

static s7_pointer log_error(s7_scheme *sc, s7_pointer args) {
  if (s7_is_string(s7_car(args))) {
    // because of variadic stuff, we cant use the formatting facilities built
    // into spdlog we have to use the (slightly inferior) scheme format function
    // instead
    game.logger->error("[scheme] "+std::string(
        s7_format(sc, s7_cons(sc, s7_make_boolean(sc, false), args))));
    return s7_list(sc, 0);
  } else {
    return s7_wrong_type_arg_error(sc, __func__, 1, s7_car(args),
                                   "first argument must be format");
  }
}

static s7_pointer log_debug(s7_scheme *sc, s7_pointer args) {
  if (s7_is_string(s7_car(args))) {
    // because of variadic stuff, we cant use the formatting facilities built
    // into spdlog we have to use the (slightly inferior) scheme format function
    // instead
    game.logger->debug("[scheme] "+std::string(
        s7_format(sc, s7_cons(sc, s7_make_boolean(sc, false), args))));
    return s7_list(sc, 0);
  } else {
    return s7_wrong_type_arg_error(sc, __func__, 1, s7_car(args),
                                   "first argument must be format");
  }
}

static s7_pointer log_critical(s7_scheme *sc, s7_pointer args) {
  if (s7_is_string(s7_car(args))) {
    // because of variadic stuff, we cant use the formatting facilities built
    // into spdlog we have to use the (slightly inferior) scheme format function
    // instead
    game.logger->critical("[scheme] "+std::string(
        s7_format(sc, s7_cons(sc, s7_make_boolean(sc, false), args))));
    return s7_list(sc, 0);
  } else {
    return s7_wrong_type_arg_error(sc, __func__, 1, s7_car(args),
                                   "first argument must be format");
  }
}

static s7_pointer log_flush(s7_scheme *sc, s7_pointer args) {
    game.logger->flush();
    return s7_list(sc, 0);
}

static const char *key_code_names[] = {
    "Unknown",   "A",         "B",        "C",        "D",          "E",
    "F",         "G",         "H",        "I",        "J",          "K",
    "L",         "M",         "N",        "O",        "P",          "Q",
    "R",         "S",         "T",        "U",        "V",          "W",
    "X",         "Y",         "Z",        "Num0",     "Num1",       "Num2",
    "Num3",      "Num4",      "Num5",     "Num6",     "Num7",       "Num8",
    "Num9",      "Escape",    "LControl", "LShift",   "LAlt",       "LSystem",
    "RControl",  "RShift",    "RAlt",     "RSystem",  "Menu",       "LBracket",
    "RBracket",  "Semicolon", "Comma",    "Period",   "Apostrophe", "Slash",
    "Backslash", "Grave",     "Equal",    "Hyphen",   "Space",      "Enter",
    "Backspace", "Tab",       "PageUp",   "PageDown", "End",        "Home",
    "Insert",    "Delete",    "Add",      "Subtract", "Multiply",   "Divide",
    "Left",      "Right",     "Up",       "Down",     "Numpad0",    "Numpad1",
    "Numpad2",   "Numpad3",   "Numpad4",  "Numpad5",  "Numpad6",    "Numpad7",
    "Numpad8",   "Numpad9",   "F1",       "F2",       "F3",         "F4",
    "F5",        "F6",        "F7",       "F8",       "F9",         "F10",
    "F11",       "F12",       "F13",      "F14",      "F15",        "Pause",
    "KeyCount"};

static void make_sfml_key_values(s7_scheme *sc) {

  std::string name, help;

  for (int i = 0; i < sf::Keyboard::Key::KeyCount + 1; i++) {
    name = "key-";
    help = "sf::Keyboard::Key::";
    name += key_code_names[i];
    help += key_code_names[i];
    s7_define_variable_with_documentation(
        sc, name.c_str(), s7_make_integer(sc, i - 1), help.c_str());
  }
}

void gettext_init(s7_scheme *sc) {
  setlocale(LC_ALL, "es");
  setenv("LANGUAGE", "es", 1);
  // bindtextdomain("main", "/usr/share/locale/");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  s7_define_function(sc, "_", scm_gettext, 1, 0, false, "gettext in scheme");
  s7_define_function(sc, "textdomain", scm_textdomain, 1, 0, false,
                     "set the text domain/catalog");

  // s7_load(sc, "./gettext.scm");
}

static int font_type_tag, texture_type_tag, render_states_type_tag, vertex_buffer_type_tag, drawable_type_tag, text_type_tag;

int main(int argc, char **argv) {

  game.logger->info("schemod starting...");

  game.logger->info("Initializing gettext...");
  gettext_init(game.sc);

  game.logger->info("Setting up events...");
  s7_define_function(game.sc, "call-event", call_event, 1, 0, false,
                     "call in game event");
  s7_define_function(game.sc, "add-event-hook", add_event_hook, 2, 0, false,
                     "add hook to in game event");

  // Logging functions
  s7_define_function(game.sc, "info", log_info, 1, 0, true,
                     "log a lisp formatted string at the info level");
  s7_define_function(game.sc, "debug", log_debug, 1, 0, true,
                     "log a lisp formatted string at the debug level");
  s7_define_function(game.sc, "warn", log_warn, 1, 0, true,
                     "log a lisp formatted string at the warning level");
  s7_define_function(game.sc, "error", log_error, 1, 0, true,
                     "log a lisp formatted string at the error level");
  s7_define_function(game.sc, "critical", log_critical, 1, 0, true,
                     "log a lisp formatted string at the critical level");
  s7_define_function(game.sc, "log-flush", log_flush, 0, 0, true,
                     "flush the log");

  // Set up required functions to make a basic game

  // TODO: GC?
  font_type_tag = s7_make_c_type(game.sc, "Font");

  s7_define_function(
      game.sc, "load-font-from-file",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if (s7_is_string(s7_car(args))) {
          sf::Font* font = new sf::Font();
          if (font->loadFromFile(s7_string(s7_car(args)))) {
            game.logger->debug("[scheme] loaded font from {}",
                               s7_string(s7_car(args)));
            return s7_make_c_object(sc, font_type_tag, font);
          } else {
            game.logger->error("[scheme] could not load font from {}",
                               s7_string(s7_car(args)));
            return s7_list(sc, 0);
          }
        } else {
          return s7_wrong_type_arg_error(sc, "load-font-from-file", 1,
                                         s7_car(args), "a string");
        }
      },
      1, 0, true, "return a font loaded from a file (font-path)");

  // TODO: other methods?

  texture_type_tag = s7_make_c_type(game.sc, "Texture");

  s7_define_function(
      game.sc, "load-texture-from-file",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if (s7_is_string(s7_car(args))) {
          sf::Texture* Texture = new sf::Texture;
          if (Texture->loadFromFile(s7_string(s7_car(args)))) {
            game.logger->debug("[scheme] loaded texture from {}",
                             s7_string(s7_car(args)));
            return s7_make_c_object(sc, texture_type_tag, Texture);
          } // else {
          game.logger->error("[scheme] could not load texture from {}",
                             s7_string(s7_car(args)));
          return s7_list(sc, 0);
          //}
        } else {
          return s7_wrong_type_arg_error(sc, "load-texture-from-file", 1,
                                         s7_car(args), "a string");
        }
      },
      1, 0, true, "return a texture loaded from a file (texture-path)");

  render_states_type_tag = s7_make_c_type(game.sc, "RenderStates");
  vertex_buffer_type_tag = s7_make_c_type(game.sc, "VertexBuffer");
  drawable_type_tag = s7_make_c_type(game.sc, "Drawable");
  text_type_tag = s7_make_c_type(game.sc, "Text");
// TODO: gcs
  s7_define_function(
      game.sc, "make-renderstates-with-texture",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if (s7_is_c_object(s7_car(args)) &&
            s7_c_object_type(s7_car(args)) == texture_type_tag) {
          sf::RenderStates* render_states = new sf::RenderStates(
              (sf::Texture *)s7_c_object_value(s7_car(args)));
          return s7_make_c_object(sc, render_states_type_tag, render_states);
        } else {
          return s7_wrong_type_arg_error(sc, "make-renderstates-with-texture",
                                         1, s7_car(args), "a texture");
        }
      },
      1, 0, false, "make a RenderStates object from Texture (texture)");

  s7_define_function(
      game.sc, "window-set-size",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if (s7_is_integer(s7_car(args)) && s7_integer(s7_car(args)) > 0) {
          if (s7_is_integer(s7_cadr(args)) && s7_integer(s7_cadr(args)) > 0) {
            window.setSize(sf::Vector2u(s7_integer(s7_car(args)),
                                        s7_integer(s7_cadr(args))));
            game.logger->info("[scheme] window size set to {} by {}",
                              s7_integer(s7_car(args)),
                              s7_integer(s7_cadr(args)));
            return s7_list(sc, 0);
          }
          return s7_wrong_type_arg_error(sc, "window-set-size", 2,
                                         s7_cadr(args), "an unsigned integer");
        }
        return s7_wrong_type_arg_error(sc, "window-set-size", 1, s7_car(args),
                                       "an unsigned integer");
      },
      2, 0, false, "set the size of the window (x y)");

  s7_define_function(
      game.sc, "window-set-title",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if (s7_is_string(s7_car(args))) {
          window.setTitle(s7_string(s7_car(args)));
          return s7_list(sc, 0);
        }
        return s7_wrong_type_arg_error(sc, "window-set-title", 1, s7_car(args),
                                       "a string");
      },
      1, 0, false, "set the title of the window (title)");

  s7_define_function(
      game.sc, "window-set-framerate-limit",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if (s7_is_integer(s7_car(args))) {
          window.setFramerateLimit(s7_integer(s7_car(args)));
          return s7_list(sc, 0);
        }
        return s7_wrong_type_arg_error(sc, "window-set-title", 1, s7_car(args),
                                       "an integer");
      },
      1, 0, false, "limit the frame rate of the window (framerate)");

  s7_define_function(
      game.sc, "window-clear",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        window.clear();
        return s7_list(sc, 0);
      },
      0, 0, false, "clear the window ()");
  s7_define_function(
      game.sc, "window-display",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        window.display();
        return s7_list(sc, 0);
      },
      0, 0, false, "update the window ()");
  s7_define_function(
      game.sc, "window-draw",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if (s7_is_c_object(s7_car(args))) {
          if (s7_c_object_type(s7_car(args)) == vertex_buffer_type_tag) {
            if (s7_is_c_object(s7_cadr(args)) &&
                s7_c_object_type(s7_cadr(args)) == render_states_type_tag) {
              window.draw(
                  *(sf::VertexBuffer *)s7_c_object_value(s7_car(args)),
                  *(sf::RenderStates *)s7_c_object_value(s7_cadr(args)));
            } else {
              window.draw(*(sf::VertexBuffer *)s7_c_object_value(s7_car(args)));
            }
            return s7_list(sc, 0);
          } else /*if (s7_c_object_type(s7_car(args)) == drawable_type_tag) */{
            if (s7_is_c_object(s7_cadr(args)) &&
                s7_c_object_type(s7_cadr(args)) == render_states_type_tag) {
              window.draw(
                  *(sf::Drawable *)s7_c_object_value(s7_car(args)),
                  *(sf::RenderStates *)s7_c_object_value(s7_cadr(args)));
            } else {
              window.draw(*(sf::Drawable *)s7_c_object_value(s7_car(args)));
            }
            return s7_list(sc, 0);
          } /*else {
            return s7_wrong_type_arg_error(game.sc, "window-draw", 1,
                                           s7_car(args),
                                           "a drawable or vertex buffer");
          }*/
        }
        return s7_wrong_type_arg_error(game.sc, "window-draw", 1, s7_car(args),
                                       "a drawable or vertex buffer");
      },
      1, 1, false,
      "draw to the screen (drawable-or-vertex-buffer renderstates-optional)");

  s7_define_function(
      game.sc, "is-key-pressed",
      [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if (s7_is_integer(s7_car(args))) {
          return s7_make_boolean(
              sc, sf::Keyboard::isKeyPressed(
                      (sf::Keyboard::Key)s7_integer(s7_car(args))));
        }
        return s7_wrong_type_arg_error(game.sc, "is-key-pressed", 1,
                                       s7_car(args), "an integer");
      },
      1, 0, false, "returns true if key is pressed (key)");


    s7_define_function(game.sc, "make-text", [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        // TODO: other parameters
        if(s7_is_string(s7_car(args)) && s7_c_object_type(s7_cadr(args)) == font_type_tag) {
            if(s7_is_integer(s7_caddr(args))) {
                sf::Text* text = new sf::Text(std::string(s7_string(s7_car(args))), *(sf::Font*)s7_c_object_value(s7_cadr(args)), s7_integer(s7_caddr(args)));
                return s7_make_c_object(sc, text_type_tag, text);
            }
            sf::Text* text = new sf::Text(std::string(s7_string(s7_car(args))), *(sf::Font*)s7_c_object_value(s7_cadr(args)));
            return s7_make_c_object(sc, text_type_tag, text);
        }
        sf::Text* text;
        return s7_make_c_object(sc, text_type_tag, text);
    }, 0, 3, false, "make text (text font size)");


    // TODO: the other ones
    s7_define_function(game.sc, "set-text", [](s7_scheme *sc, s7_pointer args) -> s7_pointer {
        if(s7_is_c_object(s7_car(args)) && s7_c_object_type(s7_car(args)) == text_type_tag) {
            if(s7_is_string(s7_cadr(args))) {
                ((sf::Text*)s7_c_object_value(s7_car(args)))->setString(s7_string(s7_cadr(args)));
                return s7_list(sc, 0);
            }
            return s7_wrong_type_arg_error(game.sc, "set-text", 2, s7_cadr(args), "a string");
        }
        return s7_wrong_type_arg_error(game.sc, "set-text", 1, s7_car(args), "a Text object");
    }, 2, 0, false, "set the text of a Text object (Text text)");

  make_sfml_key_values(game.sc);

  // TODO: null checking?

  // C++ callbacks MUST have the prototype "void (sf::Event*);"
  // Scheme callback parameters are event specific

  // Parameters: none
  game.add_event(Event("closed"));
  // Parameters: (width, height)
  game.add_event(Event("resized", nullptr,
                       [](s7_scheme *sc, sf::Event *sfml_event) -> s7_pointer {
                         return s7_list(
                             sc, 2, s7_make_integer(sc, sfml_event->size.width),
                             s7_make_integer(sc, sfml_event->size.height));
                       }));
  // Parameters: none
  game.add_event(Event("lost_focus"));
  game.add_event(Event("gained_focus"));
  // Don't use for movement or anything that needs to be smooth
  // Parameters: (key)
  game.add_event(Event(
      "text_entered", nullptr,
      [](s7_scheme *sc, sf::Event *sfml_event) -> s7_pointer {
        return s7_list(
            sc, 1,
            s7_make_character(sc, static_cast<char>(sfml_event->text.unicode)));
      }));
  // Parameters: (is-vertical-wheel delta x y)
  game.add_event(Event(
      "mousewheel_scrolled", nullptr,
      [](s7_scheme *sc, sf::Event *sfml_event) -> s7_pointer {
        return s7_list(sc, 4,
                       s7_make_boolean(sc, sfml_event->mouseWheelScroll.wheel ==
                                               sf::Mouse::VerticalWheel),
                       s7_make_real(sc, sfml_event->mouseWheelScroll.delta),
                       s7_make_integer(sc, sfml_event->mouseWheelScroll.x),
                       s7_make_integer(sc, sfml_event->mouseWheelScroll.y));
      }));
  // Parameters: (button-enum x y)
  game.add_event(Event(
      "mousebutton_pressed", nullptr,
      [](s7_scheme *sc, sf::Event *sfml_event) -> s7_pointer {
        return s7_list(sc, 3,
                       s7_make_integer(sc, sfml_event->mouseButton.button),
                       s7_make_integer(sc, sfml_event->mouseButton.x),
                       s7_make_integer(sc, sfml_event->mouseButton.y));
      }));
  game.add_event(Event(
      "mousebutton_released", nullptr,
      [](s7_scheme *sc, sf::Event *sfml_event) -> s7_pointer {
        return s7_list(sc, 3,
                       s7_make_integer(sc, sfml_event->mouseButton.button),
                       s7_make_integer(sc, sfml_event->mouseButton.x),
                       s7_make_integer(sc, sfml_event->mouseButton.y));
      }));
  // Parameters: (x y)
  game.add_event(Event(
      "mouse_moved", nullptr,
      [](s7_scheme *sc, sf::Event *sfml_event) -> s7_pointer {
        return s7_list(sc, 2, s7_make_integer(sc, sfml_event->mouseMove.x),
                       s7_make_integer(sc, sfml_event->mouseMove.y));
      }));
  // Parameters: none
  game.add_event(Event("mouse_entered"));
  game.add_event(Event("mouse_left"));
  game.add_event(Event("loop"));

  game.logger->info("Loading game module(s)...");
  s7_load(game.sc, "./hello.scm");

  game.logger->info("Entering main game loop...");
  while (window.isOpen()) {
    sf::Event event;
    game.run_event("loop", nullptr, true);
    while (window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
        game.run_event("closed");
        window.close();
        break;
      case sf::Event::TextEntered:
        game.run_event("text_entered", &event);
        break;
      case sf::Event::Resized:
        game.run_event("resized", &event);
        break;
      case sf::Event::LostFocus:
        game.run_event("lost_focus", &event);
        break;
      case sf::Event::GainedFocus:
        game.run_event("gained_focus", &event);
        break;
      // TODO?: key events
      case sf::Event::MouseWheelScrolled:
        game.run_event("mousewheel_scrolled", &event);
        break;
      case sf::Event::MouseButtonPressed:
        game.run_event("mousebutton_pressed", &event);
        break;
      case sf::Event::MouseButtonReleased:
        game.run_event("mousebutton_released", &event);
        break;
      case sf::Event::MouseMoved:
        game.run_event("mouse_moved", &event);
        break;
      case sf::Event::MouseEntered:
        game.run_event("mouse_entered");
        break;
      case sf::Event::MouseLeft:
        game.run_event("mouse_left");
        break;
      // TODO?: joystick events
      default:
        break;
      }
    }
  }
}