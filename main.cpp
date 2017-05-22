#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <map>
#include <vector>

// Transforms a string s into a vector of words (substrings not containing 
// spaces)
void tokenize(const std::string &s, std::vector<std::string> &tokens) {
    std::istringstream f(s);
    std::string word;    
    while (f >> word) {
        tokens.push_back(word);
    }
}

void solveDaphnia(std::string method){;}

void solveModel(std::string model, std::string method){ 
    if((model == "daphnia")  || (model == "Daphnia")){
        solveDaphnia(method);
    }
    else{
        std::cout << "The model you want to solve is : " << model << "which we don't have" << std::endl;

    }
}

void changeParameter(std::string model, std::string parameter, double value){;}
void evaluate_command(std::string line){
    std::vector<std::string> command;
    std::string model,method,parameter;
    double value;
    tokenize(line,command);
    int i = 0;
    while (i < command.size()){
        if (command[i] == "-model"){
            model = command[i+1];
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
            for (int j=0; j<command[i+2].size();j++){ //converts a string in char* to use fonction atof
                str_value[j] = command[i+2][j];
            }
            value = atof(str_value);
            changeParameter(model,parameter,value);
            i += 3;
        }
    }
    solveModel(model,method);
}










int main() {
    std::string line;
    while(true){
        std::cout << ">" ;
        getline(std::cin,line);
        evaluate_command(line);
    }
        return 0;
}

