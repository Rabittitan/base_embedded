include sources/app/gravity_rush_game/assets/Makefile.mk
include sources/app/gravity_rush_game/framework/Makefile.mk
include sources/app/gravity_rush_game/objects/Makefile.mk
include sources/app/gravity_rush_game/screens/Makefile.mk


CFLAGS		+= -I./sources/app/gravity_rush_game
CPPFLAGS	+= -I./sources/app/gravity_rush_game

VPATH += sources/app/gravity_rush_game