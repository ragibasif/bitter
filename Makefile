GREEN=\033[0;92m
RED=\033[0;91m
MAGENTA=\033[0;95m
RESET=\033[0m

CC := clang
CFLAGS := -std=c99  -Wall -Wextra -Weverything -O1 -g -pedantic -v -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
DBG = lldb

LDFLAGS := -v -lc

EXECUTABLE := exe


SRCS := $(wildcard *.c)
SRCS += $(wildcard */*.c)
SRCS += $(wildcard */*/*.c)

HDRS := $(wildcard *.h)
HDRS += $(wildcard */*.h)
HDRS += $(wildcard */*/*.h)

OBJS := $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean help run

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo "🔧 Linking ${MAGENTA}$@${RESET} ..."
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) && \
		echo "$(GREEN)✅ Build successful: $@$(RESET)" || \
		(echo "$(RED)❌ Linking failed: $@$(RESET)" && exit 1)

%.o: %.c
	@echo "🔧 Compiling ${MAGENTA}$<${RESET} ..."
	@$(CC) $(CFLAGS) -c $< -o $@ && \
		echo "$(GREEN)✅ Compiled: $<$(RESET)" || \
		(echo "$(RED)❌ Compile failed: $<$(RESET)" && exit 1)

clean:
	@echo "${MAGENTA}make clean${RESET} $(RM) -r $(EXECUTABLE) $(OBJS) *~ *.bak *.dSYM *.out"
	@$(RM) -r $(EXECUTABLE) $(OBJS) *~ *.bak *.dSYM *.out


run: $(EXECUTABLE)
	@echo "${MAGENTA}make run${RESET}"
	@make clean
	@make all
	@./$(EXECUTABLE)
	@make clean


help:
	@echo "${MAGENTA}make help${RESET}"
	@echo "Makefile for Building ${MAGENTA}${EXECUTABLE}${RESET}."
	@echo "Usage:"
	@echo "  ${MAGENTA}make${RESET}             — build"
	@echo "  ${MAGENTA}make clean${RESET}       — remove build files"
	@echo "  ${MAGENTA}make run${RESET}         — run program"

