#include "../include/model.hpp"
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
    model *usedModel = new model();
    solver *solveModel = new solver();
    solveModel->setModel(usedModel);
    Engine *ep = engOpen("");
    //by default, the path is in /bin
    engEvalString(ep,"cd ../src");

    while(true){
         std::cout << ">" ;
         getline(std::cin,line);
         evaluate_command(line, &usedModel, &solveModel,ep);
         solveModel->reInitialize();
         //solveModel->displayEquilibrum();
     }
    engClose(ep);
    delete solveModel;
    return 0;
}



