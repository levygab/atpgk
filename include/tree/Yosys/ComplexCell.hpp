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
 * @file ComplexCell.hpp
 * @brief Definition of the ComplexCell class and all the sub-classes that inherits from it
*/

#pragma once

#include "YosysCell.hpp"

/**
 * @namespace Complexcell
 * @brief All the class related to Yosys complex cells (more than 2 inputs)
*/
namespace Complexcell{

/**
 * @class ComplexCell
 * @brief Class representing a basic logic complex cell (with more than 2 inputs).
 *
 * This class is a super-class for basic logic complex gates such as MUX, AOIn, OAIn ...
 * This class inherits from YosysCell
 */
class ComplexCell : public YosysCell {
public:
    /**
     * @brief Constructs a ComplexCell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    ComplexCell(size_t identifier, std::string netlistName);
};

/**
 * @class AOI3
 * @brief Represents a 3-input AND-OR-INVERT gate.
 *
 * This class defines the behavior of a 3-input AOI gate which performs an AND operation on the inputs,
 * followed by an OR and an INVERT operation. Inherits from the ComplexCell class.
 */
class AOI3 : public ComplexCell {
public:

    /**
     * @brief Constructs an AOI3 cell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    AOI3(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
* @class OAI3
* @brief Represents a 3-input OR-AND-INVERT logic gate.
*
* Inherits from ComplexCell and models a gate that performs an OR operation on the first two inputs, an AND operation 
* with the third input, and then an INVERT operation on the result.
*/
class OAI3 : public ComplexCell {
public:

    /**
     * @brief Constructor for OAI3 cell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    OAI3(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
 * @class AOI4
 * @brief Represents a 4-input AND-OR-INVERT logic gate.
 *
 * Inherits from ComplexCell and defines the behavior of a 4-input AOI gate which performs an AND operation on the inputs,
 * followed by an OR and an INVERT operation.
 */
class AOI4 : public ComplexCell {
public:

    /**
     * @brief Constructor for AOI4 cell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    AOI4(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
 * @class OAI4
 * @brief Represents a 4-input OR-AND-INVERT logic gate.
 *
 * Inherits from ComplexCell and models a gate that performs an OR operation on the first three inputs, an AND operation 
 * with the fourth input, and then an INVERT operation on the result.
 */
class OAI4 : public ComplexCell {
public:

    /**
     * @brief Constructor for OAI4 cell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    OAI4(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
 * @class Mux
 * @brief Represents a 2:1 Multiplexer logic gate.
 *
 * Inherits from ComplexCell and models a Multiplexer that selects one of the two inputs to pass to the output based on a 
 * select signal.
 */
class Mux : public ComplexCell {
public:

    /**
     * @brief Constructor for Mux cell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    Mux(size_t identifier, std::string netlistName);

    /**
     * @brief find the value of the signals that gave the input number input_number as out
     * @param input_number Number of the input
     */
    int findValueSignal(int input_number);

    /**
     * @brief find the number of the port that will be use as out for the value of the signal
     * @param value_signal Value of the signal
     */
    int findPortNumber(int value_signal);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
 * @class Nmux
 * @brief Represents a 2:1 Multiplexer logic gate with an inverted output.
 *
 * Inherits from ComplexCell and models a Multiplexer that selects one of two inputs to pass to the output based on a
 * select signal, with the output being inverted.
 */
class Nmux : public ComplexCell {
public:

    /**
     * @brief Constructor for Nmux cell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    Nmux(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
 * @class Mux4
 * @brief Represents a 4-to-1 multiplexer with two select lines.
 *
 * This class inherits from the ComplexCell class and models a 4-to-1 multiplexer
 * with two select inputs determining which of the four data inputs is
 * connected to the output.
 */
class Mux4 : public ComplexCell {
public:

    /**
     * @brief Constructor for Mux4.
     * @param identifier The unique identifier for this multiplexer.
     */
    Mux4(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    /**
     * @brief find the value of the two signals that gave the input number input_number as out
     * @param input_number Number of the input
     */
    std::pair<int, int> findValueSignals(int input_number);

    /**
     * @brief find the number of the port that will be use as out for the value of the two signals
     * @param value_signal1 Value of the first signal
     * @param value_signal2 Value of the second signal
     */
    int findPortNumber(int value_signal1, int value_signal2);

    /**
     * @brief find a safe combination of signal in order to avoid aving input_wrong1 or input_wrong2 as out
     * @param input_wrong1 Value of the first wrong input
     * @param input_wrong2 Value of the second wrong input
     */
    std::pair<int, int> findSafeCombination(int input_wrong1, int input_wrong2);

    std::pair<int, int> findSafeCombination(int input_wrong);

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
 * @class Mux8
 * @brief Represents an 8-to-1 multiplexer with three select lines.
 *
 * This class inherits from the ComplexCell class and models an 8-to-1 multiplexer
 * with three select inputs determining which of the eight data inputs is
 * connected to the output.
 */
class Mux8 : public ComplexCell {
public:

    /**
     * @brief Constructor for Mux8.
     * @param identifier The unique identifier for this multiplexer.
     */
    Mux8(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
 * @class Mux16
 * @brief Represents a 16-to-1 multiplexer with four select lines.
 *
 * Inherits from the ComplexCell class and models a 16-to-1 multiplexer
 * with select inputs determining which one of the sixteen data inputs
 * is connected to the output.
 */
class Mux16 : public ComplexCell {
public:

    /**
     * @brief Constructor for Mux16.
     * @param identifier The unique identifier for this multiplexer.
     */
    Mux16(size_t identifier, std::string netlistName);

    /**
     * @brief Sets the value of the V select line.
     * @param val The value to set for the V select line.
     */
    void writeValueSignal4(int val);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};

/**
 * @class Tbuf
 * @brief Represents a tree-state buffer cell.
 *
 * This class models a tree-state buffer which passes the value of one input (A) to the output
 * when the enable signal (EN) is asserted. If enable is not asserted, the output is in high impedance state (Z).
 * Inherits from ComplexCell class.
 */
class Tbuf : public ComplexCell {
public:

    /**
     * @brief Constructs a Tbuf with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    Tbuf(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};
} // namespace Complexcell