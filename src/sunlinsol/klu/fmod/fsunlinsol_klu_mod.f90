! This file was automatically generated by SWIG (http://www.swig.org).
! Version 4.0.0
!
! Do not make changes to this file unless you know what you are doing--modify
! the SWIG interface file instead.

! ---------------------------------------------------------------
! Programmer(s): Auto-generated by swig.
! ---------------------------------------------------------------
! SUNDIALS Copyright Start
! Copyright (c) 2002-2023, Lawrence Livermore National Security
! and Southern Methodist University.
! All rights reserved.
!
! See the top-level LICENSE and NOTICE files for details.
!
! SPDX-License-Identifier: BSD-3-Clause
! SUNDIALS Copyright End
! ---------------------------------------------------------------

module fsunlinsol_klu_mod
 use, intrinsic :: ISO_C_BINDING
 use fsundials_types_mod
 use fsundials_linearsolver_mod
 use fsundials_context_mod
 use fsundials_types_mod
 use fsundials_types_mod
 use fsundials_nvector_mod
 use fsundials_context_mod
 use fsundials_types_mod
 use fsundials_matrix_mod
 use fsundials_nvector_mod
 use fsundials_context_mod
 use fsundials_types_mod
 implicit none
 private

 ! DECLARATION CONSTRUCTS
 integer(C_INT), parameter, public :: SUNKLU_ORDERING_DEFAULT = 1_C_INT
 integer(C_INT), parameter, public :: SUNKLU_REINIT_FULL = 1_C_INT
 integer(C_INT), parameter, public :: SUNKLU_REINIT_PARTIAL = 2_C_INT
 public :: FSUNLinSol_KLU
 public :: FSUNLinSol_KLUReInit
 public :: FSUNLinSol_KLUSetOrdering

 integer, parameter :: swig_cmem_own_bit = 0
 integer, parameter :: swig_cmem_rvalue_bit = 1
 integer, parameter :: swig_cmem_const_bit = 2
 type, bind(C) :: SwigClassWrapper
  type(C_PTR), public :: cptr = C_NULL_PTR
  integer(C_INT), public :: cmemflags = 0
 end type
 type, public :: SWIGTYPE_p_klu_l_symbolic
  type(SwigClassWrapper), public :: swigdata
 end type
 public :: FSUNLinSol_KLUGetSymbolic
 type, public :: SWIGTYPE_p_klu_l_numeric
  type(SwigClassWrapper), public :: swigdata
 end type
 public :: FSUNLinSol_KLUGetNumeric
 type, public :: SWIGTYPE_p_klu_l_common
  type(SwigClassWrapper), public :: swigdata
 end type
 public :: FSUNLinSol_KLUGetCommon
 public :: FSUNLinSolGetType_KLU
 public :: FSUNLinSolGetID_KLU
 public :: FSUNLinSolInitialize_KLU
 public :: FSUNLinSolSetup_KLU
 public :: FSUNLinSolSolve_KLU
 public :: FSUNLinSolLastFlag_KLU
 public :: FSUNLinSolSpace_KLU
 public :: FSUNLinSolFree_KLU

! WRAPPER DECLARATIONS
interface
function swigc_FSUNLinSol_KLU(farg1, farg2, farg3) &
bind(C, name="_wrap_FSUNLinSol_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
type(C_PTR), value :: farg2
type(C_PTR), value :: farg3
type(C_PTR) :: fresult
end function

function swigc_FSUNLinSol_KLUReInit(farg1, farg2, farg3, farg4) &
bind(C, name="_wrap_FSUNLinSol_KLUReInit") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
type(C_PTR), value :: farg2
integer(C_INT64_T), intent(in) :: farg3
integer(C_INT), intent(in) :: farg4
integer(C_INT) :: fresult
end function

function swigc_FSUNLinSol_KLUSetOrdering(farg1, farg2) &
bind(C, name="_wrap_FSUNLinSol_KLUSetOrdering") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT), intent(in) :: farg2
integer(C_INT) :: fresult
end function

function swigc_FSUNLinSol_KLUGetSymbolic(farg1) &
bind(C, name="_wrap_FSUNLinSol_KLUGetSymbolic") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
import :: swigclasswrapper
type(C_PTR), value :: farg1
type(SwigClassWrapper) :: fresult
end function

function swigc_FSUNLinSol_KLUGetNumeric(farg1) &
bind(C, name="_wrap_FSUNLinSol_KLUGetNumeric") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
import :: swigclasswrapper
type(C_PTR), value :: farg1
type(SwigClassWrapper) :: fresult
end function

function swigc_FSUNLinSol_KLUGetCommon(farg1) &
bind(C, name="_wrap_FSUNLinSol_KLUGetCommon") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
import :: swigclasswrapper
type(C_PTR), value :: farg1
type(SwigClassWrapper) :: fresult
end function

function swigc_FSUNLinSolGetType_KLU(farg1) &
bind(C, name="_wrap_FSUNLinSolGetType_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT) :: fresult
end function

function swigc_FSUNLinSolGetID_KLU(farg1) &
bind(C, name="_wrap_FSUNLinSolGetID_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT) :: fresult
end function

function swigc_FSUNLinSolInitialize_KLU(farg1) &
bind(C, name="_wrap_FSUNLinSolInitialize_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT) :: fresult
end function

function swigc_FSUNLinSolSetup_KLU(farg1, farg2) &
bind(C, name="_wrap_FSUNLinSolSetup_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
type(C_PTR), value :: farg2
integer(C_INT) :: fresult
end function

function swigc_FSUNLinSolSolve_KLU(farg1, farg2, farg3, farg4, farg5) &
bind(C, name="_wrap_FSUNLinSolSolve_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
type(C_PTR), value :: farg2
type(C_PTR), value :: farg3
type(C_PTR), value :: farg4
real(C_DOUBLE), intent(in) :: farg5
integer(C_INT) :: fresult
end function

function swigc_FSUNLinSolLastFlag_KLU(farg1) &
bind(C, name="_wrap_FSUNLinSolLastFlag_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT64_T) :: fresult
end function

function swigc_FSUNLinSolSpace_KLU(farg1, farg2, farg3) &
bind(C, name="_wrap_FSUNLinSolSpace_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
type(C_PTR), value :: farg2
type(C_PTR), value :: farg3
integer(C_INT) :: fresult
end function

function swigc_FSUNLinSolFree_KLU(farg1) &
bind(C, name="_wrap_FSUNLinSolFree_KLU") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: farg1
integer(C_INT) :: fresult
end function

end interface


contains
 ! MODULE SUBPROGRAMS
function FSUNLinSol_KLU(y, a, sunctx) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
type(SUNLinearSolver), pointer :: swig_result
type(N_Vector), target, intent(inout) :: y
type(SUNMatrix), target, intent(inout) :: a
type(C_PTR) :: sunctx
type(C_PTR) :: fresult 
type(C_PTR) :: farg1 
type(C_PTR) :: farg2 
type(C_PTR) :: farg3 

farg1 = c_loc(y)
farg2 = c_loc(a)
farg3 = sunctx
fresult = swigc_FSUNLinSol_KLU(farg1, farg2, farg3)
call c_f_pointer(fresult, swig_result)
end function

function FSUNLinSol_KLUReInit(s, a, nnz, reinit_type) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
type(SUNMatrix), target, intent(inout) :: a
integer(C_INT64_T), intent(in) :: nnz
integer(C_INT), intent(in) :: reinit_type
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
type(C_PTR) :: farg2 
integer(C_INT64_T) :: farg3 
integer(C_INT) :: farg4 

farg1 = c_loc(s)
farg2 = c_loc(a)
farg3 = nnz
farg4 = reinit_type
fresult = swigc_FSUNLinSol_KLUReInit(farg1, farg2, farg3, farg4)
swig_result = fresult
end function

function FSUNLinSol_KLUSetOrdering(s, ordering_choice) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
integer(C_INT), intent(in) :: ordering_choice
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
integer(C_INT) :: farg2 

farg1 = c_loc(s)
farg2 = ordering_choice
fresult = swigc_FSUNLinSol_KLUSetOrdering(farg1, farg2)
swig_result = fresult
end function

function FSUNLinSol_KLUGetSymbolic(s) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
type(SWIGTYPE_p_klu_l_symbolic) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
type(SwigClassWrapper) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(s)
fresult = swigc_FSUNLinSol_KLUGetSymbolic(farg1)
swig_result%swigdata = fresult
end function

function FSUNLinSol_KLUGetNumeric(s) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
type(SWIGTYPE_p_klu_l_numeric) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
type(SwigClassWrapper) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(s)
fresult = swigc_FSUNLinSol_KLUGetNumeric(farg1)
swig_result%swigdata = fresult
end function

function FSUNLinSol_KLUGetCommon(s) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
type(SWIGTYPE_p_klu_l_common) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
type(SwigClassWrapper) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(s)
fresult = swigc_FSUNLinSol_KLUGetCommon(farg1)
swig_result%swigdata = fresult
end function

function FSUNLinSolGetType_KLU(s) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(SUNLinearSolver_Type) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(s)
fresult = swigc_FSUNLinSolGetType_KLU(farg1)
swig_result = fresult
end function

function FSUNLinSolGetID_KLU(s) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(SUNLinearSolver_ID) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(s)
fresult = swigc_FSUNLinSolGetID_KLU(farg1)
swig_result = fresult
end function

function FSUNLinSolInitialize_KLU(s) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(s)
fresult = swigc_FSUNLinSolInitialize_KLU(farg1)
swig_result = fresult
end function

function FSUNLinSolSetup_KLU(s, a) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
type(SUNMatrix), target, intent(inout) :: a
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
type(C_PTR) :: farg2 

farg1 = c_loc(s)
farg2 = c_loc(a)
fresult = swigc_FSUNLinSolSetup_KLU(farg1, farg2)
swig_result = fresult
end function

function FSUNLinSolSolve_KLU(s, a, x, b, tol) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
type(SUNMatrix), target, intent(inout) :: a
type(N_Vector), target, intent(inout) :: x
type(N_Vector), target, intent(inout) :: b
real(C_DOUBLE), intent(in) :: tol
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
type(C_PTR) :: farg2 
type(C_PTR) :: farg3 
type(C_PTR) :: farg4 
real(C_DOUBLE) :: farg5 

farg1 = c_loc(s)
farg2 = c_loc(a)
farg3 = c_loc(x)
farg4 = c_loc(b)
farg5 = tol
fresult = swigc_FSUNLinSolSolve_KLU(farg1, farg2, farg3, farg4, farg5)
swig_result = fresult
end function

function FSUNLinSolLastFlag_KLU(s) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT64_T) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
integer(C_INT64_T) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(s)
fresult = swigc_FSUNLinSolLastFlag_KLU(farg1)
swig_result = fresult
end function

function FSUNLinSolSpace_KLU(s, lenrwls, leniwls) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
integer(C_LONG), dimension(*), target, intent(inout) :: lenrwls
integer(C_LONG), dimension(*), target, intent(inout) :: leniwls
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 
type(C_PTR) :: farg2 
type(C_PTR) :: farg3 

farg1 = c_loc(s)
farg2 = c_loc(lenrwls(1))
farg3 = c_loc(leniwls(1))
fresult = swigc_FSUNLinSolSpace_KLU(farg1, farg2, farg3)
swig_result = fresult
end function

function FSUNLinSolFree_KLU(s) &
result(swig_result)
use, intrinsic :: ISO_C_BINDING
integer(C_INT) :: swig_result
type(SUNLinearSolver), target, intent(inout) :: s
integer(C_INT) :: fresult 
type(C_PTR) :: farg1 

farg1 = c_loc(s)
fresult = swigc_FSUNLinSolFree_KLU(farg1)
swig_result = fresult
end function


end module
