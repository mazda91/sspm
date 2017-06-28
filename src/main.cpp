#include "../include/model.hpp"
#include "../include/daphnia.hpp"
#include "../include/mmi.hpp"
#include "../include/solver.hpp"
#include "mex.h"
#include <fstream>
#include <string>
#include "matrix.h"

#define MAXCHAR 100000
using namespace std;
int main() {
    std::string line;
    model *usedModel(0);
    std::vector<model *> allModels;//saves all the models allocations done in evaluate_command in case the user decides to change the usedModel during the utilisation : we need to delete them all at the end and not only the last model allocated
    solver *solveModel = new solver();
    Engine *ep = engOpen("");
    //by default, the path is in /bin
    engEvalString(ep,"cd ../src");

    while(true){
         std::cout << ">" ;
         getline(std::cin,line);
         evaluate_command(line, &usedModel, &solveModel, &allModels,ep);
         solveModel->reInitialize();
         //solveModel->displayEquilibrum();
     }
     for (unsigned int i=0;i<allModels.size();i++){
         delete allModels[allModels.size()-1-i];
     }
    engClose(ep);
    delete solveModel;
    return 0;
}



