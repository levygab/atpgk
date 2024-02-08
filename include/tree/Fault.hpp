/******************************************************************************

    ATPGK - An automated test pattern generator for integrated circuit

    Copyright (C) 2023-2024 Hugo Brisset & Gabriel Levy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or 
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Contact: hugo.brisset@proton.me / gabriel.levy@skiff.com

******************************************************************************/

/**
 * @file Fault.hpp
 * @brief Definition of the generic Fault class
*/

#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

/**
 * @namespace FaultModel
 * 
 * @brief All the class, enum and functions related to fault models
 */
namespace FaultModel {

/**
 * @enum FaultModelType
 * @brief Enumerates different fault models for ATPG (Automatic Test Pattern Generation).
 *
 * The FaultModelType enum class defines different fault models that can be used in ATPG algorithms.
 * Each fault model represents a specific type of fault that may occur in a digital circuit.
 * The integer values associated with each fault model can be used for numerical comparisons or indexing.
 */
enum class FaultModelType : int {
    StuckAtZero = 0, /**< Stuck-at-0 fault model. */
    StuckAtOne = 1,   /**< Stuck-at-1 fault model. */
    Count
};

/**
 * @brief Define aliases for each type of fault contained in FaultModelType
 * 
 */
static std::unordered_map<FaultModelType, std::string> faultAliases = {
    {FaultModelType::StuckAtZero, "Stuck-at-0"},
    {FaultModelType::StuckAtOne, "Stuck-at-1"}
};

/**
 * @brief Get the Fault Alias
 * 
 * @param fault 
 * @return std::string 
 */
std::string getFaultAlias(FaultModelType fault);

/**
 * @class Fault
 * @brief Represents a fault in a digital circuit.
 *
 * The Fault class encapsulates information about a fault in a digital circuit.
 * It includes details such as the fault type, coverage status, and the port where the fault is applied.
 */
class Fault {
public:
    /**
     * @brief Default constructor for the Fault class.
     * @param _type The type of fault created
     * @param _port The index of the port in the cell
     */
    Fault(FaultModelType _type, int _port);

    /**
     * @brief Destroy the Fault object
     */
    ~Fault();

    /**
     * @brief Set the coverage flag to true, indicating that the fault has been covered.
     */
    void setCovered();

    /**
     * @brief Set the coverage flag to false, indicating that the fault has not been covered.
     */
    void setUncovered();

    /**
     * @brief Getter for the coverage flag
     * @return bool - The coverage flag
     */
    bool getCoverageFlag();

    /**
     * @brief Getter for the port where the fault is applied.
     * @return int - The integer value corresponding to the port.
     */
    int getPort();

    /**
     * @brief Getter for the type of the fault.
     * @return FaultModelType - An integer (from the enumerated class FaultModelType) corresponding to the type of the fault.
     */
    FaultModelType getType();

    /**
     * @brief Setter for the value member
     * @param _value The value to set
     */
    void setValue(bool _value);

    /**
     * @brief Getter for the value member
     * 
     * @return bool
     */
    bool getValue();

    /**
     * @brief Get the Failure object
     * 
     * @return bool
     */
    bool getFailure() {
        return this->failure;
    }

    /**
     * @brief Set the failure boolean flag to True 
     */
    void setFailure() {
        this->failure = true;
    }

    /**
     * @brief Get the stuck-at-1 Counter value
     * 
     * @return int 
     */
    static int getStuckAt1Counter() {
        return stuck_at_1_counter;
    }

    /**
     * @brief Get the stuck-at-0 Counter value
     * 
     * @return int 
     */
    static int getStuckAt0Counter() {
        return stuck_at_0_counter;
    };


private:
    /**
     * @brief Type of the error (stuck-at-1, stuck-at-0, ...).
     */
    FaultModelType type;

    /**
     * @brief A boolean flag indicating whether the Fault has been covered or not.
     */
    bool covered;

    /**
     * @brief A boolean flag to know if the Fault can be test or not
     * 
     */
    bool failure = false;

    /**
     * @brief The port of the gate concerned by the fault.
     */
    int port;

    /**
     * @brief The expected value to test on the port
     */
    bool value;

    /**
     * @brief Global counter of stuck at 1 fault in the circuit
     */
    static int stuck_at_1_counter;

    /**
     * @brief Global counter of stuck at 0 fault in the circuit
     */
    static int stuck_at_0_counter;
};

} // namespace FaultModel