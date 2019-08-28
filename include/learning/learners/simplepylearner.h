/**
 * @file simplepylearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class SimplePyLearner, used for executing simple computation to determine positions, using python script
 * @version 0.1
 * @date 2019-06-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef SIMPLEPYLEARNER_H
#define SIMPLEPYLEARNER_H

#include <algorithm>

#include "pylearner.h"
#include "converter.h"
#include "learnsettings.h"

namespace armlearn {
    namespace learning {

/**
 * @class SimplePyLearner
 * @brief Class containing the basics methods for a simple learner based on a verifier, that learns how to compute servomotor positions from coordinates, via python script
 * 
 */
class SimplePyLearner : public PyLearner{

    protected:
        kinematics::Converter* verifier;

        /**
         * @brief Computes resulting reward of the output for the corresponding input and servomotor states, based on the converter
         * 
         * @tparam R class of target values
         * @tparam T class of position values
         * @param input target to reach
         * @param output servomotor positions computed, to evaluate
         * @return double output reward
         * 
         * Takes into account for the computation:
         *  - the distance between the target coordinates and the coordinates resulting from the output positions, the greater it is the greater the error will be
         *  - the distance browsed by servomotors, the greater it is the greater the error will be
         *  - if the distance to the target increased or decreased after output applied
         * 
         * Template method, defined for int, uint16_t and double
         */
        template<class R, class T> double computeReward(const std::vector<R>& input, const std::vector<T>& output) const;

        /**
         * @brief Applies a funcion to each element of a vector
         * 
         * @param R class of inital vector values
         * @param T class of resulting vector values
         * @param vect initial vector
         * @param func funcion to apply to each value of initial vector
         * @return std::vector<T> resulting vector
         * 
         * Template method, defined for int, uint16_t and double
         */
        template<class R, class T> std::vector<T> apply(const std::vector<R>& vect, const std::function< T(R) >& func) const;

        /**
         * @brief Formats the computation output for the device
         * 
         * 
         * @param output the output to format
         * @return std::vector<uint16_t> the formatted output
         * 
         * Abstract method
         */
        virtual std::vector<uint16_t> formatOutput(const std::vector<double>& output) const=0;


    public:

        /**
         * @brief Constructs a new SimplePyLearner object
         * 
         * @param controller the controller to link the learner to
         * @param learningScriptSettings the name of the json file containing the required settings of the python modules to use  
         * @param testProp the proportion of test couples compared to learning couples
         */
        SimplePyLearner(communication::AbstractController* controller, kinematics::Converter* converter, std::string learningScriptSettings = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the SimplePyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~SimplePyLearner();


         /**
         * @brief Computes an output from an input (to use after learning and validation with testing)
         * 
         * @param input
         * @return Output
         */
        virtual Output<std::vector<uint16_t>>* produce(const Input<uint16_t>& input) override;


        /**
         * @brief Returns the state of the learner under string format
         * 
         * @return std::string the state of the learner
         */
        virtual std::string toString() const override;

};


    }
}


#endif