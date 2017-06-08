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
    u.clear();
    U.clear();
    this->N = nbSizeIntervals;
    srand(time(NULL));
    double tmp1;
    for (unsigned int i=0; i<N;i++){
        tmp1 = (double (rand()%N))/N;
        U.push_back(tmp1);
        u.push_back(tmp1);
    }
    u.push_back(double (rand()%N)/N);

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
    srand(time(NULL));
    double tmp1;
    for (unsigned int i=0; i<N;i++){
        tmp1 = (double (rand()%N))/N;
        U.push_back(tmp1);
        u.push_back(tmp1);
    }
    u.push_back((double (rand()%N))/N);
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
    Engine *ep = engOpen("");
    //displayEquilibrum(); //set u to the equilibrum state
    //showContent(u);
    mxArray *A_mx = mxCreateDoubleMatrix(u.size(),u.size(),mxREAL);
    mxArray *u_mx = mxCreateDoubleMatrix(u.size(),1,mxREAL);
    mxArray *dx_mx;// = mxCreateDoubleMatrix(u.size(),1,mxREAL);
    mxArray *b_mx = mxCreateDoubleMatrix(u.size(),1,mxREAL);
    mxArray *x_mx = mxCreateDoubleMatrix(u.size(),1,mxREAL);
    mxArray *m_mx = mxCreateDoubleScalar(M);
    mxArray *n_mx = mxCreateDoubleScalar(N);
    mxArray *t_mx = mxCreateDoubleScalar(0);
    mxArray *ft_mx = mxCreateDoubleScalar(0);
    mxArray *tmp = mxCreateDoubleScalar(0);

    double *pA = mxGetPr(A_mx);
    double *pu = mxGetPr(u_mx);
    double *pdx;// = mxGetPr(dx_mx);
    double *pb = mxGetPr(b_mx);
    double *px = mxGetPr(x_mx);
    double *pm = mxGetPr(m_mx);
    double *pn = mxGetPr(n_mx);
    double *pt = mxGetPr(t_mx);
    double *pft = mxGetPr(ft_mx);

    pft[0] = Tf;
    pm[0] = M;
    pn[0]=N;
    engPutVariable(ep,"Tf",ft_mx);
    engPutVariable(ep,"M",m_mx);
    engPutVariable(ep,"N",n_mx); 
    //showContent(x);
    for(unsigned int j =0; j<1; j++){//time
        pt[0] = j*step;
        engPutVariable(ep,"ti",t_mx);
        for(unsigned int i=0; i< u.size(); i++){
            px[i] = x[i];
            pu[i] = u[i];
        }
        engPutVariable(ep,"x",x_mx);
        engPutVariable(ep,"u",u_mx);
        engEvalString(ep,"plot(x,u);");
        //engEvalString(ep,"axis([0 1 -1 3]);");
        //engEvalString(ep,"grid on;");
        engEvalString(ep,"title(sprintf('t = %0.3f',ti));");
        engEvalString(ep,"pause(Tf/M);");
        //FIRST WE compute the variations of xi's
        if(move==1){
            factor = regularizingFactor();
            //std::cout << "factor : " << factor << std::endl;
            for (unsigned int i = 1; i<N; i++){
                rhoMinus = (monitor(i-1,factor)+monitor(i,factor))/2;
                rhoPlus = (monitor(i+1,factor)+monitor(i,factor))/2;
                tmp1 = -(1/tau)*(rhoPlus*(x[i+1]-x[i]) - rhoMinus*(x[i]-x[i-1]));
                pA[i *N +i] = -(rhoPlus + rhoMinus);
                pA[i*N + i-1] = rhoMinus;
                pA[i *N + i+1] = rhoPlus;
                pb[i] = tmp1;
                //std::cout << i << " : " << rhoMinus << " / " << rhoPlus <<  " / " << tmp1 << std::endl;
            }
            pb[0] = 0;pb[N] = 0;
            pA[0] = 1; pA[N * N] = 1;
             
            engPutVariable(ep,"A",A_mx);
            engPutVariable(ep,"b",b_mx);
            
            engEvalString(ep,"tmp = cond(A);");
            double *val =  (double *)mxGetData(engGetVariable(ep,"tmp"));
            std::cout << "cond : " << *val << std::endl;
            engEvalString(ep,"tmp = det(A);");
            val =  (double *)mxGetData(engGetVariable(ep,"tmp"));
            std::cout << "det : " << *val << std::endl;
            engEvalString(ep,"dx=linsolve(A,b);");
            //engEvalString(ep,"dx = linsolve(eye(N+1),b);");
            dx_mx = engGetVariable(ep,"dx");
            pdx = (double *)mxGetData(dx_mx);
            std::cout << "" << std::endl;
            std::cout << "---------------------------" << std::endl;
            //for (unsigned int k=0; k<u.size();k++){
              //std::cout << "(" << k << "," << pdx[k] << ")" << "/" ;
            //}
           // std::cout << "---------------------------" << std::endl;
           for (unsigned int k=0; k<N*N;k++){
                std::cout << "(" << k << "," << pA[k] << ")" << "/" ;
           }

              //Update the xi
              //std::cout << "------------" << std::endl;
              for (unsigned int i =0; i<=N; i++){
                  x[i] = x[i] + step*pdx[i];
                 // std::cout << dx[i] << "/";
              }
                            //std::cout << "------------" << std::endl;
              //std::cout << "****** " << j << " ******" << std::endl;


        }
        sumBirth = 0;
        for(unsigned int i=0; i<N; i++){//size
            dUi=0;
            if(i==0){ //taking into account equation (1b) : newborn individuals
                for(unsigned int k=0;k<N; k++){
                    sumBirth +=(x[k+1] - x[k]) * usedModel->beta(X(k),S)*U[k];
                }
                tmp1 = usedModel->g(usedModel->lengthAtBirth,S);
                u[0] = (1/tmp1)*sumBirth;
                U[0] = u[0];
                u[1] = u[0];//assumption made before finding a solution to the boundary pbm
                U[1] = u[1];
            }else if (i==1){;}
            else{ 
                rMinus = (U[i]-U[i-1])*(x[i]-x[i-2])/((U[i-1]-U[i-2])*(x[i-1]-x[i-2]));
                rPlus = (U[i-1]-U[i])*(x[i]-x[i+2])/((U[i]-U[i+1])*(x[i-1]-x[i]));
                if(usedModel->g(x[i],S) >=0){
                     u[i] = U[i-1] + phi(rMinus)*(U[i-1]-U[i-2])*(x[i]-x[i-1])/(x[i+1]-x[i-1]); 
                }   
                else{
                    u[i] = U[i] - phi(rPlus)*(U[i+1]-U[i])*(x[i+1]-x[i])/(x[i+2]-x[i]); 
                }
                dUi += -usedModel->mu(X(i),S)*U[i] - (1/(x[i+1]-x[i]))*(usedModel->g(x[i+1],S)*u[i+1] -  usedModel->g(x[i],S)*u[i] );
                if(move == 1){
                    dUi += (1/(x[i+1]-x[i]))*(pdx[i+1]*u[i+1] - pdx[i]*u[i] - (pdx[i+1] - pdx[i])*U[i]); 
                }
                U[i] = U[i] + step*dUi;//for all i, we have the average distribution on each interval
            }
        }
        u[N] = u[N-1];//assumption made before finding a solution to the boundary pbm
    //must compute the new resourceDynamics S
    S = S + step*usedModel->dS(S,x,u);
    std::cout << " j : " << j << std::endl;
  }
    engClose(ep);
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
        //std::cout << " i : " << i << " dx : " << x[i+1] << " - " << x[i] << "dxU : " << u[i+1] << " - " << u[i] << std::endl;
        //std::cout << integral << std::endl;
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
//     }                                                                          
//     // Affiche le sous forme de graphique.                                     
//     CImg<>(array,u.size()).display_graph("distribution",1); 
//}

//void solver::display(){
//// Read command line argument   cimg_usage("Simple plotter of ECG signal");
// //const char *const formula = cimg_option("-f", "u", "Formula to plot");
// //const float x0 = cimg_option("-x0", 0.0f, "Minimal X-value");
// //const float x1 = cimg_option("-x1", 1.0f, "Maximal X-value");
// int sizeu = u.size();
// //const int resolution = cimg_option("-r", sizeu, "Plot resolution");
// const unsigned int nresolution = sizeu;
//// const unsigned int plot_type = cimg_option("-p", 1, "Plot type");
// //const unsigned int vertex_type = cimg_option("-v", 1, "Vertex type");
//
// // Create plot data.
// CImg<double> values(1, nresolution, 1, 1, 0);
//
// const unsigned int r = nresolution - 1;
//
// for (int i1 = 0; i1 < sizeu; ++i1)
// {
//     double xtime = i1/ r;
//         values(0, i1) = u.at(i1);
//         }  
//         // Display interactive plot window.
//         values.display_graph();
//}
//
//void solver::displayEquilibrum(){
//    double tmp,Seq, xeq,ustar;
//    std::vector<double> ueq;
//    unsigned int i = 0;
//    tmp = 0.1*(1+0.1)*(2+0.1)/(2*0.75);
//    xeq = pow(tmp,(double (1)/3));
//    Seq = xeq/(1-xeq);
//    while (x[i] <= xeq){
//        ustar = 0.75*0.5*Seq*(1-Seq/3)*pow((xeq-x[i]),0.1-1)/pow(xeq,0.1);
//        ueq.push_back(ustar);
//        i++;
//    }
//    u = ueq;
//    double array[ueq.size()];                                                       
//    for (unsigned int i = 0; i< ueq.size(); ++i){                                              
//        array[i] = ueq[i];                          
//    }                                                                          
//    // Affiche le sous forme de graphique.                                     
//    CImg<>(array,ueq.size()).display_graph("Equilibrum distribution",1); 
//
//}
//
void solver::showContent(std::vector<double> vec){
    for(unsigned int i=0;i<vec.size();i++){
        std::cout << vec[i] << std::endl;
    }
}
