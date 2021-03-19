/*
Este es un Codigo fuente para la biblioteca de Drivers
del Dron-Carga beta 0
 

@file test_test_uCuint_sample.c
@date 18 mar. 2021
@author Madera
@brief test_test_uCuint_sample

@version 2021
*/
#include "uCUnit.h"
#include "test_Function.c"
/*   System Headers   	*/
/*   Used Headers   	*/
/*   Own Headers   		*/
void Test(void) {
  Chem_t res;
  UCUNIT_Init(); /* initialize framework */

  UCUNIT_TestcaseBegin("Crazy Scientist");
  res = crazyScientist(Oxygen, Oxygen);
  UCUNIT_CheckIsEqual(res, Oxygen2);
  UCUNIT_CheckIsEqual(Unknown, crazyScientist(Water, Helium));
  UCUNIT_CheckIsEqual(Water, crazyScientist(Hydrogen, Oxygen2));
  UCUNIT_CheckIsEqual(Water, crazyScientist(Oxygen2, Hydrogen));
  UCUNIT_CheckIsInRange(crazyScientist(Unknown, Unknown), Unknown, ChemLast);
  UCUNIT_TestcaseEnd();

  /* finish all the tests */
  UCUNIT_WriteSummary();
  UCUNIT_Shutdown();
}
