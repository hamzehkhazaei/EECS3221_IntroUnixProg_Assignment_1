CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
TARGET=calculator
SOURCE=calculator.c

$(TARGET): $(SOURCE)
        $(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
        rm -f $(TARGET)

debug: $(TARGET)
        gdb ./$(TARGET)

.PHONY: clean debug
