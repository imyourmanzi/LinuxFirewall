# File: proj2tests/Makefile
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-04-15
# Assignment: Project 2
# Description:
# Top makefile for tests for firewall and file access control system

MAKE += -s
DRIVERS := ../proj2driver/

CC = @gcc
CFLAGS ?= -g -Wall -Wextra -pedantic
LDFLAGS ?= -g

SRC = assertl.c asserts.c
OBJ = $(SRC:.c=.o)
BIN = $(SRC:.c=)

# let other makefiles and scripts down the line use these variables
export DRIVERS
export CC
export CFLAGS
export LDFLAGS

PHONY += all
all: $(BIN) $(DRIVERS)

$(BIN): $(OBJ) assert.o
	@echo "  LD\t  $@.o"
	$(CC) $(LDFLAGS) $@.o assert.o -o $@

$(OBJ): $(SRC)
	@echo "  CC\t  $(@:.o=.c)"
	$(CC) $(CFLAGS) -c $(@:.o=.c)

assert.o: assert.c assert.h
	@echo "  CC\t  $<"
	$(CC) $(CFLAGS) -c $< -o $@

PHONY += $(DRIVERS)
$(DRIVERS):
	@echo "  MAKE\t  $@ -> all"
	@$(MAKE) -C $@ all

PHONY += driversonly
driversonly:
	@echo "  MAKE\t  $(DRIVERS) -> $@"
	@$(MAKE) -C $(DRIVERS) $@

PHONY += run
run: all
	-@for test in $(shell ls -1 t_*.sh); do \
		echo "\033[38;5;33mRunning $${test}...\033[0m"; \
		./$${test} 2>/dev/null; \
		echo; \
	done;

PHONY += stress
stress: all
	-@for test in $(shell ls -1 s_*.sh); do \
		echo "\033[38;5;33mRunning $${test}...\033[0m"; \
		./$${test}; \
		echo; \
	done;

PHONY += clean
clean:
	@echo "  CLEAN\t  *.o $(BIN)"
	-@rm -f *.o $(BIN)
	@echo "  MAKE\t  $(DRIVERS) -> $@"
	@$(MAKE) -C $(DRIVERS) $@

.PHONY: $(PHONY)
