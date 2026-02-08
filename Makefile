CC = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP

SRC_DIR = src
OBJ_DIR = build

TARGET = argon


SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


-include $(DEPS)


run: $(TARGET)
	./$(TARGET) test/test.arg


clean:
	rm -rf $(OBJ_DIR) $(TARGET)


debug: CFLAGS += -DDEBUG
debug: clean all
