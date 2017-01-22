CC = gcc
CFLAGS = -fPIC -Wall -Wextra -O0 -g
LDFLAGS = -L ../lib -lmasslog
TARGET = masslog_test_client

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
