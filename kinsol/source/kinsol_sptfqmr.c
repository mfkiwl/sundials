/*
 * -----------------------------------------------------------------
 * $Revision: 1.6 $
 * $Date: 2006-03-18 01:54:42 $
 * -----------------------------------------------------------------
 * Programmer(s): Aaron Collier and Radu Serban @ LLNL
 * -----------------------------------------------------------------
 * Copyright (c) 2005, The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see sundials/kinsol/LICENSE.
 * -----------------------------------------------------------------
 * This is the implementation file for the KINSOL interface to the
 * scaled, preconditioned TFQMR (SPTFQMR) iterative linear solver.
 * -----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "kinsol_impl.h"
#include "kinsol_sptfqmr.h"
#include "kinsol_spils_impl.h"

#include "sundials_sptfqmr.h"
#include "sundials_math.h"

/*
 * -----------------------------------------------------------------
 * private constants
 * -----------------------------------------------------------------
 */

#define ZERO RCONST(0.0)

/*
 * -----------------------------------------------------------------
 * function prototypes
 * -----------------------------------------------------------------
 */

/* KINSptfqmr linit, lsetup, lsolve, and lfree routines */

static int KINSptfqmrInit(KINMem kin_mem);
static int KINSptfqmrSetup(KINMem kin_mem);
static int KINSptfqmrSolve(KINMem kin_mem, N_Vector xx,
			   N_Vector bb, realtype *res_norm);
static void KINSptfqmrFree(KINMem kin_mem);

/*
 * -----------------------------------------------------------------
 * readability replacements
 * -----------------------------------------------------------------
 */

#define nni          (kin_mem->kin_nni)
#define nnilset      (kin_mem->kin_nnilset)
#define func         (kin_mem->kin_func)
#define f_data       (kin_mem->kin_f_data)
#define printfl      (kin_mem->kin_printfl)
#define linit        (kin_mem->kin_linit)
#define lsetup       (kin_mem->kin_lsetup)
#define lsolve       (kin_mem->kin_lsolve)
#define lfree        (kin_mem->kin_lfree)
#define lmem         (kin_mem->kin_lmem)
#define inexact_ls     (kin_mem->kin_inexact_ls)
#define uu           (kin_mem->kin_uu)
#define fval         (kin_mem->kin_fval)
#define uscale       (kin_mem->kin_uscale)
#define fscale       (kin_mem->kin_fscale)
#define sqrt_relfunc (kin_mem->kin_sqrt_relfunc)
#define eps          (kin_mem->kin_eps)
#define sJpnorm      (kin_mem->kin_sJpnorm)
#define sfdotJp      (kin_mem->kin_sfdotJp)
#define errfp        (kin_mem->kin_errfp)
#define infofp       (kin_mem->kin_infofp)
#define setupNonNull (kin_mem->kin_setupNonNull)
#define vtemp1       (kin_mem->kin_vtemp1)
#define vec_tmpl     (kin_mem->kin_vtemp1)
#define vtemp2       (kin_mem->kin_vtemp2)

#define pretype     (kinspils_mem->s_pretype)
#define nli         (kinspils_mem->s_nli)
#define npe         (kinspils_mem->s_npe)
#define nps         (kinspils_mem->s_nps)
#define ncfl        (kinspils_mem->s_ncfl)
#define njtimes     (kinspils_mem->s_njtimes)
#define nfes        (kinspils_mem->s_nfes)
#define new_uu      (kinspils_mem->s_new_uu)
#define spils_mem   (kinspils_mem->s_spils_mem)
#define last_flag   (kinspils_mem->s_last_flag)

/*
 * -----------------------------------------------------------------
 * Function : KINSptfqmr
 * -----------------------------------------------------------------
 * This routine allocates and initializes the memory record and
 * sets function fields specific to the SPTFQMR linear solver module.
 * KINSptfqmr sets the kin_linit, kin_lsetup, kin_lsolve, and
 * kin_lfree fields in *kinmem to be KINSptfqmrInit, KINSptfqmrSetup,
 * KINSptfqmrSolve, and KINSptfqmrFree, respectively. It allocates
 * memory for a structure of type KINSpilsMemRec and sets the
 * kin_lmem field in *kinmem to the address of this structure. It
 * also calls SptfqmrMalloc to allocate memory for the module
 * SPTFQMR. In summary, KINSptfqmr sets the following fields in the
 * KINSpilsMemRec structure:
 *
 *  s_pretype   = PREC_NONE
 *  s_maxl      = KINSPILS_MAXL  if maxl <= 0
 *              = maxl             if maxl >  0
 *  s_pset      = NULL
 *  s_psolve    = NULL
 *  s_P_data    = NULL
 *  s_jtimes    = NULL
 *  s_J_data    = NULL
 *  s_last_flag = KINSPILS_SUCCESS
 * -----------------------------------------------------------------
 */

int KINSptfqmr(void *kinmem, int maxl)
{
  KINMem kin_mem;
  KINSpilsMem kinspils_mem;
  SptfqmrMem sptfqmr_mem;
  int maxl1;

  if (kinmem == NULL){
    KINProcessError(NULL, KINSPILS_MEM_NULL, "KINSPILS", "KINSptfqmr", MSGS_KINMEM_NULL);
    return(KINSPILS_MEM_NULL);  
  }
  kin_mem = (KINMem) kinmem;

  /* check for required vector operations */

  /* Note: do NOT need to check for N_VLinearSum, N_VProd, N_VScale, N_VDiv, 
     or N_VWL2Norm because they are required by KINSOL */

  if ((vec_tmpl->ops->nvconst == NULL) ||
      (vec_tmpl->ops->nvdotprod == NULL) ||
      (vec_tmpl->ops->nvl1norm == NULL)) {
    KINProcessError(NULL, KINSPILS_ILL_INPUT, "KINSPILS", "KINSptfqmr", MSGS_BAD_NVECTOR);
    return(KINSPILS_ILL_INPUT);
  }

  if (lfree != NULL) lfree(kin_mem);

  /* set four main function fields in kin_mem */

  linit  = KINSptfqmrInit; 
  lsetup = KINSptfqmrSetup;
  lsolve = KINSptfqmrSolve;
  lfree  = KINSptfqmrFree;

  /* get memory for KINSpilsMemRec */
  kinspils_mem = NULL;
  kinspils_mem = (KINSpilsMem) malloc(sizeof(KINSpilsMemRec));
  if (kinspils_mem == NULL){
    KINProcessError(NULL, KINSPILS_MEM_FAIL, "KINSPILS", "KINSptfqmr", MSGS_MEM_FAIL);
    return(KINSPILS_MEM_FAIL);  
  }

  /* Set ILS type */
  kinspils_mem->s_type = SPILS_SPTFQMR;

  /* set SPTFQMR parameters that were passed in call sequence */

  maxl1 = (maxl <= 0) ? KINSPILS_MAXL : maxl;
  kinspils_mem->s_maxl = maxl1;  

  /* set default values for the rest of the SPTFQMR parameters */

  kinspils_mem->s_pretype   = PREC_NONE;
  kinspils_mem->s_last_flag = KINSPILS_SUCCESS;
  kinspils_mem->s_pset      = NULL;
  kinspils_mem->s_psolve    = NULL;
  kinspils_mem->s_P_data    = NULL;
  kinspils_mem->s_jtimes    = NULL;
  kinspils_mem->s_J_data    = NULL;

  /* call SptfqmrMalloc to allocate workspace for SPTFQMR */

  /* vec_tmpl passed as template vector */

  sptfqmr_mem = NULL;
  sptfqmr_mem = SptfqmrMalloc(maxl1, vec_tmpl);
  if (sptfqmr_mem == NULL) {
    KINProcessError(NULL, KINSPILS_MEM_FAIL, "KINSPILS", "KINSptfqmr", MSGS_MEM_FAIL);
    free(kinspils_mem); kinspils_mem = NULL;
    return(KINSPILS_MEM_FAIL);
  }

  /* this is an iterative linear solver */

  inexact_ls = TRUE;

  /* Attach SPTFQMR memory to spils memory structure */
  spils_mem = (void *) sptfqmr_mem;

  /* attach linear solver memory to KINSOL memory */
  lmem = kinspils_mem;

  return(KINSPILS_SUCCESS);
}

/*
 * -----------------------------------------------------------------
 * additional readability replacements
 * -----------------------------------------------------------------
 */

#define maxl   (kinspils_mem->s_maxl)
#define pset   (kinspils_mem->s_pset)
#define psolve (kinspils_mem->s_psolve)
#define P_data (kinspils_mem->s_P_data)
#define jtimes (kinspils_mem->s_jtimes)
#define J_data (kinspils_mem->s_J_data)

/*
 * -----------------------------------------------------------------
 * Function : KINSptfqmrInit
 * -----------------------------------------------------------------
 * This routine initializes variables associated with the SPTFQMR
 * iterative linear solver. Memory allocation was done previously
 * in KINSptfqmr.
 * -----------------------------------------------------------------
 */

static int KINSptfqmrInit(KINMem kin_mem)
{
  KINSpilsMem kinspils_mem;
  SptfqmrMem sptfqmr_mem;

  kinspils_mem = (KINSpilsMem) lmem;
  sptfqmr_mem = (SptfqmrMem) spils_mem;

  /* initialize counters */

  npe = nli = nps = ncfl = 0;
  njtimes = nfes = 0;

  /* set preconditioner type */

  if (psolve != NULL) {
    pretype = PREC_RIGHT;
  } else {
    pretype = PREC_NONE;
  }
  
  /* set setupNonNull to TRUE iff there is preconditioning with setup */

  setupNonNull = ((psolve != NULL) && (pset != NULL));

  /* if jtimes is NULL at this time, set it to private DQ routine */

  if (jtimes == NULL) {
    jtimes = KINSpilsDQJtimes;
    J_data = kin_mem;
  }

  /*  Set maxl in the SPTFQMR memory in case it was changed by the user */
  sptfqmr_mem->l_max  = maxl;

  last_flag = KINSPILS_SUCCESS;
  return(0);
}

/*
 * -----------------------------------------------------------------
 * Function : KINSptfqmrSetup
 * -----------------------------------------------------------------
 * This routine does the setup operations for the SPTFQMR linear
 * solver, that is, it is an interface to the user-supplied
 * routine pset.
 * -----------------------------------------------------------------
 */

static int KINSptfqmrSetup(KINMem kin_mem)
{
  KINSpilsMem kinspils_mem;
  int ret;

  kinspils_mem = (KINSpilsMem) lmem;

  /* call pset routine */

  ret = pset(uu, uscale, fval, fscale, P_data, vtemp1, vtemp2);

  last_flag = ret;

  if (ret != 0) return(1);

  npe++;
  nnilset = nni;

  return(0);
}

/*
 * -----------------------------------------------------------------
 * Function : KINSptfqmrSolve
 * -----------------------------------------------------------------
 * This routine handles the call to the generic SPTFQMR solver routine
 * called SptfqmrSolve for the solution of the linear system Ax = b.
 *
 * Appropriate variables are passed to SptfqmrSolve and the counters
 * nli, nps, and ncfl are incremented, and the return value is set
 * according to the success of SptfqmrSolve. The success flag is
 * returned if SptfqmrSolve converged, or if the residual was reduced.
 * Of the other error conditions, only preconditioner solver
 * failure is specifically returned. Otherwise a generic flag is
 * returned to denote failure of this routine.
 * -----------------------------------------------------------------
 */

static int KINSptfqmrSolve(KINMem kin_mem, N_Vector xx, N_Vector bb, 
			   realtype *res_norm)
{
  KINSpilsMem kinspils_mem;
  SptfqmrMem sptfqmr_mem;
  int ret, nli_inc, nps_inc;
  
  kinspils_mem = (KINSpilsMem) lmem;
  sptfqmr_mem = (SptfqmrMem) spils_mem;

  /* Set initial guess to xx = 0. bb is set, by the routine
     calling KINSptfqmrSolve, to the RHS vector for the system
     to be solved. */ 
 
  N_VConst(ZERO, xx);

  new_uu = TRUE;  /* set flag required for user Jacobian routine */

  /* call SptfqmrSolve */

  ret = SptfqmrSolve(sptfqmr_mem, kin_mem, xx, bb, pretype, eps,
		     kin_mem, fscale, fscale, KINSpilsAtimes,
		     KINSpilsPSolve, res_norm, &nli_inc, &nps_inc);

  /* increment counters nli, nps, and ncfl 
     (nni is updated in the KINSol main iteration loop) */

  nli = nli + (long int) nli_inc;
  nps = nps + (long int) nps_inc;

  if (printfl > 2) 
    KINPrintInfo(kin_mem, PRNT_NLI, "KINSPTFQMR", "KINSptfqmrSolve", INFO_NLI, nli_inc);

  if (ret != 0) ncfl++;

  /* Compute the terms sJpnorm and sfdotJp for use in the global strategy
     routines and in KINForcingTerm. Both of these terms are subsequently
     corrected if the step is reduced by constraints or the line search.

     sJpnorm is the norm of the scaled product (scaled by fscale) of
     the current Jacobian matrix J and the step vector p.

     sfdotJp is the dot product of the scaled f vector and the scaled
     vector J*p, where the scaling uses fscale. */

  KINSpilsAtimes(kin_mem, xx, bb);
  sJpnorm = N_VWL2Norm(bb,fscale);
  N_VProd(bb, fscale, bb);
  N_VProd(bb, fscale, bb);
  sfdotJp = N_VDotProd(fval, bb);

  if (printfl > 2) 
    KINPrintInfo(kin_mem, PRNT_EPS, "KINSPTFQMR", "KINSptfqmrSolve", INFO_EPS, *res_norm, eps);

  /* set return value to appropriate value */

  last_flag = ret;

  if ((ret == SPTFQMR_SUCCESS) || (ret == SPTFQMR_RES_REDUCED)) return(0);
  else if (ret == SPTFQMR_PSOLVE_FAIL_REC) return(1);
  else return(-1);
}

/*
 * -----------------------------------------------------------------
 * Function : KINSptfqmrFree
 * -----------------------------------------------------------------
 * Frees memory specific to the SPTFQMR linear solver module.
 * -----------------------------------------------------------------
 */

static void KINSptfqmrFree(KINMem kin_mem)
{
  KINSpilsMem kinspils_mem;
  SptfqmrMem sptfqmr_mem;

  kinspils_mem = (KINSpilsMem) lmem;
  sptfqmr_mem = (SptfqmrMem) spils_mem;

  SptfqmrFree(sptfqmr_mem);
  free(lmem); lmem = NULL;
}
