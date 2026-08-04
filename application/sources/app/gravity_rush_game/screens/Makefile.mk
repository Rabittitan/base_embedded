CFLAGS		+= -I./sources/app/gravity_rush_game/screens
CPPFLAGS	+= -I./sources/app/gravity_rush_game/screens
VPATH += sources/app/gravity_rush_game/screens


SOURCES_CPP += sources/app/gravity_rush_game/screens/startup_screen.cpp												
SOURCES_CPP += sources/app/gravity_rush_game/screens/menu_screen.cpp
SOURCES_CPP += sources/app/gravity_rush_game/screens/game_over_screen.cpp
SOURCES_CPP += sources/app/gravity_rush_game/screens/game_screen.cpp
SOURCES_CPP += sources/app/gravity_rush_game/screens/game_victory_screen.cpp
SOURCES_CPP += sources/app/gravity_rush_game/screens/setting_screen.cpp