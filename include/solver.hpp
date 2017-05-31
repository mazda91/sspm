#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "model.hpp"
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
    std::vector<double> x;//sizeMesh  
    std::vector<double> U;//vector of Ui's (average of the distribution we look for on each subdivsion interval)
public:

    std::vector<double> u;//approximation of the distribution ( our solution)

    solver();
    ~solver();

    void setModel(model *model);
    void setMethod(std::string method);
    void setN(unsigned int nbSizeIntervals);
    void setM(unsigned int nbTimeIntervals);
    void setTf(float finalTime);
    void setTemporalRegularization(double tau);
    void setSizeMesh();
    void setInitialDistribution();
    void solve();
    void solve_MU(unsigned int move);    
    void setMeshBoundary(unsigned int i, double value);
    double getMeshBoundary(unsigned int i);
    double regularizingFactor() const; //need u(t) and u(t+dt) to compute the derivative
    double monitor(unsigned int i, double regularizingFactor);
    inline double X(unsigned int k){return (x[k]+x[k+1])/2;}
    void  GaussThomasAlgo(std::vector<double> a, std::vector<double> b, std::vector<double> c, std::vector<double> d,std::vector<double> & solution);
    void display();

};

#endif
