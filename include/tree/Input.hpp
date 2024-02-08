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
 * @file Input.hpp
 * @brief Definition of the Input class.
*/

#pragma once

#include <vector>

#include "Node.hpp"

/**
 * @class Input
 * @brief A class representing an input node in a circuit model.
 * 
 * Input is a subclass of Node and represents a node that serves as an input
 * to a circuit model. It defines methods for printing the node details, receiving
 * values, sending values to children, and computing input from output, among others.
 */
class Input : public Node {
public:
    /**
     * @brief Constructs an Input node with a specified identifier.
     * 
     * @param identifier The unique identifier for this Input node.
     */
    Input(size_t identifier, std::string _name, std::string netlistName);

    // Methods

    /**
     * @brief Prints details of the Input node.
     */
    void printNodeRecursive() override; 

    /**
     * @brief Computes input from output for the Input node.
     * 
     * @param value The value of the output of the node to compute.
     * @return A vector of pairs of nodes and integers representing a combination
     *         for the parent node to achieve the desired output.
     */
    std::vector<std::pair<std::shared_ptr<Node>, int>> virtual computeInputFromOutput(int value) override;

    /**
     * @brief Computes the expected value for a specific port.
     * 
     * @param value The value of the output of the node.
     * @param port_number The port number for which to compute the expected value.
     * @return A vector of tuples representing the expected values for the parent
     *         node to achieve the desired output at the specified port.
     */
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    /**
     * @brief Gets a shared pointer to this Input node.
     * 
     * @return A shared pointer to this Input node.
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
