#include "osqp/osqp.h"
#include "osqp_helper.h"
#include <iostream>
#include <sstream>

using namespace std;
// written by Weidong Sun, ws134 [at] duke [dot] com
// 
// All the naming conventions follow osqp's documentation, make sure you are familiar with them.


#define TRIPLET 1 // whether the P matrix is defined

int main(int argc, char **argv) {
    // Load problem data
#if TRIPLET
    c_float P_x[4] = {4.00, 1.00, 1.00, 2.00, };
    c_int P_nnz = 4;
    c_int P_i[4] = {0, 1, 0, 1, };
    c_int P_p[4] = {0, 0, 1, 1};
#else
    c_float P_x[4] = {4.00, 1.00, 1.00, 2.00, };
    c_int P_nnz = 4;
    c_int P_i[4] = {0, 1, 0, 1, };
    c_int P_p[3] = {0, 2, 4, };
#endif
    c_float q[2] = {1.00, 1.00, };
    c_float A_x[4] = {1.00, 1.00, 1.00, 1.00, };
    c_int A_nnz = 4;
    c_int A_i[4] = {0, 1, 0, 2, };
    c_int A_p[3] = {0, 2, 4, };
    c_float l[3] = {1.00, 0.00, 0.00, };
    c_float u[3] = {1.00, 0.70, 0.70, };
    c_int n = 2;
    c_int m = 3;

    // Problem settings
    OSQPSettings * settings = (OSQPSettings *)c_malloc(sizeof(OSQPSettings));

    // Structures
    OSQPWorkspace * work;  // Workspace
    OSQPData * data;  // OSQPData

    // Populate data
    data = (OSQPData *)c_malloc(sizeof(OSQPData));
    data->n = n;
    data->m = m;

#if TRIPLET
    // the function below is from osqp_helper
    data->P = tripletArrayToCSC(data->n, data->n, P_nnz, P_x, P_i, P_p);
#else
    data->P = csc_matrix(data->n, data->n, P_nnz, P_x, P_i, P_p);
#endif

    data->q = q;
    data->A = csc_matrix(data->m, data->n, A_nnz, A_x, A_i, A_p);
    data->l = l;
    data->u = u;

    // function below is from osqp_helper
    printMatrix(data->P, "P Matrix in CSC");
    
    // functions below are from osqp_helper
    csc* triM = cscToTriplet(data->P);
    printMatrix(triM, "P Matrix in triplet");
    
    // the function below is from osqp_helper
    printProblem(data);


    // Define Solver settings as default
    osqp_set_default_settings(settings);
    settings->verbose = false;
    settings->alpha = 1.0; // Change alpha parameter

    // Setup workspace
    work = osqp_setup(data, settings);

    // Solve Problem
    osqp_solve(work);

    // Cleanup
    osqp_cleanup(work);
    c_free(data->A);
    c_free(data->P);
    c_free(data);
    c_free(settings);

    return 0;
};
