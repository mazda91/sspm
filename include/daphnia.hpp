#ifndef DAPHNIA_HPP
#define DAPHNIA_HPP

#include "model.hpp"

//---------------attributes macros --------------
#define lb lengthAtBirth
#define lj lengthAtMaturation
#define lm maximumLength
#define kappa fractionOfIngestedEnergy
#define nux maximumFeedingRatePerUnitSurfaceArea
#define gamma timeConstantOfGrowth
#define xi shapeParameterOfTheFunctionalResponse
#define rm maximumReproductionRatePerUnitSurfaceArea
#define alpha scaledReproductionRate
#define mu0 naturalMortalityRate
#define r intrinsicGrowthRateForResource
#define K resourceCarryingCapacity
#define lm maximumLength
//-------------functions macros ------------------
#define S resourceDynamics




class daphnia : public model{
public:
   daphnia();
   virtual ~daphnia();
   virtual void defaultParameters() ;  
   virtual void setAttribute(std::string attributeName,double value);
   virtual void showAttributeList() const;

   virtual double individualGrowthRate(double size, double time) const;            
   virtual double individualMortalityRate(double size, double time) const; 
   virtual double individualBirthRate(double size, double time) const ;
    double resourceDynamics(double time) const;
 
   //the idea is the following : for calculation functions and a better readability(avoiding this like lengthAtBirth.value or get("lengthAtBirth").value for each parameter, enumeration of attributes corresponding to parameters, but just a name and a value 
   //then, if we want details for the parameter : name, unit... vector of parameters
    double lengthAtBirth = 0.8;
    double lengthAtMaturation = 2.5;
    double maximumLength = 6;
    double fractionOfIngestedEnergy = 0.3;
    double maximumFeedingRatePerUnitSurfaceArea = 0.0000018;
    double timeConstantOfGrowth = 0.15;
    double shapeParameterOfTheFunctionalResponse = 0.000007;
    double maximumReproductionRatePerUnitSurfaceArea = 0.1;
    double scaledReproductionRate = 0.75;
    double naturalMortalityRate = 0.1;
    double intrinsicGrowthRateForResource = 0.5;
    double resourceCarryingCapacity = 3;
};


#endif
