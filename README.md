# Documentación del Proyecto: Sistema de Gestión de Librería

## 1. Portada

**Proyecto**: Gestión de Librería   
**Lenguaje**: C (C11)  
**Sistema Operativo**: Windows (Compatible con Linux)  

## 2. Manual de Usuario

### Compilación y Ejecución

Para compilar el proyecto, asegúrese de tener instaladas las herramientas de desarrollo (`gcc`, `make` o `mingw32-make`).
Abra una terminal en la carpeta `programa` y ejecute:

```bash
mingw32-make clean all
```

Esto generará el ejecutable `libreria.exe`. Ejecútelo con:

```bash
./libreria.exe
```

### Uso del Sistema

1. **Menú Principal**: Seleccione "Opciones Administrativas" para gestión o "Opciones Generales" para consultas públicas.
2. **Login**: Ingrese las credenciales (por defecto en `data/admin.json`).
3. **Gestión de Inventario**: Puede agregar libros manualmente o cargar un archivo `.txt` masivo.
4. **Pedidos**:
    - Seleccione "Crear Pedido".
    - Elija libros del catálogo por código.
    - Confirme para generar (descuenta stock).
5. **Estadísticas Avanzadas**: Consulte "Estadísticas de negocio" para ver el cliente con mayor inversión y el libro más rentable.

## 3. Descripción del Problema

El objetivo fue desarrollar un sistema de consola en C para administrar el flujo de una librería, controlando inventario, clientes y ventas, utilizando estructuras de datos dinámicas y persistencia en archivos de texto.

## 4. Diseño del Programa

### Arquitectura Modular

El sistema se divide en módulos independientes (.h / .c) para asegurar bajo acoplamiento:

- **Main**: Control de flujo y menús.
- **Libro**: Gestión de inventario (CRUD).
- **Cliente**: Gestión de base de datos de clientes.
- **Pedido**: Lógica de carrito de compras, cálculo de impuestos y persistencia.
- **Estadistica**: Reportes avanzados de negocio.
- **Interfaz**: Capa de presentación (Colores ANSI, Banners).
- **Utilidades**: Validaciones y helpers genéricos.

### Diagrama de Archivos

- `programa/`: Código fuente.
- `programa/include/`: Encabezados.
- `programa/data/`:
  - `libros.txt`: Base cuadrada de datos de libros.
  - `clientes.txt`: Datos de clientes.
  - `pedidos.txt`: Histórico de transacciones.
  - `admin.json`: Configuración y credenciales.

## 5. Justificación de Decisiones Técnicas

### Uso de Punteros y Memoria Dinámica

Se utilizaron punteros (`malloc`, `realloc`) para los arreglos de Libros, Clientes y Pedidos. Se eligió esta aproximación sobre arreglos estáticos para permitir que el sistema escale sin límites codificados, adaptándose a cualquier cantidad de datos disponible en la memoria del equipo.

### Paso por Referencia

Las funciones como `agregarLibro` reciben punteros dobles (`Libro**`) para poder modificar la dirección de memoria del arreglo original al redimensionarlo, asegurando que los cambios se reflejen en todo el programa.

## 6. Análisis de Resultados

- [x] **Registrar Libros/Clientes**: Funcional, con validación de unicidad.
- [x] **Crear Pedido**: Completo, con control de stock en tiempo real.
- [x] **Persistencia**: Todos los datos se guardan y recuperan correctamente.
- [x] **Estadísticas**: Se implementaron los reportes básicos y avanzados.
- [x] **Extras**: Se implementó "Eliminar Libro" (con validación), "Eliminar Cliente", "Eliminar Pedido" (con reversión de stock) y "Modificar Pedido".

## 7. Pruebas de Funcionalidad

El sistema ha sido verificado mediante escenarios de prueba:

1. **Prueba de Stock**: Intentar comprar más unidades de las disponibles -> Muestra error de stock.
2. **Prueba de Persistencia**: Agregar libro, cerrar programa, abrir -> El libro persiste.
3. **Prueba de Eliminación**: Eliminar un pedido -> El stock de los libros retorna al inventario.

## 8. Librerías Usadas

- `stdio.h`, `stdlib.h`, `string.h`: Estándar de C.
- `ctype.h`: Para validaciones de caracteres.
- `Interfaz.h`: Librería propia para estilizado TUI.
