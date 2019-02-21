# ---------------------------------------------------------------
# Programmer: Cody J. Balos @ LLNL
# ---------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2019, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# ---------------------------------------------------------------
# CMake macro for adding F2003 interface libraries.
# Wraps the add_library command for adding Fortran 2003 interfaces.
#
# It appends the library type to the Fortran_MODULE_DIRECTORY so
# that the .mod file generated during compilation of shared and
# static libraries do not collide.
# It also adds the .mod file directory to the target's includes.
# It also adds the C library as a library to link to.
#
# REQUIRES following the F2003 interface library naming convention.
# I.e. if the C library/target is sundials_cvode_static then the
# F2003 target must be sundials_fcvode_mod_static.
# ---------------------------------------------------------------

MACRO(sundials_add_f2003_interface_library target libtype)

  set(options NO_OBJECT_LIBRARY)
  set(oneValueArgs )
  set(multiValueArgs )

  # parse keyword arguments/options
  cmake_parse_arguments(sundials_add_f2003_interface_library
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # source files are unparsed additional arguments
  set(sources ${sundials_add_f2003_interface_library_UNPARSED_ARGUMENTS})

  if(NOT ${sundials_add_f2003_interface_library_NO_OBJECT_LIBRARY})
    # give the object library its own target name
    set(obj_target ${target}_obj)
    # create the target for the object library
    add_library(${obj_target} OBJECT ${sources})
    # now create the real library
    add_library(${target} ${libtype} $<TARGET_OBJECTS:${obj_target}>)
    # for shared libs, the objects must be PIC
    if("${libtype}" MATCHES "SHARED")
      set_target_properties(${obj_target} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
    endif()
  else()
    # alias obj_target variable to target since there is no obj_target
    set(obj_target ${target})
    add_library(${target} ${libtype} ${sources})
  endif()

  # set target properties and target dependencies so that includes
  # and links get passed on when this target is used
  if(CMAKE_Fortran_MODULE_DIRECTORY)
    target_include_directories(${target}
      PUBLIC
        $<BUILD_INTERFACE:${CMAKE_Fortran_MODULE_DIRECTORY}_${libtype}>
        $<INSTALL_INTERFACE:${Fortran_INSTALL_MODDIR}>
      )
    set_target_properties(${obj_target} PROPERTIES Fortran_MODULE_DIRECTORY "${CMAKE_Fortran_MODULE_DIRECTORY}_${libtype}")
  else()
    target_include_directories(${target}
      PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir>
        $<INSTALL_INTERFACE:${Fortran_INSTALL_MODDIR}>
      )
    set_target_properties(${obj_target} PROPERTIES Fortran_MODULE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir")
  endif()

  # get the name of the C library which the fortran library interfaces too
  string(REPLACE "sundials_f" "sundials_" c_lib_name "${target}")
  string(REPLACE "_mod_" "_" c_lib_name "${c_lib_name}")

  # depend on the C library
  target_link_libraries(${target} PUBLIC ${c_lib_name})

ENDMACRO(sundials_add_f2003_interface_library)

