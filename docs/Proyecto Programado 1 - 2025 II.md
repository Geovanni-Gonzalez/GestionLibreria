

Instituto Tecnológico de Costa Rica
Ingeniería en Computación
Lenguajes de Programación
Semestre II, 2025
## Profesor: Allan Rodríguez Dávila

## Proyecto Programado #1
Gestión de Librería
## Introducción
La gestión eficiente de inventarios y pedidos es un aspecto fundamental para pequeñas y
medianas empresas, ya que permite mantener un control adecuado de productos, clientes y
transacciones de compra. En un contexto académico, este tipo de problemas resulta ideal para el
aprendizaje y aplicación de los principios del paradigma imperativo en el lenguaje de
programación C.

El Sistema de Gestión de Inventario y Pedidos se plantea como un proyecto que permitirá a los
estudiantes poner en práctica conceptos clave como punteros, arreglos dinámicos, estructuras,
modularidad, paso de parámetros, ocultamiento de información y uso del preprocesador. Además,
se busca que los estudiantes desarrollen habilidades de razonamiento algorítmico y de diseño,
más allá de una implementación mecánica.
Proyecto a desarrollar
Se deberá crear un sistema de gestión de inventario de una libreria en C que administra la
información de libros, clientes y pedidos. El programa se utilizará a través de la consola, no se
implementará interfaz gráfica.  La información se almacenará y consultará desde archivos txt.

El programa deberá desplegar un menú al usuario con dos submenús o grupo de funciones (el
menú debe mostrarse recurrentemente después de cada opción realizada -a excepción del salir- y
tener la opción de subir al menú principal desde los menús secundarios):
## Opciones Administrativas
La solución debe disponer de una sección de opciones administrativas en donde se accederá a
todas estas funcionalidades indicando un usuario y una contraseña (almacenadas en un archivo txt
o json). Al indicar el usuario y la contraseña se habilitará un menú donde se habilitarán las
siguientes funcionalidades:

- Registrar libros
- Manejo de inventario
- Registrar clientes
- Crear pedido
## • Estadísticas
Valores iniciales
Al iniciar la ejecución, la aplicación debe tener disponible la siguiente información: nombre del
local comercial, teléfono, cédula jurídica, horario de atención, número secuencial del siguiente
pedido, usuario y contraseña de administrador.

Registrar libros
El usuario tiene la función de registrar nuevos libros, con información solicitada por el sistema.
Los libros deben almacenarse en un arreglo dinámico de estructuras (struct) manejado con
punteros.
El código del libro es único y debe validarse antes de insertar un nuevo registro.
El título y autor deben almacenarse como arreglos de caracteres (se deben implementar
manualmente funciones auxiliares de copia y comparación).
Precio debe ser flotante y cantidad debe ser entero, ambos positivos.

Carga de inventario (actualizar cantidades)
El sistema debe permitir dar mantenimiento del stock de libros por medio de una carga en lote.
Esta información debe cargarse por medio de un archivo que incluirá el código del libro y la
cantidad, de cada uno, esta cantidad se sumará o restará al stock existente. Validar datos del
archivo (el stock no puede quedar en negativo). Reportar líneas no procesadas.
## Lib1,23
## Lib2,5

Registrar cliente
El usuario tiene la función de registrar nuevos clientes, con información solicitada por el sistema.
Los clientes deben almacenarse en un arreglo dinámico de struct Cliente.
La cédula será el identificador único y debe validarse que no se repita.
El nombre y teléfono deben almacenarse en cadenas de caracteres con tamaño fijo.
Se deben implementar validaciones básicas (longitud mínima del teléfono, que contenga solo
dígitos).

Crear pedido
Se debe permitir la selección de los elementos del catálogo y colocarlos en una lista visible al
cliente (código, nombre y precio), se debe poder filtrar por autor. Se debe permitir elegir varios
libros e indicar la cantidad (detalle).
Además, se puede seleccionar algún libro agregado al pedido y removerlo de la misma.

El cliente puede elegir la cantidad de libros que desee. Si al agregar un nuevo libro al pedido éste
ya existe, lo que se hace es que se aumenta la cantidad. Se debe validar que exista la cantidad de
libros en stock, si no, no se agrega la línea y se informa al usuario (informar al usuario el stock
existente).
La información debe incluir la lista de libros, con las cantidades y precios.
Después de agregar cada línea se debe preguntar si desea agregar línea, eliminar línea (elimina por
número de línea), generar o salir sin guardar. Se deben mostrar las líneas del pedido con su
número.
Cuando se genera el pedido se solicita la cédula del cliente y fecha, se deben descontar de stock la
cantidad facturada de cada libro. Y mostrar todo el detalle del pedido con el encabezado
(información del comercio y cliente) con el identificador generado. Se debe hacer el cálculo del
impuesto de venta del pedido.

Consulta de pedidos
Muestra una lista de todos los pedidos generados. Por cada pedido debe mostrar lo siguiente:
identificador, cliente, fecha, subtotal, total. Al seleccionar un pedido se debe permitir mostrar el
detalle del pedido, que incluye toda la información que se generó (encabezado y líneas).

## Estadísticas
Se debe mostrar la siguiente información, relacionada a los pedidos del sistema:
o Total de ventas realizadas: total de pedidos, adicional mostrar monto por año.
o Clientes con más pedidos: implementarlo con un algoritmo de ordenamiento.
o Libros más vendidos: mostrar código, título, cantidad. Se debe implementar un filtro por
año, el cual es opcional.


## Opciones Generales
Para acceder a estas funcionalidades no se requiere contraseña, serán las siguientes funciones:
o Consulta de catálogo
o Consulta de cliente
o Salir
Consulta de catálogo
Muestra una lista de todos los libros que vende la librería. Por cada uno debe mostrar lo siguiente:
código, título, autor, precio sin impuestos y cantidad en stock. Se debe poder filtrar la información
por autor opcionalmente.

Consulta de cliente
El usuario indica la cédula del cliente y el sistema deberá mostrar los datos y la información
general de cada pedido realizado: código, fecha, subtotal, total, cantidad de libros.


## Salir
Finaliza el programa. Al seleccionar esta opción, se debe asegurar que toda la información del
programa se almacena de forma persistente, es decir, que al volver a correr el programa, todos los
datos se encuentran disponibles.
Todos los datos deben guardarse en archivos de texto plano (no binarios).
Al iniciar, el sistema debe verificar si existen archivos y cargarlos a memoria usando funciones de
lectura (fscanf, fgets).

Funcionalidad Extra (caso de grupos de tres personas)
- Eliminar libro: validar que no existan pedidos asociados.
- Eliminar cliente: validar que no existan pedidos asociados.
- Eliminar pedido: debe revertir el cambio en el stock de los libros (aumenta stock).
- Modificar pedido: se ingresa con el código.
- Estadísticas adicionales: total de ventas agrupado por mes-año y autor con más ventas por
año.
La conformación de grupos de tres personas será únicamente con autorización del profesor en la
primera semana de entregado el enunciado.
Aspectos técnicos
El proyecto deberá estar escrito en el lenguaje de programación C (no C++), y deberá de funcionar
en el sistema operativo elegido por el estudiante, se recomienda Linux. En caso de requerir
librerías adicionales para compilar y ejecutar el programa, deberán especificarlo en la
documentación, ya que de lo contrario se descontarán puntos en la evaluación.

Deberán utilizar el sistema de control de versiones GitHub, el repositorio deberá ser público o
incluir al profesor en el control de acceso de este. Se deben visualizar avances periódicos en el
proyecto por parte de los commits.

Otros aspectos:
- El programa deberá hacer uso de struct.
-  Uso de punteros y memoria dinámica para manejar arreglos (no se permite el uso de
arreglos estáticos con tamaño fijo).
- Utilizar Funciones con paso de parámetros por valor y referencia.
- Implementar archivos de encabezado (.h) para asegurar la modularidad y ocultamiento de
información. El archivo principal (main.c) debe interactuar únicamente con funciones
declaradas en los encabezados, sin acceso directo a los detalles internos.
- Utilizar macros del preprocesador para constantes y configuraciones.
- Se debe cuidar el uso correcto de memoria (liberación de malloc/free).


No seguir estas disposiciones implicará penalizaciones en las funcionalidades o consideradas como
no desarrolladas.
## Documentación
La documentación es un aspecto de gran importancia en el desarrollo de programas,
especialmente en tareas relacionadas con el mantenimiento de estos.
Para la documentación interna, deberán incluir comentarios descriptivos para cada función, con
sus entradas, salidas, restricciones y objetivo.

La documentación externa deberá incluir:
## 1. Portada.
- Manual de usuario: instrucciones de compilación, ejecución y uso.
- Pruebas de funcionalidad: incluir screenshots.
- Descripción del problema.
- Diseño del programa: decisiones de diseño, algoritmos usados, diagrama de archivos.
- Librerías usadas: manejo de archivos, análisis, etc.
- Análisis de resultados (lista de cotejo): objetivos alcanzados, objetivos no alcanzados, y
razones por las cuales no se alcanzaron los objetivos (en caso de haberlos).
- Justificación de toma de decisiones:
a. Por qué usó punteros en cada sección del código. Ejemplos concretos de paso por
valor y por referencia.
b. Cómo se asegura la modularidad y el ocultamiento de información
- Bitácora (autogenerada en git, commit por usuario incluyendo comentario).
Forma de trabajo
El trabajo se debe realizar en grupos de dos, salvo excepción aprobada por el profesor. La
prioridad serán las parejas de trabajo.
## Evaluación
La evaluación se va a centrar en dos elementos: programación y documentación.

El proyecto programado tiene un valor de 10% de la nota final, en el rubro de Proyectos.

Desglose de la evaluación del proyecto programado:
- Documentación interna 2 ptos.
- Documentación externa 8 ptos.

- Funcionalidad 80 ptos (ver detalle en Proyecto a Desarrollar)
- Revisión del proyecto (según completitud del proyecto y gestión del tiempo) 5 ptos.
- Hora de Entrega 5 ptos.
Aspectos administrativos
Debe crear un archivo .zip (“PP1_Est.zip”) que contenga únicamente un archivo info.txt y 2
carpetas llamadas documentacion y programa, en la primera deberá incluir el documento de word
o pdf solicitado y en la segunda los archivos y carpetas necesarias para la implementación
de este proyecto programado, y/o link en git del repositorio. El archivo info.txt debe contener la
siguiente información (cualidades):
a. Nombre del curso
b. Número de semestre y año lectivo
c. Nombre de los Estudiante
d. Número de carnet de los estudiantes
e. Número de proyecto programado
f. Fecha de entrega
g. Estatus de la entrega (debe ser CONGRUENTE con la solución entregada):
[Deplorable|Regular|Buena|MuyBuena|Excelente|Superior]
## Entrega
Deberá subir el archivo antes mencionado al TEC Digital en el curso de LENGUAJES DE
PROGRAMACIÓN GR 60, en la asignación llamada “P1” debajo del rubro de “Proyectos”.  En la
evaluación del Proyecto el rubro de “Hora de Entrega” valdrá por 5 puntos de la nota total del
proyecto, según la siguiente escala:
a. Si se entrega antes de las 11:55:55 PM del lunes 15 de septiembre de 2025, 5 puntos.
b. Si se entrega antes de las 11:55:55 AM del martes 16 de septiembre de 2025, 2.5 puntos.
c. Si se entrega antes de las 11:55:55 PM del 16 de septiembre de 2025, 0 puntos.
NO SE ACEPTARÁN trabajos que contengan “commits” posterior a esta fecha.

Cada estudiante deberá participar en la revisión del proyecto, demostrando tanto la funcionalidad
como su autoría. Durante esta revisión, se podrán solicitar ajustes en tiempo real; la incapacidad
de realizarlos se considerará como una falta de dominio del programa (código).
