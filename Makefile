# Makefile for building and testing shape.c

CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude -g
SRCDIR := src
BINDIR := bin
TARGET := $(BINDIR)/shape_test
SRCS := $(SRCDIR)/shape.c $(SRCDIR)/coordinate.c $(SRCDIR)/grid.c
OBJS := $(SRCS:.c=.o)

.PHONY: all test check run clean

all: $(TARGET)

$(BINDIR):
	mkdir -p $(BINDIR)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(SRCDIR)/shape.o $(SRCDIR)/coordinate.o $(SRCDIR)/grid.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

# Run the test binary (prints output to stdout)
test: $(TARGET)
	@echo "Running shape test..."
	@$(TARGET)

# Run test and verify expected output
check: $(TARGET)
	@echo "Verifying shape test output..."
	@$(TARGET) | grep -q "Shape Object created successfully." && echo "PASS" || (echo "FAIL"; exit 1)

run: test

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)
