#include "include/model.hpp"
#include "include/mmi.hpp"
#include "include/solver.hpp"
#include "mex.h"
#include <fstream>
#include <string>
#include "matrix.h"

using namespace std;

solver* mexGate(){
    Engine *ep = engOpen("");
    engEvalString(ep,"nbLoops = 0;");
    solver *solveModel = new solver(ep);
    model *usedModel = new model();
    solveModel->setModel(usedModel);

    bool quit = false;
    while (quit != true){
         //call the m.file command1.m
         engEvalString(ep,"command1;");

         //get the user-requested command and evaluate it
         mxArray *mxCommand = engGetVariable(ep,"command_line");
         char* command_line =  mxArrayToString(mxCommand);
         string str(command_line);
         evaluate_command(command_line, &usedModel, &solveModel, quit);

         //destroy the created mxArray
         mxDestroyArray(mxCommand);
    }
    return solveModel;
}



/* The gateway function. */ 
void mexFunction(int nlhs, mxArray* plhs[],int nrhs, const mxArray* prhs[]) {

//    /* Check for proper number of arguments */
    if(nrhs != 0) {
        mexErrMsgIdAndTxt("MATLAB:mexcpp:nargin","MEXCPP requires no input arguments.");
    }
    if(nlhs != 4){
        mexErrMsgIdAndTxt("MATLAB:mexcpp:nargin","MEXCPP requires three output arguments : size mesh / density distribution / environment / cohorts(for EBT method)");
    }

    //call the package and returns the solver instance
    solver *solveModel = mexGate();

    //create mxArrays matrices to pass as left-hand side (plhs)
    mxArray *xmx = mxCreateDoubleMatrix(solveModel->J + 1,1,mxREAL);
    mxArray *umx = mxCreateDoubleMatrix(solveModel->J + 1,1,mxREAL);
   mxArray *Nmx = mxCreateDoubleMatrix(solveModel->J + 1,1,mxREAL);
    mxArray *Emx = mxCreateDoubleMatrix(solveModel->E.size(),1,mxREAL);

    //copy the final values of x, u and E (mesh, distribution density and environmental variable) into those mxArrays
    for (unsigned int i=0; i<=solveModel->J; i++){
        mxGetPr(xmx)[i] = solveModel->x[i];
        mxGetPr(umx)[i] = solveModel->u[i];
        mxGetPr(Nmx)[i] = solveModel->N[i];
   }
    for (unsigned int i=0; i<1;i++){
        mxGetPr(Emx)[i] = solveModel->E[i];
    }

    plhs[0] = xmx; plhs[1] = umx; plhs[2] = Emx; plhs[3] = Nmx;

    delete solveModel;

}

