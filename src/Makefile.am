bin_PROGRAMS = schemod


if USE_NLS
  schemod_CPPFLAGS = -DENABLE_NLS  -DLOCALEDIR='"../po"' # -DLOCALEDIR='"$(localedir)"'
else
    schemod_CPPFLAGS = -DLOCALEDIR
endif

schemod_CXXFLAGS = -std=c++11
schemod_SOURCES = game.cpp hook.cpp event.cpp main.cpp s7/s7.c
schemod_LDFLAGS = -lintl -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lfmt -lspdlog