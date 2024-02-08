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
 * @file Output.hpp
 * @brief Definition of the Output class.
 */

#pragma once

#include <vector>
#include "Node.hpp"

/**
 * @class Output
 * @brief Represents an output node in a circuit model.
 * 
 * The Output class is a concrete subclass of the Node class, representing an
 * output node in a circuit model. It inherits common properties and methods
 * from the Node class and provides specific implementations where needed.
 */
class Output : public Node {
public:
    /**
     * @brief Constructor for the Output class.
     * @param identifier Unique identifier for the output node.
     */
    Output(size_t identifier, std::string _name, std::string netlistName);

    /**
     * @brief Prints information about the output node.
     */
    void printNodeRecursive() override;

    /**
     * @brief Computes the input nodes based on the output value.
     * @param value The value of the output node.
     * @return A vector of pairs representing input nodes and their corresponding input numbers.
     */
    std::vector<std::pair<std::shared_ptr<Node>, int>> virtual computeInputFromOutput(int value) override;

    /**
     * @brief Computes the expected value for a given port number.
     * @param value The value to compute the expected value for.
     * @param port_number The port number for which to compute the expected value.
     * @return A vector of tuples representing parent nodes, input numbers, and expected values.
     */
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    /**
     * @brief Gets a shared pointer to the current output node.
     * @return A shared pointer to the current output node.
     */
    std::shared_ptr<Node> getSharedPtrToThis() override;

    /**
     * @brief Get the Name of the Input
     * 
     * @return std::string 
     */
    std::string getName() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};
