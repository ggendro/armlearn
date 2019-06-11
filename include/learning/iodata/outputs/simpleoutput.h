/**
 * @file simpleoutput.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class SimpleOutput, output used by simple learners classes
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef SIMPLEOUTPUT_H
#define SIMPLEOUTPUT_H

#include "output.h"

/**
 * @class SimpleOutput
 * @brief Output of simple learning algorithms
 */
class SimpleOutput : public Output{

    public:

        /**
         * @brief Constructs a new SimpleOutput object
         * 
         */
        SimpleOutput(const std::vector<uint16_t>& out);

        /**
         * @brief Construct a new SimpleOutput object by copy
         * 
         * @param in the SimpleOutput to copy
         */
        SimpleOutput(const SimpleOutput& out);

        /**
         * @brief Destroys the SimpleOutput object
         * 
         */
        virtual ~SimpleOutput();


        /**
         * @brief Returns the output under string format
         * 
         * @return std::string
         */
        virtual std::string toString() const override;

};

#endif