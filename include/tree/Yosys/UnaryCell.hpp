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
 * @file UnaryCell.hpp
 * @brief Definition of the UnaryCell class and all the sub-classes that inherits from it
*/

#pragma once

#include "YosysCell.hpp"

/**
 * @namespace Unarycell
 * @brief All the class related to Yosys unary cells (1 input)
*/
namespace Unarycell{

/**
* @class UnaryCell
* @brief Class representing a basic logic unary cell.
* 
* This class is a super-class for basic logic unary gates such as BUF, NOT, ...
* This class inherits from YosysCell
*/
class UnaryCell : public YosysCell {
public:
    /**
     * @brief Value from the parent input of the cell.
     */
    int valueParent;

    /**
     * @brief Constructs an UnaryCell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     * @param netlistName The name of the cell in the netlist
     */
    UnaryCell(size_t identifier, std::string netlistName);

};

/**
 * @class BufCell
 * @brief Class representing a buffer cell.
 *
 * Inherits from the UnaryCell class and encapsulates the behavior of a buffer gate.
 */
class BufCell : public UnaryCell {
public:
    /**
     * @brief Constructs a BufCell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    BufCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};


/**
 * @class NotCell
 * @brief Class representing a NOT logic cell.
 *
 * Inherits from the UnaryCell class and encapsulates the behavior of a NOT logic gate.
 */
class NotCell : public UnaryCell {
public:
    /**
     * @brief Constructs a NotCell with a unique identifier.
     * @param identifier The unique identifier for this NOT cell.
     */
    NotCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};
} // namespace Unarycell