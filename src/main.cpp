#include "../include/model.hpp"
#include "../include/daphnia.hpp"
#include "../include/mmi.hpp"
#include "../include/solver.hpp"

int main() {
    std::string line;
    model *usedModel(0);
    solver *solveModel = new solver();
    while(true){
        std::cout << ">" ;
        getline(std::cin,line);
        evaluate_command(line, &usedModel, &solveModel);
        solveModel->solve();
    }
        return 0;
}

