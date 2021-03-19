/*
Este es un Codigo fuente para la biblioteca de Drivers
del Dron-Carga beta 0
 

@file tets_uCUnit.c
@date 18 mar. 2021
@author Madera
@brief tets_uCUnit

@version 2021
*/

/*   System Headers   	*/
/*   Used Headers   	*/
/*   Own Headers   		*/

#include "uCUnit.h"
/*     [EXPORTED]		*/

/*public variables*/

/*     [INTERNAL]		*/

/*internal__pre-processor macros*/

/*internal__types  */
typedef enum {
    Unknown,  /* first, generic item */
    Hydrogen, /* H */
    Helium,   /* He */
    Oxygen,   /* O */
    Oxygen2,  /* O2 */
    Water,    /* H2O */
    ChemLast  /* last, sentinel */
} Chem_t;

/*private__variables */

/*private__functions */

/*public_functions */

Chem_t crazyScientist(Chem_t a, Chem_t b);



Chem_t crazyScientist(Chem_t a, Chem_t b) {
    if (a==Oxygen && b==Oxygen) {
        return Oxygen2;
    }
    if (a==Hydrogen && b==Oxygen2) {
        return Water;
    }
    return Unknown;
}
