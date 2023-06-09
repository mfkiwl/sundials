/* -----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 * -----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2023, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------
 * SUNDIALS accuracy-based adaptivity controller class. These
 * objects estimate step sizes for time integration methods such
 * that the next step solution should satisfy a desired temporal
 * accuracy.
 * ----------------------------------------------------------------*/

#ifndef _SUNDIALS_CONTROLLER_H
#define _SUNDIALS_CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <sundials/sundials_context.h>
#include "sundials/sundials_types.h"

#ifdef __cplusplus /* wrapper to enable C++ usage */
extern "C" {
#endif

/* -----------------------------------------------------------------
 * SUNControl types (currently, only "H" is implemented; others
 * are planned):
 *    H       - controls a single-rate step size
 *    HQ      - controls a single-rate step size and method order
 *    MRI_H   - controls two multirate step sizes
 *    MRI_M   - controls slow step size and multirate ratio
 *    MRI_TOL - controls slow and fast relative tolerances
 * ----------------------------------------------------------------- */

typedef enum
{
  SUNDIALS_CONTROL_H,
  SUNDIALS_CONTROL_HQ,
  SUNDIALS_CONTROL_MRI_H,
  SUNDIALS_CONTROL_MRI_M,
  SUNDIALS_CONTROL_MRI_TOL
} SUNControl_ID;

/* -----------------------------------------------------------------
 * Generic definition of SUNControl
 * ----------------------------------------------------------------- */

/* Forward reference for pointer to SUNControl_Ops object */
typedef _SUNDIALS_STRUCT_ _generic_SUNControl_Ops* SUNControl_Ops;

/* Forward reference for pointer to SUNControl object */
typedef _SUNDIALS_STRUCT_ _generic_SUNControl* SUNControl;

/* Structure containing function pointers to controller operations  */
struct _generic_SUNControl_Ops
{
  /* REQUIRED of all controller implementations. */
  int (*reset)(SUNControl C);
  int (*estimatestep)(SUNControl C, realtype h,
                      realtype dsm, realtype* hnew);

  /* REQUIRED for controllers of SUNDIALS_CONTROL_HQ type. */
  int (*estimatestepandorder)(SUNControl C, realtype h, int q,
                              realtype dsm, realtype* hnew, int *qnew);

  /* REQUIRED for controllers of SUNDIALS_CONTROL_MRI_H type. */
  int (*estimatefaststep)(SUNControl C, realtype hslow, realtype* hfast);

  /* REQUIRED for controllers of SUNDIALS_CONTROL_MRI_M type. */
  int (*estimatemratio)(SUNControl C, realtype hslow, int* mratio);

  /* REQUIRED for controllers of SUNDIALS_CONTROL_MRI_TOL type. */
  int (*estimatefasttol)(SUNControl C, realtype slowrtol, realtype* fastrtol);

  /* OPTIONAL for all SUNControl implementations. */
  int (*setdefaultparameters)(SUNControl C);
  int (*writeparameters)(SUNControl C, FILE* fptr);
  int (*setmethodorder)(SUNControl C, int q);
  int (*setembeddingorder)(SUNControl C, int p);
  int (*setsafetyfactor)(SUNControl C, realtype safety);
  int (*seterrorbias)(SUNControl C, realtype bias);
  int (*update)(SUNControl C, realtype h, realtype dsm);
  int (*space)(SUNControl C, long int *lenrw, long int *leniw);
#ifdef __cplusplus
  _generic_SUNControl_Ops() = default;
#endif

};

/* A SUNControl is a structure with an implementation-dependent
   'content' field, and a pointer to a structure of
   operations corresponding to that implementation. */
struct _generic_SUNControl
{
  void* content;
  SUNControl_Ops ops;
  SUNContext sunctx;
#ifdef __cplusplus
  _generic_SUNControl() = default;
#endif
};

/* -----------------------------------------------------------------
 * Functions exported by SUNControl module
 * ----------------------------------------------------------------- */

SUNDIALS_EXPORT SUNControl SUNControlNewEmpty(SUNContext sunctx);
SUNDIALS_EXPORT void SUNControlFreeEmpty(SUNControl C);
SUNDIALS_EXPORT SUNControl_ID SUNControlGetID(SUNControl C);
SUNDIALS_EXPORT int SUNControlReset(SUNControl C);
SUNDIALS_EXPORT int SUNControlEstimateStep(
                       SUNControl C, realtype h, realtype dsm,
                       realtype* hnew);
SUNDIALS_EXPORT int SUNControlEstimateStepAndOrder(
                       SUNControl C, realtype h, int q,
                       realtype dsm, realtype* hnew, int *qnew);
SUNDIALS_EXPORT int SUNControlEstimateFastStep(
                       SUNControl C, realtype hslow, realtype* hfast);
SUNDIALS_EXPORT int SUNControlEstimateMRatio(
                       SUNControl C, realtype hslow, int* mratio);
SUNDIALS_EXPORT int SUNControlEstimateFastTol(
                       SUNControl C, realtype stol, realtype* ftol);
SUNDIALS_EXPORT int SUNControlSetDefaultParameters(SUNControl C);
SUNDIALS_EXPORT int SUNControlWriteParameters(SUNControl C, FILE* fptr);
SUNDIALS_EXPORT int SUNControlSetMethodOrder(SUNControl C, int q);
SUNDIALS_EXPORT int SUNControlSetEmbeddingOrder(SUNControl C, int p);
SUNDIALS_EXPORT int SUNControlSetSafetyFactor(SUNControl C, realtype safety);
SUNDIALS_EXPORT int SUNControlSetErrorBias(SUNControl C, realtype bias);
SUNDIALS_EXPORT int SUNControlUpdate(SUNControl C, realtype h, realtype dsm);
SUNDIALS_EXPORT int SUNControlSpace(
                       SUNControl C, long int *lenrw, long int *leniw);


/* -----------------------------------------------------------------
 * SUNControl error codes
 * ----------------------------------------------------------------- */

#define SUNCONTROL_SUCCESS           0     /* function successfull        */
#define SUNCONTROL_ILL_INPUT         -1001 /* illegal function input      */
#define SUNCONTROL_MEM_FAIL          -1002 /* failed memory access/alloc  */
#define SUNCONTROL_OPERATION_FAIL    -1003 /* catchall failure code       */

#ifdef __cplusplus
}
#endif

#endif /* _SUNDIALS_CONTROLLER_H */
