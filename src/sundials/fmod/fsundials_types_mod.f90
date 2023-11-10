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

module fsundials_types_mod
 use, intrinsic :: ISO_C_BINDING
 implicit none
 private

 ! DECLARATION CONSTRUCTS
 integer(C_INT), parameter, public :: SUNFALSE = 0_C_INT
 integer(C_INT), parameter, public :: SUNTRUE = 1_C_INT
 ! typedef enum SUNOutputFormat
 enum, bind(c)
  enumerator :: SUN_OUTPUTFORMAT_TABLE
  enumerator :: SUN_OUTPUTFORMAT_CSV
 end enum
 integer, parameter, public :: SUNOutputFormat = kind(SUN_OUTPUTFORMAT_TABLE)
 public :: SUN_OUTPUTFORMAT_TABLE, SUN_OUTPUTFORMAT_CSV
 integer(C_INT), parameter, public :: SUNComm_NULL = 0_C_INT

end module
