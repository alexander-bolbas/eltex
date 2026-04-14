# CC="gcc" compiler
CC = gcc
# Оптимизация: -O2, проверки: -Wall -Wextra
CFLAGS = -Wall -Wextra -std=c99

# Исходные файлы
SRC = main.c phone_book.c contact.c ui.c
# Объектные файлы (будут в build/)
OBJS = $(patsubst %.c,build/%.o,$(SRC))

# Финальный исполняемый файл
TARGET = build/phonebook

# Все - собрать целевой файл
all: $(TARGET)

# Целевой файл зависит от объектных файлов
$(TARGET): $(OBJS) | build
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Правило для компиляции объектных файлов в build/
build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Создание директории build если её нет
build:
	@mkdir -p build

# Очистка всех артефактов
clean:
	rm -rf build

.PHONY: all clean