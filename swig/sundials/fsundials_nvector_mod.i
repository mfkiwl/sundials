// ---------------------------------------------------------------
// Programmer: Cody J. Balos @ LLNL
// ---------------------------------------------------------------
// SUNDIALS Copyright Start
// Copyright (c) 2002-2019, Lawrence Livermore National Security
// and Southern Methodist University.
// All rights reserved.
//
// See the top-level LICENSE and NOTICE files for details.
//
// SPDX-License-Identifier: BSD-3-Clause
// SUNDIALS Copyright End
// ---------------------------------------------------------------
// Swig interface file
// ---------------------------------------------------------------

%module fsundials_nvector_mod

// Load the typedefs and generate a "use fsundials_types_mod" statement in the module
%import "../sundials/fsundials_types_mod.i"

// insert the include into the swig wrapper
%{
#include "sundials/sundials_nvector.h"  
%}

// Ignore functions with arrays of vector arrays since they are not supported
%ignore N_VScaleAddMultiVectorArray;
%ignore N_VLinearCombinationVectorArray;

// Process and wrap functions in the following files
%include "sundials/sundials_nvector.h"

