# File: proj2driver/Makefile
# Author: Matt Manzi <mmanzi1@umbc.edu>
# Date: 2019-04-15
# Assignment: Project 2
# Description:
# Makefile for drivers for firewall and file access control system

MAKE += -s
TESTS := ../proj2tests/

SRC = $(shell ls *.c)
OBJ = $(SRC:.c=.o)
BIN = $(OBJ:.o=)

DRV = $(shell ls f*.c)

PHONY += driversonly
driversonly: $(DRV:.c=)

PHONY += all
all: $(BIN)

$(BIN): $(OBJ)
	@echo "  LD\t  $@.o"
	$(CC) $(LDFLAGS) $@.o -o $@

%.o: %.c wrappers.h
	@echo "  CC\t  $<"
	$(CC) $(CFLAGS) -c $<

PHONY += clean
clean:
	@echo "  CLEAN\t  $(OBJ) $(BIN)"
	-@rm -f $(OBJ) $(BIN)

.PHONY: $(PHONY)
