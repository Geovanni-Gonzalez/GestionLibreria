# Nombre del ejecutable
TARGET = libreria.exe

# Compilador y banderas
CC = g++
CFLAGS = -Wall -Wextra -std=c11

# Archivos fuente y objetos
SRC = main.c Libro.c Cliente.c Pedido.c Estadistica.c Utilidades.c
OBJ = $(SRC:.c=.o)

# Carpeta de includes
INC = include

# Regla por defecto
all: $(TARGET)

# Enlazar objetos para crear ejecutable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilar cada .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

# Limpiar archivos compilados
clean:
	del /Q *.o *.exe

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)
