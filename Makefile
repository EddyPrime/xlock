CC = gcc
INCDIR = include
CFLAGS = -Wall -I$(INCDIR)
LDFLAGS = -lcrypto
SRCDIR = src
MAINDIR = test
OBJDIR = obj
BINDIR = bin
TARGET = $(BINDIR)/main

CFLAGS_ALL = $(CFLAGS) -O3
CFLAGS_DEBUG = $(CFLAGS) -g3 -Wextra -D_DEBUG_
CFLAGS_TEST = $(CFLAGS) -g3 -Wextra -D_DEBUG_ -D_SPEED_

SRCS := $(wildcard $(SRCDIR)/*.c) $(wildcard $(MAINDIR)/*.c)
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRCS)))

all: CFLAGS := $(CFLAGS_ALL)
all: $(TARGET)

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: $(TARGET)

test: CFLAGS := $(CFLAGS_TEST)
test: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(MAINDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	$(TARGET)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all debug run clean
