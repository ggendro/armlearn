/**
 * @file outputchain.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class OutputChain, output used by device learner classes
 * @version 0.1
 * @date 2019-08-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef OUTPUTCHAIN_H
#define OUTPUTCHAIN_H

#include "output.h"

/**
 * @class OutputChain
 * @brief OutputChain of device learning algorithms, inherited from Output
 * 
 * Used when output is not a single vector but a chain of vectors
 */
template<class T> class OutputChain : public Output<T>{

    protected:
        std::vector<Output<T>*> outChain;

    public:

        /**
         * @brief Constructs a new Output Chain object
         * 
         */
        OutputChain():Output<T>(){
            
        }

        /**
         * @brief Constructs a new Output Chain object
         * 
         * @param vectChain the output chain vector
         */
        OutputChain(const std::vector<std::vector<T>>& vectChain):Output<T>(){
            for(auto ptr = vectChain.cbegin(); ptr < vectChain.cend(); ptr++){
                outChain.push_back(new Output<T>(*ptr));
            }
        }

        /**
         * @brief Constructs a new Output Chain object
         * 
         * @param chain the chain to copy
         */
        OutputChain(const IOVector<T>& chain):Output<T>(chain){
            try {
                const OutputChain<T>& objc = dynamic_cast<const OutputChain<T>&>(chain);

                for(auto ptr = objc.outChain.cbegin(); ptr < objc.outChain.cend(); ptr++){
                    outChain.push_back(new Output<T>(*ptr));
                }
            }catch(const std::bad_cast& e) {
                
            }
        }

        /**
         * @brief Destroys the OutputChain object
         * 
         */
        virtual ~OutputChain(){
            for(auto ptr = outChain.begin(); ptr < outChain.end(); ptr++) delete *ptr;
        }

        /**
         * @brief Adds a new vector to the chain
         * 
         * @param vect the vector to add
         */
        virtual void addToChain(const std::vector<T>& vect){
            outChain.push_back(new Output<T>(vect));
        }

        /**
         * @brief Removes the last vector from the chain
         * 
         */
        virtual void removeFromChain(){
            delete *(outChain.end());
            outChain.pop_back();
        }


        /**
         * @brief Returns the output under the form of an array of intermediary positions
         * 
         * @return std::vector<Output<T>> the output
         */
        virtual std::vector<Output<T>*> getOutputChain() const{
            return outChain;
        }




        /**
         * @brief assignement operator
         * 
         * @param out the in copied in the current object
         * @return IOVector& the current IOVector object
         */
        virtual IOVector<T>& operator=(const IOVector<T>& obj){
            auto vector = obj.getVector();
            
            for(auto ptr = outChain.begin(); ptr < outChain.end(); ptr++) delete *ptr;
            outChain = {};


            try {
                const OutputChain<T>& objc = dynamic_cast<const OutputChain<T>&>(obj);
                
                for(auto ptr = objc.outChain.begin(); ptr < objc.outChain.end(); ptr++){
                    outChain.push_back(new Output<T>(**ptr));
                }
            }catch(const std::bad_cast& e) {
                
            }

            return *this;
        }

        /**
         * @brief equals comparison operator
         * 
         * @param obj object to compare to
         * @return true if the vectors are identical
         * @return false otherwise
         */
        virtual bool operator==(const IOVector<T>& obj) const{
            try {
                const OutputChain<T>& objc = dynamic_cast<const OutputChain<T>&>(obj);

                if(outChain.size() != objc.outChain.size()) return false;
                auto objcptr = objc.outChain.begin();

                for(auto ptr = outChain.begin(); ptr < outChain.end(); ptr++){
                    if(*ptr != *objcptr) return false;
                    objcptr++;
                }

                return IOVector<T>::vector == objc.vector;

            }catch(const std::bad_cast& e) {
                return false;
            }
        }


        /**
         * @brief Returns the vector under string format
         * 
         * @return std::string
         */
        virtual std::string toString() const{
            std::stringstream res;
            res << "{ ";
            for(auto&& v : IOVector<T>::vector) res << v << " ";
            res << "} + { ";
            for(auto&& v : outChain) res << v << " ";
            res << "}";

            return res.str();
        }
};

#endif