#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iostream>

struct parameter{
    std::string name;
    double value;
    std::string unit;
};

class model{
private:
     std::vector<parameter> setParameters;
     std::string method;
    
    
    
    
    
public:
     model();
     model(std::string method);
     virtual ~model();

    virtual void defaultParameters() =0;
//    virtual void individualGrowthRate() const = 0;           
//    virtual void individualMortalityRate() const = 0;
//    virtual void individualBirthRate() const = 0;
//    virtual void resourceDynamics() const = 0;           
//    virtual void solve() const = 0;
    virtual void setAttribute(std::string attirbuteName, double value);
    virtual void showAttributeList() const=0;
    void setParameter(std::string parameterName, double value);
    void setMethod(std::string method);
    std::string getMethod();
    parameter & getParameter(std::string parameterName);
    
    std::string getUnitParameter(std::string parameterName);

    void addParameter(std::string parameterName, double value, std::string unit);
    void addParameter(parameter Parameter);
    void showParameter(std::string parameterName);
};


#endif

