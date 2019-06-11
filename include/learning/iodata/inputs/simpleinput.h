/**
 * @file simpleinput.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class SimpleInput, input used by simple learners classes
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef SIMPLEINPUT_H
#define SIMPLEINPUT_H

#include "input.h"

/**
 * @class SimpleInput
 * @brief Input of simple learning algorithms
 */
class SimpleInput : public Input{

    public:

        /**
         * @brief Constructs a new Input object
         * 
         */
        SimpleInput(const std::vector<uint16_t>& in);

        /**
         * @brief Constructs a new Input object by copy
         * 
         * @param in the Input to copy
         */
        SimpleInput(const SimpleInput& in);

        /**
         * @brief Destroys the Input object
         * 
         */
        virtual ~SimpleInput();


        /**
         * @brief Returns the input under string format
         * 
         * @return std::string
         */
        virtual std::string toString() const override;


};

#endif