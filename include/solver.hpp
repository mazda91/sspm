/** \file solver.hpp
 * \brief Contains any function and element related to the resolutuin of the SSPMs.
 */

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "model.hpp"
#include "engine.h"
#include <time.h>

#define phi superbeeFluxLimiter
#define tau temporalRegularization

class solver{
private:
    model *usedModel;
    std::string usedMethod;
    unsigned int N = 10;//default value for the number of size range intervals
    unsigned int M = 10;//default value for the number of time range intervals
    double Tf = 10.0; //  default value for the solver ending time 
    double temporalRegularization = 1;
    std::vector<double> U;//vector of Ui's (average of the distribution we look for on each subdivsion interval)
    std::vector<double> x;//sizeMesh  
    std::vector<double> u;//approximation of the distribution ( our solution)
    
    /** \brief Partitiones the size range into N equidistant intervals */
    void setSizeMesh();

    /** \brief Sets the distribution at interval boundaries with random values between 0 and 1 */
    void setInitialDistribution();

    /** \brief Computes the regularizing factor needed in the monitor function*/
    double regularizingFactor() const; 

    /** \brief Computes the monitor function at size x[i] */
    double monitor(unsigned int i, double regularizingFactor);

    /** \brief Computes the middle size of the k-th interval*/
    inline double X(unsigned int k){return (x[k]+x[k+1])/2;}

    /** \brief Computes the solution of a linear system using Gauss-Thomas algorithm*/
    void  GaussThomasAlgo(std::vector<double> a, std::vector<double> b, std::vector<double> c, std::vector<double> d,std::vector<double> & solution);

    /** \brief Computes the distribution through time using mesh-upwind method.*/          
    void solve_MU(unsigned int move);    


public:
    
    solver();
    ~solver();
    
    /** \brief Sets the pointer to the model instance to deal with.  */
    void setModel(model *model);

    /** \brief  Sets the method to the string method.*/
    void setMethod(std::string method);

    /** \brief Sets N to nbSizeIntervals*/
    void setN(unsigned int nbSizeIntervals);

    /** \brief Sets M to nbTimeIntervals*/
    void setM(unsigned int nbTimeIntervals);

    /** \brief Sets Tf to finalTime */
    void setTf(float finalTime);

    /** \brief Sets temporalRegularization to tau */
    void setTemporalRegularization(double tau);

    /** \brief reinitialize u,U and x at each main loop*/
    void reInitialize();

    /** \brief Sets the solver parameters to their default value  */
    void initParameters();

    /** \brief Calls the solving function matching the attribute method*/
    void solve();

    //void displayEquilibrum();

    /** \brief  Displays the content of the vector given as a parameter*/
    void showContent(std::vector<double>);
};

#endif
