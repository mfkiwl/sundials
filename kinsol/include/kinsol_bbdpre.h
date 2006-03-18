/*
 * -----------------------------------------------------------------
 * $Revision: 1.5 $
 * $Date: 2006-03-18 01:54:40 $
 * -----------------------------------------------------------------
 * Programmer(s): Allan Taylor, Alan Hindmarsh, Radu Serban, and
 *                Aaron Collier @ LLNL
 * -----------------------------------------------------------------
 * Copyright (c) 2002, The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see sundials/kinsol/LICENSE.
 * -----------------------------------------------------------------
 * This is the header file for the KINBBDPRE module, for a
 * band-block-diagonal preconditioner, i.e. a block-diagonal
 * matrix with banded blocks, for use with KINSol, KINSp*,
 * and the parallel implementaion of the NVECTOR module.
 *
 * Summary:
 *
 * These routines provide a preconditioner matrix for KINSol that
 * is block-diagonal with banded blocks. The blocking corresponds
 * to the distribution of the dependent variable vector u amongst
 * the processes. Each preconditioner block is generated from
 * the Jacobian of the local part (associated with the current
 * process) of a given function g(u) approximating f(u). The blocks
 * are generated by each process via a difference quotient scheme,
 * utilizing the assumed banded structure with given half-bandwidths,
 * mudq and mldq. However, the banded Jacobian block kept by the
 * scheme has half-bandwidths mukeep and mlkeep, which may be smaller.
 *
 * The user's calling program should have the following form:
 *
 *   #include "sundialstypes.h"
 *   #include "sundialsmath.h"
 *   #include "iterative.h"
 *   #include "nvector_parallel.h"
 *   #include "kinsol.h"
 *   #include "kinbbdpre.h"
 *   ...
 *   void *p_data;
 *   ...
 *   MPI_Init(&argc,&argv);
 *   ...
 *   tmpl = N_VNew_Parallel(...);
 *   ...
 *   kin_mem = KINCreate();
 *   KINMalloc(kin_mem,...,tmpl);
 *   ...
 *   p_data = KINBBDPrecAlloc(kin_mem,...);
 *   ...
 *   KINBBDSptfqmr(kin_mem,...,p_data);
 *         -or-
 *   KINBBDSpbcg(kin_mem,...,p_data);
 *         -or-
 *   KINBBDSpgmr(kin_mem,...,p_data);
 *   ...
 *   KINSol(kin_mem,...);
 *   ...
 *   KINBBDPrecFree(&p_data);
 *   ...
 *   KINFree(kin_mem);
 *   ...
 *   N_VDestroy_Parallel(tmpl);
 *   ...
 *   MPI_Finalize();
 *
 * The user-supplied routines required are:
 *
 *  func    the function f(u) defining the system to be solved:
 *          f(u) = 0
 *
 *  glocal  the function defining the approximation g(u) to f(u)
 *
 *  gcomm   the function to do necessary communication for glocal
 *
 * Notes:
 *
 * 1) This header file (kinbbdpre.h) is included by the user for
 *    the definition of the KBBDData data type and for needed
 *    function prototypes.
 *
 * 2) The KINBBDPrecAlloc call includes half-bandwiths mudq and mldq
 *    to be used in the difference quotient calculation of the
 *    approximate Jacobian. They need not be the true half-bandwidths
 *    of the Jacobian of the local block of g, when smaller values may
 *    provide greater efficiency. Also, the half-bandwidths mukeep and
 *    mlkeep of the retained banded approximate Jacobian block may be
 *    even smaller, to furhter reduce storage and computational costs.
 *    For all four half-bandwidths, the values need not be the same
 *    for every process.
 *
 * 3) The actual name of the user's f function is passed to
 *    KINMalloc, and the names of the user's glocal and gcomm
 *    functions are passed to KINBBDPrecAlloc.
 *
 * 4) Optional outputs specific to this module are available by
 *    way of the functions listed below. These include work space
 *    sizes and the cumulative number of glocal calls.
 * -----------------------------------------------------------------
 */

#ifndef _KBBDPRE_H
#define _KBBDPRE_H

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#include "sundials_nvector.h"

/* KINBBDPRE return values */

#define KINBBDPRE_SUCCESS          0
#define KINBBDPRE_PDATA_NULL     -11
#define KINBBDPRE_FUNC_UNRECVR   -12
/*
 * -----------------------------------------------------------------
 * Type : KINCommFn
 * -----------------------------------------------------------------
 * The user must supply a function of type KINCommFn which
 * performs all inter-process communication necessary to
 * evaluate the approximate system function described above.
 *
 * This function takes as input the local vector size Nlocal,
 * the solution vector u, and a pointer to the user-defined
 * data block f_data.
 *
 * The KINCommFn gcomm is expected to save communicated data in
 * space defined with the structure *f_data.
 *
 * Each call to the KINCommFn is preceded by a call to the system
 * function func at the current iterate uu. Thus functions of the
 * type KINCommFn can omit any communications done by f (func) if
 * relevant to the evaluation of the KINLocalFn function. If all
 * necessary communication was done in func, the user can pass
 * NULL for gcomm in the call to KINBBDPrecAlloc (see below).
 *
 * A KINCommFn function should return 0 if successful or
 * a non-zero value if an error occured.
 * -----------------------------------------------------------------
 */

typedef int (*KINCommFn)(long int Nlocal, N_Vector u, void *f_data);

/*
 * -----------------------------------------------------------------
 * Type : KINLocalFn
 * -----------------------------------------------------------------
 * The user must supply a function g(u) which approximates the
 * function f for the system f(u) = 0, and which is computed
 * locally (without inter-process communication). Note: The case
 * where g is mathematically identical to f is allowed.
 *
 * The implementation of this function must have type KINLocalFn
 * and take as input the local vector size Nlocal, the local
 * solution vector uu, the returned local g values vector, and a
 * pointer to the user-defined data block f_data. It is to
 * compute the local part of g(u) and store the result in the
 * vector gval. (Note: Memory for uu and gval is handled within the
 * preconditioner module.) It is expected that this routine will
 * save communicated data in work space defined by the user and
 * made available to the preconditioner function for the problem.
 *
 * A KINLocalFn function should return 0 if successful or
 * a non-zero value if an error occured.
 * -----------------------------------------------------------------
 */

typedef int (*KINLocalFn)(long int Nlocal, N_Vector uu,
                          N_Vector gval, void *f_data);

/*
 * -----------------------------------------------------------------
 * Function : KINBBDPrecAlloc
 * -----------------------------------------------------------------
 * KINBBDPrecAlloc allocates and initializes a KBBDData data
 * structure to be passed to KINSpgmr/KINSpbcg (and then used by
 * KINBBDPrecSetup and KINBBDPrecSolve).
 *
 * The parameters of KINBBDPrecAlloc are as follows:
 *
 * kinmem  is a pointer to the KINSol memory block.
 *
 * Nlocal  is the length of the local block of the vectors
 *         on the current process.
 *
 * mudq, mldq  are the upper and lower half-bandwidths to be used
 *             in the computation of the local Jacobian blocks.
 *
 * mukeep, mlkeep  are the upper and lower half-bandwidths of the
 *                 retained banded approximation to the local
 *                 Jacobian block.
 *
 * dq_rel_uu  is the relative error to be used in the difference
 *            quotient Jacobian calculation in the preconditioner.
 *            The default is sqrt(unit roundoff), obtained by
 *            passing 0.
 *
 * gloc    is the name of the user-supplied function g(u) that
 *         approximates f and whose local Jacobian blocks are
 *         to form the preconditioner.
 *
 * gcomm   is the name of the user-defined function that performs
 *         necessary inter-process communication for the
 *         execution of gloc.
 *
 * Note: KINBBDPrecAlloc returns a pointer to the storage allocated,
 * or NULL if the request for storage cannot be satisfied.
 * -----------------------------------------------------------------
 */

void *KINBBDPrecAlloc(void *kinmem, long int Nlocal, 
		      long int mudq, long int mldq,
		      long int mukeep, long int mlkeep,
		      realtype dq_rel_uu, 
		      KINLocalFn gloc, KINCommFn gcomm);

/*
 * -----------------------------------------------------------------
 * Function : KINBBDSptfqmr
 * -----------------------------------------------------------------
 * KINBBDSptfqmr links the KINBBDPRE preconditioner to the KINSPTFQMR
 * linear solver. It performs the following actions:
 *  1) Calls the KINSPTFQMR specification routine and attaches the
 *     KINSPTFQMR linear solver to the KINSOL solver;
 *  2) Sets the preconditioner data structure for KINSPTFQMR
 *  3) Sets the preconditioner setup routine for KINSPTFQMR
 *  4) Sets the preconditioner solve routine for KINSPTFQMR
 *
 * Its first 2 arguments are the same as for KINSptfqmr (see
 * kinsptfqmr.h). The last argument is the pointer to the KBBDPRE
 * memory block returned by KINBBDPrecAlloc.
 *
 * Possible return values are:
 *   (from kinsptfqmr.h) KINSPTFQMR_SUCCESS
 *                       KINSPTFQMR_MEM_NULL
 *                       KINSPTFQMR_LMEM_NULL
 *                       KINSPTFQMR_MEM_FAIL
 *                       KINSPTFQMR_ILL_INPUT
 *
 *   Additionaly, if KINBBDPrecAlloc was not previously called,
 *   KINBBDSptfqmr returns KINBBDPRE_PDATA_NULL.
 * -----------------------------------------------------------------
 */

int KINBBDSptfqmr(void *kinmem, int maxl, void *p_data);

/*
 * -----------------------------------------------------------------
 * Function : KINBBDSpbcg
 * -----------------------------------------------------------------
 * KINBBDSpbcg links the KINBBDPRE preconditioner to the KINSPBCG
 * linear solver. It performs the following actions:
 *  1) Calls the KINSPBCG specification routine and attaches the
 *     KINSPBCG linear solver to the KINSOL solver;
 *  2) Sets the preconditioner data structure for KINSPBCG
 *  3) Sets the preconditioner setup routine for KINSPBCG
 *  4) Sets the preconditioner solve routine for KINSPBCG
 *
 * Its first 2 arguments are the same as for KINSpbcg (see
 * kinspbcg.h). The last argument is the pointer to the KBBDPRE
 * memory block returned by KINBBDPrecAlloc.
 *
 * Possible return values are:
 *   (from kinspbcg.h) KINSPBCG_SUCCESS
 *                     KINSPBCG_MEM_NULL
 *                     KINSPBCG_LMEM_NULL
 *                     KINSPBCG_MEM_FAIL
 *                     KINSPBCG_ILL_INPUT
 *
 *   Additionaly, if KINBBDPrecAlloc was not previously called,
 *   KINBBDSpbcg returns KINBBDPRE_PDATA_NULL.
 * -----------------------------------------------------------------
 */

int KINBBDSpbcg(void *kinmem, int maxl, void *p_data);

/*
 * -----------------------------------------------------------------
 * Function : KINBBDSpgmr
 * -----------------------------------------------------------------
 * KINBBDSpgmr links the KINBBDPRE preconditioner to the KINSPGMR
 * linear solver. It performs the following actions:
 *  1) Calls the KINSPGMR specification routine and attaches the
 *     KINSPGMR linear solver to the KINSOL solver;
 *  2) Sets the preconditioner data structure for KINSPGMR
 *  3) Sets the preconditioner setup routine for KINSPGMR
 *  4) Sets the preconditioner solve routine for KINSPGMR
 *
 * Its first 2 arguments are the same as for KINSpgmr (see
 * kinspgmr.h). The last argument is the pointer to the KBBDPRE
 * memory block returned by KINBBDPrecAlloc.
 *
 * Possible return values are:
 *   (from kinspgmr.h) KINSPGMR_SUCCESS
 *                     KINSPGMR_MEM_NULL
 *                     KINSPGMR_LMEM_NULL
 *                     KINSPGMR_MEM_FAIL
 *                     KINSPGMR_ILL_INPUT
 *
 *   Additionaly, if KINBBDPrecAlloc was not previously called,
 *   KINBBDSpgmr returns KINBBDPRE_PDATA_NULL.
 * -----------------------------------------------------------------
 */

int KINBBDSpgmr(void *kinmem, int maxl, void *p_data);

/*
 * -----------------------------------------------------------------
 * Function : KINBBDPrecFree
 * -----------------------------------------------------------------
 * KINBBDPrecFree frees the memory block p_data allocated by the
 * call to KINBBDPrecAlloc.
 * -----------------------------------------------------------------
 */

void KINBBDPrecFree(void **p_data);

/*
 * -----------------------------------------------------------------
 * Function : KINBBDPrecGet*
 *
 * The return value of KINBBDPrecGet* is one of:
 *    KINBBDPRE_SUCCESS    if successful
 *    KINBBDPRE_PDATA_NULL if the p_data memory was NULL
 * -----------------------------------------------------------------
 */

int KINBBDPrecGetWorkSpace(void *p_data, long int *lenrwBBDP, long int *leniwBBDP);
int KINBBDPrecGetNumGfnEvals(void *p_data, long int *ngevalsBBDP);

/*
 * -----------------------------------------------------------------
 * The following function returns the name of the constant 
 * associated with a KINBBDPRE return flag
 * -----------------------------------------------------------------
 */

char *KINBBDPrecGetReturnFlagName(int flag);

/* prototypes for functions KINBBDPrecSetup and KINBBDPrecSolve */

int KINBBDPrecSetup(N_Vector uu, N_Vector uscale,
		    N_Vector fval, N_Vector fscale, 
		    void *p_data,
		    N_Vector vtemp1, N_Vector vtemp2);

int KINBBDPrecSolve(N_Vector uu, N_Vector uscale,
		    N_Vector fval, N_Vector fscale, 
		    N_Vector vv, void *p_data,
		    N_Vector vtemp);

#ifdef __cplusplus
}
#endif

#endif
