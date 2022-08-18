/*
 * -----------------------------------------------------------------
 * Programmer(s): Cody J. Balos @ LLNL
 * -----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2022, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------
 */

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sundials/sundials_math.h>
#include <sundials/sundials_types.h>
#include <sunmatrix/sunmatrix_dense.h>
#include <sunmatrix/sunmatrix_ginkgo.hpp>

#include "test_sunmatrix.h"

#if defined(USE_HIP)
#define REF_OR_OMP_OR_HIP_OR_CUDA(a, b, c, d) c
#elif defined(USE_CUDA)
#define REF_OR_OMP_OR_HIP_OR_CUDA(a, b, c, d) d
#elif defined(USE_OMP)
#define REF_OR_OMP_OR_HIP_OR_CUDA(a, b, c, d) b
#else
#define REF_OR_OMP_OR_HIP_OR_CUDA(a, b, c, d) a
#endif

#if defined(USE_CUDA)
#include <nvector/nvector_cuda.h>
#elif defined(USE_HIP)
#include <nvector/nvector_hip.h>
#else
#include <nvector/nvector_serial.h>
#endif

using namespace sundials::ginkgo;

using GkoCsrMat   = gko::matrix::Csr<sunrealtype, sunindextype>;
using GkoDenseMat = gko::matrix::Dense<sunrealtype>;

bool using_csr_matrix_type   = false;
bool using_dense_matrix_type = false;

/* ----------------------------------------------------------------------
 * Main SUNMatrix Testing Routine
 * --------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  int fails{0}; /* counter for test failures */

  /* Create SUNDIALS context before calling any other SUNDIALS function*/
  sundials::Context sunctx;

  auto gko_exec{REF_OR_OMP_OR_HIP_OR_CUDA(gko::ReferenceExecutor::create(), gko::OmpExecutor::create(),
                                          gko::HipExecutor::create(0, gko::OmpExecutor::create(), true),
                                          gko::CudaExecutor::create(0, gko::OmpExecutor::create(), true))};

  /* check input and set vector length */
  if (argc < 4) {
    std::cerr << "ERROR: THREE (3) Input required: matrix rows, matrix cols, format (0 = csr, 1 = dense)\n";
    return -1;
  }

  int argi{0};

  auto matrows{static_cast<sunindextype>(atol(argv[++argi]))};
  if (matrows <= 0) {
    std::cerr << "ERROR: number of rows must be a positive integer \n";
    return -1;
  }

  auto matcols{static_cast<sunindextype>(atol(argv[++argi]))};
  if (matcols <= 0) {
    std::cerr << "ERROR: number of cols must be a positive integer \n";
    return -1;
  }

  auto format{static_cast<int>(atoi(argv[++argi]))};
  if (format != 0 && format != 1) {
    std::cerr << "ERROR: format must be 0 (csr) or 1 (dense) \n";
    return -1;
  }

  if (format == 0) {
    using_csr_matrix_type = true;
  } else if (format == 1) {
    using_dense_matrix_type = true;
  }

  SetTiming(0);

  int square{matrows == matcols ? 1 : 0};
  std::cout << "\n SUNMATRIX_GINKGO test: size " << matrows << " x " << matcols << ", format " << format << "\n";

  /* Create vectors and matrices */
  std::default_random_engine generator;
  std::uniform_real_distribution<sunrealtype> distribution{0.0, static_cast<sunrealtype>(matrows)};

  N_Vector x{REF_OR_OMP_OR_HIP_OR_CUDA(N_VNew_Serial(matcols, sunctx), N_VNew_Serial(matcols, sunctx),
                                       N_VNew_Hip(matcols, sunctx), N_VNew_Cuda(matcols, sunctx))};
  N_Vector y{REF_OR_OMP_OR_HIP_OR_CUDA(N_VNew_Serial(matcols, sunctx), N_VNew_Serial(matcols, sunctx),
                                       N_VNew_Hip(matcols, sunctx), N_VNew_Cuda(matcols, sunctx))};

  auto matrix_dim{gko::dim<2>(matrows, matcols)};
  auto gko_matdata{gko::matrix_data<sunrealtype, sunindextype>(matrix_dim, distribution, generator)};

  /* Wrap ginkgo matrices for SUNDIALS.
     sundials::ginkgo::Matrix is overloaded to a SUNMatrix. */

  std::unique_ptr<sundials::ConvertibleTo<SUNMatrix>> A;
  std::unique_ptr<sundials::ConvertibleTo<SUNMatrix>> I;

  auto xdata{N_VGetArrayPointer(x)};
  for (sunindextype i = 0; i < matcols; i++) {
    xdata[i] = distribution(generator);
  }
  REF_OR_OMP_OR_HIP_OR_CUDA(, , N_VCopyToDevice_Hip(x), N_VCopyToDevice_Cuda(x));

  /* Compute true solution */
  SUNMatrix Aref{SUNDenseMatrix(matrows, matcols, sunctx)};
  if (using_csr_matrix_type) {
    auto gko_matrix{gko::share(GkoCsrMat::create(gko_exec, matrix_dim))};
    auto gko_ident{gko::share(GkoCsrMat::create(gko_exec, matrix_dim))};
    gko_matrix->read(gko_matdata);
    if (square) {
      gko_ident->read(gko::matrix_data<sunrealtype, sunindextype>::diag(matrix_dim, 1.0));
    }
    auto Arowptrs{gko_matrix->get_const_row_ptrs()};
    auto Acolidxs{gko_matrix->get_const_col_idxs()};
    auto Avalues{gko_matrix->get_const_values()};
    for (auto irow = 0; irow < gko_matrix->get_size()[0]; irow++) {
      for (auto inz = Arowptrs[irow]; inz < Arowptrs[irow + 1]; inz++) {
        SM_ELEMENT_D(Aref, irow, Acolidxs[inz]) = Avalues[inz];
      }
    }
    A = std::make_unique<Matrix<GkoCsrMat>>(gko_matrix, sunctx);
    I = std::make_unique<Matrix<GkoCsrMat>>(gko_ident, sunctx);
  } else if (using_dense_matrix_type) {
    auto gko_matrix = gko::share(GkoDenseMat::create(gko_exec, matrix_dim));
    auto gko_ident{gko::share(GkoDenseMat::create(gko_exec, matrix_dim))};
    gko_matrix->read(gko_matdata);
    if (square) {
      gko_ident->read(gko::matrix_data<sunrealtype, sunindextype>::diag(matrix_dim, 1.0));
    }
    for (sunindextype j = 0; j < matcols; j++) {
      for (sunindextype i = 0; i < matrows; i++) {
        SM_ELEMENT_D(Aref, i, j) = gko_matrix->at(i, j);
      }
    }
    A = std::make_unique<Matrix<GkoDenseMat>>(gko_matrix, sunctx);
    I = std::make_unique<Matrix<GkoDenseMat>>(gko_ident, sunctx);
  }
  SUNMatMatvec_Dense(Aref, x, y);
  SUNMatDestroy(Aref);

  REF_OR_OMP_OR_HIP_OR_CUDA(, , N_VCopyToDevice_Hip(y), N_VCopyToDevice_Cuda(y));

  /* SUNMatrix Tests */
  fails += Test_SUNMatGetID(*A, SUNMATRIX_GINKGO, 0);
  fails += Test_SUNMatClone(*A, 0);
  fails += Test_SUNMatCopy(*A, 0);
  fails += Test_SUNMatZero(*A, 0);
  if (square) {
    if (!using_dense_matrix_type) { // TODO(CJB): I think this is fixed on the batch-develop branch?
      fails += Test_SUNMatScaleAdd(*A, *I, 0);
    }
    fails += Test_SUNMatScaleAddI(*A, *I, 0);
  }
  fails += Test_SUNMatMatvec(*A, x, y, 0);

  /* Print result */
  if (fails) {
    std::cerr << "FAIL: SUNMatrix module failed " << fails << " tests \n\n";
  } else {
    std::cout << "SUCCESS: SUNMatrix module passed all tests \n\n";
  }

  /* Free vectors */
  N_VDestroy(x);
  N_VDestroy(y);

  return fails;
}

/* ----------------------------------------------------------------------
 * Check matrix
 * --------------------------------------------------------------------*/
int check_matrix_csr(SUNMatrix A, SUNMatrix B, realtype tol)
{
  int failure{0};
  auto Amat{static_cast<Matrix<GkoCsrMat>*>(A->content)->gkomtx()};
  auto Bmat{static_cast<Matrix<GkoCsrMat>*>(B->content)->gkomtx()};
  auto Arowptrs{Amat->get_const_row_ptrs()};
  auto Acolidxs{Amat->get_const_col_idxs()};
  auto Avalues{Amat->get_const_values()};
  auto Browptrs{Bmat->get_const_row_ptrs()};
  auto Bcolidxs{Bmat->get_const_col_idxs()};
  auto Bvalues{Bmat->get_const_values()};

  /* check lengths */
  if (Amat->get_size() != Bmat->get_size()) {
    std::cerr << ">>> ERROR: check_matrix: Different data array lengths \n";
    return (1);
  }

  /* compare data */
  for (sunindextype irow = 0; irow < Amat->get_size()[0]; irow++) {
    for (sunindextype inz = Arowptrs[irow]; inz < Arowptrs[irow + 1]; inz++) {
      failure += SUNRCompareTol(Avalues[inz], Bvalues[inz], tol);
    }
  }

  return failure > 0;
}

int check_matrix_dense(SUNMatrix A, SUNMatrix B, realtype tol)
{
  int failure{0};
  auto Amat{static_cast<Matrix<GkoDenseMat>*>(A->content)->gkomtx()};
  auto Bmat{static_cast<Matrix<GkoDenseMat>*>(B->content)->gkomtx()};
  auto rows{Amat->get_size()[0]};
  auto cols{Amat->get_size()[1]};

  /* check lengths */
  if (Amat->get_size() != Bmat->get_size()) {
    printf(">>> ERROR: check_matrix: Different data array lengths \n");
    return (1);
  }

  /* compare data */
  for (sunindextype i = 0; i < rows; i++) {
    for (sunindextype j = 0; j < cols; j++) {
      int check = SUNRCompareTol(Amat->at(i, j), Bmat->at(i, j), tol);
      if (check) {
        printf("  A[%ld,%ld] = %g != B[%ld,%ld] = %g (err = %g)\n", (long int)i, (long int)j, Amat->at(i, j),
               (long int)i, (long int)j, Bmat->at(i, j), SUNRabs(Amat->at(i, j) - Bmat->at(i, j)));
        failure += check;
      }
    }
  }

  return failure > 0;
}

extern "C" int check_matrix(SUNMatrix A, SUNMatrix B, realtype tol)
{
  if (using_csr_matrix_type)
    return check_matrix_csr(A, B, tol);
  else if (using_dense_matrix_type)
    return check_matrix_dense(A, B, tol);
  else
    return -1;
}

int check_matrix_entry_csr(SUNMatrix A, realtype val, realtype tol)
{
  int failure{0};
  auto Amat{static_cast<Matrix<GkoCsrMat>*>(A->content)->gkomtx()};
  auto Arowptrs{Amat->get_const_row_ptrs()};
  auto Acolidxs{Amat->get_const_col_idxs()};
  auto Avalues{Amat->get_const_values()};

  /* compare data */
  for (sunindextype irow = 0; irow < Amat->get_size()[0]; irow++) {
    for (sunindextype inz = Arowptrs[irow]; inz < Arowptrs[irow + 1]; inz++) {
      int check = SUNRCompareTol(Avalues[inz], val, tol);
      if (check) {
        std::cerr << "  actual = " << Avalues[inz] << " != " << val << " (err = " << SUNRabs(Avalues[inz] - val) << ")\n";
        failure += check;
      }
    }
  }

  return failure > 0;
}

int check_matrix_entry_dense(SUNMatrix A, realtype val, realtype tol)
{
  int failure{0};
  auto Amat{static_cast<Matrix<GkoDenseMat>*>(A->content)->gkomtx()};
  auto rows{Amat->get_size()[0]};
  auto cols{Amat->get_size()[1]};

  /* compare data */
  for (sunindextype i = 0; i < rows; i++) {
    for (sunindextype j = 0; j < cols; j++) {
      int check = SUNRCompareTol(Amat->at(i, j), val, tol);
      if (check) {
        printf("  actual[%ld] = %g != %e (err = %g)\n", (long int)i, Amat->at(i, j), val, SUNRabs(Amat->at(i, j) - val));
        failure += check;
      }
    }
  }

  return failure > 0;
}

extern "C" int check_matrix_entry(SUNMatrix A, realtype val, realtype tol)
{
  if (using_csr_matrix_type) {
    return check_matrix_entry_csr(A, val, tol);
  } else if (using_dense_matrix_type) {
    return check_matrix_entry_dense(A, val, tol);
  } else {
    return -1;
  }
}

extern "C" int check_vector(N_Vector expected, N_Vector computed, realtype tol)
{
  int failure{0};

  /* copy vectors to host */
  REF_OR_OMP_OR_HIP_OR_CUDA(, , N_VCopyFromDevice_Hip(computed), N_VCopyFromDevice_Cuda(computed));
  REF_OR_OMP_OR_HIP_OR_CUDA(, , N_VCopyFromDevice_Hip(expected), N_VCopyFromDevice_Cuda(expected));

  /* get vector data */
  auto xdata{N_VGetArrayPointer(computed)};
  auto ydata{N_VGetArrayPointer(expected)};

  /* check data lengths */
  auto xldata{N_VGetLength(computed)};
  auto yldata{N_VGetLength(expected)};

  if (xldata != yldata) {
    std::cerr << "ERROR check_vector: different vector lengths\n";
    return (1);
  }

  /* check vector data */
  for (sunindextype i = 0; i < xldata; i++)
    failure += SUNRCompareTol(xdata[i], ydata[i], tol);

  if (failure > ZERO) {
    std::cerr << "Check_vector failures:\n";
    for (sunindextype i = 0; i < xldata; i++)
      if (SUNRCompareTol(xdata[i], ydata[i], tol) != 0)
        std::cerr << "  computed[" << i << "] = " << xdata[i] << " != " << ydata[i]
                  << " (err = " << SUNRabs(xdata[i] - ydata[i]) << ")\n";
  }

  return failure > 0;
}

extern "C" booleantype has_data(SUNMatrix A)
{
  // auto Amat = static_cast<SUNMatrixType*>(A->content)->gkomtx();
  // if (Amat->get_values() == NULL || Amat->get_size()[0] == 0 || Amat->get_size()[1] == 0)
  // return SUNFALSE;
  // else
  return SUNTRUE;
}

extern "C" booleantype is_square(SUNMatrix A)
{
  // auto Amat = static_cast<SUNMatrixType*>(A->content)->gkomtx();
  // if (Amat->get_size()[0] == Amat->get_size()[1])
  // return SUNTRUE;
  // else
  // return SUNFALSE;
  return SUNTRUE;
}

extern "C" void sync_device(SUNMatrix A)
{
  REF_OR_OMP_OR_HIP_OR_CUDA(, , hipDeviceSynchronize(), cudaDeviceSynchronize());
}
