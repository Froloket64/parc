##
# Parc
#
# @file
# @version 0.1

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wfloat-conversion -Wfloat-equal -Wconversion
SRC = src/parser.c src/combinators.c
OBJ = $(SRC:src/%.c=build/%.o)
TEST_OBJ = $(SRC:src/%.c=build/%.test.o)
BUILDDIR = build
EXAMPLES = $(patsubst examples/%.c,%,$(wildcard examples/*.c))

.PHONY = clean all examples examples-test

all: $(OBJ)

examples: $(addprefix build/,$(EXAMPLES))

examples-test: $(addprefix build/,$(addsuffix .test,$(EXAMPLES)))

build/%.o: src/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

build/%: examples/%.c $(OBJ) | $(BUILDDIR)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@

build/%.test.o: src/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -g3 -D TEST -c $< -o $@

build/%.test: examples/%.c $(TEST_OBJ) | $(BUILDDIR)
	$(CC) $(CFLAGS) -g3 -D TEST $< $(TEST_OBJ) -o $@

$(BUILDDIR):
	mkdir -p build

clean:
	rm -rf build

# end
