#include "include/model.hpp"
#include "include/daphnia.hpp"
#include "include/mmi.hpp"
#include "include/solver.hpp"
#include "mex.h"
#include <fstream>
#include <string>
#include "matrix.h"

#define MAXCHAR 100000
using namespace std;

void mexGate(){
    std::vector<model *> allModels;//saves all the models allocations done in evaluate_command in case the user decides to change the usedModel during the utilisation : we need to delete them all at the end and not only the last model allocated
    solver *solveModel = new solver();
    model *usedModel(0);

    Engine *ep = engOpen("");
    ifstream myfile("src/command1.m",ios::in);         
    char c;                                                                     
    char content[MAXCHAR];                                                      
    unsigned int i = 0;                                                         
    if (myfile){                                                                
        while (myfile.get(c)){                                                  
           content[i] = c;                                                     
           i += 1;                                                             
        }                                                                       
      myfile.close();                                                         
    }    
    //std::cout << content << std::endl;
    while (true){
         engEvalString(ep,content);
         mxArray *mxCommand = engGetVariable(ep,"command_line");
         char* command_line =  mxArrayToString(mxCommand);
         string str(command_line);
        // cout << command_line << std::endl;
         evaluate_command(command_line, &usedModel, &solveModel, &allModels,ep);
         solveModel->reInitialize();
        //solveModel->displayEquilibrum();

         mxDestroyArray(mxCommand);
    }
 
    //}
    for (unsigned int i=0;i<allModels.size();i++){
        delete allModels[allModels.size()-1-i];
    }
    engClose(ep);
    delete solveModel;
}



/* The gateway function. */ 
void mexFunction(int nlhs, mxArray* plhs[],int nrhs, const mxArray* prhs[]) {

//    /* Check for proper number of arguments */
//    if(nrhs != 0) {
//        mexErrMsgIdAndTxt("MATLAB:mexcpp:nargin","MEXCPP requires no input arguments.");
//    }
//    if(nlhs != 0){
//        mexErrMsgIdAndTxt("MATLAB:mexcpp:nargin","MEXCPP requires no output argument.");
//    }
//
//    /* Check if the input is of proper type */
//    if(!mxIsChar(prhs[0])){
//           mexErrMsgIdAndTxt("MATLAB:mexcpp:typeargin","Argument has to be a char array.");
//    }
//
     /* Acquire pointers to the input data */
     mexGate();
}

