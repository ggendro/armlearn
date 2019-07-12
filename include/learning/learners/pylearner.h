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
#include <algorithm>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "devicelearner.h"
#include "input.h"
#include "output.h"
#include "fileerror.h"

// Directory containging the python scripts
#define PY_LEARN_DIR "../../python/"
// Name of the python script containing the learning algorithm
#define PY_LEARN_FILE "../../files/learnSettings/ddpglearnersettings.json"



/**
 * @class PyLearner
 * @brief Class containing a python script doing the learning computations
 * 
 * The json file containg the settings must have the following format:
 *  - standard json containing at least the following fields: 
 *      - "fileName" : string, name of the python module
 *      - "className" : string, name of the python class
 *      - "learnMethod" : string, name of the python training method
 *      - "computeMethod" : string, name of the python compute method
 *  - optional field "settings" containg the following:
 *      - fields corresponding to constructor parameters of the python class matching "className"
 *      - each field must contain two subfields:
 *          - "value" : the parameter value
 *          - "type" : the parameter type, using python c api syntax (https://docs.python.org/3/c-api/arg.html), only 'd', 'f', 'i', 's' or arrays of the preceding characters are allowed 
 */
class PyLearner : public DeviceLearner{

    protected:
        std::string learnerFile;
        std::string learnerClass;
        std::string learnMethod;
        std::string computeMethod;
            
        PyObject *pModule, *pLearner;

        /**
         * @brief Initializes python interpreter
         * 
         */
        void pyInit(nlohmann::json settings = NULL);

        /**
         * @brief Desallocates memory used for python interpreter 
         * 
         */
        void pyEnd();


        /**
         * @brief Converts vector into PyObject
         * 
         * @tparam T the type of the data stored (created for int, uint16_t and double)
         * @param vect the vector to convert
         * @return PyObject* the corresponding Python Object
         */
        template<class T> PyObject* vectorToPyObject(const std::vector<T> vect) const;

        /**
         * @brief Converts PyList into vector
         * 
         * @tparam T the type of the data
         * @param obj the Python List to convert
         * @return std::vector<T> the resulting vector
         */
        template<class T> std::vector<T> vectorFromPyObject(PyObject* pObj) const;

        /**
         * @brief Converts a numeric value into PyObject
         * 
         * @tparam T the type of the data stored (created for int, uint16_t and double)
         * @param value the numeric value to convert
         * @return PyObject* the corresponding Python Object
         */
        template<class T> PyObject* valueToPyObject(const T value) const;

        /**
         * @brief Converts PyList into numeric value
         * 
         * @tparam T the type of the data
         * @param obj the Python Object to convert
         * @return T the resulting value
         */
        template<class T> T valueFromPyObject(PyObject* pObj) const;


        /**
         * @brief Gets and prints error handled by python interpreter
         * 
         */
        void pyManageError() const;


        /**
         * @brief Sends learning computation to external python script
         * 
         * @param input the input of the learning
         * @param reward vector containing the reward associated to the input, in order to correct the output given for this input
         * @param reductionFactor the reduction factor of the reward
         * 
         * Size of Reward vector:
         *  - if size is 0, assumes that no correction has to be made
         *  - if size is 1, assumes that it is a global reward
         *  - otherwise, assumes that each value is a correction value for one servomotor
         */
        void pyLearn(const std::vector<uint16_t> input, const std::vector<double> reward = {}, const std::vector<uint16_t> nextInput = {}, double reductionFactor = 1) const;

        /**
         * @brief Ask computation to external python script
         * 
         * @param input the input of the asked computation
         * @return std::vector<uint16_t> the output given by the python learner
         */
        std::vector<int> pyCompute(const std::vector<uint16_t> input) const;


    public:

        /**
         * @brief Construct a new Py Learner object
         * 
         * @param controller the controller to link the learner to
         * @param learningScriptSettings the name of the json file containing the required settings of the python modules to use  
         * @param testProp the proportion of test couples compared to learning couples
         */
        PyLearner(AbstractController* controller, std::string learningScriptSettings = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

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