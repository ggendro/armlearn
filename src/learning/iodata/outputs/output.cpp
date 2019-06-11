/**
 * @copyright Copyright (c) 2019
 */


#include "output.h"

Output::~Output(){
    
}

std::vector<std::vector<uint16_t>> Output::getOutput(){
    std::vector<std::vector<uint16_t>> res;
    res.push_back(vector);

    return res;
}