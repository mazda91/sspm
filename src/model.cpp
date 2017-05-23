#include "../include/model.hpp"



model::model(){;}

model::model(std::string method){this->method = method;}

void model::setMethod(std::string method){ this->method = method;}

model::~model(){}

std::string model::getMethod(){ return method;}

void model::setParameter(std::string parameterName, double value){
    getParameter(parameterName).value = value;
}

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





