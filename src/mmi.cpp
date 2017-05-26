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
    char str_value[par.size()];
    for (unsigned int j=0; j<par.size();j++){ //converts a string in char* to use fonction atof
        str_value[j] = par[j];
    }
    return atof(str_value);
}

void evaluate_command(std::string line, model **usedModel, solver **solveModel, std::vector<model *> *allModels){
    std::vector<std::string> command;
    std::string model,parameter;
    std::string method = "";
    double value;
    tokenize(line,command);
    unsigned int i = 0;
    while (i < command.size()){
        if (command[i] == "-model"){
            model = command[i+1];
            if ((model == "daphnia") || (model == "Daphnia")){
               *usedModel = new daphnia();
            }           
            allModels->push_back(*usedModel);
            (*solveModel)->setModel(*usedModel);
            i += 2;
        } else if (command[i] == "-method"){
            method = command[i+1];
            (*solveModel)->setMethod(method);
            i +=2;
        }else if (command[i] == "-ft"){//final time
            (*solveModel)->setTf(convert(command[i+1]));
            i += 2;
        }else if(command[i] ==  "-nti"){//Number of Time Intervals
            (*solveModel)->setM(convert(command[i+1]));
            i += 2;
        }else if (command[i] == "-nci"){ //Number of Class Intervals
            (*solveModel)->setN(convert(command[i+1]));
            i += 2;
        }else if ((command[i] ==  "-h") ||(command[i] == "-help")){
            std::cout << "here you can do ..." << std::endl;
            i += 1;
        } else if ((command[i] ==  "-q") ||(command[i] == "-quit")){
            exit(0);
        }else if (command[i] == "-cp"){ //to change a parameter of a model which must be provided before
            parameter = command[i+1];
            value = convert(command[i+2]);
            (*usedModel)->setParameter(parameter,value);
            (*usedModel)->setAttribute(parameter,value);
            i += 3;
        }else if(command[i] == "-sp"){//to show a parameter's info
            (*usedModel)->showParameter(command[i+1]);
            i += 2;
        }
        else{
            std::cout << "command " << command[i] << " not found. Type -h or -help for further information " << std::endl;;
        }
    }
}


