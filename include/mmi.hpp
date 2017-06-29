#ifndef MMI_HPP
#define MMI_HPP
/** \file mmi.hpp
 * \brief Contains any function related to the man-machine interface
 */
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "model.hpp"
#include "solver.hpp"

/** \brief Converts a string into the corresponding double number.
 *
 *  \param[in] string to be converted
 *  \param[out] corresponding double value
 *  \todo Manage a typing error
 */
double convert(std::string);

/** \brief store the words in s in the vector tokens
 *
 *  Using space as a separating character, the words contained in the string s are stored in a vector of strings tokens
 *  \param[in]         s : string to be tokenized
 * \param[in,out] tokens : vector of words
 */
void tokenize(const std::string &s, std::vector<std::string> &tokens);

/** \brief Responds to the command line typed by the user
 *
 *  \param[in]          line : string corresponding to the command line
 *  \param[in,out] usedModel : pointer to the model type instance to deal with.
 *  \param[in,out] solver    : pointer to a solver instance.
 *  \todo complete options 
 */ 
void evaluate_command(std::string line, model **usedModel, solver **solveModel, Engine *ep);

#endif
