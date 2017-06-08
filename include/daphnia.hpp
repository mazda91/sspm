/** \file daphnia.hpp
 * \brief Contains Life history elements and Parameters of the size-structured Daphnia model
 */

#ifndef DAPHNIA_HPP
#define DAPHNIA_HPP

#include "model.hpp"

//---------------attributes macros --------------
#define lb lengthAtBirth
#define lm maximumLength
#define alpha scaledReproductionRate
#define mu0 naturalMortalityRate
#define r intrinsicGrowthRateForResource
#define K resourceCarryingCapacity
#define lm maximumLength
//-------------functions macros ------------------



class daphnia : public model{
private:
    double fractionOfIngestedEnergy;
    double scaledReproductionRate;
    double naturalMortalityRate;
    double intrinsicGrowthRateForResource;
    double resourceCarryingCapacity;


public:
    double lengthAtBirth;
    double maximumLength;

   daphnia();
   virtual ~daphnia();

   /** \brief set parameters to a default value*/
   virtual void defaultParameters() ;  

   /** \brief set the parameter attributeName to value*/
   virtual void setAttribute(std::string attributeName,double value);

   /** \brief displays all the attributes with their value */
   virtual void showAttributeList() const;

   /** \brief computes the growth rate of a size size-individual given the available resources S */
   virtual double individualGrowthRate(double size, double S) const;            

   /** \brief computes the mortality rate of a size size-individual given the available resources S */
   virtual double individualMortalityRate(double size, double S) const; 

   /** \brief computes the fecundity rate of a size size-individual given the available resources S */
   virtual double individualBirthRate(double size, double S) const ;

  /** \brief computes the variation of resources during dt given the available resources S and the size-population distribution */
  virtual double resourceDynamics(double S, std::vector<double> &x, std::vector<double> &u);
};


#endif
