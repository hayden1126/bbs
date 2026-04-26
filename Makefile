CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude -O2 -g
CORE := src/bbs.o
CLI := bin/bbs
TEST := bin/bbs_test

.PHONY: all test check clean

all: $(CLI) $(TEST)

bin:
	mkdir -p bin

src/%.o: src/%.c include/bbs.h
	$(CC) $(CFLAGS) -c $< -o $@

$(CLI): src/cli.o $(CORE) | bin
	$(CC) $(CFLAGS) -o $@ $^

$(TEST): src/tests.o $(CORE) | bin
	$(CC) $(CFLAGS) -o $@ $^

test: $(TEST)
	@$(TEST)

check: $(TEST)
	@$(TEST) | grep -q "All tests passed." && echo PASS || (echo FAIL; exit 1)

clean:
	rm -f src/*.o $(CLI) $(TEST)
	rmdir bin 2>/dev/null || true
