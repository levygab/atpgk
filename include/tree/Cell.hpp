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
 * @file Cell.hpp
 * @brief Definition of the generic Cell class
*/

#pragma once

#include "Node.hpp"

/**
 * @class Cell
 * @brief Class that represents a generic cell of the circuit.
 * 
 * The Cell class is derived from the Node class and represents a specific type of node in a circuit.
 */

class Cell : public Node {
public:
    /**
     * @brief Constructor for the Cell class.
     *
     * Constructs a Cell with a unique identifier. Calls the constructor of the Node base class.
     * @param identifier An integer uniquely identifying the cell, passed to the base Node constructor.
     * @param netlistName The name of the cell in the netlist
     */
    Cell(size_t identifier, std::string netlistName);

    /**
     * @brief return all the available inputs for a given output
    */
    virtual std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) = 0;

    /**
     * @brief Get the Name of the Cell
     * 
     * @return std::string 
     */
    std::string getName() override;
};