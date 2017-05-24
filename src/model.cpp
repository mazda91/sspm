#include "../include/model.hpp"



model::model(){;}

model::model(std::string method){this->method = method;}

void model::setMethod(std::string method){ this->method = method;}

model::~model(){}

std::string model::getMethod(){ return method;}

//in the vector of parameters
void model::setParameter(std::string parameterName, double value){
    getParameter(parameterName).value = value;
}


void model::setAttribute(std::string attributeName, double value){}

/*
 *@todo : Throwing exception here in case the parameter is not in the set
 */
parameter& model::getParameter(std::string parameterName){
    unsigned int i=0;
    while (i<setParameters.size()){
        if (setParameters[i].name == parameterName){
            return setParameters[i];
        }
        i++;
    }
    std::cout << "parameter : " << parameterName <<" not in the set " << std::endl;
    exit(1);
}

std::string model::getUnitParameter(std::string parameterName){
    return getParameter(parameterName).unit;
}

void model::addParameter(std::string parameterName, double value, std::string unit){
    parameter newParameter;
    newParameter.name = parameterName; newParameter.value = value; newParameter.unit = unit;
    setParameters.push_back(newParameter);
}

void model::addParameter(parameter Parameter){
    setParameters.push_back(Parameter);
    
}

void model::showParameter(std::string parameterName){
    std::cout << "name";
    for (unsigned int j=0; j < parameterName.size() - 4 ; j++){
        std::cout << " ";
    }            
    std::cout << "value";
    for (unsigned int j=0; j < std::to_string((this->getParameter(parameterName)).value).size() - 5 ; j++){
        std::cout << " ";
    }            
    std::cout << "unit " << std::endl;
    std::cout << parameterName << " " << this->getParameter(parameterName).value << " " << this->getParameter(parameterName).unit << std::endl;   


} 


