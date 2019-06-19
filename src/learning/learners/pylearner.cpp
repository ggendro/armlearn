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


template<class T> PyObject* PyLearner::toPyObject(const std::vector<T> vect) const{
    PyObject *pObj;
    
    pObj = PyList_New(vect.size()); // Create python input from input vector
    int i=0;
    for(auto ptr = vect.cbegin(); ptr < vect.cend(); ptr++){
        PyList_SetItem(pObj, i, PyLong_FromLong(*ptr));
        i++;
    }

    return pObj;
}
template PyObject* PyLearner::toPyObject<uint16_t>(const std::vector<uint16_t> vect) const;
template PyObject* PyLearner::toPyObject<int>(const std::vector<int> vect) const;

template<class T> std::vector<T> PyLearner::fromPyObject(PyObject* pObj) const{
    std::vector<T> res; // Get output vector from python output
    for(int j = 0; j < PyList_Size(pObj); j++){
        res.push_back(PyLong_AsLong(PyList_GetItem(pObj, j)));
    }

    return res;
}
template std::vector<uint16_t> PyLearner::fromPyObject<uint16_t>(PyObject* pObj) const;
template std::vector<int> PyLearner::fromPyObject<int>(PyObject* pObj) const;

void PyLearner::pyManageError() const{
    PyObject *pErr = PyErr_Occurred();
    if(pErr != NULL) {
        std::cout << "Error from python interpreter : ";
        PyErr_Print();
    }
}



void PyLearner::pyLearn(const std::vector<uint16_t> input, const std::vector<double> error) const{
    if(error.size() == 0 || (error.size() == 1 && error[0] == 0)) return;
    PyObject *pInput, *pErr, *pLearn;
    
    pInput = toPyObject(input);
    pErr = toPyObject(error);

    pLearn = PyUnicode_FromString(PY_LEARN_METHOD_LEARN);
    PyObject_CallMethodObjArgs(pLearner, pLearn, pInput, pErr, NULL); // Python call for learning
    pyManageError();

    Py_DECREF(pLearn);
    Py_DECREF(pErr);
    Py_DECREF(pInput);
}

std::vector<int> PyLearner::pyCompute(const std::vector<uint16_t> input) const{
    PyObject *pInput, *pComp, *pValue;
    
    pInput = toPyObject(input);
    
    pComp = PyUnicode_FromString(PY_LEARN_METHOD_COMPUTE);
    pValue = PyObject_CallMethodObjArgs(pLearner, pComp, pInput, NULL); // Python call for computation
    pyManageError();

    if (pValue == NULL) throw FileError("Error while extracting result from python learner");

    auto res = fromPyObject<int>(pValue);

    Py_DECREF(pValue);
    Py_DECREF(pComp);
    Py_DECREF(pInput);

    return res;
}


std::string PyLearner::toString() const{
    std::stringstream rep;
    rep << "Py " << DeviceLearner::toString();

    return rep.str();
}