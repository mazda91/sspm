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
    Engine *ep = engOpen("");
    //by default, the path is in /bin
    engEvalString(ep,"cd ../src");

    std::string line;
    model *usedModel = new model(0,1,10);
    solver *solveModel = new solver(ep);
    solveModel->setModel(usedModel);

    while(true){
         std::cout << ">" ;
         getline(std::cin,line);
         evaluate_command(line, &usedModel, &solveModel);
         solveModel->reInitialize();
         //solveModel->displayEquilibrum();
     }
    engClose(ep);
    delete solveModel;
    return 0;
}



