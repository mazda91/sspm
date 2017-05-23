#ifndef DAPHNIA_HPP
#define DAPHNIA_HPP

#include "model.hpp"

class daphnia : public model{
private:

        
               


public:
   daphnia();
   daphnia(std::string method);
   virtual ~daphnia(){};
   virtual void defaultParameters(); 

    //void individualGrowthRate();            
    //void individualMortalityRate(); 
    //void individualBirthRate() ;
    //void resourceDynamics();            
    //void solve(); 
  
   //faire correspondre cet attribut avec celui de la liste setParameters via une référence
   double lengthAtBirth = 0.8;

};


#endif
