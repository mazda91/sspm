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
void solver::setTau(double tau){this->tau = tau;}

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
    double randVal;
    for (unsigned int i=0; i<J;i++){
        randVal = (double (rand()%J))/J;
        U.push_back(randVal);
        u.push_back(randVal);
    }
    u.push_back((double (rand()%J))/J);
}

void solver::setInitialCohorts(){
    N.clear();
    srand(time(NULL));
    for (unsigned int i = 0; i<J+1; i++){
        N.push_back(rand()%J);
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
    setJ(100);setM(100); setTf(100); tau = 1;
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
   double dN0,dPi0,Pi0,dNi,dxi = 0;//WARNING : do not confuse N[0] and N[0]
   double S = usedModel->S0;
   double fecundity;
   int min;
   unsigned int sizeArrays = u.size();
   std::vector<double> fixedMesh(x);
    Pi0 = 0;
  
  //  //displayEquilibrum(); //set u to the equilibrum state
  //  //showContent(u);
    mxArray *u_mx = mxCreateDoubleMatrix(sizeArrays,1,mxREAL);
    mxArray *x_mx = mxCreateDoubleMatrix(sizeArrays,1,mxREAL);
    mxArray *m_mx = mxCreateDoubleScalar(M);
    mxArray *t_mx = mxCreateDoubleScalar(0);
    mxArray *ft_mx = mxCreateDoubleScalar(Tf);
    mxArray *S_mx = mxCreateDoubleScalar(S);
   double *pu = mxGetPr(u_mx);
    double *px = mxGetPr(x_mx);
    double *pm = mxGetPr(m_mx);
    double *pt = mxGetPr(t_mx);
    double *pft = mxGetPr(ft_mx);
    double *pS = mxGetPr(S_mx);
    double *punew; double *pxnew;


    double *birthArray,*mortalityArray,*growthArray;

     engPutVariable(ep,"S",S_mx);
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
            px[i] = fixedMesh[i];
            pu[i] = u[i];
        }
        display(ep,x_mx,u_mx,t_mx);


        //Matlab life-processes functions call, one time for the whole vector to limit the number of calls
        //growth rate
        engPutVariable(ep,"x",x_mx);
        engEvalString(ep,"growthRate(x,S);");
        growthArray = mxGetPr(engGetVariable(ep,"ans"));

        //birth rate
        engEvalString(ep,"birthRate(x,S);");
        birthArray = mxGetPr(engGetVariable(ep,"ans"));

        //mortality rate
       engEvalString(ep,"mortalityRate(x,S);");
        mortalityArray = mxGetPr(engGetVariable(ep,"ans"));


        fecundity = 0;
//        dN0 = -usedModel->mu(usedModel->lb,S)*N[0] - Pi0*(usedModel->mu(x[1],S) - usedModel->mu(x[0],S))/(x[1] - x [0]);
        dN0 = -mortalityArray[0]*N[0] - Pi0*(mortalityArray[1] - mortalityArray[0])/(x[1] - x [0]);
        for (unsigned int k = 0;k<sizeArrays;k++){
//            fecundity += usedModel->beta(x[k],S)*N[k];
            fecundity += birthArray[k]*N[k];
        }
        dN0 += fecundity;

        dPi0 = growthArray[0]*N[0] + Pi0*(growthArray[1] - growthArray[0])/(x[1] - x [0]) - Pi0*(mortalityArray[1] - mortalityArray[0])/(x[1] - x [0]);

        N[0] = N[0] + step*dN0;
        Pi0 = Pi0 + step*dPi0;

        for (unsigned int i =0;i<sizeArrays;i++){
            dNi = -mortalityArray[i]*N[i];
            dxi = growthArray[i];
            N[i] = N[i] + step*dNi;
            x[i] = x[i] + step*dxi;
        }
     //   //reorganize cohorts
        if (N[0] != 0){
            x[0] = usedModel->lb + Pi0/N[0];
        }
        N.insert(N.begin(),0);
        x.insert(x.begin(),usedModel->lb);
        sizeArrays += 1;
       // u.resize(sizeArrays);
        
       // mxSetM(u_mx, sizeArrays);
       // mxSetM(x_mx, sizeArrays);

    //N[0] = 0; Pi0 = 0;
    min = removeCohort(N,ep);
    if (min != -1){
        x.erase(x.begin() + min);
        sizeArrays -= 1;
       // mxSetM(u_mx, sizeArrays);
       // mxSetM(x_mx, sizeArrays);

    }
  //  u.resize(sizeArrays);

        //convert cohorts abundances into a density distribution
   // for (unsigned int k =0; k<sizeArrays;k++){
        //std::cout << "(" << k << "," << &N[k]  << "," << N[k] << ")" << " / ";
     //   u[k] = N[k] / totalAbundance();
   // }
    compareMethods(fixedMesh,x,u,N);

    //environment
    //must compute the new resourceDynamics S
    *pS = S;
    engPutVariable(ep,"S",S_mx);
    engEvalString(ep,"environment(x,u,S);");
    std::cout << mxGetScalar(engGetVariable(ep,"S")) << std::endl;

   
   }
    mxDestroyArray(u_mx);
    mxDestroyArray(x_mx);
    mxDestroyArray(t_mx);
    mxDestroyArray(m_mx);
    mxDestroyArray(ft_mx);
}

void solver::compareMethods(std::vector<double> & fixedMesh, std::vector<double> & newX, std::vector<double> & u, std::vector<double> & abundance){
    unsigned int cmpt;
    double maxDiff = fixedMesh[1] - fixedMesh[0];
    for (unsigned int i=0;i<u.size();i++){
        u[i] = 0;
    }
    for (unsigned int i=0;i<u.size();i++){
        cmpt = 0;
        while (fabs(newX[i] - fixedMesh[cmpt]) > maxDiff){
            cmpt += 1;
        }
        if (fabs(newX[i]-fixedMesh[cmpt]) > fabs(newX[i]-fixedMesh[cmpt+1])){
            cmpt += 1;
        } 
        u[cmpt] += abundance[cmpt]/totalAbundance();
    }
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
    double S,dUi,sumBirth, rMinus, rPlus,rhoMinus, rhoPlus;
    double factor;
    double rightSide;
    //necessary vectors to run the Gauss-Thomas Algorithm, with dx solution of the linear system(moving mesh)
    std::vector<double> a(J+1,0);
    std::vector<double> b(J+1,0);
    std::vector<double> c(J+1,0);
    std::vector<double> d(J+1,0);
    std::vector<double> dx(J+1,0);

    //vector to keep  u and x : necessary to compute the new one
    std::vector<double> usave(u);
    std::vector<double> xsave(x);

    //matlab arrays and c++ corresponding pointers
    mxArray *u_mx = mxCreateDoubleMatrix(J+1,1,mxREAL); //stands for the distribution array
    double *pu = mxGetPr(u_mx);
    mxArray *dx_mx = mxCreateDoubleMatrix(J+1,1,mxREAL); //useful to display the motion of the mesh
    double *pdx= mxGetPr(dx_mx);
    mxArray *x_mx = mxCreateDoubleMatrix(J+1,1,mxREAL); //mesh array
    double *px = mxGetPr(x_mx);
    mxArray *X_mx = mxCreateDoubleMatrix(J,1,mxREAL); //midpoints mesh array : useful only to compute the integrals in (S2)
    double *pX = mxGetPr(X_mx);
    mxArray *m_mx = mxCreateDoubleScalar(M); //time discretization value
    mxArray *j_mx = mxCreateDoubleScalar(J);//space discretization value
    mxArray *t_mx = mxCreateDoubleScalar(0);//time
    double *pt = mxGetPr(t_mx);
    mxArray *ft_mx = mxCreateDoubleScalar(Tf);//ending time
    mxArray *size_mx = mxCreateDoubleScalar(0);//single mesh value
    double *psize = mxGetPr(size_mx);


    double *birthArray,*mortalityArray,*growthArray;

    engPutVariable(ep,"size",size_mx);
    //bind variables that won't change during computation
    engPutVariable(ep,"Tf",ft_mx);
    engPutVariable(ep,"M",m_mx);
    engPutVariable(ep,"J",j_mx); 

    engPutVariable(ep,"x",x_mx);
    engEvalString(ep,"init(x);");
    S = *mxGetPr(engGetVariable(ep,"ans"));
    std::cout << usedModel->S0 << std::endl;
    mxArray *S_mx = mxCreateDoubleScalar(usedModel->S0);//resource available

    double *pS = mxGetPr(S_mx);
    engPutVariable(ep,"S",S_mx);


    //init the vector of midpoints mesh before loop (because won't change for FMU so no need to update in the loop)
    for (unsigned int i = 0;i<J;i++){
        pX[i] = X(x,i);
    }
    for(unsigned int j =0; j<M; j++){//time
        //-------------------- setting all the data necessary to matlab calculus ---------------------------------
        // time / space grid / distribution /vector of mid-nodes
        pt[0] = j*step;
        for (unsigned int i = 0;i<J;i++){
            if (move == 1){//if MMU, update the midpoints mesh 
                pX[i] = X(x,i);
            }
            px[i] = x[i];
            pu[i] = u[i];
        }
        px[J] = x[J];
        pu[J] = u[J];
       
        //Matlab life-processes functions call, one time for the whole vector to limit the number of calls
        //growth rate
        engPutVariable(ep,"x",x_mx);
        engEvalString(ep,"growthRate(x,S);");
        growthArray = mxGetPr(engGetVariable(ep,"ans"));

        //birth rate
        engPutVariable(ep,"x",X_mx);
        engEvalString(ep,"birthRate(x,S);");
        birthArray = mxGetPr(engGetVariable(ep,"ans"));

        //mortality rate
       engEvalString(ep,"mortalityRate(x,S);");
        mortalityArray = mxGetPr(engGetVariable(ep,"ans"));

        
        display(ep,x_mx,u_mx,t_mx);

        if(move==1){//FIRST WE compute the variations of xi's if MMU
            factor = regularizingFactor();
            for (unsigned int i = 1; i<J; i++){ //fill the necesseray vectors to compute Gauss-Thomas algorithm on linear system (S3)
                rhoMinus = (monitor(i-1,factor)+monitor(i,factor))/2;
                rhoPlus = (monitor(i+1,factor)+monitor(i,factor))/2;
                rightSide = -(1/tau)*(rhoPlus*(x[i+1]-x[i]) - rhoMinus*(x[i]-x[i-1]));
                a[i] = rhoMinus;
                b[i] = -(rhoMinus + rhoPlus);
                c[i] = rhoPlus;
                d[i] = rightSide;
            }
            a[0] = 0;a[J]=0;
            c[0]=0;c[J]=0;
            b[0]=1;b[J]=1;
            d[0]=0;d[J]=0;
            GaussThomasAlgo(a,b,c,d,dx);
            //update the array related to MAtlab engine
            for (unsigned k=0;k<=J; k++){
                pdx[k] = dx[k];
                x[k] = x[k] + step*pdx[k];
           }
            engPutVariable(ep,"dx",dx_mx);
        }
        sumBirth = 0;
        for(unsigned int i=0; i<J; i++){//size
            dUi=0;
            if(i==0){ //taking into account equation (1b) : newborn individuals
                for(unsigned int k=0;k<J; k++){
                     //sumBirth +=(x[k+1] - x[k]) * usedModel->beta(X(k),S)*U[k];
                     //std::cout << birthArray[k] << "/" ; 
                    sumBirth +=(xsave[k+1] - xsave[k]) * birthArray[k]*U[k];
               }
               //u[0] = (1/usedModel->g(usedModel->lengthAtBirth,S))*sumBirth;
                u[0] = (1/growthArray[0])*sumBirth;
                U[0] = u[0];
                u[1] = u[0];//assumption made before finding a solution to the boundary pbm
                U[1] = u[1];
            }else if (i==1){;}
            else{ 
                rMinus = (U[i]-U[i-1])*(xsave[i]-xsave[i-2])/((U[i-1]-U[i-2])*(xsave[i-1]-xsave[i-2]));
                rPlus = (U[i-1]-U[i])*(xsave[i]-xsave[i+2])/((U[i]-U[i+1])*(xsave[i-1]-xsave[i]));
                //if(usedModel->g(xsave[i],S) - dx[i] >=0){
                if(growthArray[i] - dx[i] >=0){
                     u[i] = U[i-1] + phi(rMinus)*(U[i-1]-U[i-2])*(xsave[i]-xsave[i-1])/(xsave[i+1]-xsave[i-1]); 
                }   
                else{
                    u[i] = U[i] - phi(rPlus)*(U[i+1]-U[i])*(xsave[i+1]-xsave[i])/(xsave[i+2]-xsave[i]); 
                }
                dUi += -mortalityArray[i]*U[i] - (1/(xsave[i+1]-xsave[i]))*(growthArray[i+1]*usave[i+1] -  growthArray[i]*usave[i] );
                if(move == 1){
                    dUi += (1/(xsave[i+1]-xsave[i]))*(pdx[i+1]*usave[i+1] - pdx[i]*usave[i] - (pdx[i+1] - pdx[i])*U[i]); 
                }
                U[i] = U[i] + step*dUi;//for all i, we have the average distribution on each interval
            }
        }

        //environment
        //must compute the new resourceDynamics S
        *pS = S;
        engPutVariable(ep,"S",S_mx);
        engEvalString(ep,"environment(x,u,S);");
        std::cout << mxGetScalar(engGetVariable(ep,"S")) << std::endl;

        u[J] = u[J-1];//assumption made before finding a solution to the boundary pbm
        usave = u;
        xsave = x;
  }
    mxDestroyArray(S_mx);mxDestroyArray(size_mx);
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

void solver::setParameter(std::string parameter,double value){
    if (parameter == "Tf"){//final time                             
        setTf(value);                   
    }else if(parameter ==  "M"){//Number of Time Intervals          
        setM(value);                    
    }else if (parameter == "J"){ //Number of Class Intervals        
        setJ(value);                    
    }else if (parameter == "tau"){ //temporal regularization parameter
        setTau(value);                  
    }else{                                                             
        std::cout << parameter << " is not a solver parameter " << std::endl;
        exit(1);                                                                                                                    
  }
}
void solver::showParameter(std::string parameter){
    if (parameter == "Tf"){//final time                             
        std::cout << "Tf : " << Tf << std::endl; 
    }else if(parameter ==  "M"){//Number of Time Intervals          
        std::cout << "M :" << M << std::endl;          
    }else if (parameter == "J"){ //Number of Class Intervals        
        std::cout << "J : " << J << std::endl;                  
    }else if (parameter == "tau"){ //temporal regularization parameter
        std::cout << "tau : " << tau << std::endl;                  
    }else{                                                             
        std::cout << parameter << " is not a solver parameter " << std::endl;
        exit(1);                                                       
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

