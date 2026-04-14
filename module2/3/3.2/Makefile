# Compiler
CC = gcc
# Опции: проверки выполнения и стандарт
CFLAGS = -Wall -Wextra -std=c99

# Исходные файлы
SRC = main.c ipv4.c stats.c
# Объектные файлы в build/
OBJS = $(patsubst %.c,build/%.o,$(SRC))
# Целевой исполняемый
TARGET = build/ipv4_router

# Собрать всё
all: $(TARGET)

# Линкование
$(TARGET): $(OBJS) | build
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Компиляция
build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Креирование директории
build:
	@mkdir -p build

# Очистка
clean:
	rm -rf build

.PHONY: all clean
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
