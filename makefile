#! /usr/bin/make

PROGRAM = ${shell basename "$$PWD"}
CC = gcc

FLAGS += -pedantic -g

SOURCE = src
HEADER = src
OUTPUT = out

CFILES = $(shell [ -d $(SOURCE) ] && find $(SOURCE) -name '*.c' -print)
HFILES = $(shell [ -d $(HEADER) ] && find $(HEADER) -name '*.h' -print)
OFILES = $(patsubst $(SOURCE)/%.c,$(OUTPUT)/%.o,$(CFILES))

# INCLUDE any directories you want to search for headers in
# This works for including <> style headers as well
INCLUDE =

# Compiles the main program, depends on the object files
# $(patsubst %,-I %,$(INCLUDE))
$(PROGRAM): $(OFILES)
	$(info Compiling program `$@`)
	@$(CC) $(FLAGS) -o $(OUTPUT)/$(PROGRAM) $(OFILES)

# Compiles object files into OUTPUT
# @mkdir -p $(shell echo "$@" | grep -E -o "((.*)\/)")
$(OUTPUT)/%.o: $(SOURCE)/%.c $(HFILES)
	@dirname $@ | xargs mkdir -p
	$(info Compiling object file `$@`)
	@$(CC) $(FLAGS) -c -o $@ $< -I $(HEADER)

# All the user-defined make routines
.PHONY: preprocessed run clean initialize dummy

# Produces a preprocessed file into OUTPUT
preprocessed:
	$(info Preprocessing...)
	@$(CC) $(SOURCE)/main.c -E -o $(OUTPUT)/$(PROGRAM).i -I $(HEADER) $(INCLUDE)

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):dummy;@:)
endif

# Makes and runs the program
run: $(PROGRAM)
	@echo --- RUNNING ---
	@./$(OUTPUT)/$(PROGRAM) $(RUN_ARGS)
	@echo --- STOPPED ---

dbg: $(PROGRAM)
	@echo --- DEBUGMD ---
	@gdb -q -ex=run ./$(OUTPUT)/$(PROGRAM) --batch
	@echo --- STOPPED ---

clean:
	$(info Cleaning object files...)
	@$(RM) -rf $(OUTPUT)

initialize:
	@mkdir -p $(SOURCE)
	@mkdir -p $(HEADER)

	@if [ ! -e $(SOURCE)/main.c ]; then\
		echo "#include <stdio.h>" > $(SOURCE)/main.c;\
		echo >> $(SOURCE)/main.c;\
		echo "int main() {" >> $(SOURCE)/main.c;\
		echo "	printf(\"Hello, world!\n\");" >> $(SOURCE)/main.c;\
		echo "}" >> $(SOURCE)/main.c;\
	fi;
