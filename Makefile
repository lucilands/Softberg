BUILDDIR=build
LIBDIR=build/lib
BINDIR=build/bin

CFLAGS=-Wall -Wextra -std=c99 -pedantic -Iinclude -ggdb -O3
LDFLAGS=-L$(LIBDIR) -lsoftberg -lm -lSDL2

BUILD_STATIC=Yes
BUILD_DEMO=Yes

SOURCES=$(wildcard src/*.c)
OBJECTS=$(addprefix $(BUILDDIR)/,$(notdir $(SOURCES:%.c=%.o)))

ifeq ($(BUILD_STATIC),Yes)
	TARGET=$(LIBDIR)/libsoftberg.a
else
	TARGET=$(LIBDIR)/libsoftberg.so
endif

DEMO=$(BINDIR)/demo

.PHONY: all

ifeq ($(BUILD_DEMO),Yes)
all: $(TARGET) $(DEMO)
else
all: $(TARGET)
endif


$(TARGET): $(OBJECTS) $(LIBDIR)
ifeq ($(BUILD_STATIC),Yes)
	$(AR) rcs $@ $(OBJECTS)
else
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
endif

$(OBJECTS):$(BUILDDIR)/%.o: src/%.c $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEMO): demo/main.c $(BINDIR) $(TARGET)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(LIBDIR) $(BUILDDIR) $(BINDIR):
	mkdir -p $@
