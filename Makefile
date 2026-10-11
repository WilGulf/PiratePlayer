TARGET := pirate-player
CC := clang
CXX := clang++

SRCDIR := src
OUTDIR := output

SDLFLAGS = $(shell sdl2-config --cflags)
SDLLIBS = $(shell sdl2-config --libs)

FLAGS := -Wall -Wextra -O2 -g -MMD -I$(SRCDIR)
CFLAGS := $(FLAGS) -std=c20
CXXFLAGS := $(FLAGS) -std=c++20

CPPFLAGS := $(SDLFLAGS)
LDFLAGS := 
LDLIBS := $(SDLLIBS)

SRC_C := $(wildcard $(SRCDIR)/*.c) \
	$(wildcard $(SRCDIR)/*/*.c) \
	$(wildcard $(SRCDIR)/*/*/*.c)

SRC_CXX := $(wildcard $(SRCDIR)/*.cpp) \
	$(wildcard $(SRCDIR)/*/*.cpp) \
	$(wildcard $(SRCDIR)/*/*/*.cpp)

OBJ_C := $(SRC_C:$(SRCDIR)/%.c=$(OUTDIR)/%.o)
OBJ_CXX := $(SRC_CXX:$(SRCDIR)/%.cpp=$(OUTDIR)/%.o)
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