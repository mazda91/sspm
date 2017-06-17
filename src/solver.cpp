#include "../include/solver.hpp"

/*
 *@todo : resize the vectors x,u and U if J changes
 *
 */
solver::solver(){
    this->x = std::vector<double>();
    setInitialDistribution();
    setInitialCohorts();
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
void solver::setJ(unsigned int nbSizeIntervals){
    u.clear();
    U.clear();
    this->J = nbSizeIntervals;
    setInitialDistribution();
    setInitialCohorts();
    setSizeMesh();
}

void solver::setM(unsigned int nbTimeIntervals){this->M = nbTimeIntervals;}

void solver::setTf(float finalTime){this->Tf = finalTime;}
void solver::setTemporalRegularization(double tau){this->tau = tau;}

void solver::setSizeMesh(){
    x.clear(); //x of size 0
   double stepSize = (this->usedModel->maximumLength - this->usedModel->lengthAtBirth)/this->J;
   for (unsigned int i=0; i<= this->J ;i++){
       x.push_back(i*stepSize); 
   }

}

void solver::setInitialDistribution(){ //we assume that, initially, the distribution is equal between classes
    srand(time(NULL));
    u.clear(); U.clear();
    double tmp1;
    for (unsigned int i=0; i<J;i++){
        tmp1 = (double (rand()%J))/J;
        U.push_back(tmp1);
        u.push_back(tmp1);
    }
    u.push_back((double (rand()%J))/J);
}

void solver::setInitialCohorts(){
    N.clear();
    for (unsigned int i = 0; i<J+1; i++){
        N.push_back(10);
    }
}

void solver::reInitialize(){
    setInitialDistribution();
    setInitialCohorts();
    if (this->usedModel != NULL){
        setSizeMesh();
    }
}

void solver::initParameters(){
    setJ(10);setM(10); setTf(10); tau = 1;
}

double phi(double r){
    return fmax(fmax(0,fmin(2*r,1)),fmin(r,2));
}


void solver::solve(Engine *ep){
    if (this->usedMethod == "FMU"){
        solve_MU(ep,0);
    }else if (this->usedMethod == "MMU"){
        solve_MU(ep,1);
    }else if (this->usedMethod == "EBT"){
        solve_EBT(ep);
    }
}

double solver::totalAbundance(){
    double res = 0;
    for (unsigned int i=0;i<N.size();i++){
        res += N[i];
    }
    return res;
}
void solver::solve_EBT(Engine *ep){
   double step = Tf/M; 
   double dN0,dPi0,N0,Pi0,dNi,dxi = 0;//WARNING : do not confuse N0 and N[0]
   double S = usedModel->S0;
   double fecundity;
   int min;
   unsigned int sizeArrays = u.size();

    N0 = 0; Pi0 = 0;
  
  //  //displayEquilibrum(); //set u to the equilibrum state
  //  //showContent(u);
    mxArray *u_mx = mxCreateDoubleMatrix(sizeArrays,1,mxREAL);
    mxArray *x_mx = mxCreateDoubleMatrix(sizeArrays,1,mxREAL);
    mxArray *m_mx = mxCreateDoubleScalar(M);
    mxArray *t_mx = mxCreateDoubleScalar(0);
    mxArray *ft_mx = mxCreateDoubleScalar(0);
    double *pu = mxGetPr(u_mx);
    double *px = mxGetPr(x_mx);
    double *pm = mxGetPr(m_mx);
    double *pt = mxGetPr(t_mx);
    double *pft = mxGetPr(ft_mx);
    double *punew; double *pxnew;

    pft[0] = Tf;
    pm[0] = M;
    engPutVariable(ep,"Tf",ft_mx);
    engPutVariable(ep,"M",m_mx);

   for (unsigned int j=0;j<M;j++){
        pt[0] = j*step;
        pxnew = px;
        punew = pu;
        pu = (double*)mxRealloc(punew,sizeArrays * sizeof(double));
        mxSetPr(u_mx,pu);
        px = (double*)mxRealloc(pxnew,sizeArrays * sizeof(double));
        mxSetPr(x_mx,px);
        for(unsigned int i=0; i< sizeArrays; i++){
            px[i] = x[i];
            pu[i] = u[i];
           
        }


        engPutVariable(ep,"ti",t_mx);
        engPutVariable(ep,"x",x_mx);
        engPutVariable(ep,"u",u_mx);
        engEvalString(ep,"plot(x,u);");
        engEvalString(ep,"title(sprintf('t = %0.3f',ti));");
        engEvalString(ep,"pause(Tf/M);");

        //display(ep,x_mx,u_mx,t_mx);

        fecundity = 0;
        dN0 = usedModel->mu(usedModel->lb,S)*N[0] - Pi0*(usedModel->mu(x[1],S) - usedModel->mu(usedModel->lb,S))/(x[1] - x [0]);
        for (unsigned int k = 0;k<sizeArrays;k++){
            fecundity += usedModel->beta(x[k],S)*N[k];
        }
        dN0 += fecundity;

        dPi0 = usedModel->g(usedModel->lb,S)*N[0] + Pi0*(usedModel->g(x[1],S) - usedModel->g(usedModel->lb,S))/(x[1] - x [0]) - Pi0*(usedModel->mu(x[1],S) - usedModel->mu(usedModel->lb,S))/(x[1] - x [0]);

        N0 = N0 + step*dN0;
        Pi0 = Pi0 + step*dPi0;

        for (unsigned int i =0;i<sizeArrays;i++){
            dNi = -usedModel->mu(x[i],S)*N[i];
            dxi = usedModel->g(x[i],S);
            N[i] = N[i] + step*dNi;
            x[i] = x[i] + step*dxi;
        }
        //reorganize cohorts
        N.insert(N.begin(),N0);
        x.insert(x.begin(),usedModel->lb);
        sizeArrays += 1;
         if (N0 != 0){
            x[0] = usedModel->lb + Pi0/N0;
        }
        u.resize(sizeArrays);
        
        mxSetM(u_mx, sizeArrays);
        mxSetM(x_mx, sizeArrays);

    N0 = 0; Pi0 = 0;
    min = removeCohort(N,ep);
    if (min != -1){
        x.erase(x.begin() + min);
        sizeArrays -= 1;
        mxSetM(u_mx, sizeArrays);
        mxSetM(x_mx, sizeArrays);

    }
    u.resize(sizeArrays);

        //convert cohorts abundances into a density distribution
    for (unsigned int k =0; k<sizeArrays;k++){
        //std::cout << "(" << k << "," << &N[k]  << "," << N[k] << ")" << " / ";
        u[k] = N[k] / totalAbundance();
    }
    S = S + step* usedModel->dS(S,x,u);

   
   }
    mxDestroyArray(u_mx);
    mxDestroyArray(x_mx);
    mxDestroyArray(t_mx);
    mxDestroyArray(m_mx);
    mxDestroyArray(ft_mx);
}

int solver::removeCohort(std::vector<double> &vec, Engine *ep){
    //look for the minimum
    int min = 0;
    for (unsigned int i = 1; i<vec.size(); i++){
        if (vec[i] < vec[min]){
            min = i;
        }
    }
    //remove min-th cohort ?
    if (vec[min] <= pow(10,-10)){
        vec.erase(vec.begin() + min);
        return min;
    }
    return -1;
}

void solver::display(Engine *ep,mxArray *mesh,mxArray *distribution, mxArray *time){
   engPutVariable(ep,"ti",time);
   engPutVariable(ep,"x",mesh);
   engPutVariable(ep,"u",distribution);
   engEvalString(ep,"plot(x,u);");
   engEvalString(ep,"title(sprintf('t = %0.3f',ti));");
   engEvalString(ep,"pause(Tf/M);");


}

/*
 *@todo changing the values for u[1], u[J-1] and u[J] after finding a solution to the boundary value pbl
 *
 */
void solver::solve_MU(Engine *ep, unsigned int move){
    double step = ((double) Tf)/M; 
    double dUi,sumBirth, rMinus, rPlus,rhoMinus, rhoPlus;
    double S = usedModel->S0;double factor;
    double tmp1;
    std::vector<double> a(J+1,0);
    std::vector<double> b(J+1,0);
    std::vector<double> c(J+1,0);
    std::vector<double> d(J+1,0);
    std::vector<double> dx(J+1,0);

    //displayEquilibrum(); //set u to the equilibrum state
    //showContent(u);
    mxArray *u_mx = mxCreateDoubleMatrix(u.size(),1,mxREAL);
    mxArray *dx_mx = mxCreateDoubleMatrix(u.size(),1,mxREAL);
    mxArray *x_mx = mxCreateDoubleMatrix(u.size(),1,mxREAL);
    mxArray *m_mx = mxCreateDoubleScalar(M);
    mxArray *j_mx = mxCreateDoubleScalar(J);
    mxArray *t_mx = mxCreateDoubleScalar(0);
    mxArray *ft_mx = mxCreateDoubleScalar(0);

    double *pu = mxGetPr(u_mx);
    double *pdx= mxGetPr(dx_mx);
    double *px = mxGetPr(x_mx);
    double *pm = mxGetPr(m_mx);
    double *pj = mxGetPr(j_mx);
    double *pt = mxGetPr(t_mx);
    double *pft = mxGetPr(ft_mx);

    pft[0] = Tf;
    pm[0] = M;
    pj[0]=J;
    engPutVariable(ep,"Tf",ft_mx);
    engPutVariable(ep,"M",m_mx);
    engPutVariable(ep,"J",j_mx); 
    for(unsigned int j =0; j<M; j++){//time
        pt[0] = j*step;
        for(unsigned int i=0; i< u.size(); i++){
            px[i] = x[i];
            pu[i] = u[i];
        }
        display(ep,x_mx,u_mx,t_mx);
        //FIRST WE compute the variations of xi's
        if(move==1){
            factor = regularizingFactor();
            for (unsigned int i = 1; i<J; i++){
                rhoMinus = (monitor(i-1,factor)+monitor(i,factor))/2;
                rhoPlus = (monitor(i+1,factor)+monitor(i,factor))/2;
                tmp1 = -(1/tau)*(rhoPlus*(x[i+1]-x[i]) - rhoMinus*(x[i]-x[i-1]));
                a[i] = rhoMinus;
                b[i] = -(rhoMinus + rhoPlus);
                c[i] = rhoPlus;
                d[i] = tmp1;
            }
            a[0] = 0;a[J]=0;
            c[0]=0;c[J]=0;
            b[0]=1;b[J]=1;
            d[0]=0;d[J]=0;
            GaussThomasAlgo(a,b,c,d,dx);
            //update the array related to MAtlab engine
            for (unsigned k=0;k<=J; k++){
                pdx[k] = dx[k];
            }
            engPutVariable(ep,"dx",dx_mx);
              //Update the xi
              for (unsigned int i =0; i<=J; i++){
                  x[i] = x[i] + step*pdx[i];
              }
        }
        sumBirth = 0;
        for(unsigned int i=0; i<J; i++){//size
            dUi=0;
            if(i==0){ //taking into account equation (1b) : newborn individuals
                for(unsigned int k=0;k<J; k++){
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
        u[J] = u[J-1];//assumption made before finding a solution to the boundary pbm
    //must compute the new resourceDynamics S
    S = S + step*usedModel->dS(S,x,u);
  }
    mxDestroyArray(u_mx);mxDestroyArray(dx_mx);mxDestroyArray(x_mx);
    mxDestroyArray(t_mx);mxDestroyArray(j_mx);mxDestroyArray(m_mx);
    mxDestroyArray(ft_mx);
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
void solver::showContent(std::vector<double> vec){
    for(unsigned int i=0;i<vec.size();i++){
        std::cout << vec[i] << std::endl;
    }
}


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

