/*
 * -----------------------------------------------------------------
 * $Revision: 1.19 $
 * $Date: 2004-07-22 21:14:15 $
 * ----------------------------------------------------------------- 
 * Programmers: Scott D. Cohen, Alan C. Hindmarsh, Radu Serban
 *              and Dan Shumaker @ LLNL
 * -----------------------------------------------------------------
 * Copyright (c) 2002, The Regents of the University of California
 * Produced at the Lawrence Livermore National Laboratory
 * All rights reserved
 * For details, see sundials/cvode/LICENSE
 * -----------------------------------------------------------------
 * This is the interface file for the main CVODE integrator.
 * -----------------------------------------------------------------
 */

#ifdef __cplusplus     /* wrapper to enable C++ usage */
extern "C" {
#endif

#ifndef _cvode_h
#define _cvode_h

#include <stdio.h>
#include "sundialstypes.h"
#include "nvector.h"

/*
 * -----------------------------------------------------------------
 * CVODES is used to solve numerically the ordinary initial value 
 * problem :                                                      
 *                                                                
 *                 y' = f(t,y),                                   
 *                 y(t0) = y0,                                    
 *                                                                
 * where t0, y0 in R^N, and f: R x R^N -> R^N are given.          
 *                                                                
 * -----------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------
 * Enumerations for inputs to CVodeCreate, CVodeMalloc,           
 * CVodeReInit, and CVode.                         
 * -----------------------------------------------------------------
 * Symbolic constants for the lmm, iter, and itol input           
 * parameters to CVodeMalloc and CVodeReInit, as well as the      
 * input parameter itask to CVode, are given below.               
 *                                                                
 * lmm:   The user of the CVODES package specifies whether to use 
 *        the ADAMS or BDF (backward differentiation formula)     
 *        linear multistep method. The BDF method is recommended  
 *        for stiff problems, and the ADAMS method is recommended 
 *        for nonstiff problems.                                  
 *                                                                
 * iter:  At each internal time step, a nonlinear equation must   
 *        be solved. The user can specify either FUNCTIONAL       
 *        iteration, which does not require linear algebra, or a  
 *        NEWTON iteration, which requires the solution of linear 
 *        systems. In the NEWTON case, the user also specifies a  
 *        CVODE linear solver. NEWTON is recommended in case of   
 *        stiff problems.                                         
 *                                                                
 * itol:  This parameter specifies the relative and absolute      
 *        tolerance types to be used. The SS tolerance type means 
 *        a scalar relative and absolute tolerance, while the SV  
 *        tolerance type means a scalar relative tolerance and a  
 *        vector absolute tolerance (a potentially different      
 *        absolute tolerance for each vector component).          
 *                                                                
 * itask: The itask input parameter to CVode indicates the job    
 *        of the solver for the next user step. The NORMAL        
 *        itask is to have the solver take internal steps until   
 *        it has reached or just passed the user specified tout   
 *        parameter. The solver then interpolates in order to     
 *        return an approximate value of y(tout). The ONE_STEP    
 *        option tells the solver to just take one internal step  
 *        and return the solution at the point reached by that    
 *        step. The NORMAL_TSTOP and ONE_STEP_TSTOP modes are     
 *        similar to NORMAL and ONE_STEP, respectively, except    
 *        that the integration never proceeds past the value      
 *        tstop (specified through the routine CVodeSetStopTime). 
 * -----------------------------------------------------------------
 */

enum { ADAMS=1, BDF=2 };                                   /* lmm */

enum { FUNCTIONAL=1, NEWTON=2 };                          /* iter */

enum { SS=1, SV=2 };                                      /* itol */

enum { NORMAL=1, ONE_STEP=2, 
       NORMAL_TSTOP=3, ONE_STEP_TSTOP=4 };               /* itask */

/*
 * =================================================================
 *              F U N C T I O N   T Y P E S                       
 * =================================================================
 */

/*
 * -----------------------------------------------------------------
 * Type : RhsFn                                                   
 * -----------------------------------------------------------------
 * The f function which defines the right hand side of the ODE    
 * system y' = f(t,y) must have type RhsFn.                       
 * f takes as input the independent variable value t, and the     
 * dependent variable vector y.  It stores the result of f(t,y)   
 * in the vector ydot.  The y and ydot arguments are of type      
 * N_Vector.                                                      
 * (Allocation of memory for ydot is handled within CVODES)       
 * The f_data parameter is the same as the f_data                 
 * parameter set by the user through the CVodeSetFdata routine.   
 * This user-supplied pointer is passed to the user's f function  
 * every time it is called.                                       
 * A RhsFn f does not have a return value.                        
 * -----------------------------------------------------------------
 */

typedef void (*RhsFn)(realtype t, N_Vector y, 
                      N_Vector ydot, void *f_data);

/*
 * -----------------------------------------------------------------
 * Type : RootFn                                                  
 * -----------------------------------------------------------------
 * A function g, which defines a set of functions g_i(t,y) whose  
 * roots are sought during the integration, must have type RootFn.
 * The function g takes as input the independent variable value   
 * t, and the dependent variable vector y.  It stores the nrtfn   
 * values g_i(t,y) in the realtype array gout.                    
 * (Allocation of memory for gout is handled within CVODE.)       
 * The g_data parameter is the same as that passed by the user    
 * to the CVodeSetGdata routine.  This user-supplied pointer is   
 * passed to the user's g function every time it is called.       
 * A RootFn g does not have a return value.                       
 * -----------------------------------------------------------------
 */

typedef void (*RootFn)(realtype t, N_Vector y, realtype *gout, 
                       void *g_data);


/* 
 * =================================================================
 *          U S E R - C A L L A B L E   R O U T I N E S           
 * =================================================================
 */

/*
 * -----------------------------------------------------------------
 * Function : CVodeCreate                                         
 * -----------------------------------------------------------------
 * CVodeCreate creates an internal memory block for a problem to  
 * be solved by CVODES.                                           
 *                                                                
 * lmm     is the type of linear multistep method to be used.     
 *            The legal values are ADAMS and BDF (see previous    
 *            description).                                       
 *                                                                
 * iter    is the type of iteration used to solve the nonlinear   
 *            system that arises during each internal time step.  
 *            The legal values are FUNCTIONAL and NEWTON.         
 *                                                                
 * If successful, CVodeCreate returns a pointer to initialized    
 * problem memory. This pointer should be passed to CVodeMalloc.  
 * If an initialization error occurs, CVodeCreate prints an error 
 * message to standard err and returns NULL.                      
 * -----------------------------------------------------------------
 */

void *CVodeCreate(int lmm, int iter);

/*
 * -----------------------------------------------------------------
 * Function : CVodeResetIterType                                  
 * -----------------------------------------------------------------
 * CVodeResetIterType changes the cuurent nonlinear iteration     
 * type. The legal values for iter are FUNCTIONAL or NEWTON.      
 *                                                                
 * If successful, CVodeResetIterType returns SUCCESS. Otherwise,  
 * it returns one of the error values defined below for the       
 * optional input specification routines.                         
 * -----------------------------------------------------------------
 */

int CVodeResetIterType(void *cvode_mem, int iter);

/*
 * -----------------------------------------------------------------
 * Integrator optional input specification functions              
 * -----------------------------------------------------------------
 * The following functions can be called to set optional inputs   
 * to values other than the defaults given below:                 
 *                                                                
 * Function                |  Optional input / [ default value ]     
 * -----------------------------------------------------------------
 *                         | 
 * CVodeSetErrFile         | the file pointer for an error file      
 *                         | where all CVODES warning and error      
 *                         | messages will be written. This parameter
 *                         | can be stdout (standard output), stderr 
 *                         | (standard error), a file pointer        
 *                         | (corresponding to a user error file     
 *                         | opened for writing) returned by fopen.  
 *                         | If not called, then all messages will   
 *                         | be written to standard output.          
 *                         | [stderr]                                
 *                         | 
 * CVodeSetFdata           | a pointer to user data that will be     
 *                         | passed to the user's f function every   
 *                         | time f is called.                       
 *                         | [NULL]                                  
 *                         | 
 * CVodeSetGdata           | a pointer to user data that will be     
 *                         | passed to the user's g function every   
 *                         | time g is called.                       
 *                         | [NULL]                                  
 *                         | 
 * CVodeSetMaxOrd          | maximum lmm order to be used by the 
 *                         | solver.
 *                         | [12 for Adams , 5 for BDF]
 *                         |
 * CVodeSetMaxNumSteps     | maximum number of internal steps to be  
 *                         | taken by the solver in its attempt to   
 *                         | reach tout.                             
 *                         | [500]                                   
 *                         |
 * CVodeSetMaxHnilWarns    | maximum number of warning messages      
 *                         | issued by the solver that t+h==t on the 
 *                         | next internal step. A value of -1 means 
 *                         | no such messages are issued.            
 *                         | [10] 
 *                         |
 * CVodeSetStabLimDet      | flag to turn on/off stability limit     
 *                         | detection (TRUE = on, FALSE = off).     
 *                         | When BDF is used and order is 3 or      
 *                         | greater, CVsldet is called to detect    
 *                         | stability limit.  If limit is detected, 
 *                         | the order is reduced.                   
 *                         | [FALSE]                                 
 *                         |
 * CVodeSetInitStep        | initial step size.
 *                         | [estimated by CVODES]
 *                         |
 * CVodeSetMinStep         | minimum absolute value of step size     
 *                         | allowed.                                
 *                         | [0.0]                                   
 *                         |
 * CVodeSetMaxStep         | maximum absolute value of step size     
 *                         | allowed.                                
 *                         | [infinity]                              
 *                         |
 * CVodeSetStopTime        | the independent variable value past     
 *                         | which the solution is not to proceed.   
 *                         | [infinity]                              
 *                         |                                      
 * CVodeSetMaxErrTestFails | Maximum number of error test failures
 *                         | in attempting one step.              
 *                         | [7]                                  
 *                         |                                      
 * CVodeSetMaxNonlinIters  | Maximum number of nonlinear solver   
 *                         | iterations at one solution.          
 *                         | [3]                                  
 *                         |                                      
 * CVodeSetMaxConvFails    | Maximum number of allowable conv.    
 *                         | failures in attempting one step.     
 *                         | [10]                                 
 *                         |                                      
 * CVodeSetNonlinConvCoef  | Coeficient in the nonlinear conv.    
 *                         | test.                                
 *                         | [0.1]                                
 *                         |                                      
 * -----------------------------------------------------------------
 * If successful, these functions return SUCCESS. If an argument  
 * has an illegal value, they print an error message to the file
 * specified by errfp and return one of the error flags defined below.
 * -----------------------------------------------------------------
 */

int CVodeSetErrFile(void *cvode_mem, FILE *errfp);
int CVodeSetFdata(void *cvode_mem, void *f_data);
int CVodeSetGdata(void *cvode_mem, void *g_data);
int CVodeSetMaxOrd(void *cvode_mem, int maxord);
int CVodeSetMaxNumSteps(void *cvode_mem, long int mxsteps);
int CVodeSetMaxHnilWarns(void *cvode_mem, int mxhnil);
int CVodeSetStabLimDet(void *cvode_mem, booleantype stldet);
int CVodeSetInitStep(void *cvode_mem, realtype hin);
int CVodeSetMinStep(void *cvode_mem, realtype hmin);
int CVodeSetMaxStep(void *cvode_mem, realtype hmax);
int CVodeSetStopTime(void *cvode_mem, realtype tstop);
int CVodeSetMaxErrTestFails(void *cvode_mem, int maxnef);
int CVodeSetMaxNonlinIters(void *cvode_mem, int maxcor);
int CVodeSetMaxConvFails(void *cvode_mem, int maxncf);
int CVodeSetNonlinConvCoef(void *cvode_mem, realtype nlscoef);

/* Error return values for CVodeSet* functions */
/* SUCCESS = 0*/
enum {CVS_NO_MEM = -1, CVS_ILL_INPUT = -2};

/*
 * -----------------------------------------------------------------
 * Function : CVodeMalloc                                         
 * -----------------------------------------------------------------
 * CVodeMalloc allocates and initializes memory for a problem to  
 * to be solved by CVODES.                                        
 *                                                                
 * cvode_mem is pointer to CVODES memory returned by CVodeCreate. 
 *                                                                
 * f       is the right hand side function in y' = f(t,y).
 *                                                                
 * t0      is the initial value of t.                             
 *                                                                
 * y0      is the initial condition vector y(t0).                 
 *                                                                
 * itol    is the type of tolerances to be used.                  
 *            The legal values are:                               
 *               SS (scalar relative and absolute  tolerances),   
 *               SV (scalar relative tolerance and vector         
 *                   absolute tolerance).                         
 *                                                                
 * reltol  is a pointer to the relative tolerance scalar.         
 *                                                                
 * abstol  is a pointer to the absolute tolerance scalar or       
 *            an N_Vector of absolute tolerances.                 
 *                                                                
 * The parameters itol, reltol, and abstol define a vector of     
 * error weights, ewt, with components                            
 *   ewt[i] = 1/(reltol*abs(y[i]) + abstol)   (if itol = SS), or  
 *   ewt[i] = 1/(reltol*abs(y[i]) + abstol[i])   (if itol = SV).  
 * This vector is used in all error and convergence tests, which  
 * use a weighted RMS norm on all error-like vectors v:           
 *    WRMSnorm(v) = sqrt( (1/N) sum(i=1..N) (v[i]*ewt[i])^2 ),    
 * where N is the problem dimension.                              
 *                                                                
 * Note: The tolerance values may be changed in between calls to  
 *       CVode for the same problem. These values refer to        
 *       (*reltol) and either (*abstol), for a scalar absolute    
 *       tolerance, or the components of abstol, for a vector     
 *       absolute tolerance.                                      
 * 
 * If successful, CVodeMalloc returns SUCCESS. If an argument has 
 * an illegal value, CVodeMalloc prints an error message to the   
 * file specified by errfp and returns one of the error flags
 * defined below.                                                 
 * -----------------------------------------------------------------
 */

int CVodeMalloc(void *cvode_mem, RhsFn f,
                realtype t0, N_Vector y0, 
                int itol, realtype *reltol, void *abstol);

/* Error return values for CVodeMalloc */
/* SUCCESS = 0 */
enum {CVM_NO_MEM = -1, CVM_MEM_FAIL=-2, CVM_ILL_INPUT = -3};

/*
 * -----------------------------------------------------------------
 * Function : CVodeReInit                                         
 * -----------------------------------------------------------------
 * CVodeReInit re-initializes CVode for the solution of a problem,
 * where a prior call to CVodeMalloc has been made with the same  
 * problem size N. CVodeReInit performs the same input checking   
 * and initializations that CVodeMalloc does.                     
 * But it does no memory allocation, assuming that the existing   
 * internal memory is sufficient for the new problem.             
 *                                                                
 * The use of CVodeReInit requires that the maximum method order, 
 * maxord, is no larger for the new problem than for the problem  
 * specified in the last call to CVodeMalloc.  This condition is  
 * automatically fulfilled if the multistep method parameter lmm  
 * is unchanged (or changed from ADAMS to BDF) and the default    
 * value for maxord is specified.                                 
 *                                                                
 * The first argument to CVodeReInit is:                          
 *                                                                
 * cvode_mem = pointer to CVODES memory returned by CVodeCreate.  
 *                                                                
 * All the remaining arguments to CVodeReInit have names and      
 * meanings identical to those of CVodeMalloc.                    
 *                                                                
 * The return value of CVodeReInit is equal to SUCCESS = 0 if     
 * there were no errors; otherwise it is a negative int equal to: 
 *   CVREI_NO_MEM     indicating cvode_mem was NULL (i.e.,        
 *                    CVodeCreate has not been called).           
 *   CVREI_NO_MALLOC  indicating that cvode_mem has not been      
 *                    allocated (i.e., CVodeMalloc has not been   
 *                    called).                                    
 *   CVREI_ILL_INPUT  indicating an input argument was illegal    
 *                    (including an attempt to increase maxord).  
 * In case of an error return, an error message is also printed.  
 * -----------------------------------------------------------------
 */

int CVodeReInit(void *cvode_mem, RhsFn f,
                realtype t0, N_Vector y0, 
                int itol, realtype *reltol, void *abstol);

/* Error return values for CVodeReInit */
/* SUCCESS = 0 */ 
enum {CVREI_NO_MEM = -1, CVREI_NO_MALLOC = -2, CVREI_ILL_INPUT = -3};

/*
 * -----------------------------------------------------------------
 * Function : CVodeRootInit                                       
 * -----------------------------------------------------------------
 * CVodeRootInit initializes a rootfinding problem to be solved   
 * during the integration of the ODE system.  It must be called   
 * after CVodeCreate, and before CVode.  The arguments are:       
 *                                                                
 * cvode_mem = pointer to CVODE memory returned by CVodeCreate.   
 *                                                                
 * g         = name of user-supplied function, of type RootFn,    
 *             defining the functions g_i whose roots are sought. 
 *                                                                
 * nrtfn     = number of functions g_i, an int >= 0.              
 *                                                                
 * If a new problem is to be solved with a call to CVodeReInit,   
 * where the new problem has no root functions but the prior one  
 * did, then call CVodeRootInit with nrtfn = 0.                   
 *                                                                
 * The return value of CVodeRootInit is SUCCESS = 0 if there were 
 * no errors; otherwise it is a negative int equal to:            
 *   CVRT_NO_MEM     indicating cvode_mem was NULL, or            
 *   CVRT_MEM_FAIL   indicating a memory allocation failed.       
 *                    (including an attempt to increase maxord).  
 *   CVRT_FUNC_NULL  indicating nrtfn > 0 but g = NULL.           
 * In case of an error return, an error message is also printed.  
 * -----------------------------------------------------------------
 */

int CVodeRootInit(void *cvode_mem, RootFn g, int nrtfn);
/* CVodeRootInit return values: */
/* SUCCESS = 0 */ 
enum {CVRT_NO_MEM = -1, CVRT_MEM_FAIL = -2, CVRT_FUNC_NULL = -3};

/*
 * -----------------------------------------------------------------
 * Function : CVode                                               
 * -----------------------------------------------------------------
 * CVode integrates the ODE over an interval in t.                
 * If itask is NORMAL, then the solver integrates from its        
 * current internal t value to a point at or beyond tout, then    
 * interpolates to t = tout and returns y(tout) in the user-      
 * allocated vector yout. If itask is ONE_STEP, then the solver   
 * takes one internal time step and returns in yout the value of  
 * y at the new internal time. In this case, tout is used only    
 * during the first call to CVode to determine the direction of   
 * integration and the rough scale of the problem. In either      
 * case, the time reached by the solver is placed in (*t). The    
 * user is responsible for allocating the memory for this value.  
 *                                                                
 * cvode_mem is the pointer to CVODES memory returned by          
 *              CVodeCreate.                                      
 *                                                                
 * tout  is the next time at which a computed solution is desired.
 *                                                                
 * yout  is the computed solution vector. In NORMAL mode with no  
 *          errors and no roots found, yout=y(tout).              
 *                                                                
 * tret  is a pointer to a real location. CVode sets (*tret) to   
 *          the time reached by the solver and returns            
 *          yout=y(*tret).                                        
 *                                                                
 * itask is NORMAL, ONE_STEP, NORMAL_TSTOP, or ONE_STEP_TSTOP.    
 *          These four modes are described above.                 
 *                                                                
 * Here is a brief description of each return value:              
 *                                                                
 * SUCCESS       : CVode succeeded and no roots were found.       
 *                                                                
 * ROOT_RETURN   : CVode succeeded, and found one or more roots.  
 *                 If nrtfn > 1, call CVodeGetRootInfo to see     
 *                 which g_i were found to have a root at (*tret).
 *                                                                
 * TSTOP_RETURN  : CVode succeded and returned at tstop.          
 *                                                                
 * CVODE_NO_MEM  : The cvode_mem argument was NULL.               
 *                                                                
 * CVODE_NO_MALLOC: cvode_mem was not allocated.                  
 *                                                                
 * ILL_INPUT     : One of the inputs to CVode is illegal. This    
 *                 includes the situation when a component of the 
 *                 error weight vectors becomes < 0 during        
 *                 internal time-stepping. The ILL_INPUT flag     
 *                 will also be returned if the linear solver     
 *                 routine CV--- (called by the user after        
 *                 calling CVodeCreate) failed to set one of the  
 *                 linear solver-related fields in cvode_mem or   
 *                 if the linear solver's init routine failed. In 
 *                 any case, the user should see the printed      
 *                 error message for more details.                
 *                                                                
 * TOO_MUCH_WORK : The solver took mxstep internal steps but      
 *                 could not reach tout. The default value for    
 *                 mxstep is MXSTEP_DEFAULT = 500.                
 *                                                                
 * TOO_MUCH_ACC  : The solver could not satisfy the accuracy      
 *                 demanded by the user for some internal step.   
 *                                                                
 * ERR_FAILURE   : Error test failures occurred too many times    
 *                 (= MXNEF = 7) during one internal time step or 
 *                 occurred with |h| = hmin.                      
 *                                                                
 * CONV_FAILURE  : Convergence test failures occurred too many    
 *                 times (= MXNCF = 10) during one internal time  
 *                 step or occurred with |h| = hmin.              
 *                                                                
 * SETUP_FAILURE : The linear solver's setup routine failed in an 
 *                 unrecoverable manner.                          
 *                                                                
 * SOLVE_FAILURE : The linear solver's solve routine failed in an 
 *                 unrecoverable manner.                          
 * -----------------------------------------------------------------
 */

int CVode(void *cvode_mem, realtype tout, N_Vector yout, 
          realtype *tret, int itask);

/* CVode return values */
enum { SUCCESS=0, TSTOP_RETURN=1, ROOT_RETURN=2, 
       CVODE_NO_MEM=-1, CVODE_NO_MALLOC=-2, ILL_INPUT=-3, 
       TOO_MUCH_WORK=-4, TOO_MUCH_ACC=-5, ERR_FAILURE=-6, 
       CONV_FAILURE=-7, SETUP_FAILURE=-8, SOLVE_FAILURE=-9 };

/*
 * -----------------------------------------------------------------
 * Function : CVodeGetDky                                         
 * -----------------------------------------------------------------
 * CVodeGetDky computes the kth derivative of the y function at   
 * time t, where tn-hu <= t <= tn, tn denotes the current         
 * internal time reached, and hu is the last internal step size   
 * successfully used by the solver. The user may request          
 * k=0, 1, ..., qu, where qu is the current order. The            
 * derivative vector is returned in dky. This vector must be      
 * allocated by the caller. It is only legal to call this         
 * function after a successful return from CVode.                 
 *                                                                
 * cvode_mem is the pointer to CVODES memory returned by          
 *              CVodeCreate.                                      
 *                                                                
 * t   is the time at which the kth derivative of y is evaluated. 
 *        The legal range for t is [tn-hu,tn] as described above. 
 *                                                                
 * k   is the order of the derivative of y to be computed. The    
 *        legal range for k is [0,qu] as described above.         
 *                                                                
 * dky is the output derivative vector [(D_k)y](t).               
 *                                                                
 * The return values for CVodeGetDky are defined below.           
 * Here is a brief description of each return value:              
 *                                                                
 * OKAY : CVodeGetDky succeeded.                                  
 *                                                                
 * BAD_K : k is not in the range 0, 1, ..., qu.                   
 *                                                                
 * BAD_T : t is not in the interval [tn-hu,tn].                   
 *                                                                
 * BAD_DKY : The dky argument was NULL.                           
 *                                                                
 * DKY_NO_MEM : The cvode_mem argument was NULL.                  
 * -----------------------------------------------------------------
 */

int CVodeGetDky(void *cvode_mem, realtype t, int k, N_Vector dky);

/*
 * -----------------------------------------------------------------
 * Integrator optional output extraction functions                
 * -----------------------------------------------------------------
 * The following functions can be called to get optional outputs  
 * and statistics related to the main integrator.                 
 * -----------------------------------------------------------------
 *                                                                
 * CVodeGetIntWorkSpace returns the CVODES integer workspace size 
 * CVodeGetRealWorkSpace returns the CVODES real workspace size   
 * CVodeGetNumSteps returns the cumulative number of internal     
 *       steps taken by the solver                                
 * CVodeGetNumRhsEvals returns the number of calls to the user's  
 *       f function                                               
 * CVodeGetNumLinSolvSetups returns the number of calls made to   
 *       the linear solver's setup routine                        
 * CVodeGetNumErrTestFails returns the number of local error test 
 *       failures that have occured                               
 * CVodeGetLastOrder returns the order used during the last       
 *       internal step                                            
 * CVodeGetCurrentOrder returns the order to be used on the next  
 *       internal step                                            
 * CVodeGetNumStabLimOrderReds returns the number of order        
 *       reductions due to stability limit detection              
 * CVodeGetActualInitStep returns the actual initial step size    
 *       used by CVODES                                           
 * CVodeGetLastStep returns the step size for the last internal   
 *       step                                                     
 * CVodeGetCurrentStep returns the step size to be attempted on   
 *       the next internal step                                   
 * CVodeGetCurrentTime returns the current internal time reached  
 *       by the solver                                            
 * CVodeGetTolScaleFactor returns a suggested factor by which the 
 *       user's tolerances should be scaled when too much         
 *       accuracy has been requested for some internal step       
 * CVodeGetErrWeights returns the state error weight vector.      
 *       The user need not allocate space for ewt.                
 * CVodeGetEstLocalErrors returns the vector of estimated local   
 *       errors. The user need not allocate space for ele.        
 * CVodeGetNumGEvals returns the number of calls to the user's    
 *       g function (for rootfinding)                             
 * CVodeGetRootInfo returns an array of int's showing the indices 
 *       for which g_i was found to have a root.                  
 *       For i = 0 ... nrtfn-1, rootsfound[i] = 1 if g_i has a    
 *       root, and = 0 if not.                                    
 * -----------------------------------------------------------------
 */

int CVodeGetIntWorkSpace(void *cvode_mem, long int *leniw);
int CVodeGetRealWorkSpace(void *cvode_mem, long int *lenrw);
int CVodeGetNumSteps(void *cvode_mem, long int *nsteps);
int CVodeGetNumRhsEvals(void *cvode_mem, long int *nfevals);
int CVodeGetNumLinSolvSetups(void *cvode_mem, long int *nlinsetups);
int CVodeGetNumErrTestFails(void *cvode_mem, long int *netfails);
int CVodeGetLastOrder(void *cvode_mem, int *qlast);
int CVodeGetCurrentOrder(void *cvode_mem, int *qcur);
int CVodeGetNumStabLimOrderReds(void *cvode_mem, long int *nslred);
int CVodeGetActualInitStep(void *cvode_mem, realtype *hinused);
int CVodeGetLastStep(void *cvode_mem, realtype *hlast);
int CVodeGetCurrentStep(void *cvode_mem, realtype *hcur);
int CVodeGetCurrentTime(void *cvode_mem, realtype *tcur);
int CVodeGetTolScaleFactor(void *cvode_mem, realtype *tolsfac);
int CVodeGetErrWeights(void *cvode_mem, N_Vector *eweight);
int CVodeGetEstLocalErrors(void *cvode_mem, N_Vector *ele);
int CVodeGetNumGEvals(void *cvode_mem, long int *ngevals);
int CVodeGetRootInfo(void *cvode_mem, int **rootsfound);

/*
 * -----------------------------------------------------------------
 * As a convenience, the following two functions provide the      
 * optional outputs in groups.                                    
 * -----------------------------------------------------------------
 */

int CVodeGetWorkSpace(void *cvode_mem, long int *leniw, long int *lenrw);
int CVodeGetIntegratorStats(void *cvode_mem, long int *nsteps, 
                            long int *nfevals, long int *nlinsetups, 
                            long int *netfails, int *qlast, 
                            int *qcur, realtype *hinused, realtype *hlast,
                            realtype *hcur, realtype *tcur);

/*
 * -----------------------------------------------------------------
 * Nonlinear solver optional output extraction functions          
 * -----------------------------------------------------------------
 * The following functions can be called to get optional outputs  
 * and statistics related to the nonlinear solver.                
 * -----------------------------------------------------------------
 *                                                                
 * CVodeGetNumNonlinSolvIters returns the number of nonlinear     
 *       solver iterations performed.                             
 * CVodeGetNumNonlinSolvConvFails returns the number of nonlinear 
 *       convergence failures.                                    
 * -----------------------------------------------------------------
 */

int CVodeGetNumNonlinSolvIters(void *cvode_mem, long int *nniters);
int CVodeGetNumNonlinSolvConvFails(void *cvode_mem, long int *nncfails);

/*
 * -----------------------------------------------------------------
 * As a convenience, the following function provides the          
 * optional outputs in a group.                                   
 * -----------------------------------------------------------------
 */

int CVodeGetNonlinSolvStats(void *cvode_mem, long int *nniters, 
                            long int *nncfails);

/* CVodeGet* return values */
enum { OKAY=0, CVG_NO_MEM=-1, CVG_NO_SLDET=-2, 
       BAD_K=-3, BAD_T=-4, BAD_DKY=-5 };

/*
 * -----------------------------------------------------------------
 * Function : CVodeFree                                           
 * -----------------------------------------------------------------
 * CVodeFree frees the problem memory cvode_mem allocated by      
 * CVodeMalloc.  Its only argument is the pointer cvode_mem       
 * returned by CVodeCreate.                                       
 * -----------------------------------------------------------------
 */

void CVodeFree(void *cvode_mem);

/*
 * =================================================================
 *     I N T E R F A C E   T O    L I N E A R   S O L V E R S     
 * =================================================================
 */

/*
 * -----------------------------------------------------------------
 * Communication between user and a CVODES Linear Solver          
 * -----------------------------------------------------------------
 * Return values of the linear solver specification routine       
 * and of the linear solver set / get routines:                   
 *                                                                
 *    SUCCESS      : The routine was successful.                  
 *                                                                
 *    LIN_NO_MEM   : CVODES memory = NULL.                        
 *                                                                
 *    LMEM_FAIL    : A memory allocation failed.                  
 *                                                                
 *    LIN_ILL_INPUT: Some input was illegal (see message).        
 *                                                                
 *    LIN_NO_LMEM  : The linear solver's memory = NULL.           
 * -----------------------------------------------------------------
 */

/* SUCCESS = 0  */
enum {LMEM_FAIL=-1, LIN_ILL_INPUT=-2, LIN_NO_MEM=-3, LIN_NO_LMEM=-4};

/*
 * -----------------------------------------------------------------
 * Communication between CVODES and a CVODE Linear Solver         
 * -----------------------------------------------------------------
 * (1) cv_linit return values                                     
 *                                                                
 * LINIT_OK    : The cv_linit routine succeeded.                  
 *                                                                
 * LINIT_ERR   : The cv_linit routine failed. Each linear solver  
 *               init routine should print an appropriate error   
 *               message to (cv_mem->errfp).                      
 *                                                                
 * (2) convfail (input to cv_lsetup)                              
 *                                                                
 * NO_FAILURES : Either this is the first cv_setup call for this  
 *               step, or the local error test failed on the      
 *               previous attempt at this step (but the Newton    
 *               iteration converged).   
 *                                                                
 * FAIL_BAD_J  : This value is passed to cv_lsetup if             
 *                                                                
 *               (1) The previous Newton corrector iteration      
 *                   did not converge and the linear solver's     
 *                   setup routine indicated that its Jacobian-   
 *                   related data is not current                  
 *                                   or                           
 *               (2) During the previous Newton corrector         
 *                   iteration, the linear solver's solve routine 
 *                   failed in a recoverable manner and the       
 *                   linear solver's setup routine indicated that 
 *                   its Jacobian-related data is not current.    
 *                                                                
 * FAIL_OTHER  : During the current internal step try, the        
 *               previous Newton iteration failed to converge     
 *               even though the linear solver was using current  
 *               Jacobian-related data.                           
 *                                                                
 * (3) Parameter documentation, as well as a brief description    
 *     of purpose, for each CVODES linear solver routine to be    
 *     called in cvodes.c is given below the constant declarations
 *     that follow.                                               
 * -----------------------------------------------------------------
 */

/* cv_linit return values */

#define LINIT_OK        0
#define LINIT_ERR      -1

/* Constants for convfail (input to cv_lsetup) */

#define NO_FAILURES 0   
#define FAIL_BAD_J  1  
#define FAIL_OTHER  2  

/*
 * -----------------------------------------------------------------
 * int (*cv_linit)(CVodeMem cv_mem);                              
 * -----------------------------------------------------------------
 * The purpose of cv_linit is to complete initializations for a   
 * specific linear solver, such as counters and statistics.       
 * An LInitFn should return LINIT_OK (= 0) if it has successfully 
 * initialized the CVODES linear solver and LINIT_ERR (= -1)      
 * otherwise. These constants are defined above.  If an error does
 * occur, an appropriate message should be sent to (cv_mem->errfp)
 * -----------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------
 * int (*cv_lsetup)(CVodeMem cv_mem, int convfail, N_Vector ypred,
 *                 N_Vector fpred, booleantype *jcurPtr,          
 *                 N_Vector vtemp1, N_Vector vtemp2,              
 *                 N_Vector vtemp3);                              
 * -----------------------------------------------------------------
 * The job of cv_lsetup is to prepare the linear solver for       
 * subsequent calls to cv_lsolve. It may recompute Jacobian-      
 * related data is it deems necessary. Its parameters are as      
 * follows:                                                       
 *                                                                
 * cv_mem - problem memory pointer of type CVodeMem. See the big  
 *          typedef earlier in this file.                         
 *                                                                
 * convfail - a flag to indicate any problem that occurred during 
 *            the solution of the nonlinear equation on the       
 *            current time step for which the linear solver is    
 *            being used. This flag can be used to help decide    
 *            whether the Jacobian data kept by a CVODES linear   
 *            solver needs to be updated or not.                  
 *            Its possible values have been documented above.     
 *                                                                
 * ypred - the predicted y vector for the current CVODES internal 
 *         step.                                                  
 *                                                                
 * fpred - f(tn, ypred).                                          
 *                                                                
 * jcurPtr - a pointer to a boolean to be filled in by cv_lsetup. 
 *           The function should set *jcurPtr=TRUE if its Jacobian
 *           data is current after the call and should set        
 *           *jcurPtr=FALSE if its Jacobian data is not current.  
 *           Note: If cv_lsetup calls for re-evaluation of        
 *           Jacobian data (based on convfail and CVODES state    
 *           data), it should return *jcurPtr=TRUE always;        
 *           otherwise an infinite loop can result.               
 *                                                                
 * vtemp1 - temporary N_Vector provided for use by cv_lsetup.     
 *                                                                
 * vtemp3 - temporary N_Vector provided for use by cv_lsetup.     
 *                                                                
 * vtemp3 - temporary N_Vector provided for use by cv_lsetup.     
 *                                                                
 * The cv_lsetup routine should return 0 if successful,           
 * a positive value for a recoverable error, and a negative value 
 * for an unrecoverable error.                                    
 * -----------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------
 * int (*cv_lsolve)(CVodeMem cv_mem, N_Vector b, N_Vector weight, 
 *                  N_Vector ycur, N_Vector fcur);                
 * -----------------------------------------------------------------
 * cv_lsolve must solve the linear equation P x = b, where        
 * P is some approximation to (I - gamma J), J = (df/dy)(tn,ycur) 
 * and the RHS vector b is input. The N-vector ycur contains      
 * the solver's current approximation to y(tn) and the vector     
 * fcur contains the N_Vector f(tn,ycur). The solution is to be   
 * returned in the vector b. cv_lsolve returns a positive value   
 * for a recoverable error and a negative value for an            
 * unrecoverable error. Success is indicated by a 0 return value. 
 * -----------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------
 * void (*cv_lfree)(CVodeMem cv_mem);                             
 * -----------------------------------------------------------------
 * cv_lfree should free up any memory allocated by the linear     
 * solver. This routine is called once a problem has been         
 * completed and the linear solver is no longer needed.           
 * -----------------------------------------------------------------
 */


#endif

#ifdef __cplusplus
}
#endif
