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
 * @file BinaryCell.hpp
 * @brief Definition of the BinaryCell class and all the sub-classes that inherits from it
*/

#pragma once

#include "YosysCell.hpp"

/**
 * @namespace Binarycell
 * @brief All the class related to Yosys binary cells (2 inputs)
*/
namespace Binarycell{

/**
* @class BinaryCell
* @brief Class representing a basic logic binary cell.
* 
* This class is a super-class for basic logic binary gates such as AND, OR, ...
* This class inherits from YosysCell
*/
class BinaryCell : public YosysCell {
public:

    /**
     * @brief Constructs a BinaryCell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    BinaryCell(size_t identifier, std::string netlistName);

};

/**
* @class AndCell
* @brief Class representing an AND logic cell.
*
* Inherits from the BinaryCell class and encapsulates the behavior of an AND logic gate.
*/
class AndCell : public BinaryCell {
public:
    /**
     * @brief Constructs an AndCell with a unique identifier.
     * @param identifier Unique identifier for this cell.
     */
    AndCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};


/**
 * @class OrCell
 * @brief Class representing an OR logic cell.
 *
 * Inherits from the BinaryCell class and encapsulates the behavior of an OR logic gate.
 */
class OrCell : public BinaryCell {
public:
    /**
     * @brief Constructs an OrCell with a unique identifier.
     * @param identifier The unique identifier for this OR cell.
     */
    OrCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};


/**
 * @class NandCell
 * @brief Class representing a NAND logic cell.
 *
 * Inherits from the BinaryCell class and encapsulates the behavior of a NAND logic gate.
 */
class NandCell : public BinaryCell {
public:
    /**
     * @brief Constructs a NandCell with a unique identifier.
     * @param identifier The unique identifier for this NAND cell.
     */
    NandCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};


/**
 * @class NorCell
 * @brief Class representing a NOR logic cell.
 *
 * Inherits from the BinaryCell class and encapsulates the behavior of a NOR logic gate.
 */
class NorCell : public BinaryCell {
public:
    /**
     * @brief Constructs a NorCell with a unique identifier.
     * @param identifier The unique identifier for this NOR cell.
     */
    NorCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};


/**
 * @class XorCell
 * @brief Class representing an XOR logic cell.
 *
 * Inherits from the BinaryCell class and encapsulates the behavior of an XOR logic gate.
 */
class XorCell : public BinaryCell {
public:
    /**
     * @brief Constructs an XorCell with a unique identifier.
     * @param identifier The unique identifier for this XOR cell.
     */
    XorCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};


/**
 * @class XnorCell
 * @brief Class representing an XNOR logic cell.
 *
 * Inherits from the BinaryCell class and encapsulates the behavior of an XNOR logic gate.
 */
class XnorCell : public BinaryCell {
public:
    /**
     * @brief Constructs an XnorCell with a unique identifier.
     * @param identifier The unique identifier for this XNOR cell.
     */
    XnorCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};


/**
 * @class OrnotCell
 * @brief Class representing an ORNOT logic cell.
 *
 * Inherits from the BinaryCell class and encapsulates the behavior of an ORNOT logic gate.
 * @note The ORNOT gate is a hypothetical gate that gives the result of OR operation followed by NOT.
 */
class OrnotCell : public BinaryCell {
public:
    /**
     * @brief Constructs an OrnotCell with a unique identifier.
     * @param identifier The unique identifier for this ORNOT cell.
     */
    OrnotCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};


/**
 * @class AndnotCell
 * @brief Class representing an ANDNOT logic cell.
 *
 * Inherits from the BinaryCell class and encapsulates the behavior of an ANDNOT logic gate
 * @note This gate performs an AND operation of the first parent with the negation of the second parent.
 */
class AndnotCell : public BinaryCell {
public:
    /**
     * @brief Constructs an AndnotCell with a unique identifier.
     * @param identifier The unique identifier for this ANDNOT cell.
     */
    AndnotCell(size_t identifier, std::string netlistName);

    void printNodeRecursive() override;

    std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) override;

    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) override;

    std::shared_ptr<Node> getSharedPtrToThis() override;

    bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) override;
};
} // namespace Binarycell