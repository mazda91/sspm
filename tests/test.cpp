#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <map>
#include <vector>
#include <typeinfo>
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
        //---------------------TEST 1 : 
        std::cout << "model is : " << (typeid(daphnia) == typeid(usedModel)) << " method is : " << usedModel->getMethod() << std::endl;//savoir si le type dynamique du pointeur est bien daphnia et pas model(marche pas)
        //std::cout << typeid(model).name() << std::endl;
        //std::cout << typeid(dynamic_cast<model *>(usedModel)).name() << std::endl;
        std::cout << "lengthAtBirth parameter value is : " << usedModel->getParameter("lengthAtBirth").value << std::endl;
        usedModel->showAttributeList();
        //usedModel->solve();
    }
    delete usedModel;
        return 0;
}

