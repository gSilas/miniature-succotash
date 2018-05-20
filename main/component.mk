#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#


# This converts all the tile files into one big graphics.[ch] file.
GFX_TILE_FILES := gfx/a_btn.tmx gfx/b_btn.tmx gfx/start_btn_play.tmx gfx/start_btn_pause.tmx gfx/up_btn.tmx gfx/down_btn.tmx gfx/right_btn.tmx gfx/left_btn.tmx gfx/running_man_idle_1.tmx gfx/running_man_idle_2.tmx gfx/running_man_run_1.tmx gfx/running_man_run_2.tmx gfx/start_game.tmx
$(eval $(call ConvertTiles,$(GFX_TILE_FILES),graphics))

# Because we use the ConvertTiles macro, we need to tell the make system which other objects
# to compile manually.
COMPONENT_OBJS += app_main.o

# The wave files get embedded as binary files.
#COMPONENT_EMBED_FILES := sound/slap.wav sound/whoosh.wav
