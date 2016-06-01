objlist := editor cJSON util draw gui level squirrel dialogs
program_title = editor
 
 
CC := gcc
LD := g++
 
objdir := obj
srcdir := src
objlisto := $(foreach o,$(objlist),$(objdir)/$(o).o)
 
# FL4SHK updated this makefile to work on Linux.  Date of update:  Jun 1, 2016
ifeq ($(OS),Windows_NT)
  CFLAGS := -Wall -O2 -std=gnu99
  LDLIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lsquirrel -lsqstdlib
  LDFLAGS := -Wl,-subsystem,windows
else
  SQUIRREL=../SQUIRREL3
  CFLAGS := -Wall -O2 -std=gnu99 `sdl2-config --cflags` -I$(SQUIRREL)/include
  LDLIBS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lsquirrel -lsqstdlib -L. -L$(SQUIRREL)/lib
  #LDFLAGS := -Wl
endif
 
editor: $(objlisto)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
 
$(objdir)/%.o: $(srcdir)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
 
.PHONY: clean
 
clean:
	-rm $(objdir)/*.o
