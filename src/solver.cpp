#include "../include/solver.hpp"




/*
 *@todo : resize the vectors x,u and U if N changes
 *
 */
solver::solver(){
    this->x = std::vector<double>();
    setInitialDistribution();
}

solver::~solver(){}

void solver::setModel(model *model){
    this->usedModel = model;
    setSizeMesh();//to set the mesh, we need to know on which model we are working as we need maximumLength and lengthAtBirth, that's why setSizeMesh is settled IN the setModel function
}

void solver::setMethod(std::string method){this->usedMethod = method;}

/*
 *resize u,U and x
 */
void solver::setN(unsigned int nbSizeIntervals){
    this->N = nbSizeIntervals;
    u.resize(this->N + 1,(double (1))/(this->N + 1));
    U.resize(this->N,(double (1))/this->N);
    setSizeMesh();
}

void solver::setM(unsigned int nbTimeIntervals){this->M = nbTimeIntervals;}

void solver::setTf(float finalTime){this->Tf = finalTime;}
void solver::setTemporalRegularization(double tau){this->tau = tau;}

void solver::setSizeMesh(){
    x.clear(); //x of size 0
   double stepSize = (this->usedModel->maximumLength - this->usedModel->lengthAtBirth)/this->N;
   for (unsigned int i=0; i<= this->N ;i++){
       x.push_back(i*stepSize); 
   }

}

void solver::setInitialDistribution(){ //we assume that, initially, the distribution is equal between classes
    U = std::vector<double>(N,(double (1))/N);
    u = std::vector<double>(N+1,(double (1))/(N+1));
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

/*
 *@todo changing the values for u[1], u[N-1] and u[N] after finding a solution to the boundary value pbl
 *
 */
void solver::solve_MU(unsigned int move){
    double step = ((double) Tf)/M; 
    double dUi,sumBirth, rMinus, rPlus,rhoMinus, rhoPlus;
    double S = usedModel->S0;double factor;
    double tmp1;
    //displayEquilibrum(); //set u to the equilibrum state
    //showContent(u);
    std::vector<double> a,b,c,d,dx;
    for(unsigned int j =0; j<M; j++){//time
        //FIRST WE compute the variations of xi's
        if(move==1){
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
        sumBirth = 0;
        for(unsigned int i=0; i<N; i++){//size
            dUi=0;
            if(i==0){ //taking into account equation (1b) : newborn individuals
                for(unsigned int k=0;k<N; k++){
                    sumBirth +=(x[k+1] - x[k]) * usedModel->beta(X(k),S)*U[k];
                }
                tmp1 = usedModel->g(usedModel->lengthAtBirth,S);
                U[0] += (1/tmp1)*sumBirth;
            }
            else{
                dUi += -usedModel->mu(X(i),S)*U[i] - (1/(x[i+1]-x[i]))*(usedModel->g(x[i+1],S)*u[i+1] -  usedModel->g(x[i],S)*u[i] );
                if(move == 1){
                    dUi += (1/(x[i+1]-x[i]))*(dx[i+1]*u[i+1] - dx[i]*u[i] - (dx[i+1] - dx[i])*U[i]); 
                }
                
                U[i] = U[i] + step*dUi;//for all i, we have the average distribution on each interval
            }
        }
        //now, we calculate the density at each boundary
        u[0] = U[0]; //cf equation (1b)
        u[1] = u[0];//assumption made before finding a solution to the boundary pbm
        //std::cout << u[0] << std::endl;
        for (unsigned int i=2; i<=(N-1);i++){//still size
            rMinus = (U[i]-U[i-1])*(x[i]-x[i-2])/((U[i-1]-U[i-2])*(x[i-1]-x[i-2]));
            rPlus = (U[i-1]-U[i])*(x[i]-x[i+2])/((U[i]-U[i+1])*(x[i-1]-x[i]));
            if(usedModel->g(x[i],S) >=0){
               tmp1 = phi(rMinus)*(U[i-1]-U[i-2])*(x[i]-x[i-1])/(x[i+1]-x[i-1]); 
                //std::cout << tmp1 << std::endl;
               u[i] = U[i-1] + tmp1;            
            }   
            else{
                u[i] = U[i] - phi(rPlus)*(U[i+1]-U[i])*(x[i+1]-x[i])/(x[i+2]-x[i]); 
            }
        }
        u[N] = u[N-1];//assumption made before finding a solution to the boundary pbm

    //must compute the new resourceDynamics S
    S = S + step*usedModel->dS(S,x,u);
    //std::cout << j << " : " << S << std::endl;
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


//void solver::display(){
//// Crée un tableau de valeurs.                                             
//     double array[u.size()];                                                       
//     for (unsigned int i = 0; i< u.size(); ++i){                                              
//         array[i] = u[i];                          
//         //std::cout << "index "<< i << " : " << u[i] << std::endl;
//     }                                                                          
//     // Affiche le sous forme de graphique.                                     
//     CImg<>(array,u.size()).display_graph("distribution",1); 
//}

void solver::display(){
// Read command line argument   cimg_usage("Simple plotter of ECG signal");
 //const char *const formula = cimg_option("-f", "u", "Formula to plot");
 //const float x0 = cimg_option("-x0", 0.0f, "Minimal X-value");
 //const float x1 = cimg_option("-x1", 1.0f, "Maximal X-value");
 int sizeu = u.size();
 //const int resolution = cimg_option("-r", sizeu, "Plot resolution");
 const unsigned int nresolution = sizeu;
// const unsigned int plot_type = cimg_option("-p", 1, "Plot type");
 //const unsigned int vertex_type = cimg_option("-v", 1, "Vertex type");

 // Create plot data.
 CImg<double> values(1, nresolution, 1, 1, 0);

 const unsigned int r = nresolution - 1;

 for (int i1 = 0; i1 < sizeu; ++i1)
 {
     double xtime = i1/ r;
         values(0, i1) = u.at(i1);
         }  
         // Display interactive plot window.
         values.display_graph();
}

void solver::displayEquilibrum(){
    double tmp,Seq, xeq,ustar;
    std::vector<double> ueq;
    unsigned int i = 0;
    tmp = 0.1*(1+0.1)*(2+0.1)/(2*0.75);
    xeq = pow(tmp,(double (1)/3));
    std::cout << "xeq = " << xeq << std::endl;
    Seq = xeq/(1-xeq);
    while (x[i] <= xeq){
        ustar = 0.75*0.5*Seq*(1-Seq/3)*pow((xeq-x[i]),0.1-1)/pow(xeq,0.1);
        ueq.push_back(ustar);
        i++;
    }
    u = ueq;
    double array[ueq.size()];                                                       
    for (unsigned int i = 0; i< ueq.size(); ++i){                                              
        array[i] = ueq[i];                          
        //std::cout << "index "<< i << " : " << ueq[i] << std::endl;
    }                                                                          
    // Affiche le sous forme de graphique.                                     
    CImg<>(array,ueq.size()).display_graph("Equilibrum distribution",1); 

}

void solver::showContent(std::vector<double> vec){
    for(unsigned int i=0;i<vec.size();i++){
        std::cout << vec[i] << std::endl;
    }
}
