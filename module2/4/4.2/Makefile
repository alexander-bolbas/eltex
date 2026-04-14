CC = gcc
# Flags: -Wall -Wextra for warnings, -std=c99 for C99 standard
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRC = main.c pq.c ui.c
# Object files (will be in build/)
OBJS = $(patsubst %.c,build/%.o,$(SRC))
# Target executable
TARGET = build/pq_demo

# Build all
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS) | build
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Compile source files to object files in build/
build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
build:
	@mkdir -p build

# Clean build artifacts
clean:
	rm -rf build

.PHONY: all clean
