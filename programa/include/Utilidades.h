#ifndef UTILIDADES_H
#define UTILIDADES_H
#include <stdbool.h>
#include "Cliente.h"

/**
 * @file Utilidades.h
 * @brief Declaraciones de funciones utilitarias para validación de datos.
 *
 * Este archivo contiene las declaraciones de funciones para validar
 * cédulas únicas y números de teléfono.
 */ 

/**
 * @brief Verifica si la cédula proporcionada es única entre los clientes registrados.
 *
 * @param cedula Cédula a validar.
 * @return true si la cédula es única, false si ya existe en la lista.
 */

bool validarCedulaUnica(const char* cedula, Cliente* arregloClientes, int cantidadDeClientes);

/**
 * @brief Valida si el número de teléfono proporcionado cumple con el formato esperado.
 * 
 * @param telefono Cadena de caracteres que representa el número de teléfono a validar.
 * @return true si el número de teléfono es válido, false en caso contrario.
 */
bool validarTelefono(const char* telefono);

int generarCodigoUnicoLibro(Libro* libros, int totalLibros, char* nuevoCodigo);

#endif