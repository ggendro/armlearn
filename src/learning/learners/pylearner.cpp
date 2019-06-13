/**
 * @copyright Copyright (c) 2019
 */


#include "pylearner.h"


PyLearner::PyLearner(AbstractController* controller, Converter* converter, std::string learningScript, double testProp):DeviceLearner(controller, testProp){
    verifier = converter;
    learnerFile = learningScript;
    
    pyInit();
}

PyLearner::~PyLearner(){
    pyEnd();
}


void PyLearner::pyInit(){
    PyObject *pName, *pClassDict, *pCallClass;
    Py_Initialize();

    std::stringstream addInludePath; // Append the directory containing python scripts to the path
    addInludePath << "import sys\n" << "sys.path.insert(0,'" << PY_LEARN_DIR << "')\n";
    PyRun_SimpleString(addInludePath.str().c_str());

    pName = PyUnicode_FromString(learnerFile.c_str()); // Get name of the script as a py object
    pModule = PyImport_Import(pName); // Get the script itself
    Py_DECREF(pName);
    if (pModule == NULL) throw FileError("Error while opening python learning script");

    pClassDict = PyModule_GetDict(pModule); // Get the list of classes contained in the module
    pCallClass = PyDict_GetItemString(pClassDict, PY_LEARN_OBJECT); // Get the wanted class
    Py_DECREF(pClassDict);
    if(pCallClass == NULL || !PyCallable_Check(pCallClass)) {
        std::stringstream errMsg;
        errMsg << "Error : learning class not found - verify that module " << PyModule_GetName(pModule) << " contains a class named " << PY_LEARN_OBJECT;
        throw FileError(errMsg.str());
    }

    pLearner = PyObject_CallObject(pCallClass, NULL); // Create the instance of the class
    Py_DECREF(pCallClass);
    if (pLearner == NULL) throw FileError("Error while extracting python learner from script");
}

void PyLearner::pyEnd(){
    Py_XDECREF(pLearner);
    Py_DECREF(pModule);

    Py_Finalize();
}


std::vector<uint16_t> PyLearner::pyLearn(std::vector<uint16_t> input){
    PyObject *pArgs, *pValue, *pFunc;
    
    pArgs = PyList_New(input.size()); // Create python input from input
    int i=0;
    for(auto ptr = input.cbegin(); ptr < input.cend(); ptr++){
        PyList_SetItem(pArgs, i, PyLong_FromLong(*ptr));
        i++;
    }
    
    pFunc = PyUnicode_FromString(PY_LEARN_METHOD_COMPUTE);
    pValue = PyObject_CallMethodObjArgs(pLearner, pFunc, pArgs, NULL); // Python call
    if (pValue == NULL) throw FileError("Error while extracting result from python learner");

    std::vector<uint16_t> res; // Get output from python output
    for(int j = 0; j < PyList_Size(pValue); j++){
        res.push_back(PyLong_AsLong(PyList_GetItem(pValue, j)));
    }

    Py_DECREF(pFunc);
    Py_DECREF(pValue);
    Py_DECREF(pArgs);

    return res;
}



void PyLearner::learn(){

}

void PyLearner::test(){

}

Output* PyLearner::produce(const Input& input){
    std::vector<uint16_t> fullInput(input.getInput()); // Create input of the DNN, add the target coordinates

    auto state = DeviceLearner::getDeviceState(); // get state of servomotors
    auto initPos = device->getPosition(); // Save current position

    for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
        fullInput.insert(fullInput.end(), ptr->begin(), ptr->end()); // Add the current state of the servomotors to the input
    }

    std::vector<uint16_t> output;
    for(int nbIt = 0; nbIt < LEARN_NB_ITERATIONS; nbIt++){

        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

        output = pyLearn(fullInput); // Computation of output
        device->setPosition(output); // Update of servomotors

        while(!device->goalReached() && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < PY_LEARN_WAITING_TIME){ // Wait for end of movement
            std::this_thread::sleep_for((std::chrono::milliseconds) PY_LEARN_WAITING_TIME / 10);
            
            device->updateInfos();
            currentTime = std::chrono::system_clock::now(); // TODO: make a real passive wait
        }
        
        auto finalPos = device->getPosition();
        double error = device->positionSumSquaredError() + computeSquaredError(finalPos, initPos);

        // TODO: feedback to agent

        if(error < LEARN_ERROR_MARGIN) break;
    }

    return new Output(output);
}


std::string PyLearner::toString() const{
    std::stringstream rep;
    rep << "Simple " << DeviceLearner::toString();

    return rep.str();
}