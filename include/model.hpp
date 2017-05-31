#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdlib.h>
#include <vector>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>

#include "../../../software/CImg-2.0.0/CImg.h"

#define g individualGrowthRate
#define beta individualBirthRate
#define mu individualMortalityRate
//#define x size
#define t time
#define S0 initResourceAvailable
#define dS resourceDynamics
#define dR resourceDynamics

using namespace cimg_library;
struct parameter{
    std::string name;
    double value;
    std::string unit;
};

class model{
private:
     std::vector<parameter> setParameters;
    
    
public:
     double lengthAtBirth;
     double maximumLength; 
     double initResourceAvailable;

     model();
     model(double lengthAtBirth, double maximumLength, double initResourceAvailable);
     virtual ~model();

    virtual void defaultParameters() =0;

    virtual double resourceDynamics(double resource, std::vector<double> &mesh, std::vector<double> &distribution) = 0;
    virtual double individualGrowthRate(double size, double S) const = 0;           
    virtual double individualMortalityRate(double size, double S) const = 0;
    virtual double individualBirthRate(double size, double S) const  = 0;
    virtual void setAttribute(std::string attirbuteName, double value)=0;
    virtual void showAttributeList() const=0;
    void setParameter(std::string parameterName, double value);
    parameter & getParameter(std::string parameterName);
    
    std::string getUnitParameter(std::string parameterName);

    void addParameter(std::string parameterName, double value, std::string unit);
    void addParameter(parameter Parameter);
    void showParameter(std::string parameterName);
};


#endif

