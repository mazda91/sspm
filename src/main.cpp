#include "../include/model.hpp"
#include "../include/daphnia.hpp"
#include "../include/mmi.hpp"


int main() {
    std::string line;
    model *usedModel(0);
    while(true){
        std::cout << ">" ;
        getline(std::cin,line);
        evaluate_command(line, &usedModel);
        //usedModel->solve();
    }
        return 0;
}

