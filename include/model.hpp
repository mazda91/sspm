#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdlib.h>
#include <vector>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>

#define g individualGrowthRate
#define beta individualBirthRate
#define mu individualMortalityRate
#define x size
#define t time
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
     model();
     model(double lengthAtBirth, double maximumLength);
     virtual ~model();

    virtual void defaultParameters() =0;

    virtual double individualGrowthRate(double size, double time) const = 0;           
    virtual double individualMortalityRate(double size, double time) const = 0;
    virtual double individualBirthRate(double size, double time) const  = 0;
//    virtual void resourceDynamics()  = 0;           
//    virtual void solve()  = 0;
    virtual void setAttribute(std::string attirbuteName, double value);
    virtual void showAttributeList() const=0;
    void setParameter(std::string parameterName, double value);
    parameter & getParameter(std::string parameterName);
    
    std::string getUnitParameter(std::string parameterName);

    void addParameter(std::string parameterName, double value, std::string unit);
    void addParameter(parameter Parameter);
    void showParameter(std::string parameterName);
};


#endif

