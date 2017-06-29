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


//#define x size
#define t time
#define S0 initResourceAvailable

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
     ~model();

     /** \brief set parameters to a default value*/
     void defaultParameters();

    /** \brief set the parameter attributeName to value*/
     void setAttribute(std::string attirbuteName, double value);

    /** \brief displays all the attributes with their value */
     void showAttributeList() const;

    void setParameter(std::string parameterName, double value);
    parameter & getParameter(std::string parameterName);
    
    std::string getUnitParameter(std::string parameterName);

    void addParameter(std::string parameterName, double value, std::string unit);
    void addParameter(parameter Parameter);
    void showParameter(std::string parameterName);
};


#endif

