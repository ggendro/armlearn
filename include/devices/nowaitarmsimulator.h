/**
 * @file nowaitarmsimulator.h
 * @author Gaël Gendron
 * @brief File containing the NoWaitArmSimulator class, inherited from ArmSimulator, used to control a simulation of the hardware with instants movements
 * @version 0.1
 * @date 2019-07-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef NOWAITARMSIMULATOR_H
#define NOWAITARMSIMULATOR_H

#include "armsimulator.h"

class NoWaitArmSimulator : public ArmSimulator{

    public:

        /**
         * @brief Construct a new NoWaitArmSimulator object
         * 
         * @param displayMode mode of display (see DisplayMode enum for more details)
         * @param out the output stream to display the results, standard std output by default
         */
        NoWaitArmSimulator(DisplayMode displayMode = except, std::ostream& out = std::cout);

        /**
         * @brief Destroys the NoWaitArmSimulator object
         * 
         */
        ~NoWaitArmSimulator();

        /**
         * @brief Asks information from servomotor device and update the values in the class representing it
         * 
         * @param id the id of the servomotor to update
         * @return true if information has successfully been updated
         * @return false otherwise
         * 
         * Inherited method from ArmSimulator
         */
        using AbstractController::updateInfos;
        virtual bool updateInfos(uint8_t id) override;


};

#endif