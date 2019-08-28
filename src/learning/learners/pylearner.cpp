/**
 * @copyright Copyright (c) 2019
 */


#include "pylearner.h"

using namespace armlearn;
using namespace learning;


PyLearner::PyLearner(communication::AbstractController* controller, std::string learningScriptSettings, double testProp):DeviceLearner(controller, testProp){
    std::ifstream f(learningScriptSettings);
    nlohmann::json fileReader;
    f >> fileReader;

    int nbArgs = 0;
    try{
        learnerFile = fileReader["fileName"];
        nbArgs++;

        learnerClass = fileReader["className"];
        nbArgs++;

        learnMethod = fileReader["learnMethod"];
        nbArgs++;

        computeMethod = fileReader["computeMethod"];

    }catch(nlohmann::detail::type_error){
        std::stringstream errMsg;
        errMsg << "Error : missing argument ";
        switch(nbArgs){
            case 0:
                errMsg << "'fileName'";
                break;

            case 1:
                errMsg << "'className'";
                break;

            case 2:
                errMsg << "'learnMethod'";
                break;

            case 3:
                errMsg << "'computeMethod'";
                break;
        }
        throw FileError(errMsg.str());

    }

    auto settings = fileReader.find("settings");
    if (settings != fileReader.end()){
        pyInit(*settings);
    }else{
        pyInit();
    }
    
}

PyLearner::~PyLearner(){
    pyEnd();
}


void PyLearner::pyInit(nlohmann::json settings){
    PyObject *pName, *pClassDict, *pCallClass, *pTupleArgs, *pTupleKwargs;
    Py_Initialize();

    std::stringstream addInludePath; // Append the directory containing python scripts to the path
    addInludePath << "import sys\n" << "sys.path.insert(0,'" << PY_LEARN_DIR << "')\n";
    PyRun_SimpleString(addInludePath.str().c_str());

    pName = PyUnicode_FromString(learnerFile.c_str()); // Get name of the script as a py object
    pModule = PyImport_Import(pName); // Get the script itself
    Py_DECREF(pName);


    pyManageError();
    if (pModule == NULL) throw FileError("Error while opening python learning script");

    pClassDict = PyModule_GetDict(pModule); // Get the list of classes contained in the module
    pCallClass = PyDict_GetItemString(pClassDict, learnerClass.c_str()); // Get the wanted class
    Py_DECREF(pClassDict);
    if(pCallClass == NULL || !PyCallable_Check(pCallClass)) {
        std::stringstream errMsg;
        errMsg << "Error : learning class not found - verify that module " << PyModule_GetName(pModule) << " contains a class named " << learnerClass;
        throw FileError(errMsg.str());
    }

    // Create the instance of the class
    if(settings == NULL){
        pLearner = PyObject_CallObject(pCallClass, NULL);

    }else{
        pTupleArgs = PyTuple_New(0);
        pTupleKwargs = PyDict_New();

        for(nlohmann::json::iterator it = settings.begin(); it != settings.end(); it++) {
            try{
                if (it.value()["value"].is_boolean() || it.value()["type"] == "i"){
                    PyDict_SetItemString(pTupleKwargs, it.key().c_str(), Py_BuildValue(((std::string) it.value()["type"]).c_str(), (int) it.value()["value"]));

                } else if(it.value()["value"].is_number() || it.value()["type"] == "f" || it.value()["type"] == "d"){
                    PyDict_SetItemString(pTupleKwargs, it.key().c_str(), Py_BuildValue(((std::string) it.value()["type"]).c_str(), (float) it.value()["value"]));

                }else if(it.value()["value"].is_string() || it.value()["type"] == "s"){
                    PyDict_SetItemString(pTupleKwargs, it.key().c_str(), Py_BuildValue(((std::string) it.value()["type"]).c_str(), ((std::string) it.value()["value"]).c_str()));
                
                }else if(it.value()["value"].is_array()){
                    PyObject* list = PyList_New(0);

                    auto strType = ((std::string) it.value()["type"]);
                    auto ptr = 0;
                    char arr[1];
                    
                    for(int i=0; i < strType.size(); i++){
                            arr[0] = strType[i];

                        switch(strType[i]){
                            case 'i':
                                PyList_Append(list, Py_BuildValue(arr, (int) it.value()["value"][ptr]));
                                ptr++;
                                break;
                            
                            case 'd':
                            case 'f':
                                PyList_Append(list, Py_BuildValue(arr, (float) it.value()["value"][ptr]));
                                ptr++;
                                break;
                            
                            case 's':
                                PyList_Append(list, Py_BuildValue(arr, ((std::string) it.value()["value"][ptr]).c_str()));
                                ptr++;
                                break;
                        }
                    }
                    PyDict_SetItemString(pTupleKwargs, it.key().c_str(), list);
                    
                }else{
                    PyDict_SetItemString(pTupleKwargs, it.key().c_str(), Py_BuildValue(((std::string) it.value()["type"]).c_str(), it.value()["value"]));
                }

            }catch(nlohmann::detail::type_error){
                std::stringstream errMsg;
                errMsg << "Error : invalid setting format : " << *it;
                throw FileError(errMsg.str());
            }
        }
        pLearner = PyObject_Call(pCallClass, pTupleArgs, pTupleKwargs);

        Py_DECREF(pTupleKwargs);
        Py_DECREF(pTupleArgs);

    }
    Py_DECREF(pCallClass);

    pyManageError();
    if (pLearner == NULL) throw FileError("Error while extracting python learner from script");
}

void PyLearner::pyEnd(){
    Py_XDECREF(pLearner);
    Py_XDECREF(pModule);

    Py_Finalize();
}


template<class T> PyObject* PyLearner::vectorToPyObject(const std::vector<T> vect) const{
    PyObject *pObj;
    
    pObj = PyList_New(vect.size()); // Create python input from input vector
    int i=0;
    for(auto ptr = vect.cbegin(); ptr < vect.cend(); ptr++){
        PyList_SetItem(pObj, i, valueToPyObject(*ptr));
        i++;
    }

    return pObj;
}
template PyObject* PyLearner::vectorToPyObject<uint16_t>(const std::vector<uint16_t> vect) const;
template PyObject* PyLearner::vectorToPyObject<int>(const std::vector<int> vect) const;
template PyObject* PyLearner::vectorToPyObject<double>(const std::vector<double> vect) const;

template<class T> std::vector<T> PyLearner::vectorFromPyObject(PyObject* pObj) const{
    std::vector<T> res; // Get output vector from python output
    for(int j = 0; j < PyList_Size(pObj); j++){
        res.push_back(valueFromPyObject<T>(PyList_GetItem(pObj, j)));
    }

    return res;
}
template std::vector<uint16_t> PyLearner::vectorFromPyObject<uint16_t>(PyObject* pObj) const;
template std::vector<int> PyLearner::vectorFromPyObject<int>(PyObject* pObj) const;
template std::vector<double> PyLearner::vectorFromPyObject<double>(PyObject* pObj) const;

template<class T> PyObject* PyLearner::valueToPyObject(T value) const{
    return PyFloat_FromDouble((double) value);
}
template PyObject* PyLearner::valueToPyObject<uint16_t>(uint16_t value) const;
template PyObject* PyLearner::valueToPyObject<int>(int value) const;
template PyObject* PyLearner::valueToPyObject<double>(double value) const;
template PyObject* PyLearner::valueToPyObject<bool>(bool value) const;

template<class T> T PyLearner::valueFromPyObject(PyObject* pObj) const{
    return (T) PyFloat_AsDouble(pObj);
}
template uint16_t PyLearner::valueFromPyObject<uint16_t>(PyObject* pObj) const;
template int PyLearner::valueFromPyObject<int>(PyObject* pObj) const;
template double PyLearner::valueFromPyObject<double>(PyObject* pObj) const;


void PyLearner::pyManageError() const{
    PyObject *pErr = PyErr_Occurred();
    if(pErr != NULL) {
        std::cout << "Error from python interpreter : ";
        PyErr_Print();
    }
}



void PyLearner::pyLearn(const std::vector<uint16_t> input, const std::vector<double> output, const std::vector<double> reward, const std::vector<uint16_t> nextInput, double reductionFactor) const{
    if(reward.size() == 0 || (reward.size() == 1 && reward[0] == 0)) return;
    PyObject *pInput, *pOutput, *pErr, *pNextInput, *pRed, *pLearn;
    
    pInput = vectorToPyObject(input);
    pOutput = vectorToPyObject(output);
    pErr = vectorToPyObject(reward);
    pNextInput = vectorToPyObject(nextInput);
    pRed = valueToPyObject(reductionFactor);
    
    pLearn = PyUnicode_FromString(learnMethod.c_str());
    PyObject_CallMethodObjArgs(pLearner, pLearn, pInput, pOutput, pErr, pNextInput, pRed, NULL); // Python call for learning
    pyManageError();

    Py_DECREF(pLearn);
    Py_DECREF(pRed);
    Py_DECREF(pNextInput);
    Py_DECREF(pErr);
    Py_DECREF(pOutput);
    Py_DECREF(pInput);
}

std::vector<double> PyLearner::pyCompute(const std::vector<uint16_t> input, bool noise) const{
    PyObject *pInput, *pComp, *pValue, *pNoise;
    
    pInput = vectorToPyObject(input);
    pNoise = valueToPyObject(noise);
    
    pComp = PyUnicode_FromString(computeMethod.c_str());
    pValue = PyObject_CallMethodObjArgs(pLearner, pComp, pInput, pNoise, NULL); // Python call for computation
    pyManageError();

    if (pValue == NULL) throw FileError("Error while extracting result from python learner");

    auto res = vectorFromPyObject<double>(pValue);

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