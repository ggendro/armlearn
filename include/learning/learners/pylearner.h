/**
 * @file pylearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class PyLearner, used for computing positions using AI methods
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef PYLEARNER_H
#define PYLEARNER_H

#include <Python.h>
#include <thread>

#include "devicelearner.h"
#include "input.h"
#include "output.h"
#include "converter.h"
#include "fileerror.h"


// Number of iterations of the learning
#define LEARN_NB_ITERATIONS 200
// Margin of error allowing to stop learning if error is below the number
#define LEARN_ERROR_MARGIN 0.05

// Directory containging the python scripts
#define PY_LEARN_DIR "../../python/"
// Name of the python script containing the learning algorithm
#define PY_LEARN_FILE "learner"
// Name of the python learner class
#define PY_LEARN_OBJECT "Learner"
// Name of the python class method used for learning
#define PY_LEARN_METHOD_LEARN "learn"
// Name of the python class mehod used for predicting result
#define PY_LEARN_METHOD_COMPUTE "compute"
// Time to wait for servomotors to reach each position (in milliseconds)
#define PY_LEARN_WAITING_TIME 5000


/**
 * @class PyLearner
 * @brief Class containing a simple learner that learns how to compute servomotor positions from coordinates without obstacles or disabled servomotors
 * 
 */
class PyLearner : public DeviceLearner{

    protected:
        Converter* verifier;
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
         * @return std::vector<uint16_t> the output given by the python learner
         */
        std::vector<uint16_t> pyLearn(std::vector<uint16_t> input);


    public:

        /**
         * @brief Constructs a new PyLearner object
         * 
         */
        PyLearner(AbstractController* controller, Converter* converter, std::string learningScript = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the PyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~PyLearner();



        /**
         * @brief Executes a learning algorithm on the learning set
         * 
         * Abstract method
         */
        virtual void learn() override;

        /**
         * @brief Tests the efficiency of the learning 
         * 
         * Abstract method
         */
        virtual void test() override;

        /**
         * @brief Computes an output from an input (to use afetr learning and validation with testing)
         * 
         * @param input
         * @return Output
         */
        virtual Output* produce(const Input& input) override;

        /**
         * @brief Returns the state of the learner under string format
         * 
         * @return std::string the state of the learner
         */
        virtual std::string toString() const override;

};





#endif