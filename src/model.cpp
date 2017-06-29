#include "../include/model.hpp"



model::model(){;}

model::model(double lengthAtBirth, double maximumLength, double initResourceAvailable){
    this->lengthAtBirth = lengthAtBirth;
    this->maximumLength = maximumLength;
    this->initResourceAvailable = initResourceAvailable;
}


model::~model(){}


void model::defaultParameters(){
    setParameters.clear();
    addParameter("lengthAtBirth", 0, "mm");
    addParameter("maximumLength ", 1  ,"mm ");
    addParameter("initResourceAvailable",10,"cell.m^-1 ");
}
void model::setAttribute(std::string attributeName, double value){
   if(attributeName == "lengthAtBirth"){
       lengthAtBirth = value;
   }else if (attributeName == "maximumLength"){
       maximumLength = value;
   }
}

void model::showAttributeList() const{
    std::cout << "length at birth : " << this->lengthAtBirth << std::endl;
    std::cout << "maximum length : " << this->maximumLength << std::endl;
}

//in the vector of parameters
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


