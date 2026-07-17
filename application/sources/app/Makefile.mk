include sources/app/screens/Makefile.mk

CFLAGS		+= -I./sources/app
CPPFLAGS	+= -I./sources/app

VPATH += sources/app

# CPP source files
SOURCES_CPP += sources/app/task_system.cpp
SOURCES_CPP += sources/app/app.cpp
SOURCES_CPP += sources/app/app_data.cpp
SOURCES_CPP += sources/app/app_non_clear_ram.cpp
SOURCES_CPP += sources/app/app_bsp.cpp
SOURCES_CPP += sources/app/shell.cpp

SOURCES_CPP += sources/app/task_shell.cpp
SOURCES_CPP += sources/app/task_life.cpp
SOURCES_CPP += sources/app/task_game.cpp
SOURCES_CPP += sources/app/task_list.cpp
# GAME_MINIMAL_DISABLED: SOURCES_CPP += sources/app/task_display.cpp



SOURCES_CPP += sources/app/task_dbg.cpp
