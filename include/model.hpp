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
         void defaultParameters();
         std::string method;
    protected:
        virtual void individualGrowthRate() const = 0; 
        virtual void individualMortalityRate() const = 0;
        virtual void individualBirthRate() const = 0;
        virtual void resourceDynamics() const = 0;
    public:
         model();
         model(std::string method);
         ~model();

        void setMethod(std::string method);
        std::string getMethod();

        void setParameter(std::string parameterName, double value);
        parameter & getParameter(std::string parameterName);
        
        std::string getUnitParameter(std::string parameterName);

        void addParameter(std::string parameterName, double value, std::string unit);
        void addParameter(parameter Parameter);


};


#endif

