CC  = lcc
GGBGFX = ../node_modules/.bin/ggbgfx

# sprite conversion steps
#   1. change white to red
#   2. change light to white
#   3. change dark to white
#   4. change red to dark

all: \
	gumdrop.sprite.o \
	rolly.sprite.o \
	ninja.sprite.o \
	game.tileset.o \
	background.game.map.o

game.tileset.png: ../images/background.png

.SUFFIXES:

#
# building tilesets
#

%.tileset.png:
	$(GGBGFX) tileset -o $@ $^

%.tileset.c: %.tileset.png
	$(GGBGFX) tiledata --name $*_tileset -o $@ $<

%.tileset.h: %.tileset.c
	echo "#ifndef __$*_tileset_h__" > $@
	echo "#define __$*_tileset_h__" >> $@
	echo "extern unsigned char $*_tileset[];" >> $@
	echo "#endif // __$*_tileset_h__" >> $@

#
# building maps
#


%.game.map.c: ../images/%.png game.tileset.png
	$(GGBGFX) tilemap --name $*_game_map -o $@ $^

%.game.map.h: %.game.map.c
	echo "#ifndef __$*_game_map_h__" > $@
	echo "#define __$*_game_map_h__" >> $@
	echo "extern unsigned char $*_game_map[];" >> $@
	echo "#endif // __$*_game_map_h__" >> $@

#
# building sprites
#

%.sprite.c: ../images/%.sprite.png
	$(GGBGFX) sprite --name  $*_sprite -o $@ $<

%.sprite.h: ../images/%.sprite.c
	echo "#ifndef __$*_sprite_h__" > $@
	echo "#define __$*_sprite_h__" >> $@
	echo "extern unsigned char $*_sprite[];" >> $@
	echo "#endif // __$*_sprite_h__" >> $@

%.o: %.c %.h
	$(CC) -c -o $@ $<

clean:
	rm -f *.o

distclean: clean
	rm -f *.png *.c *.h


.PHONY: clean distclean
.PRECIOUS: %.tileset.h %.sprite.c %.sprite.h
.PRECIOUS: %.game.map.h
