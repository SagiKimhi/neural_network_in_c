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

# Binaries
SO=libnn.so
ADDER_BIN=nn_adder
XOR_BIN=nn_xor
TEST_BIN=nn_tests
NN_GUI_BIN=nn_gui

# Gnu-Make Variables
VPATH:=$(SRC_DIRS)
VERBOSE=false
ifneq ($(VERBOSE), true)
	HIDE =
else
	HIDE = @
endif

# Flags
INCLUDES:=$(foreach dir, $(HDR_DIRS), $(addprefix -I,$(dir)))
INCLUDES+=-I$(HOME)/project/libs/raylib/include
PLIBRARIES=-L$(HOME)/project/libs/raylib/lib
CFLAGS=-Wall -fPIC -O3
OFLAGS=-c
LIBS=-lm -lraylib

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
.PHONY: all clean directories tests examples xor adder slib nn_gui

all: directories $(OBJ_FILES) examples tests

tests: CFLAGS+= -DNN_TESTS
tests: directories $(TEST_BIN)

examples: xor adder

xor: directories $(XOR_BIN)

adder: directories $(ADDER_BIN)

gui: directories $(NN_GUI_BIN)

slib: $(SO)

# Create object directory with subdirs from source directory rule
directories:
	$(HIDE)$(MKDIR) $(subst $(SEP),$(PSEP),$(OBJ_DIRS))

# Binary Rules
$(TEST_BIN): $(OBJ_FILES) example_models/run_tests.c
	@echo Linking $@
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_FILES) example_models/run_tests.c -o $(TEST_BIN) $(LIBS)
	@echo Done!

$(XOR_BIN): $(OBJ_FILES) example_models/nn_xor.c
	@echo Linking $@
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_FILES) example_models/nn_xor.c -o $(XOR_BIN) $(LIBS)
	@echo Done.

$(ADDER_BIN): $(OBJ_FILES) example_models/nn_adder.c
	@echo Linking $@
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_FILES) example_models/nn_adder.c -o $(ADDER_BIN) $(LIBS)
	@echo Done.

$(NN_GUI_BIN): $(OBJ_FILES) example_models/nn_gui.c
	@echo Linking $@
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $(PLIBRARIES) $(OBJ_FILES) example_models/nn_gui.c -o $(NN_GUI_BIN) $(LIBS)
	@echo Done.

# Shared Library Rule
$(SO): $(OBJ_FILES)
	@echo Linking $@
	$(HIDE)$(CC) $(CFLAGS) -shared $(OBJ_FILES) -o $(SO)
	@echo Done!

# C-Files to Object Files Rule
$(foreach dir, $(OBJ_DIRS), $(eval $(call generateRules, $(dir))))

clean: $(eval BINARIES:=$(strip $(shell find -maxdepth 1 -type f -executable -print)))
clean: $(eval BINARIES+=$(strip $(shell find ./example_models/ -maxdepth 1 -type f -executable -print)))
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
ifneq ($(BINARIES),)
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

