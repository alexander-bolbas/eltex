# Compiler
CC = gcc
# Опции: проверкон и стандарт C99
CFLAGS = -Wall -Wextra -std=c99

# Исходные файлы
SRC = main.c cmd_registry.c ops.c ui.c
# Объектные файлы в build/
OBJS = $(patsubst %.c,build/%.o,$(SRC))
# Целевой файл
TARGET = build/calculator

# Собрать всё
all: $(TARGET)

# Скомпононать исполняемый файл
$(TARGET): $(OBJS) | build
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Объектные файлы в build/
build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Создать build/ если нет
build:
	@mkdir -p build

# Очистить артефакты
clean:
	rm -rf build

.PHONY: all clean