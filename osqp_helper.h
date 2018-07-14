#ifndef _OSQP_HELPER_H_
#define _OSQP_HELPER_H_ 

#include "osqp/osqp.h"
#include <sstream>

/**
 * This is a helper function for creating matrix for osqp
 * Create Compressed-Column-Sparse matrix from triplet form
    (no MALLOC to create inner arrays x, i, p)
    You should always try to use this function instead of the triplet_to_csc provided by osqp
 * @param  m     First dimension (number of rows)
 * @param  n     Second dimension (number of columns)
 * @param  nzmax Number of nonzero elements
 * @param  x     Vector of data
 * @param  i     Vector of row indices
 * @param  p     Vector of column indices
 * @return       New matrix pointer, in CSC form
 */
csc* tripletArrayToCSC(c_int    m,
                       c_int    n,
                       c_int    nz,
                       c_float *x,
                       c_int   *i,
                       c_int   *p);

/**
 * Helper function that transform Compressed-Column-Sparse matrix to triplet form
 * @param  cscMatrix matrix in CSC format
 * @return           matrix in tripet format
 */
csc* cscToTriplet(const csc* cscMatrix);

/**
 * transform matrix in CSC format to CSR foramt
 * @param  cscMatrix input, a CSC matrix
 * @return           output, a CSR matrix
 */
csc* cscToCSR(const csc* cscMatrix);


/**
 * print a matrix stored in the csc data structure, can be in csc or triplet format, can have problems with csr format
 * @param matrix  matrix stored in the csc data structure
 */
void printMatrix(const csc* matrix, const std::string matrixName = "Unkown matrix");

/**
 * print OSQP problem in LP format
 * You should ontly call this function after you have put the problem in the OSQPData
 * @param data the data where you store the matrices: P, A; vectors q, l, u; scalars: m, n;
 *             matrices in data should be in csc format
 */
void printProblem(const OSQPData * data);


void printSeperationLine(const std::string title = std::string());


#endif /* _OSQP_HELPER_H_ */