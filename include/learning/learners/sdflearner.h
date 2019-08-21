/**
 * @file sdflearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class SdfLearner, used for computing positions using learner created with Preesm 
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef SDFLEARNER_H
#define SDFLEARNER_H

#include "devicelearner.h"
#include "converter.h"
#include "input.h"
#include "output.h"
#include "learnsettings.h"

#include "optimcartesianconverter.h"
#include "nowaitarmsimulator.h"
#include "widowxbuilder.h"


/**
 * @class SdfLearner
 * @brief Class containing an interface linking to a learner using SDF and created with Preesm software
 * 
 */
class SdfLearner : public DeviceLearner{

    protected:
        Converter* verifier;
        std::vector<float> rewards;
        int nbMoves;

        double computeReward(const std::vector<float> target, const std::vector<int> action) const;

    
    public:

        SdfLearner(AbstractController* controller, Converter* converter);
        ~SdfLearner();
        
        Converter* getVerifier() const;
        AbstractController* getDevice() const;


        void init(float *state_angular, int state_angular_size, float *state_observation, int state_space_size, float x_target, float y_target);
        void step(int state_space_size, int action_space_size, int state_angular_size, float x_target, float y_target,
                            float *state_angular_in, float *state_angular_out, float *input_actions, float *state_observation, float *reward);


        /**
         * @brief Executes a learning algorithm on the learning set
         * 
         */
        virtual void learn() override;

        /**
         * @brief Tests the efficiency of the learning 
         * 
         */
        virtual void test() override;

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


extern "C" SdfLearner* initWrapper(float *state_angular, int state_angular_size, float *state_observation, int state_space_size, float x_target, float y_target);
extern "C" void stepWrapper(SdfLearner* env, 
                        int state_space_size, int action_space_size, int state_angular_size, float x_target, float y_target,
                        float *state_angular_in, float *state_angular_out, float *input_actions, float *state_observation, float *reward);
extern "C" void endWrapper(SdfLearner* env);


#endif