#include "../include/daphnia.hpp"
/*
 *In the solving method, it will be really costly to access each parameter through a vector each time...: time to search the parameter name, access its value
 On the other side, can't write 30 setters and getters for each parameter... so we use both implementations : vector for adding/changing parameters, and class attributes to access them directly
 *
 *
 *
 */

daphnia::daphnia() : model(){
    defaultParameters();
}

daphnia::daphnia(std::string method) : model(method){
    defaultParameters();
}

daphnia::~daphnia(){}

void daphnia::defaultParameters() {
    addParameter("lengthAtBirth", 0.8, "mm");
    addParameter("length at maturation ",2.5   ,"mm ");
    addParameter("maximum length ", 6  ,"mm ");
    addParameter("fraction of ingested energy ",0.3 ," ");
    addParameter("maximum feeding rate per unit surface area ",0.0000018,"cell.mm^-2.day^-1");
    addParameter("time constant of growth",0.15 ,"day^-1 ");
    addParameter("shape parameter of the functional response ",0.000007 ,"ml.cell^-1 ");
    addParameter("maximum reproduction rate per unit surface area ",0.1,"mm^-2.day^-1");
    addParameter("scaled reproduction rate",0.75," ");
    addParameter("natural mortality rate",0.1,"day^-1");
    addParameter("intrinsic growth rate for resource",0.5,"day^-1");
    addParameter("resource carrying capacity",3,"cell.m^-1 ");

}

void daphnia::setAttribute(std::string attributeName, double value){
    if(attributeName == "lengthAtBirth"){
        lengthAtBirth = value;
    }else if (attributeName == "lengthAtMaturation"){
        lengthAtMaturation = value;
    }else if (attributeName == "maximumLength"){
        maximumLength = value;
    }
}

void daphnia::showAttributeList() const{
    std::cout << "length at birth : " << this->lengthAtBirth << std::endl;
    std::cout << "length at maturation : " << this ->lengthAtMaturation << std::endl;
    std::cout << "maximum length : " << this->maximumLength << std::endl;
}
