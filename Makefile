# Directories
SRC_DIR ?=./src
TESTS_DIR ?=./tests
EXAMPLES_DIR ?=./examples
TESTS_SRC_FILES ?=$(wildcard $(TESTS_DIR)/*.c)

# Compiler
CC ?=clang
CFLAGS ?=-ansi -pedantic -std=c99 -Wall -O3
LDFLAGS ?=-lm
GUI_LDFLAGS ?=$(LDFLAGS) -lraylib
GUI_FLAGS ?=$(shell pkg-config --cflags --libs raylib) $(LDFLAGS)

# Flags
INCLUDES ?=-I$(SRC_DIR)
GUI_INCLUDES ?=$(INCLUDES)
TESTS_INCLUDES ?=-I./tests $(INCLUDES)

# Binaries
XOR_BIN ?=nn_xor
TEST_BIN ?=nn_tests
ADDER_BIN ?=nn_adder
ADDER_GUI_BIN ?=nn_adder_gui
IMG_UPSCALE_GUI_BIN ?=nn_img_upscale_gui
RSA_DECRYPT_GUI_BIN ?=nn_rsa_decrypt_gui

# Verbosity
VERBOSE ?=false

ifneq ($(VERBOSE), true)
	HIDE =
else
	HIDE = @
endif

# Text colors
_SET_RED := @echo -n "\033[31m" # Red text for "printf"
_SET_GREEN := @echo -n "\033[32m" # Green text for "printf"
_SET_WHITE := @echo -n "\033[0m" # White text for "printf"

# Makefile rules
.PHONY: all\
    clean\
    tests\
    examples\
    xor\
    adder\
    adder_gui\
    rsa_decrypt_gui\
    img_upscale_gui

all: examples

tests: nn_tests nn_matrix_tests

examples:\
	xor\
	adder\
	adder_gui\
	img_upscale_gui\
	rsa_decrypt_gui# Doesn't work, but was interesting to try out

xor: $(XOR_BIN)

adder: $(ADDER_BIN)

adder_gui: $(ADDER_GUI_BIN)

rsa_decrypt_gui: $(RSA_DECRYPT_GUI_BIN)

img_upscale_gui: $(IMG_UPSCALE_GUI_BIN)

nn_tests: $(TESTS_DIR)/nn_tests.c
	$(HIDE)$(CC) $(CFLAGS) $(TESTS_INCLUDES) $^ -o $@ $(LDFLAGS)

nn_matrix_tests: $(TESTS_DIR)/nn_matrix_tests.c
	$(HIDE)$(CC) $(CFLAGS) $(TESTS_INCLUDES) $^ -o $@ $(LDFLAGS)

$(XOR_BIN): $(EXAMPLES_DIR)/nn_xor.c
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(ADDER_BIN): $(EXAMPLES_DIR)/nn_adder.c
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(ADDER_GUI_BIN): $(EXAMPLES_DIR)/nn_adder_gui.c
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $(GUI_FLAGS) $^ -o $@

$(RSA_DECRYPT_GUI_BIN): $(EXAMPLES_DIR)/nn_rsa_decrypt_gui.c
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $(GUI_FLAGS) $^ -o $@

$(IMG_UPSCALE_GUI_BIN): $(EXAMPLES_DIR)/nn_upscale_png.c
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $(GUI_FLAGS) $^ -o $@


clean: \
	$(eval BINARIES:=$(strip $(shell find -maxdepth 1 -type f -executable -print)))\
	$(eval BINARIES:=$(filter-out $(wildcard	./*.sh), $(BINARIES)))\
	$(eval RMBIN:=rm $(BINARIES))
ifneq ($(BINARIES),)
	@echo "\nmake clean wants to execute the following command:"
	$(_SET_RED)
	@echo "$(RMBIN)"
	$(_SET_WHITE)
	@echo -n "Do you accept? [y/n] " && read ans && [ $${ans:-N} = y ]
	$(RMBIN)
endif
	$(_SET_GREEN)
	@echo "Cleaning Done."
	$(_SET_WHITE)

