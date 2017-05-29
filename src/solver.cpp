#include "../include/solver.hpp"





solver::solver(){}

solver::~solver(){}

void solver::setModel(model *model){this->usedModel = model;}

void solver::setMethod(std::string method){this->usedMethod = method;}

void solver::setN(unsigned int nbSizeIntervals){this->N = nbSizeIntervals;}
void solver::setM(unsigned int nbTimeIntervals){this->M = nbTimeIntervals;}

void solver::setTf(float finalTime){this->Tf = finalTime;}
void solver::setTemporalRegularization(double tau){this->tau = tau;}

void solver::setSizeMesh(){
   double stepSize = (this->usedModel->maximumLength - this->usedModel->lengthAtBirth)/this->N;
   for (unsigned int i=0; i<= this->N ;i++){
       x.push_back(i*stepSize); 
   }

}

void solver::setInitialDistribution(){ //we assume that, initially, the distribution is equal between classes
    U = std::vector<double>(N,1/N);
    u = std::vector<double>(N+1,1/(N+1));
}

void solver::setMeshBoundary(unsigned int i, double value){
    assert(i < x.size());
    x[i] = value;
}
double solver::getMeshBoundary(unsigned int i){
    assert(i < x.size());
    return x[i];
}

double phi(double r){
    return fmax(fmax(0,fmin(2*r,1)),fmin(r,2));
}


void solver::solve(){
    if (this->usedMethod == "FMU"){
        solve_MU(0);
    }else if (this->usedMethod == "MMU"){
        solve_MU(1);
    }
}

void solver::solve_MU(unsigned int move){
    float step = ((float) Tf)/M; 
    double dUi,sumBirth, rMinus, rPlus,rhoMinus, rhoPlus;
    double S = usedModel->S0;double factor;
    std::vector<double> a,b,c,d,dx;
    for(unsigned int j =0; j<M; j++){//time
        sumBirth = 0;
        for(unsigned int i=0; i<N; i++){//size
            dUi=0;
            if(i==0){ //taking into account equation (1b) : newborn individuals
                for(unsigned int k=0;k<N; k++){
                    sumBirth += (x[k+1] - x[k]) * usedModel->beta(X(k),S)*U[k];
                }
                dUi = (1/usedModel->g(usedModel->lengthAtBirth,j*step))*sumBirth;
            }
           
            dUi += -usedModel->mu(X(i),S)*U[i] - (1/(x[i+1]-x[i]))*(usedModel->g(x[i+1],S)*u[i+1] -  usedModel->g(x[i],S)*u[i] );
            if(move == 1){
                dUi += dx[i+1]*u[i+1] - dx[i]*u[i] - (dx[i+1] - dx[i])*U[i]; // =0 if FMU
            }
            U[i] = U[i] + step*dUi;//for all i, we have the average distribution on each interval
        }
        //now, we calculate the density at each boundary
        u[0] = sumBirth; //cf equation (1b)
        for (unsigned int i=1; i<=N;i++){//still size
            rMinus = (U[i]-U[i-1])*(x[i]-x[i-2])/((U[i-1]-U[i-2])*(x[i-1]-x[i-2]));
            rPlus = (U[i-1]-U[i])*(x[i]-x[i+2])/((U[i]-U[i+1])*(x[i-1]-x[i]));
            if(usedModel->g(x[i],j*step) >=0){
               u[i] = U[i-1] + phi(rMinus)*(U[i-1]-U[i-2])*(x[i]-x[i-1])/(x[i+1]-x[i-1]); 
            }   
            else{
                u[i] = U[i] - phi(rPlus)*(U[i+1]-U[i])*(x[i+1]-x[i])/(x[i+2]-x[i]); 
            }
        }
    //must compute the new resourceDynamics S
    S = S + step*usedModel->dS(S,x,u);
    if(move==1){
        //now compute the variations of xi's
        //build the vectors of coefficients composing the matrix A s.t A.dx = d
         a.push_back(0);b.push_back(1);c.push_back(0);d.push_back(0);
         factor = regularizingFactor();
         for (unsigned int i =1 ; i<= N-1 ; i++){
            rhoMinus = (monitor(i-1,factor)+monitor(i,factor))/2;
            rhoPlus = (monitor(i+1,factor)+monitor(i,factor))/2;
            a.push_back(rhoMinus);
            b.push_back(-(rhoPlus + rhoMinus));
            c.push_back(rhoPlus);
            d.push_back(-(1/tau)*(rhoPlus*(x[i+1]-x[i]) - rhoMinus*(x[i]-x[i-1])));
         }
         a.push_back(0);b.push_back(1);c.push_back(0);d.push_back(0);
         //apply the Gauss-Thomas algo on this system to find the set of dxi
         GaussThomasAlgo(a,b,c,d,dx);
         //Update the xi
         for (unsigned int i =0; i<=N; i++){
             x[i] = x[i] + step*dx[i];
         }
    }
  }
}

double solver::monitor(unsigned int i, double regularizingFactor){
    double dx, dxU;
    dx = x[i+1] - x[i];
    dxU = (u[i+1] - u[i])/dx;
    return sqrt(1 + (1/regularizingFactor)*pow(dxU,2));
}
double solver::regularizingFactor() const{
    double integral = 0;
    double dx, dxU;
    for(unsigned int i =0; i<(u.size() - 1);i++){
        dx = x[i+1] - x[i];
        dxU = (u[i+1] - u[i])/dx;
        integral += pow(dxU,2);
    }
    return (1/(usedModel->maximumLength - usedModel->lengthAtBirth))*integral;
}

void solver::GaussThomasAlgo(std::vector<double> a, std::vector<double> b, std::vector<double> c, std::vector<double> d, std::vector<double> & solution){
    std::vector<double> cprime, dprime;
    solution.resize(a.size());
    assert(a.size() == b.size()); assert(b.size()==c.size()); assert(c.size() == d.size()); 
    for (unsigned int i = 0; i < (a.size() - 1);i++){
        if (i == 0){
            cprime.push_back(c[i]/b[i]);
            dprime.push_back(d[i]/b[i]);
        }
        else{
            cprime.push_back(c[i]/(b[i]-a[i]*cprime[i-1]));
            dprime.push_back((d[i]-a[i]*dprime[i-1])/(b[i]-a[i]*cprime[i-1]));
        }
    }
    dprime.push_back((d[a.size()-1] - a[a.size()-1]*dprime[a.size()-2])/(b[a.size()-1]-a[a.size()-1]*cprime[a.size()-2]));

    //inverse substitution
    solution[a.size()-1] = dprime[a.size()-1];
    for (unsigned int i =0; i<= (a.size() - 2); i++){
        solution[a.size()-2 - i] = dprime[a.size() - 2 - i] - cprime[a.size() - 2 -i]*solution[a.size()-1 - i];
    }
}
