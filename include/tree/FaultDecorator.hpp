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
 * @file FaultDecorator.hpp
 * @brief Defines the FaultDecorator class, which is responsible for decorating nodes with faults for testing.
 */

#pragma once

#include "NodeVisitor.hpp"

using namespace FaultModel;

/**
 * @class FaultDecorator
 * @brief Decorates nodes with faults for testing purposes.
 *
 * FaultDecorator is a subclass of NodeVisitor. It is used to traverse through nodes in a system and
 * decorate them with faults, facilitating the testing of these nodes under various fault conditions.
 */
class FaultDecorator : public NodeVisitor {
public:
    /**
     * @brief Constructs a new Fault Decorator object.
     *
     * Initializes a FaultDecorator instance, setting up any necessary state or dependencies.
     */
    FaultDecorator();
    
    /**
     * @brief Visit the node and decorate it with faults for testing.
     * 
     * This method overrides the visit method from NodeVisitor. It is called for each node in the system,
     * and it associates the node with a list of faults that need to be tested. This method is a key
     * part of the fault injection testing process, allowing faults to be systematically applied to nodes.
     *
     * @param node A shared pointer to the Node that is being visited and decorated.
     * @param top_fault_list A shared pointer to a vector of pairs. Each pair contains a shared pointer
     *                       to a Fault and a shared pointer to the Node associated with that Fault.
     *                       This list represents the faults to be tested on the node.
     */
    void visit(std::shared_ptr<Node> node, std::shared_ptr<std::vector<std::pair<std::shared_ptr<Fault>, std::shared_ptr<Node>>>> top_fault_list) override;
};
