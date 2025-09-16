# ------------------------------
# Configuración general
# ------------------------------
TARGET = libreria.exe
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I./include

SRC = main.c Cliente.c Config.c Estadistica.c Libro.c Pedido.c Utilidades.c cJSON.c 
OBJ = $(SRC:.c=.o)

# ------------------------------	
# Regla por defecto
# ------------------------------
all: $(TARGET)

# ------------------------------
# Enlazar el ejecutable con verbose
# ------------------------------
$(TARGET): $(OBJ)
	@echo "Enlazando $(TARGET)..."
	$(CC) -v $(OBJ) -o $(TARGET)

# ------------------------------
# Compilar cada .c en su .o
# ------------------------------
%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------
# Limpiar binarios (Windows o Linux)
# ------------------------------
clean:
	-del /Q $(OBJ) $(TARGET) 2>nul || rm -f $(OBJ) $(TARGET)

# ------------------------------
# Ejecutar el programa
# ------------------------------
run: $(TARGET)
	./$(TARGET)

# ------------------------------
# Ejecutar dentro de gdb
# ------------------------------
debug: $(TARGET)
	gdb ./$(TARGET)

.PHONY: all clean run debug
