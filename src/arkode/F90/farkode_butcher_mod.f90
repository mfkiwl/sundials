! This file was automatically generated by SWIG (http://www.swig.org).
! Version 4.0.0
!
! Do not make changes to this file unless you know what you are doing--modify
! the SWIG interface file instead.
module farkode_butcher_mod
 use, intrinsic :: ISO_C_BINDING
 use fsundials_types
 implicit none
 private

 ! PUBLIC METHODS AND TYPES
 public :: ARKodeButcherTableMem
 public :: FARKodeButcherTable_Alloc
 public :: FARKodeButcherTable_Create
 public :: FARKodeButcherTable_Copy
 public :: FARKodeButcherTable_Space
 public :: FARKodeButcherTable_Free
 public :: FARKodeButcherTable_Write
 public :: FARKodeButcherTable_CheckOrder
 public :: FARKodeButcherTable_CheckARKOrder
 public :: FARKodeButcherTable_LoadDIRK
 public :: FARKodeButcherTable_LoadERK

 ! PARAMETERS
 integer(C_INT), parameter, public :: SDIRK_2_1_2 = 100_C_INT
 integer(C_INT), parameter, public :: BILLINGTON_3_3_2 = 101_C_INT
 integer(C_INT), parameter, public :: TRBDF2_3_3_2 = 102_C_INT
 integer(C_INT), parameter, public :: KVAERNO_4_2_3 = 103_C_INT
 integer(C_INT), parameter, public :: ARK324L2SA_DIRK_4_2_3 = 104_C_INT
 integer(C_INT), parameter, public :: CASH_5_2_4 = 105_C_INT
 integer(C_INT), parameter, public :: CASH_5_3_4 = 106_C_INT
 integer(C_INT), parameter, public :: SDIRK_5_3_4 = 107_C_INT
 integer(C_INT), parameter, public :: KVAERNO_5_3_4 = 108_C_INT
 integer(C_INT), parameter, public :: ARK436L2SA_DIRK_6_3_4 = 109_C_INT
 integer(C_INT), parameter, public :: KVAERNO_7_4_5 = 110_C_INT
 integer(C_INT), parameter, public :: ARK548L2SA_DIRK_8_4_5 = 111_C_INT
 integer(C_INT), parameter, public :: MIN_DIRK_NUM = 100_C_INT
 integer(C_INT), parameter, public :: MAX_DIRK_NUM = 111_C_INT
 integer(C_INT), parameter, public :: HEUN_EULER_2_1_2 = 0_C_INT
 integer(C_INT), parameter, public :: BOGACKI_SHAMPINE_4_2_3 = 1_C_INT
 integer(C_INT), parameter, public :: ARK324L2SA_ERK_4_2_3 = 2_C_INT
 integer(C_INT), parameter, public :: ZONNEVELD_5_3_4 = 3_C_INT
 integer(C_INT), parameter, public :: ARK436L2SA_ERK_6_3_4 = 4_C_INT
 integer(C_INT), parameter, public :: SAYFY_ABURUB_6_3_4 = 5_C_INT
 integer(C_INT), parameter, public :: CASH_KARP_6_4_5 = 6_C_INT
 integer(C_INT), parameter, public :: FEHLBERG_6_4_5 = 7_C_INT
 integer(C_INT), parameter, public :: DORMAND_PRINCE_7_4_5 = 8_C_INT
 integer(C_INT), parameter, public :: ARK548L2SA_ERK_8_4_5 = 9_C_INT
 integer(C_INT), parameter, public :: VERNER_8_5_6 = 10_C_INT
 integer(C_INT), parameter, public :: FEHLBERG_13_7_8 = 11_C_INT
 integer(C_INT), parameter, public :: KNOTH_WOLKE_3_3 = 12_C_INT
 integer(C_INT), parameter, public :: MIN_ERK_NUM = 0_C_INT
 integer(C_INT), parameter, public :: MAX_ERK_NUM = 12_C_INT

 ! TYPES
 type, bind(C) :: ARKodeButcherTableMem
  integer(C_INT), public :: q
  integer(C_INT), public :: p
  integer(C_INT), public :: stages
  type(C_PTR), public :: A
  type(C_PTR), public :: c
  type(C_PTR), public :: b
  type(C_PTR), public :: d
 end type ARKodeButcherTableMem


 ! WRAPPER DECLARATIONS
 interface
function FARKodeButcherTable_Alloc(stages, embedded) &
bind(C, name="ARKodeButcherTable_Alloc") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
integer(C_INT), value :: stages
logical(C_BOOL), value :: embedded
type(C_PTR) :: fresult
end function

function FARKodeButcherTable_Create(s, q, p, c, a, b, d) &
bind(C, name="ARKodeButcherTable_Create") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
integer(C_INT), value :: s
integer(C_INT), value :: q
integer(C_INT), value :: p
real(C_DOUBLE) :: c
real(C_DOUBLE) :: a
real(C_DOUBLE) :: b
real(C_DOUBLE) :: d
type(C_PTR) :: fresult
end function

function FARKodeButcherTable_Copy(b) &
bind(C, name="ARKodeButcherTable_Copy") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: b
type(C_PTR) :: fresult
end function

subroutine FARKodeButcherTable_Space(b, liw, lrw) &
bind(C, name="ARKodeButcherTable_Space")
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: b
integer(C_INT64_T) :: liw
integer(C_INT64_T) :: lrw
end subroutine

subroutine FARKodeButcherTable_Free(b) &
bind(C, name="ARKodeButcherTable_Free")
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: b
end subroutine

subroutine FARKodeButcherTable_Write(b, outfile) &
bind(C, name="ARKodeButcherTable_Write")
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: b
type(C_PTR), value :: outfile
end subroutine

function FARKodeButcherTable_CheckOrder(b, q, p, outfile) &
bind(C, name="ARKodeButcherTable_CheckOrder") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: b
integer(C_INT) :: q
integer(C_INT) :: p
type(C_PTR), value :: outfile
integer(C_INT) :: fresult
end function

function FARKodeButcherTable_CheckARKOrder(b1, b2, q, p, outfile) &
bind(C, name="ARKodeButcherTable_CheckARKOrder") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
type(C_PTR), value :: b1
type(C_PTR), value :: b2
integer(C_INT) :: q
integer(C_INT) :: p
type(C_PTR), value :: outfile
integer(C_INT) :: fresult
end function

function FARKodeButcherTable_LoadDIRK(imethod) &
bind(C, name="ARKodeButcherTable_LoadDIRK") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
integer(C_INT), value :: imethod
type(C_PTR) :: fresult
end function

function FARKodeButcherTable_LoadERK(imethod) &
bind(C, name="ARKodeButcherTable_LoadERK") &
result(fresult)
use, intrinsic :: ISO_C_BINDING
integer(C_INT), value :: imethod
type(C_PTR) :: fresult
end function

 end interface


end module