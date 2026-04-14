CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BUILDDIR = build
LIBDIR = lib
APPDIR = app

# Целевая динамическая библиотека и приложение
ifeq ($(OS),Windows_NT)
    LIB = $(BUILDDIR)/dlist.dll
else
    LIB = $(BUILDDIR)/libdlist.so
endif
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

# Динамическая библиотека
$(LIB): $(LIB_OBJS)
	$(CC) -shared -fPIC $(LIB_OBJS) -o $(LIB)

# Объектные файлы библиотеки (с флагом -fPIC для позиционно-независимого кода)
$(BUILDDIR)/contact.o: $(LIBDIR)/contact.c $(LIBDIR)/contact.h $(LIBDIR)/types.h
	$(CC) $(CFLAGS) -fPIC -c $(LIBDIR)/contact.c -o $(BUILDDIR)/contact.o

$(BUILDDIR)/dlist.o: $(LIBDIR)/dlist.c $(LIBDIR)/dlist.h $(LIBDIR)/contact.h $(LIBDIR)/types.h
	$(CC) $(CFLAGS) -fPIC -c $(LIBDIR)/dlist.c -o $(BUILDDIR)/dlist.o

# Объектные файлы приложения
$(BUILDDIR)/main.o: $(APPDIR)/main.c $(APPDIR)/ui.h $(LIBDIR)/dlist.h $(LIBDIR)/types.h
	$(CC) $(CFLAGS) -I$(LIBDIR) -c $(APPDIR)/main.c -o $(BUILDDIR)/main.o

$(BUILDDIR)/ui.o: $(APPDIR)/ui.c $(APPDIR)/ui.h $(LIBDIR)/dlist.h $(LIBDIR)/types.h
	$(CC) $(CFLAGS) -I$(LIBDIR) -c $(APPDIR)/ui.c -o $(BUILDDIR)/ui.o

# Приложение (исполняемый файл) - связывается с динамической библиотекой
$(APP): $(APP_OBJS) $(LIB)
	$(CC) $(CFLAGS) -I$(LIBDIR) $(APP_OBJS) -L$(BUILDDIR) -ldlist -Wl,-rpath,$(BUILDDIR) -o $(APP)

# Очистка
clean:
	@if exist $(BUILDDIR) rmdir /s /q $(BUILDDIR)
