# Compiler
CC=clang

# Directories
SRC_DIR=src
HDR_DIR=hdr
OBJ_DIR=obj
TEST_DIR=tests
SRC_DIRS:=$(shell find $(SRC_DIR) -type d -print)
HDR_DIRS:=$(shell find $(HDR_DIR) -type d -print)
OBJ_DIRS:=$(subst $(SRC_DIR), $(OBJ_DIR), $(SRC_DIRS))

# Files
SRC_FILES=$(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJ_FILES:=$(subst $(SRC_DIR), $(OBJ_DIR), $(SRC_FILES:.c=.o))
BIN=nn_in_c

# Gnu-Make Variables
VPATH:=$(SRC_DIRS)
VERBOSE=false
ifneq ($(VERBOSE), true)
	HIDE =
else
	HIDE = @
endif

# Flags
CFLAGS=-ansi -pedantic -std=c99 -Wall
OFLAGS=-c
LIBS=-lm
INCLUDES:=$(foreach dir, $(HDR_DIRS), $(addprefix -I,$(dir)))

# Commands
RM=rm
RMDIR=rm -r
MKDIR=mkdir -p
SEP=/
PSEP=$(strip $(SEP))

# Utils
_SET_GREEN := @echo -n "\033[32m" # Green text for "printf"
_SET_RED := @echo -n "\033[31m" # Red text for "printf"
_SET_WHITE := @echo -n "\033[0m" # White text for "printf"

# Functions
define generateRules
$(1)/%.o: %.c
	@echo Building $$@
	$(HIDE)$(CC) $$(CFLAGS) $$(OFLAGS) $$(INCLUDES) $$(subst $$(SEP),$$(PSEP),$$<) -o $$(subst $$(SEP),$$(PSEP),$$@)
endef

# Rules
.PHONY: tests all clean directories

all: BIN=nn_in_c
all: directories $(BIN)

tests: BIN=nn_tests
tests: CFLAGS+= -DNN_TESTS
tests: directories $(BIN)

# Create object directory with subdirs from source directory rule
directories:
	$(HIDE)$(MKDIR) $(subst $(SEP),$(PSEP),$(OBJ_DIRS))

# Binary Rule
$(BIN): $(OBJ_FILES)
	@echo Linking $@
	$(HIDE)$(CC) $(CFLAGS) $(LIBS) $(OBJ_FILES) -o $(BIN)
	@echo Done!

# C-Files to Object Files Rule
$(foreach dir, $(OBJ_DIRS), $(eval $(call generateRules, $(dir))))

clean: $(eval BINARIES:=$(strip $(shell find -maxdepth 1 -type f -executable -name nn* -print)))
clean: $(eval OBJECTS:=$(shell find -maxdepth 1 -type d -name $(OBJ_DIR) -print))
clean: $(eval RMOBJ:=rm -r $(OBJECTS))
clean: $(eval RMBIN:=rm $(BINARIES))
clean:
ifneq ($(OBJECTS),)
	@echo "\nmake clean wishes to execute the following command:"
	$(_SET_RED)
	@echo "$(RMOBJ)"
	$(_SET_WHITE)
	@echo -n "Do you accept? [y/n] " && read ans && [ $${ans:-N} = y ]
	$(RMOBJ)
endif
ifneq "$(BINARIES)" ""
	@echo "\nmake clean wishes to execute the following command:"
	$(_SET_RED)
	@echo "$(RMBIN)"
	$(_SET_WHITE)
	@echo -n "Do you accept? [y/n] " && read ans && [ $${ans:-N} = y ]
	$(RMBIN)
endif
	$(_SET_GREEN)
	@echo "Cleaning Done."
	$(_SET_WHITE)

