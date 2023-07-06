# Compiler
CC=clang
CFLAGS=-ansi -pedantic -std=c99 -Wall -O3

# Flags
LIBS=-lm
INCLUDES=-I./hdr
GUI_LIBS:=$(LIBS) -lraylib -Wl,-rpath=/usr/local/lib
TESTS_INCLUDES:=-I./tests $(INCLUDES)
GUI_INCLUDES:=-I/usr/local/include $(INCLUDES)

# Directories
EXAMPLES_DIR=./example_models
TESTS_DIR=./tests
TESTS_SRC_FILES=$(wildcard $(TESTS_DIR)/*.c)

# Utils
_SET_GREEN := @echo -n "\033[32m" # Green text for "printf"
_SET_RED := @echo -n "\033[31m" # Red text for "printf"
_SET_WHITE := @echo -n "\033[0m" # White text for "printf"
VERBOSE=false

ifneq ($(VERBOSE), true)
	HIDE =
else
	HIDE = @
endif

# Binaries
TEST_BIN=nn_tests
XOR_BIN=nn_xor
ADDER_BIN=nn_adder
ADDER_GUI_BIN=nn_adder_gui
IMG_UPSCALE_GUI_BIN=nn_img_upscale_gui
RSA_DECRYPT_GUI_BIN=nn_rsa_decrypt_gui

# Rules
.PHONY: all clean tests examples xor adder adder_gui rsa_decrypt_gui img_upscale_gui

all: examples

tests: nn_tests nn_matrix_tests

examples: xor adder adder_gui rsa_decrypt_gui img_upscale_gui

xor: $(XOR_BIN)

adder: $(ADDER_BIN)

adder_gui: $(ADDER_GUI_BIN)

rsa_decrypt_gui: $(RSA_DECRYPT_GUI_BIN)

img_upscale_gui: $(IMG_UPSCALE_GUI_BIN)

nn_tests: $(TESTS_DIR)/nn_tests.c
	$(HIDE)$(CC) $(CFLAGS) $(TESTS_INCLUDES) $^ -o $@ $(LIBS)

nn_matrix_tests: $(TESTS_DIR)/nn_matrix_tests.c
	$(HIDE)$(CC) $(CFLAGS) $(TESTS_INCLUDES) $^ -o $@ $(LIBS)

$(XOR_BIN): $(EXAMPLES_DIR)/nn_xor.c
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

$(ADDER_BIN): $(EXAMPLES_DIR)/nn_adder.c
	$(HIDE)$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

$(ADDER_GUI_BIN): $(EXAMPLES_DIR)/nn_adder_gui.c
	$(HIDE)$(CC) $(CFLAGS) $(GUI_INCLUDES) $^ -o $@ $(GUI_LIBS)

$(RSA_DECRYPT_GUI_BIN): $(EXAMPLES_DIR)/nn_rsa_decrypt_gui.c
	$(HIDE)$(CC) $(CFLAGS) $(GUI_INCLUDES) $^ -o $@ $(GUI_LIBS)

$(IMG_UPSCALE_GUI_BIN): $(EXAMPLES_DIR)/nn_upscale_png.c
	$(HIDE)$(CC) $(CFLAGS) $(GUI_INCLUDES) $^ -o $@ $(GUI_LIBS)


clean: $(eval BINARIES:=$(strip $(shell find -maxdepth 1 -type f -executable -print)))
clean: $(eval BINARIES:=$(filter-out $(wildcard	./*.sh), $(BINARIES)))
clean: $(eval RMBIN:=rm $(BINARIES))
clean:
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

