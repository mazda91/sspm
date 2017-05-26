#ifndef MMI_HPP
#define MMI_HPP

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "model.hpp"
#include "daphnia.hpp"

void tokenize(const std::string &s, std::vector<std::string> &tokens);

void evaluate_command(std::string line, model **usedModel);










#endif
