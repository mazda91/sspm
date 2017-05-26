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

daphnia::daphnia(std::string method) : model(method,0.8,6){
    defaultParameters();
}

daphnia::~daphnia(){}

void daphnia::defaultParameters() {
    addParameter("lengthAtBirth", 0.8, "mm");
    addParameter("lengthAtMaturation ",2.5   ,"mm ");
    addParameter("maximumLength ", 6  ,"mm ");
    addParameter("fractionOfIngestedEnergy ",0.3 ," ");
    addParameter("maximumFeedingRatePerUnitSurfaceArea ",0.0000018,"cell.mm^-2.day^-1");
    addParameter("timeConstantOfGrowth",0.15 ,"day^-1 ");
    addParameter("shapeParameterOfTheFunctionalResponse ",0.000007 ,"ml.cell^-1 ");
    addParameter("maximumReproductionRatePerUnitSurfaceArea ",0.1,"mm^-2.day^-1");
    addParameter("scaledReproductionRate",0.75," ");
    addParameter("naturalMortalityRate",0.1,"day^-1");
    addParameter("intrinsicGrowthRateForResource",0.5,"day^-1");
    addParameter("resourceCarryingCapacity",3,"cell.m^-1 ");

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


double daphnia::individualGrowthRate(double size, double time) const{
    return fmax(S(t)/(1+S(t)) - size,0.0);
}


double daphnia::individualBirthRate(double size, double time) const{
    return alpha*pow(size,2)*S(t)/(1+S(t)); 
}

double daphnia::individualMortalityRate(double size, double time) const{
    return mu0;
}
double daphnia::resourceDynamics(double time) const {return 0;}


