/**
 * @file pylearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class PyLearner, used for computing positions using AI learners implemented in python scripts
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef PYLEARNER_H
#define PYLEARNER_H

#include <Python.h>

#include "devicelearner.h"
#include "input.h"
#include "output.h"
#include "fileerror.h"

// Directory containging the python scripts
#define PY_LEARN_DIR "../../python/"
// Name of the python script containing the learning algorithm
#define PY_LEARN_FILE "learner"
// Name of the python learner class
#define PY_LEARN_OBJECT "Learner"
// Name of the python class method used for learning
#define PY_LEARN_METHOD_LEARN "train"
// Name of the python class mehod used for predicting result
#define PY_LEARN_METHOD_COMPUTE "compute"


/**
 * @class PyLearner
 * @brief Class containing a python script doing the learning computations
 * 
 */
class PyLearner : public DeviceLearner{

    protected:
        std::string learnerFile;
        PyObject *pModule, *pLearner;

        /**
         * @brief Initializes python interpreter
         * 
         */
        void pyInit();

        /**
         * @brief Desallocates memory used for python interpreter 
         * 
         */
        void pyEnd();

        /**
         * @brief Sends learning computation to external python script
         * 
         * @param input the input of the learning
         * @param error vector containing the error of previous computation, in order to correct the new computation
         * @return std::vector<uint16_t> the output given by the python learner
         * 
         * Size of Error vector:
         *  - if size is 0, assumes that no correction has to be made
         *  - if size is 1, assumes that it is a global error
         *  - otherwise, assumes that each value is a correction value for one servomotor
         */
        std::vector<uint16_t> pyLearn(const std::vector<uint16_t> input, const std::vector<double> error);


    public:

        /**
         * @brief Constructs a new PyLearner object
         * 
         */
        PyLearner(AbstractController* controller, std::string learningScript = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the PyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~PyLearner();


        /**
         * @brief Returns the state of the learner under string format
         * 
         * @return std::string the state of the learner
         */
        virtual std::string toString() const override;

};





#endif