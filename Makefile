CC := clang
CFLAGS := -std=c99 -Wall -Wextra -Iinclude
AR := ar
ARFLAGS := cr

SRC_DIR := src
OBJ_DIR := obj

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: coffified.a

coffified.a: $(OBJS) | $(OBJ_DIR)
	$(AR) $(ARFLAGS) $@ $(OBJS) $^
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR):
	mkdir -p $@

test: $(OBJ_DIR)/test.o $(OBJS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $^ -o $@
$(OBJ_DIR)/test.o: test.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@
clean:
	rm -rf coffified.a test *.dSYM $(OBJ_DIR)

.PHONY: all clean
