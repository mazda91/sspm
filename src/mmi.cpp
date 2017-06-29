#include "../include/mmi.hpp"
#include <string>

// Transforms a string s into a vector of words (substrings not containing 
// spaces)
void tokenize(const std::string &s, std::vector<std::string> &tokens) {
    std::istringstream f(s);
    std::string word;    
    while (f >> word) {
       tokens.push_back(word);
    }
}

double convert(std::string par){
    char str_value[par.size()] = "";
    for (unsigned int j=0; j<par.size();j++){ //converts a string in char* to use fonction atof
        str_value[j] = par[j];
    }
    return atof(str_value);
}

void evaluate_command(std::string line, model **usedModel, solver **solveModel, Engine *ep){
    std::vector<std::string> command;
    std::string model,parameter;
    std::string method = "";
    bool found = true;
    double value;
    tokenize(line,command);
    unsigned int i = 0;
    while ((i < command.size()) && (found == true)){
        if (command[i] == "-method"){
            method = command[i+1];
            (*solveModel)->setMethod(method);
            i +=2;
        } else if(command[i] == "-csp"){
            (*solveModel)->setParameter(command[i+1],convert(command[i+2]));
            i += 3;
        }else if (command[i] == "-cmp"){ //to change a parameter of a model which must be provided before
            parameter = command[i+1];
            value = convert(command[i+2]);
            (*usedModel)->setParameter(parameter,value);
            (*usedModel)->setAttribute(parameter,value);
            i += 3;

        }else if(command[i] == "-ssp"){//to show a solver parameter's info
            (*solveModel)->showParameter(command[i+1]);
            i += 2;
        }else if(command[i] == "-smp"){//to show a current model parameter's info
            (*usedModel)->showParameter(command[i+1]);
            i += 2;
        }else if ((command[i] ==  "-h") ||(command[i] == "-help")){
            std::cout << "here you can do ..." << std::endl;
            i += 1;
        } else if ((command[i] ==  "-q") ||(command[i] == "-quit")){
            exit(0);
        }else if(command[i] == "-solve"){
            if ((*usedModel) == NULL){
                std::cout << "No model defined. Can't solve." << std::endl;
                std::cout << "All the information provided after '" << command[i] << "'have not been taken into account." << std::endl;
                found = false;
            }
            else{
                (*solveModel)->solve(ep);
                i += 1;
            }
        }else if(command[i] == "-initSolver"){
            (*solveModel)->initParameters();
            i += 1;
        }else if(command[i] == "-initModel"){
            (*usedModel)->defaultParameters();
            i += 1;
        }else{
            std::cout << "command " << command[i] << " not found. Type -h or -help for further information " << std::endl;
            std::cout << "All the information provided after '" << command[i] << "'have not been taken into account." << std::endl;
            found = false;
        }
    }
}


