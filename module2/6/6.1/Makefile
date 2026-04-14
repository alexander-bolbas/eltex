CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BUILDDIR = build
LIBDIR = lib
APPDIR = app

# Целевая статическая библиотека и приложение
LIB = $(BUILDDIR)/libdlist.a
APP = $(BUILDDIR)/phonebook

# Исходные файлы библиотеки
LIB_SOURCES = $(LIBDIR)/types.h $(LIBDIR)/contact.c $(LIBDIR)/dlist.c
LIB_OBJS = $(BUILDDIR)/contact.o $(BUILDDIR)/dlist.o

# Исходные файлы приложения
APP_SOURCES = $(APPDIR)/main.c $(APPDIR)/ui.c
APP_OBJS = $(BUILDDIR)/main.o $(BUILDDIR)/ui.o

# Создаём build директорию если её нет
$(shell if not exist $(BUILDDIR) (mkdir $(BUILDDIR)))

# Цели
.PHONY: all lib app clean

all: lib app

lib: $(LIB)

app: $(APP)

# Статическая библиотека
$(LIB): $(LIB_OBJS)
	ar rcs $(LIB) $(LIB_OBJS)

# Объектные файлы библиотеки
$(BUILDDIR)/contact.o: $(LIBDIR)/contact.c $(LIBDIR)/contact.h $(LIBDIR)/types.h
	$(CC) $(CFLAGS) -c $(LIBDIR)/contact.c -o $(BUILDDIR)/contact.o

$(BUILDDIR)/dlist.o: $(LIBDIR)/dlist.c $(LIBDIR)/dlist.h $(LIBDIR)/contact.h $(LIBDIR)/types.h
	$(CC) $(CFLAGS) -c $(LIBDIR)/dlist.c -o $(BUILDDIR)/dlist.o

# Объектные файлы приложения
$(BUILDDIR)/main.o: $(APPDIR)/main.c $(APPDIR)/ui.h $(LIBDIR)/dlist.h $(LIBDIR)/types.h
	$(CC) $(CFLAGS) -I$(LIBDIR) -c $(APPDIR)/main.c -o $(BUILDDIR)/main.o

$(BUILDDIR)/ui.o: $(APPDIR)/ui.c $(APPDIR)/ui.h $(LIBDIR)/dlist.h $(LIBDIR)/types.h
	$(CC) $(CFLAGS) -I$(LIBDIR) -c $(APPDIR)/ui.c -o $(BUILDDIR)/ui.o

# Приложение (исполняемый файл)
$(APP): $(APP_OBJS) $(LIB)
	$(CC) $(CFLAGS) -I$(LIBDIR) $(APP_OBJS) -L$(BUILDDIR) -ldlist -o $(APP)

# Очистка
clean:
	@if exist $(BUILDDIR) rmdir /s /q $(BUILDDIR)
