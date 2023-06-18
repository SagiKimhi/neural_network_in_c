# Compiler
CC=clang

# Directories
SRC_DIR=src
HDR_DIR=hdr
OBJ_DIR=obj
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

# Functions
define generateRules
$(1)/%.o: %.c
	@echo Building $$@
	$(HIDE)$(CC) $$(CFLAGS) $$(OFLAGS) $$(INCLUDES) $$(LIBS) $$(subst $$(SEP),$$(PSEP),$$<) -o $$(subst $$(SEP),$$(PSEP),$$@)
endef

# Rules
.PHONY: all clean directories

all: directories $(BIN)

# Binary Rule
$(BIN): $(OBJ_FILES)
	@echo Linking $@
	$(HIDE)$(CC) $(CFLAGS) $(OBJ_FILES) -o $(BIN)
	@echo Done!

# C-Files to Object Files Rule
$(foreach dir, $(OBJ_DIRS), $(eval $(call generateRules, $(dir))))

# Create object directory with subdirs from source directory rule
directories:
	$(HIDE)$(MKDIR) $(subst $(SEP),$(PSEP),$(OBJ_DIRS))

clean:
ifdef OBJ_DIR
	@echo Cleaning object directories
	$(RMDIR) $(OBJ_DIR)
endif
ifdef BIN
	@echo Cleaning Binary
	$(RM) $(BIN)
endif
	@echo Cleaning Done.
