! This file was automatically generated by SWIG (http://www.swig.org).
! Version 4.0.0
!
! Do not make changes to this file unless you know what you are doing--modify
! the SWIG interface file instead.
module fcvode_mod
 use, intrinsic :: ISO_C_BINDING
 use fnvector_mod
 use fsundials_types
 use fsunlinsol_mod
 use fsunmatrix_mod
 use fsunnonlinsol_mod
 implicit none
 private

 ! PUBLIC METHODS AND TYPES
 public :: FCVodeCreate
 public :: FCVodeInit
 public :: FCVodeReInit
 public :: FCVodeSStolerances
 public :: FCVodeSVtolerances
 public :: FCVodeWFtolerances
 public :: FCVodeSetErrHandlerFn
 public :: FCVodeSetErrFile
 public :: FCVodeSetUserData
 public :: FCVodeSetMaxOrd
 public :: FCVodeSetMaxNumSteps
 public :: FCVodeSetMaxHnilWarns
 public :: FCVodeSetStabLimDet
 public :: FCVodeSetInitStep
 public :: FCVodeSetMinStep
 public :: FCVodeSetMaxStep
 public :: FCVodeSetStopTime
 public :: FCVodeSetMaxErrTestFails
 public :: FCVodeSetMaxNonlinIters
 public :: FCVodeSetMaxConvFails
 public :: FCVodeSetNonlinConvCoef
 public :: FCVodeSetConstraints
 public :: FCVodeSetNonlinearSolver
 public :: FCVodeRootInit
 public :: FCVodeSetRootDirection
 public :: FCVodeSetNoInactiveRootWarn
 public :: FCVode
 public :: FCVodeGetDky
 public :: FCVodeGetWorkSpace
 public :: FCVodeGetNumSteps
 public :: FCVodeGetNumRhsEvals
 public :: FCVodeGetNumLinSolvSetups
 public :: FCVodeGetNumErrTestFails
 public :: FCVodeGetLastOrder
 public :: FCVodeGetCurrentOrder
 public :: FCVodeGetNumStabLimOrderReds
 public :: FCVodeGetActualInitStep
 public :: FCVodeGetLastStep
 public :: FCVodeGetCurrentStep
 public :: FCVodeGetCurrentTime
 public :: FCVodeGetTolScaleFactor
 public :: FCVodeGetErrWeights
 public :: FCVodeGetEstLocalErrors
 public :: FCVodeGetNumGEvals
 public :: FCVodeGetRootInfo
 public :: FCVodeGetIntegratorStats
 public :: FCVodeGetNumNonlinSolvIters
 public :: FCVodeGetNumNonlinSolvConvFails
 public :: FCVodeGetNonlinSolvStats
 public :: FCVodeGetReturnFlagName
 public :: FCVodeFree
 public :: FCVBandPrecInit
 public :: FCVBandPrecGetWorkSpace
 public :: FCVBandPrecGetNumRhsEvals
 public :: FCVBBDPrecInit
 public :: FCVBBDPrecReInit
 public :: FCVBBDPrecGetWorkSpace
 public :: FCVBBDPrecGetNumGfnEvals
 public :: FCVDiag
 public :: FCVDiagGetWorkSpace
 public :: FCVDiagGetNumRhsEvals
 public :: FCVDiagGetLastFlag
 public :: FCVDiagGetReturnFlagName
 public :: FCVodeSetLinearSolver
 public :: FCVodeSetJacFn
 public :: FCVodeSetMaxStepsBetweenJac
 public :: FCVodeSetEpsLin
 public :: FCVodeSetPreconditioner
 public :: FCVodeSetJacTimes
 public :: FCVodeGetLinWorkSpace
 public :: FCVodeGetNumJacEvals
 public :: FCVodeGetNumPrecEvals
 public :: FCVodeGetNumPrecSolves
 public :: FCVodeGetNumLinIters
 public :: FCVodeGetNumLinConvFails
 public :: FCVodeGetNumJTSetupEvals
 public :: FCVodeGetNumJtimesEvals
 public :: FCVodeGetNumLinRhsEvals
 public :: FCVodeGetLastLinFlag
 public :: FCVodeGetLinReturnFlagName

 ! PARAMETERS
 integer(C_INT), parameter, public :: CV_ADAMS = 1_C_INT
 integer(C_INT), parameter, public :: CV_BDF = 2_C_INT
 integer(C_INT), parameter, public :: CV_NORMAL = 1_C_INT
 integer(C_INT), parameter, public :: CV_ONE_STEP = 2_C_INT
 integer(C_INT), parameter, public :: CV_SUCCESS = 0_C_INT
 integer(C_INT), parameter, public :: CV_TSTOP_RETURN = 1_C_INT
 integer(C_INT), parameter, public :: CV_ROOT_RETURN = 2_C_INT
 integer(C_INT), parameter, public :: CV_WARNING = 99_C_INT
 integer(C_INT), parameter, public :: CV_TOO_MUCH_WORK = -1_C_INT
 integer(C_INT), parameter, public :: CV_TOO_MUCH_ACC = -2_C_INT
 integer(C_INT), parameter, public :: CV_ERR_FAILURE = -3_C_INT
 integer(C_INT), parameter, public :: CV_CONV_FAILURE = -4_C_INT
 integer(C_INT), parameter, public :: CV_LINIT_FAIL = -5_C_INT
 integer(C_INT), parameter, public :: CV_LSETUP_FAIL = -6_C_INT
 integer(C_INT), parameter, public :: CV_LSOLVE_FAIL = -7_C_INT
 integer(C_INT), parameter, public :: CV_RHSFUNC_FAIL = -8_C_INT
 integer(C_INT), parameter, public :: CV_FIRST_RHSFUNC_ERR = -9_C_INT
 integer(C_INT), parameter, public :: CV_REPTD_RHSFUNC_ERR = -10_C_INT
 integer(C_INT), parameter, public :: CV_UNREC_RHSFUNC_ERR = -11_C_INT
 integer(C_INT), parameter, public :: CV_RTFUNC_FAIL = -12_C_INT
 integer(C_INT), parameter, public :: CV_NLS_INIT_FAIL = -13_C_INT
 integer(C_INT), parameter, public :: CV_NLS_SETUP_FAIL = -14_C_INT
 integer(C_INT), parameter, public :: CV_CONSTR_FAIL = -15_C_INT
 integer(C_INT), parameter, public :: CV_MEM_FAIL = -20_C_INT
 integer(C_INT), parameter, public :: CV_MEM_NULL = -21_C_INT
 integer(C_INT), parameter, public :: CV_ILL_INPUT = -22_C_INT
 integer(C_INT), parameter, public :: CV_NO_MALLOC = -23_C_INT
 integer(C_INT), parameter, public :: CV_BAD_K = -24_C_INT
 integer(C_INT), parameter, public :: CV_BAD_T = -25_C_INT
 integer(C_INT), parameter, public :: CV_BAD_DKY = -26_C_INT
 integer(C_INT), parameter, public :: CV_TOO_CLOSE = -27_C_INT
 integer(C_INT), parameter, public :: CV_VECTOROP_ERR = -28_C_INT
 integer(C_INT), parameter, public :: CVDIAG_SUCCESS = 0_C_INT
 integer(C_INT), parameter, public :: CVDIAG_MEM_NULL = -1_C_INT
 integer(C_INT), parameter, public :: CVDIAG_LMEM_NULL = -2_C_INT
 integer(C_INT), parameter, public :: CVDIAG_ILL_INPUT = -3_C_INT
 integer(C_INT), parameter, public :: CVDIAG_MEM_FAIL = -4_C_INT
 integer(C_INT), parameter, public :: CVDIAG_INV_FAIL = -5_C_INT
 integer(C_INT), parameter, public :: CVDIAG_RHSFUNC_UNRECVR = -6_C_INT
 integer(C_INT), parameter, public :: CVDIAG_RHSFUNC_RECVR = -7_C_INT
 integer(C_INT), parameter, public :: CVLS_SUCCESS = 0_C_INT
 integer(C_INT), parameter, public :: CVLS_MEM_NULL = -1_C_INT
 integer(C_INT), parameter, public :: CVLS_LMEM_NULL = -2_C_INT
 integer(C_INT), parameter, public :: CVLS_ILL_INPUT = -3_C_INT
 integer(C_INT), parameter, public :: CVLS_MEM_FAIL = -4_C_INT
 integer(C_INT), parameter, public :: CVLS_PMEM_NULL = -5_C_INT
 integer(C_INT), parameter, public :: CVLS_JACFUNC_UNRECVR = -6_C_INT
 integer(C_INT), parameter, public :: CVLS_JACFUNC_RECVR = -7_C_INT
 integer(C_INT), parameter, public :: CVLS_SUNMAT_FAIL = -8_C_INT
 integer(C_INT), parameter, public :: CVLS_SUNLS_FAIL = -9_C_INT

 ! WRAPPER DECLARATIONS
 interface
function FCVodeCreate(lmm) &
bind(C, name="CVodeCreate") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
integer(C_INT), value :: lmm
type(C_PTR) :: fresult
end function

function FCVodeInit(cvode_mem, f, t0, y0) &
bind(C, name="CVodeInit") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_FUNPTR), value :: f
real(C_DOUBLE), value :: t0
type(C_PTR), value :: y0
integer(C_INT) :: fresult
end function

function FCVodeReInit(cvode_mem, t0, y0) &
bind(C, name="CVodeReInit") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: t0
type(C_PTR), value :: y0
integer(C_INT) :: fresult
end function

function FCVodeSStolerances(cvode_mem, reltol, abstol) &
bind(C, name="CVodeSStolerances") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: reltol
real(C_DOUBLE), value :: abstol
integer(C_INT) :: fresult
end function

function FCVodeSVtolerances(cvode_mem, reltol, abstol) &
bind(C, name="CVodeSVtolerances") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: reltol
type(C_PTR), value :: abstol
integer(C_INT) :: fresult
end function

function FCVodeWFtolerances(cvode_mem, efun) &
bind(C, name="CVodeWFtolerances") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_FUNPTR), value :: efun
integer(C_INT) :: fresult
end function

function FCVodeSetErrHandlerFn(cvode_mem, ehfun, eh_data) &
bind(C, name="CVodeSetErrHandlerFn") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_FUNPTR), value :: ehfun
type(C_PTR), value :: eh_data
integer(C_INT) :: fresult
end function

function FCVodeSetErrFile(cvode_mem, errfp) &
bind(C, name="CVodeSetErrFile") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_PTR), value :: errfp
integer(C_INT) :: fresult
end function

function FCVodeSetUserData(cvode_mem, user_data) &
bind(C, name="CVodeSetUserData") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_PTR), value :: user_data
integer(C_INT) :: fresult
end function

function FCVodeSetMaxOrd(cvode_mem, maxord) &
bind(C, name="CVodeSetMaxOrd") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT), value :: maxord
integer(C_INT) :: fresult
end function

function FCVodeSetMaxNumSteps(cvode_mem, mxsteps) &
bind(C, name="CVodeSetMaxNumSteps") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG), value :: mxsteps
integer(C_INT) :: fresult
end function

function FCVodeSetMaxHnilWarns(cvode_mem, mxhnil) &
bind(C, name="CVodeSetMaxHnilWarns") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT), value :: mxhnil
integer(C_INT) :: fresult
end function

function FCVodeSetStabLimDet(cvode_mem, stldet) &
bind(C, name="CVodeSetStabLimDet") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
logical(C_BOOL), value :: stldet
integer(C_INT) :: fresult
end function

function FCVodeSetInitStep(cvode_mem, hin) &
bind(C, name="CVodeSetInitStep") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: hin
integer(C_INT) :: fresult
end function

function FCVodeSetMinStep(cvode_mem, hmin) &
bind(C, name="CVodeSetMinStep") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: hmin
integer(C_INT) :: fresult
end function

function FCVodeSetMaxStep(cvode_mem, hmax) &
bind(C, name="CVodeSetMaxStep") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: hmax
integer(C_INT) :: fresult
end function

function FCVodeSetStopTime(cvode_mem, tstop) &
bind(C, name="CVodeSetStopTime") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: tstop
integer(C_INT) :: fresult
end function

function FCVodeSetMaxErrTestFails(cvode_mem, maxnef) &
bind(C, name="CVodeSetMaxErrTestFails") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT), value :: maxnef
integer(C_INT) :: fresult
end function

function FCVodeSetMaxNonlinIters(cvode_mem, maxcor) &
bind(C, name="CVodeSetMaxNonlinIters") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT), value :: maxcor
integer(C_INT) :: fresult
end function

function FCVodeSetMaxConvFails(cvode_mem, maxncf) &
bind(C, name="CVodeSetMaxConvFails") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT), value :: maxncf
integer(C_INT) :: fresult
end function

function FCVodeSetNonlinConvCoef(cvode_mem, nlscoef) &
bind(C, name="CVodeSetNonlinConvCoef") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: nlscoef
integer(C_INT) :: fresult
end function

function FCVodeSetConstraints(cvode_mem, constraints) &
bind(C, name="CVodeSetConstraints") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_PTR), value :: constraints
integer(C_INT) :: fresult
end function

function FCVodeSetNonlinearSolver(cvode_mem, nls) &
bind(C, name="CVodeSetNonlinearSolver") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_PTR), value :: nls
integer(C_INT) :: fresult
end function

function FCVodeRootInit(cvode_mem, nrtfn, g) &
bind(C, name="CVodeRootInit") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT), value :: nrtfn
type(C_FUNPTR), value :: g
integer(C_INT) :: fresult
end function

function FCVodeSetRootDirection(cvode_mem, rootdir) &
bind(C, name="CVodeSetRootDirection") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT) :: rootdir
integer(C_INT) :: fresult
end function

function FCVodeSetNoInactiveRootWarn(cvode_mem) &
bind(C, name="CVodeSetNoInactiveRootWarn") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT) :: fresult
end function

function FCVode(cvode_mem, tout, yout, tret, itask) &
bind(C, name="CVode") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: tout
type(C_PTR), value :: yout
real(C_DOUBLE) :: tret
integer(C_INT), value :: itask
integer(C_INT) :: fresult
end function

function FCVodeGetDky(cvode_mem, t, k, dky) &
bind(C, name="CVodeGetDky") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: t
integer(C_INT), value :: k
type(C_PTR), value :: dky
integer(C_INT) :: fresult
end function

function FCVodeGetWorkSpace(cvode_mem, lenrw, leniw) &
bind(C, name="CVodeGetWorkSpace") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: lenrw
integer(C_LONG) :: leniw
integer(C_INT) :: fresult
end function

function FCVodeGetNumSteps(cvode_mem, nsteps) &
bind(C, name="CVodeGetNumSteps") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nsteps
integer(C_INT) :: fresult
end function

function FCVodeGetNumRhsEvals(cvode_mem, nfevals) &
bind(C, name="CVodeGetNumRhsEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nfevals
integer(C_INT) :: fresult
end function

function FCVodeGetNumLinSolvSetups(cvode_mem, nlinsetups) &
bind(C, name="CVodeGetNumLinSolvSetups") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nlinsetups
integer(C_INT) :: fresult
end function

function FCVodeGetNumErrTestFails(cvode_mem, netfails) &
bind(C, name="CVodeGetNumErrTestFails") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: netfails
integer(C_INT) :: fresult
end function

function FCVodeGetLastOrder(cvode_mem, qlast) &
bind(C, name="CVodeGetLastOrder") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT) :: qlast
integer(C_INT) :: fresult
end function

function FCVodeGetCurrentOrder(cvode_mem, qcur) &
bind(C, name="CVodeGetCurrentOrder") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT) :: qcur
integer(C_INT) :: fresult
end function

function FCVodeGetNumStabLimOrderReds(cvode_mem, nslred) &
bind(C, name="CVodeGetNumStabLimOrderReds") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nslred
integer(C_INT) :: fresult
end function

function FCVodeGetActualInitStep(cvode_mem, hinused) &
bind(C, name="CVodeGetActualInitStep") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE) :: hinused
integer(C_INT) :: fresult
end function

function FCVodeGetLastStep(cvode_mem, hlast) &
bind(C, name="CVodeGetLastStep") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE) :: hlast
integer(C_INT) :: fresult
end function

function FCVodeGetCurrentStep(cvode_mem, hcur) &
bind(C, name="CVodeGetCurrentStep") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE) :: hcur
integer(C_INT) :: fresult
end function

function FCVodeGetCurrentTime(cvode_mem, tcur) &
bind(C, name="CVodeGetCurrentTime") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE) :: tcur
integer(C_INT) :: fresult
end function

function FCVodeGetTolScaleFactor(cvode_mem, tolsfac) &
bind(C, name="CVodeGetTolScaleFactor") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE) :: tolsfac
integer(C_INT) :: fresult
end function

function FCVodeGetErrWeights(cvode_mem, eweight) &
bind(C, name="CVodeGetErrWeights") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_PTR), value :: eweight
integer(C_INT) :: fresult
end function

function FCVodeGetEstLocalErrors(cvode_mem, ele) &
bind(C, name="CVodeGetEstLocalErrors") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_PTR), value :: ele
integer(C_INT) :: fresult
end function

function FCVodeGetNumGEvals(cvode_mem, ngevals) &
bind(C, name="CVodeGetNumGEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: ngevals
integer(C_INT) :: fresult
end function

function FCVodeGetRootInfo(cvode_mem, rootsfound) &
bind(C, name="CVodeGetRootInfo") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT) :: rootsfound
integer(C_INT) :: fresult
end function

function FCVodeGetIntegratorStats(cvode_mem, nsteps, nfevals, nlinsetups, netfails, qlast, qcur, hinused, hlast, hcur, tcur) &
bind(C, name="CVodeGetIntegratorStats") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nsteps
integer(C_LONG) :: nfevals
integer(C_LONG) :: nlinsetups
integer(C_LONG) :: netfails
integer(C_INT) :: qlast
integer(C_INT) :: qcur
real(C_DOUBLE) :: hinused
real(C_DOUBLE) :: hlast
real(C_DOUBLE) :: hcur
real(C_DOUBLE) :: tcur
integer(C_INT) :: fresult
end function

function FCVodeGetNumNonlinSolvIters(cvode_mem, nniters) &
bind(C, name="CVodeGetNumNonlinSolvIters") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nniters
integer(C_INT) :: fresult
end function

function FCVodeGetNumNonlinSolvConvFails(cvode_mem, nncfails) &
bind(C, name="CVodeGetNumNonlinSolvConvFails") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nncfails
integer(C_INT) :: fresult
end function

function FCVodeGetNonlinSolvStats(cvode_mem, nniters, nncfails) &
bind(C, name="CVodeGetNonlinSolvStats") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nniters
integer(C_LONG) :: nncfails
integer(C_INT) :: fresult
end function

function FCVodeGetReturnFlagName(flag) &
bind(C, name="CVodeGetReturnFlagName") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
integer(C_LONG), value :: flag
type(C_PTR) :: fresult
end function

subroutine FCVodeFree(cvode_mem) &
bind(C, name="CVodeFree")
use, intrinsic :: ISO_C_BINDING
type(C_PTR) :: cvode_mem
end subroutine

function FCVBandPrecInit(cvode_mem, n, mu, ml) &
bind(C, name="CVBandPrecInit") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT64_T), value :: n
integer(C_INT64_T), value :: mu
integer(C_INT64_T), value :: ml
integer(C_INT) :: fresult
end function

function FCVBandPrecGetWorkSpace(cvode_mem, lenrwls, leniwls) &
bind(C, name="CVBandPrecGetWorkSpace") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: lenrwls
integer(C_LONG) :: leniwls
integer(C_INT) :: fresult
end function

function FCVBandPrecGetNumRhsEvals(cvode_mem, nfevalsbp) &
bind(C, name="CVBandPrecGetNumRhsEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nfevalsbp
integer(C_INT) :: fresult
end function

function FCVBBDPrecInit(cvode_mem, nlocal, mudq, mldq, mukeep, mlkeep, dqrely, gloc, cfn) &
bind(C, name="CVBBDPrecInit") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT64_T), value :: nlocal
integer(C_INT64_T), value :: mudq
integer(C_INT64_T), value :: mldq
integer(C_INT64_T), value :: mukeep
integer(C_INT64_T), value :: mlkeep
real(C_DOUBLE), value :: dqrely
type(C_FUNPTR), value :: gloc
type(C_FUNPTR), value :: cfn
integer(C_INT) :: fresult
end function

function FCVBBDPrecReInit(cvode_mem, mudq, mldq, dqrely) &
bind(C, name="CVBBDPrecReInit") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT64_T), value :: mudq
integer(C_INT64_T), value :: mldq
real(C_DOUBLE), value :: dqrely
integer(C_INT) :: fresult
end function

function FCVBBDPrecGetWorkSpace(cvode_mem, lenrwbbdp, leniwbbdp) &
bind(C, name="CVBBDPrecGetWorkSpace") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: lenrwbbdp
integer(C_LONG) :: leniwbbdp
integer(C_INT) :: fresult
end function

function FCVBBDPrecGetNumGfnEvals(cvode_mem, ngevalsbbdp) &
bind(C, name="CVBBDPrecGetNumGfnEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: ngevalsbbdp
integer(C_INT) :: fresult
end function

function FCVDiag(cvode_mem) &
bind(C, name="CVDiag") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_INT) :: fresult
end function

function FCVDiagGetWorkSpace(cvode_mem, lenrwls, leniwls) &
bind(C, name="CVDiagGetWorkSpace") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: lenrwls
integer(C_LONG) :: leniwls
integer(C_INT) :: fresult
end function

function FCVDiagGetNumRhsEvals(cvode_mem, nfevalsls) &
bind(C, name="CVDiagGetNumRhsEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nfevalsls
integer(C_INT) :: fresult
end function

function FCVDiagGetLastFlag(cvode_mem, flag) &
bind(C, name="CVDiagGetLastFlag") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: flag
integer(C_INT) :: fresult
end function

function FCVDiagGetReturnFlagName(flag) &
bind(C, name="CVDiagGetReturnFlagName") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
integer(C_LONG), value :: flag
type(C_PTR) :: fresult
end function

function FCVodeSetLinearSolver(cvode_mem, ls, a) &
bind(C, name="CVodeSetLinearSolver") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_PTR), value :: ls
type(C_PTR), value :: a
integer(C_INT) :: fresult
end function

function FCVodeSetJacFn(cvode_mem, jac) &
bind(C, name="CVodeSetJacFn") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_FUNPTR), value :: jac
integer(C_INT) :: fresult
end function

function FCVodeSetMaxStepsBetweenJac(cvode_mem, msbj) &
bind(C, name="CVodeSetMaxStepsBetweenJac") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG), value :: msbj
integer(C_INT) :: fresult
end function

function FCVodeSetEpsLin(cvode_mem, eplifac) &
bind(C, name="CVodeSetEpsLin") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
real(C_DOUBLE), value :: eplifac
integer(C_INT) :: fresult
end function

function FCVodeSetPreconditioner(cvode_mem, pset, psolve) &
bind(C, name="CVodeSetPreconditioner") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_FUNPTR), value :: pset
type(C_FUNPTR), value :: psolve
integer(C_INT) :: fresult
end function

function FCVodeSetJacTimes(cvode_mem, jtsetup, jtimes) &
bind(C, name="CVodeSetJacTimes") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
type(C_FUNPTR), value :: jtsetup
type(C_FUNPTR), value :: jtimes
integer(C_INT) :: fresult
end function

function FCVodeGetLinWorkSpace(cvode_mem, lenrwls, leniwls) &
bind(C, name="CVodeGetLinWorkSpace") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: lenrwls
integer(C_LONG) :: leniwls
integer(C_INT) :: fresult
end function

function FCVodeGetNumJacEvals(cvode_mem, njevals) &
bind(C, name="CVodeGetNumJacEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: njevals
integer(C_INT) :: fresult
end function

function FCVodeGetNumPrecEvals(cvode_mem, npevals) &
bind(C, name="CVodeGetNumPrecEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: npevals
integer(C_INT) :: fresult
end function

function FCVodeGetNumPrecSolves(cvode_mem, npsolves) &
bind(C, name="CVodeGetNumPrecSolves") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: npsolves
integer(C_INT) :: fresult
end function

function FCVodeGetNumLinIters(cvode_mem, nliters) &
bind(C, name="CVodeGetNumLinIters") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nliters
integer(C_INT) :: fresult
end function

function FCVodeGetNumLinConvFails(cvode_mem, nlcfails) &
bind(C, name="CVodeGetNumLinConvFails") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nlcfails
integer(C_INT) :: fresult
end function

function FCVodeGetNumJTSetupEvals(cvode_mem, njtsetups) &
bind(C, name="CVodeGetNumJTSetupEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: njtsetups
integer(C_INT) :: fresult
end function

function FCVodeGetNumJtimesEvals(cvode_mem, njvevals) &
bind(C, name="CVodeGetNumJtimesEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: njvevals
integer(C_INT) :: fresult
end function

function FCVodeGetNumLinRhsEvals(cvode_mem, nfevalsls) &
bind(C, name="CVodeGetNumLinRhsEvals") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: nfevalsls
integer(C_INT) :: fresult
end function

function FCVodeGetLastLinFlag(cvode_mem, flag) &
bind(C, name="CVodeGetLastLinFlag") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: cvode_mem
integer(C_LONG) :: flag
integer(C_INT) :: fresult
end function

function FCVodeGetLinReturnFlagName(flag) &
bind(C, name="CVodeGetLinReturnFlagName") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
integer(C_LONG), value :: flag
type(C_PTR) :: fresult
end function

 end interface


end module
