/**
 * @copyright Copyright (c) 2019
 */


#include "pylearner.h"


PyLearner::PyLearner(AbstractController* controller, std::string learningScript, double testProp):DeviceLearner(controller, testProp){
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
    Py_XDECREF(pModule);

    Py_Finalize();
}


std::vector<uint16_t> PyLearner::pyLearn(const std::vector<uint16_t> input, const std::vector<double> error){
    PyObject *pInput, *pExpOutput, *pLearn, *pComp, *pValue;
    
    
    pInput = PyList_New(input.size()); // Create python input from input
    int i=0;
    for(auto ptr = input.cbegin(); ptr < input.cend(); ptr++){
        PyList_SetItem(pInput, i, PyLong_FromLong(*ptr));
        i++;
    }

    if(error.size() != 0){
        pExpOutput = PyList_New(error.size()); // Create python error from error
        int j=0;
        for(auto ptr = error.cbegin(); ptr < error.cend(); ptr++){
            PyList_SetItem(pExpOutput, j, PyLong_FromLong(*ptr));
            j++;
        }

        pLearn = PyUnicode_FromString(PY_LEARN_METHOD_LEARN);
        PyObject_CallMethodObjArgs(pLearner, pLearn, pInput, pExpOutput, NULL); // Python call for learning

        Py_DECREF(pExpOutput);
    }
    
    pComp = PyUnicode_FromString(PY_LEARN_METHOD_COMPUTE);
    pValue = PyObject_CallMethodObjArgs(pLearner, pComp, pInput, NULL); // Python call for computation
    if (pValue == NULL) throw FileError("Error while extracting result from python learner");

    std::vector<uint16_t> res; // Get output from python output
    for(int j = 0; j < PyList_Size(pValue); j++){
        res.push_back(PyLong_AsLong(PyList_GetItem(pValue, j)));
    }

    Py_DECREF(pValue);
    Py_DECREF(pComp);
    Py_DECREF(pLearn);
    Py_DECREF(pInput);

    return res;
}


std::string PyLearner::toString() const{
    std::stringstream rep;
    rep << "Py " << DeviceLearner::toString();

    return rep.str();
}