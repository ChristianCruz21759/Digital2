/* Universidad del Valle de Guatemala
 * Archivo:   map_lib.c
 * Autor: Christian Cruz
 * Carné: 21759
 * July 23, 2023, 8:38 AM
 */

#include <xc.h>
#include "map_lib.h"

float map(int valor, float minx, float maxx, float miny, float maxy){
    
    return (valor - minx) * (maxy - miny) / (maxx - minx) + miny;
    
}