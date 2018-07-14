#include "osqp_helper.h"
#include <iostream>


using namespace std;

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
                       c_int   *p)
{
    csc* tripletMatrix = csc_matrix(m, n, nz, x, i, p);
    tripletMatrix -> nz = nz;
    csc* cscMatrix =  triplet_to_csc(tripletMatrix, OSQP_NULL);

    c_free(tripletMatrix);

    return cscMatrix;
}


/**
 * Helper function that transform Compressed-Column-Sparse matrix to triplet form
 * @param  cscMatrix matrix in CSC format
 * @return           matrix in tripet format
 */
csc* cscToTriplet(const csc* cscMatrix)
{

    // allocate matrix in triplet format
    csc* triplet = csc_spalloc(cscMatrix -> m, cscMatrix -> n, cscMatrix -> nzmax, 1, 1);

    if (!triplet)
        return OSQP_NULL;

    triplet -> nz = cscMatrix -> p[cscMatrix -> n];

    const c_int nz = triplet -> nz;  // number of non-zeros in the matrix

    c_int currentColumn = 0;

    const c_int* cscMatrixP = cscMatrix -> p;
    const c_int* cscMatrixI = cscMatrix -> i;
    const c_float* cscMatrixX = cscMatrix -> x;

    c_int* tripletP = triplet -> p;
    c_int* tripletI = triplet -> i;
    c_float* tripletX = triplet -> x;

    for (c_int i = 0; i < nz; ++i)
    {
        if (cscMatrixP[currentColumn + 1] <= i)
            currentColumn++;

        tripletX[i] = cscMatrixX[i]; // copy data
        tripletI[i] = cscMatrixI[i]; // copy row indices
        tripletP[i] = currentColumn; // set column indices
    }

    return triplet;
}

/**
 * transform matrix in CSC format to CSR foramt
 * @param  cscMatrix input, a CSC matrix
 * @return           output, a CSR matrix
 */
csc* cscToCSR(const csc* cscMatrix)
{
    csc* triplet = cscToTriplet(cscMatrix);
    csc* csr = triplet_to_csr(triplet, OSQP_NULL);
    c_free(triplet);

    return csr;
}

void printSeperationLine(const string title)
{
    const int lineLength = 66;
    // const int HalfLength = (int) Length / 2;

    const int titleLength = title.length();

    // title too long
    if (titleLength + 2 >= lineLength) // 2 for blank space
    {
        for (int i = 0; i < lineLength; ++i)
        {
            std::cout << '-';
        }

        std::cout << std::endl;

        std::cout << title << std::endl;
    }
    else if (titleLength != 0) // has a title
    {
        const int firstHalfLength = (int) (lineLength - titleLength - 2) * 0.5; // 2 for 2 blank spaces
        const int secondHalfLength = lineLength - titleLength - 2 - firstHalfLength;

        for (int i = 0; i < firstHalfLength; ++i)
        {
            std::cout << '-';
        }
        std::cout << ' ' << title << ' ';
        for (int i = 0; i < secondHalfLength; ++i)
        {
            std::cout << '-';
        }
        std::cout << std::endl;
    }
    else // no title
    {
        for (int i = 0; i < lineLength; ++i)
        {
            std::cout << '-';
        }

        std::cout << std::endl;
    }
}

/**
 * print a matrix stored in the csc data structure, can be in csc or triplet format
 * @param matrix  matrix stored in the csc data structure
 */
void printMatrix(const csc* matrix, const string matrixName)
{
    if (matrix == OSQP_NULL)
    {
        std::cout << "the pointer to matrix is Null" << std::endl;
        return;
    }
    printSeperationLine(matrixName);

    const bool isCSC = (matrix -> nz == -1);

    if (isCSC)
        std::cout << "The matrix is in CSC (Compressed-Column-Sparse) form" << std::endl;
    else
        std::cout << "The matrix is in triplet form" << std::endl;

    c_int nz; // number of non-zeros in the matrix

    if (isCSC) // in CSC form
        nz = matrix -> p[matrix -> n]; // the last entry in p says how many non-zeros are there in the matrix
    else // in triplet form
        nz = matrix -> nz;

    std::cout << "[max number of non-zeros] nzmax: " << matrix -> nzmax << std::endl;
    std::cout << "[number of rows]          m:     " << matrix -> m << std::endl;
    std::cout << "[number of columns]       n:     " << matrix -> n << std::endl;
    std::cout << "[number of non-zeros]     nz:    " << nz << std::endl;


    const c_int* matrixP = matrix -> p;
    const c_int* matrixI = matrix -> i;
    const c_float* matrixX = matrix -> x;

    // print values
    std::cout << "[values] x: " << std::endl;
    for (c_int i = 0; i < nz; ++i)
    {
        std::cout << matrixX[i] << ", ";
    }
    std::cout << std::endl;

    // print row indices
    std::cout << "[row indices] i" << std::endl;
    for (c_int i = 0; i < nz; ++i)
    {
        std::cout << matrixI[i] << ", ";
    }
    std::cout << std::endl;

    // if in CSC form, print column pointers
    // if in triplet form, print column indices
    if (isCSC) // CSC
    {
        std::cout << "[column pointers] p" << std::endl;
        for (c_int i = 0; i < matrix -> n + 1; ++i)
        {
            std::cout << matrixP[i] << ", ";
        }
        std::cout << std::endl;
    }
    else // triplet form
    {
        std::cout << "[column indices] p" << std::endl;
        for (c_int i = 0; i < nz; ++i)
        {
            std::cout << matrixP[i] << ", ";
        }
        std::cout << std::endl;
    }

    printSeperationLine();

}


/**
 * [printObjective description]
 * @param p quadratic cost matrix, must be in csr format
 * @param q linear cost vector
 */
void printCost(const csc* P, const c_float* q)
{
    printf("objective\n");

    const c_int numberOfVariables = P -> m;

    const c_int linearBufferSize = numberOfVariables * 50;
    const c_int quadraticBufferSize = P -> p[numberOfVariables] * 80;// p is in csr format

    char linearBuffer[linearBufferSize];
    char quadraticBuffer[quadraticBufferSize];

    // first comes the linear part
    size_t nextWritingPosition = 0;
    for (c_int i = 0; i < numberOfVariables; ++i)
    {
        int numberOfCharsWritten = snprintf(&linearBuffer[nextWritingPosition], linearBufferSize - nextWritingPosition,
                                            "%+e x%04lld ", q[i], i);

        nextWritingPosition += numberOfCharsWritten;
    }

    // printf("[DB] vacancy in q buffer: %lld\n", linearBufferSize - nextWritingPosition);

    // now comes the quadratic part

    const c_int* columnIndices = P -> i; // https://www.bu.edu/pasi/files/2011/01/NathanBell1-10-1000.pdf
    const c_int* rowPointers = P -> p;
    const c_float* values = P -> x;

    nextWritingPosition = 0;
    for (c_int thisRow = 0; thisRow < numberOfVariables; ++thisRow) // for every row in P
    {
        // non-zeros columns in this row
        const c_int nonZerosStart = rowPointers[thisRow];  // the column index of the first non-zero in this row
        const c_int nonZerosEnd = rowPointers[thisRow + 1]; // actually one past the end
        const c_int numberOfNonZeros = nonZerosEnd - nonZerosStart; // number of non-zeros in this row

        for (c_int j = nonZerosStart; j < nonZerosEnd; ++j)
        {
            const c_int thisColumn = columnIndices[j];

            int numberOfCharsWritten = 0;

            if ( thisRow == thisColumn) // diagonal term, square
            {
                const c_float coefficient = values[j];
                numberOfCharsWritten = snprintf(&quadraticBuffer[nextWritingPosition], quadraticBufferSize - nextWritingPosition,
                                                "%+e x%04lld ^2 ", 2 * coefficient, thisRow);

            }
            else if (thisRow < thisColumn) // off-diagonal term in the upper triangular part of the P matrix, P is symmetric
            {
                const c_float coefficient = values[j];
                numberOfCharsWritten = snprintf(&quadraticBuffer[nextWritingPosition], quadraticBufferSize - nextWritingPosition,
                                                "%+e x%04lld x%04lld ", 2 * coefficient, thisRow, thisColumn);

            }

            nextWritingPosition += numberOfCharsWritten;
        }
    }

    // printf("[DB] vacancy in P buffer: %lld\n", quadraticBufferSize - nextWritingPosition);

    printf("%s+ [%s] / 2\n", linearBuffer, quadraticBuffer);


}


/**
 * print constraint in LP format
 * @param A A should be in csr format
 * @param l lower bound
 * @param u upper bound
 */
void printConstraints(const csc* A, const c_float* l, const c_float* u)
{

    printf("subject to\n");

    const c_int* columnIndices = A -> i; // https://www.bu.edu/pasi/files/2011/01/NathanBell1-10-1000.pdf
    const c_int* rowPointers = A -> p;
    const c_float* values = A -> x;

    const c_int numberOfConstraints = A -> m;

    for (c_int i = 0; i < numberOfConstraints; ++i)
    {
        const c_int start = rowPointers[i];
        const c_int end = rowPointers[i + 1]; // actually one past the end
        const c_int numberOfTerms = end - start;

        const size_t bufferSize = numberOfTerms * 50;

        char buffer[bufferSize];
        size_t nextWritingPosition = 0;

        for (c_int j = start; j < end; ++j)
        {

            c_float coefficient = values[j];
            c_int termIndex = columnIndices[j];

            int numberOfCharsWritten = snprintf ( &buffer[nextWritingPosition], bufferSize - nextWritingPosition,
                                                  "%+e x%04lld ", coefficient, termIndex);

            nextWritingPosition += numberOfCharsWritten;
        }
        // printf("[DB] vacancy in A buffer: %lld\n", bufferSize - nextWritingPosition);

        printf("c%04lld::  %e <= %s<= %e\n", i, l[i], buffer, u[i]);
    }

}

/**
 * print OSQP problem in LP format
 * You should ontly call this function after you have put the problem in the OSQPData
 * @param data the data where you store the matrices: P, A; vectors q, l, u; scalars: m, n;
 *             matrices in data should be in csc format
 */
void printProblem(const OSQPData * data)
{
    printSeperationLine("Problem");
    std::cout << "\\ From OSQP, written by Weidong Sun, ws134 [at] duke [dot] com " << std::endl;

    // print objective
    csc* PInCSR = cscToCSR(data -> P);
    printCost(PInCSR, data -> q);

    // print constraint
    csc* AInCSR = cscToCSR(data -> A);
    printConstraints(AInCSR, data -> l, data -> u);

    printSeperationLine("Problem End");
}

void dumpProblem(const OSQPData* data, const char fileName[])
{
	/* under development */
}