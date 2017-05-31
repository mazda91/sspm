#include "../include/daphnia.hpp"
/*
 *In the solving method, it will be really costly to access each parameter through a vector each time...: time to search the parameter name, access its value
 On the other side, can't write 30 setters and getters for each parameter... so we use both implementations : vector for adding/changing parameters, and class attributes to access them directly
 *
 *
 *
 */

daphnia::daphnia() : model(0,6/6,10){
    defaultParameters();
}

daphnia::~daphnia(){}

void daphnia::defaultParameters() {
    addParameter("lengthAtBirth", 0, "mm");
    addParameter("lengthAtMaturation ",2.5/6   ,"mm ");
    addParameter("maximumLength ", 6/6  ,"mm ");
    addParameter("fractionOfIngestedEnergy ",0.3 ," ");
    addParameter("maximumFeedingRatePerUnitSurfaceArea ",0.0000018,"cell.mm^-2.day^-1");
    addParameter("timeConstantOfGrowth",0.15 ,"day^-1 ");
    addParameter("shapeParameterOfTheFunctionalResponse ",0.000007 ,"ml.cell^-1 ");
    addParameter("maximumReproductionRatePerUnitSurfaceArea ",0.1,"mm^-2.day^-1");
    addParameter("scaledReproductionRate",0.75," ");
    addParameter("naturalMortalityRate",0.1,"day^-1");
    addParameter("intrinsicGrowthRateForResource",0.5,"day^-1");
    addParameter("resourceCarryingCapacity",3,"cell.m^-1 ");
    addParameter("initResourceAvailable",10,"cell.m^-1 ");
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


double daphnia::individualGrowthRate(double size, double S) const{
    return fmax(S/(1+S) - size,0.00001);
}


double daphnia::individualBirthRate(double size,double S) const{
    return alpha*pow(size,2)*S/(1+S); 
}

double daphnia::individualMortalityRate(double size, double S) const{
    return mu0;
}
double daphnia::dS(double S, std::vector<double> &x, std::vector<double> &u) {
    double integral = 0;
   for (unsigned int i=0;i<(x.size()-1);i++){
        integral += (x[i+1] - x[i])*(pow(x[i+1],2)*u[i+1]+ pow(x[i],2)*u[i])/2.0;
   } 
   //std::cout << integral << std::endl;
   return r*S*(1-S/K)-(S/(1+S))*integral;

}


