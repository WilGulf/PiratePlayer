-include .config

TARGET := pirate-player
CC := clang
CXX := clang++

SRCDIR := src
OUTDIR := output

FLAGS := -Wall -Wextra -O2 -g -MMD -I$(SRCDIR)
CFLAGS := $(FLAGS) -std=c23
CXXFLAGS := $(FLAGS) -std=c++20

CPPFLAGS := $(shell pkg-config --cflags libmpdclient)
LDFLAGS := 
LDLIBS := $(shell pkg-config --libs libmpdclient)

SRC_C := $(wildcard $(SRCDIR)/mpd/mpd.c)

SRC_CXX := $(wildcard $(SRCDIR)/main.cpp) \
	$(wildcard $(SRCDIR)/ui/*.cpp) \
	$(wildcard $(SRCDIR)/fb/*.cpp) \
	$(wildcard $(SRCDIR)/platform/*.cpp)

ifeq ($(DESKTOP_SDL),y)
	CFLAGS += -DDESKTOP_SDL
	CPPFLAGS += -DDESKTOP_SDL $(shell sdl2-config --cflags)
	LDLIBS += $(shell sdl2-config --libs)
	SRC_CXX += $(SRCDIR)/platform/desktop/sdl.cpp
else ifeq ($(PI_ST7789),y)
	CPPFLAGS += -DPI_ST7789
	SRC_CXX += $(SRCDIR)/platform/pi/st7789.cpp
else
	$(error Select DESKTOP_SDL=y or PI_ST7789=y)
endif

OBJ_C := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.o,$(SRC_C))
OBJ_CXX := $(patsubst $(SRCDIR)/%.cpp,$(OUTDIR)/%.o,$(SRC_CXX))
OBJS := $(OBJ_C) $(OBJ_CXX)

DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OUTDIR)/%.o: $(SRCDIR)/%.c | $(OUTDIR)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(SRCDIR)/%.cpp | $(OUTDIR)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OUTDIR):
	mkdir -p $(OUTDIR)

test: $(TARGET)
	./$(TARGET)

clean:
	$(RM) -r $(OUTDIR) $(TARGET)

-include $(DEPS)