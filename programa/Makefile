# Nombre del ejecutable
TARGET = libreria.exe

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./include

# Todos los archivos .c en la carpeta
SRC = $(wildcard *.c)

# Convertir cada .c en .o
OBJ = $(SRC:.c=.o)

# Regla principal
all: $(TARGET)

# Enlazado final
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Regla implícita: cómo compilar .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJ) $(TARGET)

# Ejecutar programa
run: $(TARGET)
	./$(TARGET) || true

