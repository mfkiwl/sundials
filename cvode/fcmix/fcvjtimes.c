/**********************************************************************
 * File          : fcvjtimes.c                                        *
 * Programmers   : Alan C. Hindmarsh and Radu Serban @ LLNL           *
 * Version of    : 27 January 2004                                    *
 *--------------------------------------------------------------------*
 * The C function FCVJtimes is to interface between the CVSPGMR module*
 * and the user-supplied Jacobian-times-vector routine FCVJTIMES.     *
 * Note the use of the generic name FCV_JTIMES below.                 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "sundialstypes.h" /* definitions of types realtype and integertype   */
#include "nvector.h"       /* definitions of type N_Vector and vector macros  */
#include "fcvode.h"        /* actual function names, prototypes, global vars. */
#include "cvspgmr.h"       /* CVSpgmr prototype                               */

/* Prototype of the Fortran routine */
void FCV_JTIMES(realtype*, realtype*, realtype*, realtype*, 
                realtype*, realtype*, int*);

/***************************************************************************/

void FCV_SPGMRSETJAC(int *flag, int *ier)
{
  if (*flag == 0) CVSpgmrSetJacTimesVecFn(CV_cvodemem, NULL);
  else            CVSpgmrSetJacTimesVecFn(CV_cvodemem, FCVJtimes);
}

/***************************************************************************/

/* C function  FCVJtimes to interface between CVODE and  user-supplied
   Fortran routine FCVJTIMES for Jacobian * vector product.
   Addresses of v, Jv, t, y, fy, vnrm, ewt, h, uround, ytemp, and
   the address nfePtr, are passed to FCVJTIMES, using the routine
   N_VGetData from NVECTOR. A return flag ier from FCVJTIMES is 
   returned by FCVJtimes.
   Auxiliary data is assumed to be communicated by common blocks. */

int FCVJtimes(N_Vector v, N_Vector Jv, realtype t, 
              N_Vector y, N_Vector fy,
              void *jac_data, N_Vector work)
{

  realtype *vdata, *Jvdata, *ydata, *fydata, *wkdata;
  int ier = 0;

  vdata = N_VGetData(v);
  Jvdata = N_VGetData(Jv);
  ydata = N_VGetData(y);
  fydata = N_VGetData(fy);
  wkdata = N_VGetData(work);

  FCV_JTIMES (vdata, Jvdata, &t, ydata, fydata, wkdata, &ier);

  N_VSetData(Jvdata, Jv);

  return(ier);
}
