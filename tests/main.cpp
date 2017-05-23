#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <map>
#include <vector>
#include "../include/model.hpp"
#include "../include/daphnia.hpp"

// Transforms a string s into a vector of words (substrings not containing 
// spaces)
void tokenize(const std::string &s, std::vector<std::string> &tokens) {
    std::istringstream f(s);
    std::string word;    
    while (f >> word) {
        tokens.push_back(word);
    }
}


void evaluate_command(std::string line){
    std::vector<std::string> command;
    std::string model,method,parameter;
    double value;
    daphnia *usedDaphnia = new daphnia();
    tokenize(line,command);
    unsigned int i = 0;
    while (i < command.size()){
        if (command[i] == "-model"){
            model = command[i+1];
            if ((model == "daphnia") || (model == "Daphnia")){
               ;
            }           
            i += 2;
        } else if (command[i] == "-method"){
            method = command[i+1];
            i +=2;
        } else if ((command[i] ==  "-h") ||(command[i] == "-help")){
            std::cout << "here you can do ..." << std::endl;
            i += 1;
        } else if ((command[i] ==  "-q") ||(command[i] == "-quit")){
            exit(0);
        }else if (command[i] == "-cp"){ //to change a parameter of a model which must be provided before
            parameter = command[i+1];
            char str_value[command[i+2].size()];
            for (unsigned int j=0; j<command[i+2].size();j++){ //converts a string in char* to use fonction atof
                str_value[j] = command[i+2][j];
            }
            value = atof(str_value);
            i += 3;
        }
    }
    std::cout << "length at birth dans les attributs : " <<   usedDaphnia->lengthAtBirth << std::endl;
    std::cout << "length at birth dans le vecteur de paramètres : " << usedDaphnia->getParameter("length at birth").value << std::endl;


}










int main() {
    std::string line;
    std::cout << ">" ;
    getline(std::cin,line);
    evaluate_command(line);
    
    //---------------------------------------Test 1 : IHM -------------------------------------
    //ligne de commande : -model daphnia -method FMU
    //sortie attendue : coucou, tu es bien dans daphnia et la méthode est FMU
    //ligne de commande : -model toto -method FMU
    //sortie attendue : erreur car modèle inconnu
    //ligne de commande : -model daphnia -method FMU -cd ok
    //sortie attendue :erreur car option inconnue
    //
    //---------------------------------------Test 2------------------------------------
    //ligne de commande : -cp length at birth 3.5
    //sortie : length at birth dans les attributs : 3.5
    //         length at birth dans le vecteur de paramètres : 3.5

    return  0;
}

