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
 * This is the implementation file for the SUNControl_ImpGus module.
 * -----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <suncontrol/suncontrol_impgus.h>
#include <sundials/sundials_math.h>


/* ---------------
 * Macro accessors
 * --------------- */

#define SC_IMPGUS_CONTENT(C)     ( (SUNControlContent_ImpGus)(C->content) )
#define SC_IMPGUS_K1(C)          ( SC_IMPGUS_CONTENT(C)->k1 )
#define SC_IMPGUS_K2(C)          ( SC_IMPGUS_CONTENT(C)->k2 )
#define SC_IMPGUS_BIAS(C)        ( SC_IMPGUS_CONTENT(C)->bias )
#define SC_IMPGUS_SAFETY(C)      ( SC_IMPGUS_CONTENT(C)->safety )
#define SC_IMPGUS_EP(C)          ( SC_IMPGUS_CONTENT(C)->ep )
#define SC_IMPGUS_HP(C)          ( SC_IMPGUS_CONTENT(C)->hp )
#define SC_IMPGUS_P(C)           ( SC_IMPGUS_CONTENT(C)->p )
#define SC_IMPGUS_PQ(C)          ( SC_IMPGUS_CONTENT(C)->pq )
#define SC_IMPGUS_FIRSTSTEP(C)   ( SC_IMPGUS_CONTENT(C)->firststep )

/* ------------------
 * Default parameters
 * ------------------ */

#define DEFAULT_K1     RCONST(0.98)
#define DEFAULT_K2     RCONST(0.95)
#define DEFAULT_BIAS   RCONST(1.5)
#define DEFAULT_SAFETY RCONST(0.96)
#define DEFAULT_PQ     SUNFALSE
#define TINY           RCONST(1.0e-10)


/* -----------------------------------------------------------------
 * exported functions
 * ----------------------------------------------------------------- */

/* -----------------------------------------------------------------
 * Function to create a new ImpGus controller
 */

SUNControl SUNControlImpGus(SUNContext sunctx)
{
  SUNControl C;
  SUNControlContent_ImpGus content;

  /* Create an empty controller object */
  C = NULL;
  C = SUNControlNewEmpty(sunctx);
  if (C == NULL) { return (NULL); }

  /* Attach operations */
  C->ops->getid             = SUNControlGetID_ImpGus;
  C->ops->destroy           = SUNControlDestroy_ImpGus;
  C->ops->estimatestep      = SUNControlEstimateStep_ImpGus;
  C->ops->reset             = SUNControlReset_ImpGus;
  C->ops->setdefaults       = SUNControlSetDefaults_ImpGus;
  C->ops->write             = SUNControlWrite_ImpGus;
  C->ops->setmethodorder    = SUNControlSetMethodOrder_ImpGus;
  C->ops->setembeddingorder = SUNControlSetMethodOrder_ImpGus;
  C->ops->setsafetyfactor   = SUNControlSetSafetyFactor_ImpGus;
  C->ops->seterrorbias      = SUNControlSetErrorBias_ImpGus;
  C->ops->update            = SUNControlUpdate_ImpGus;
  C->ops->space             = SUNControlSpace_ImpGus;

  /* Create content */
  content = NULL;
  content = (SUNControlContent_ImpGus)malloc(sizeof *content);
  if (content == NULL)
  {
    SUNControlDestroy(C);
    return (NULL);
  }

  /* Attach content */
  C->content = content;

  /* Initialize method order */
  content->p = 1;

  /* Fill content with default/reset values */
  SUNControlSetDefaults_ImpGus(C);
  SUNControlReset_ImpGus(C);

  return (C);
}

/* -----------------------------------------------------------------
 * Function to set ImpGus parameters
 */

int SUNControlImpGus_SetParams(SUNControl C, sunbooleantype pq,
                               realtype k1, realtype k2)
{
  /* store legal inputs, and return with success */
  SC_IMPGUS_PQ(C) = pq;
  if (k1 >= RCONST(0.0)) { SC_IMPGUS_K1(C) = k1; }
  if (k2 >= RCONST(0.0)) { SC_IMPGUS_K2(C) = k2; }
  return SUNCONTROL_SUCCESS;
}



/* -----------------------------------------------------------------
 * implementation of controller operations
 * ----------------------------------------------------------------- */

SUNControl_ID SUNControlGetID_ImpGus(SUNControl C) { return SUNDIALS_CONTROL_H; }

void SUNControlDestroy_ImpGus(SUNControl C)
{
  if (C == NULL) { return; }

  /* free content */
  if (C->content != NULL) {
    free(C->content);
    C->content = NULL;
  }

  /* free ops and controller */
  if (C->ops) {
    free(C->ops);
    C->ops = NULL;
  }
  free(C);
  C = NULL;

  return;
}

int SUNControlEstimateStep_ImpGus(SUNControl C, realtype h,
                               realtype dsm, realtype* hnew)
{
  /* modified method for first step */
  if (SC_IMPGUS_FIRSTSTEP(C))
  {
    /* set usable time-step adaptivity parameters */
    const realtype k = -RCONST(1.0) / SC_IMPGUS_P(C);
    const realtype e = SUNMAX(SC_IMPGUS_BIAS(C) * dsm, TINY);

    /* compute estimated optimal time step size */
    *hnew = SC_IMPGUS_SAFETY(C) * h * SUNRpowerR(e,k);
  }
  else
  {
    /* set usable time-step adaptivity parameters */
    const realtype k1 = -SC_IMPGUS_K1(C) / SC_IMPGUS_P(C);
    const realtype k2 = -SC_IMPGUS_K2(C) / SC_IMPGUS_P(C);
    const realtype ecur = SC_IMPGUS_BIAS(C) * dsm;
    const realtype e1 = SUNMAX(ecur, TINY);
    const realtype e2 = e1 / SUNMAX(SC_IMPGUS_EP(C), TINY);
    const realtype hrat = h / SC_IMPGUS_HP(C);

    /* compute estimated optimal time step size */
    *hnew = SC_IMPGUS_SAFETY(C) * h * hrat * SUNRpowerR(e1,k1) * SUNRpowerR(e2,k2);
  }

  /* return with success */
  return SUNCONTROL_SUCCESS;
}

int SUNControlReset_ImpGus(SUNControl C)
{
  SC_IMPGUS_EP(C) = RCONST(1.0);
  SC_IMPGUS_FIRSTSTEP(C) = SUNTRUE;
}

int SUNControlSetDefaults_ImpGus(SUNControl C)
{
  SC_IMPGUS_K1(C)     = DEFAULT_K1;
  SC_IMPGUS_K2(C)     = DEFAULT_K2;
  SC_IMPGUS_BIAS(C)   = DEFAULT_BIAS;
  SC_IMPGUS_SAFETY(C) = DEFAULT_SAFETY;
  SC_IMPGUS_PQ(C)     = DEFAULT_PQ;
}

int SUNControlWrite_ImpGus(SUNControl C, FILE *fptr)
{
  fprintf(fptr, "SUNControl_ImpGus module:\n");
#if defined(SUNDIALS_EXTENDED_PRECISION)
  fprintf(fptr, "  k1 = %12Lg\n", SC_IMPGUS_K1(C));
  fprintf(fptr, "  k2 = %12Lg\n", SC_IMPGUS_K2(C));
  fprintf(fptr, "  bias = %12Lg\n", SC_IMPGUS_BIAS(C));
  fprintf(fptr, "  safety = %12Lg\n", SC_IMPGUS_SAFETY(C));
  fprintf(fptr, "  ep = %12Lg\n", SC_IMPGUS_EP(C));
  fprintf(fptr, "  hp = %12Lg\n", SC_IMPGUS_HP(C));
#elif defined(SUNDIALS_DOUBLE_PRECISION)
  fprintf(fptr, "  k1 = %12g\n", SC_IMPGUS_K1(C));
  fprintf(fptr, "  k2 = %12g\n", SC_IMPGUS_K2(C));
  fprintf(fptr, "  bias = %12g\n", SC_IMPGUS_BIAS(C));
  fprintf(fptr, "  safety = %12g\n", SC_IMPGUS_SAFETY(C));
  fprintf(fptr, "  ep = %12g\n", SC_IMPGUS_EP(C));
  fprintf(fptr, "  hp = %12g\n", SC_IMPGUS_HP(C));
#else
  fprintf(fptr, "  k1 = %12g\n", SC_IMPGUS_K1(C));
  fprintf(fptr, "  k2 = %12g\n", SC_IMPGUS_K2(C));
  fprintf(fptr, "  bias = %12g\n", SC_IMPGUS_BIAS(C));
  fprintf(fptr, "  safety = %12g\n", SC_IMPGUS_SAFETY(C));
  fprintf(fptr, "  ep = %12g\n", SC_IMPGUS_EP(C));
  fprintf(fptr, "  hp = %12g\n", SC_IMPGUS_HP(C));
#endif
  if (SC_IMPGUS_PQ(C))
  {
    fprintf(fptr, "  p = %i (method order)\n", SC_IMPGUS_PI(C));
  }
  else
  {
    fprintf(fptr, "  p = %i (embedding order)\n", SC_IMPGUS_PI(C));
  }
  return SUNCONTROL_SUCCESS;
}

int SUNControlSetMethodOrder_ImpGus(SUNControl C, int q)
{
  /* check for legal input */
  if (q <= 0) { return SUNCONTROL_ILL_INPUT; }

  /* store if "pq" specifies to use method order */
  if (SC_IMPGUS_PQ(C)) { SC_IMPGUS_P(C) = q; }
  return SUNCONTROL_SUCCESS;
}

int SUNControlSetEmbeddingOrder_ImpGus(SUNControl C, int p)
{
  /* check for legal input */
  if (p <= 0) { return SUNCONTROL_ILL_INPUT; }

  /* store if "pq" specifies to use method order */
  if (!SC_IMPGUS_PQ(C)) { SC_IMPGUS_P(C) = p; }
  return SUNCONTROL_SUCCESS;
}

int SUNControlSetSafetyFactor_ImpGus(SUNControl C, realtype safety)
{
  /* check for legal input */
  if (safety >= RCONST(1.0)) { return SUNCONTROL_ILL_INPUT; }

  /* set positive-valued parameters, otherwise set default */
  if (safety <= RCONST(0.0)) {
    SC_IMPGUS_SAFETY(C) = DEFAULT_SAFETY;
  } else {
    SC_IMPGUS_SAFETY(C) = safety;
  }

  return SUNCONTROL_SUCCESS;
}

int SUNControlSetErrorBias_ImpGus(SUNControl C, realtype bias)
{
  /* set allowed value, otherwise set default */
  if (bias <= RCONST(0.0)) {
    SC_IMPGUS_BIAS(C) = DEFAULT_BIAS;
  } else {
    SC_IMPGUS_BIAS(C) = bias;
  }

  return SUNCONTROL_SUCCESS;
}

int SUNControlUpdate_ImpGus(SUNControl C, realtype h, realtype dsm)
{
  SC_IMPGUS_EP(C) = SC_IMPGUS_BIAS(C) * dsm;
  SC_IMPGUS_FIRSTSTEP(C) = SUNFALSE;
  return SUNCONTROL_SUCCESS;
}

int SUNControlSpace_ImpGus(SUNControl C, long int* lenrw, long int* leniw)
{
  *lenrw = 6;
  *leniw = 3;
  return SUNCONTROL_SUCCESS;
}
