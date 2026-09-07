CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -g

INCLUDES = \
	-I../third_party/unity \
	-I../src \
	-I../src/system

UNITY = ../third_party/unity/unity.c

STATE_MACHINE = ../src/system/state_machine.c

TEST = unit/test_state_machine.c

TARGET = unit/test_state_machine

SOURCES = \
	$(TEST) \
	$(STATE_MACHINE) \
	$(UNITY)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all test clean