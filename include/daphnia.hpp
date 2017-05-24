#ifndef DAPHNIA_HPP
#define DAPHNIA_HPP

#include "model.hpp"

class daphnia : public model{
public:
   daphnia();
   daphnia(std::string method);
   virtual ~daphnia();
   virtual void defaultParameters() ;  
   virtual void setAttribute(std::string attributeName,double value);
   virtual void showAttributeList() const;
    //void individualGrowthRate();            
    //void individualMortalityRate(); 
    //void individualBirthRate() ;
    //void resourceDynamics();            
    //void solve(); 
 
   //the idea is the following : for calculation functions and a better readability(avoiding this like lengthAtBirth.value or get("lengthAtBirth").value for each parameter, enumeration of attributes corresponding to parameters, but just a name and a value 
   //then, if we want details for the parameter : name, unit... vector of parameters
    double lengthAtBirth = 0.8;
    double lengthAtMaturation = 2.5;
    double maximumLength = 6;
};


#endif
