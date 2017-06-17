/** \file model.hpp
 * \brief contains Life history elements and parameters common to all size-structured population models
 */
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
//#define x size
#define t time
#define S0 initResourceAvailable
#define dS resourceDynamics
#define dR resourceDynamics

#define lb lengthAtBirth

struct parameter{
    std::string name;
    double value;
    std::string unit;
};

/** \brief Parent model class. 
 *
 */
class model{
protected:
     std::vector<parameter> setParameters;

public:
     double lengthAtBirth;
     double maximumLength; 
     double initResourceAvailable;

     model();
     model(double lengthAtBirth, double maximumLength, double initResourceAvailable);
     virtual ~model();

     /** \brief set parameters to a default value*/
    virtual void defaultParameters() =0;

    /** \brief computes the variation of resources during dt given the available resources S and the size-population distribution */
    virtual double resourceDynamics(double resource, std::vector<double> &mesh, std::vector<double> &distribution) = 0;

    /** \brief computes the growth rate of a size size-individual given the available resources S */
    virtual double individualGrowthRate(double size, double S) const = 0;           

    /** \brief computes the mortality rate of a size size-individual given the available resources S */
    virtual double individualMortalityRate(double size, double S) const = 0;

    /** \brief computes the fecundity rate of a size size-individual given the available resources S */
    virtual double individualBirthRate(double size, double S) const  = 0;

    /** \brief set the parameter attributeName to value*/
    virtual void setAttribute(std::string attirbuteName, double value)=0;

    /** \brief displays all the attributes with their value */
    virtual void showAttributeList() const=0;

    void setParameter(std::string parameterName, double value);
    parameter & getParameter(std::string parameterName);
    
    std::string getUnitParameter(std::string parameterName);

    void addParameter(std::string parameterName, double value, std::string unit);
    void addParameter(parameter Parameter);
    void showParameter(std::string parameterName);
};


#endif

