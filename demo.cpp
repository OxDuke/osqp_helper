#include "osqp/osqp.h"
#include "osqp_helper.h"
#include <iostream>
#include <sstream>

using namespace std;

#define TRIPLET 1



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
    //csc* temp = csc_matrix(data->n, data->n, P_nnz, P_x, P_i, P_p);
    //temp->nz = 4;
    //data->P = triplet_to_csc(temp, OSQP_NULL);
    data->P = tripletArrayToCSC(data->n, data->n, P_nnz, P_x, P_i, P_p);
#else
    data->P = csc_matrix(data->n, data->n, P_nnz, P_x, P_i, P_p);
#endif

    data->q = q;
    data->A = csc_matrix(data->m, data->n, A_nnz, A_x, A_i, A_p);
    data->l = l;
    data->u = u;

    // printing some stuff
    // printMatrix(data->P);
    // csc* tt = cscToTriplet(data->P);
    // printMatrix(tt);

    // char name[4] = {'f', 'k'};
    // print_csc_matrix(data->P, name);
    // print_trip_matrix(tt, name);

    // c_float* denseM = csc_to_dns(data->P);
    // print_dns_matrix(denseM, 2, 2, name);

    printProblem(data);


    // Define Solver settings as default
    osqp_set_default_settings(settings);
    settings->verbose = false;
    settings->alpha = 1.0; // Change alpha parameter
    // std::cout << "size: " << sizeof(c_float) << ", " << sizeof(float) << std::endl;

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
