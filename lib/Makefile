CC = gcc
CFLAGS = -fPIC -Wall -Wextra -O0 -g
LDFLAGS = -shared -lrt
TARGET = libmasslog.so

SOURCES = $(shell echo *.c)
HEADERS = $(shell echo *.h)
OBJECTS=$(SOURCES:.c=.o)

.PHONY: all
all: ${TARGET}

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	${RM} $(OBJECTS) $(TARGET)
