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
 * @file NodeVisitor.hpp
 * @brief Definition of the generic NodeVisitor class
*/

#pragma once

#include <memory>
#include <vector>

#include "Fault.hpp"

using namespace FaultModel;

/**
 * @brief Forward declaration of class Node
 */
class Node;

/**
 * @class NodeVisitor
 * @brief A generic visitor for the node class.
 * 
 * The NodeVisitor class provides a generic interface for visitors to traverse and
 * perform operations on nodes in a tree structure. Subclasses of NodeVisitor should
 * implement the visit method to define the specific behavior for visiting a node.
 */
class NodeVisitor {
public:

    /**
     * @brief Virtual method to visit a Node.
     * 
     * This method should be overridden by subclasses to define the specific
     * behavior when visiting a node in a tree.
     * 
     * @param node A shared pointer to the node being visited.
     * @param top_fault_list A shared pointer to a vector of pairs representing
     *                      faults associated with nodes in the tree.
     */
    virtual void visit(std::shared_ptr<Node> node, std::shared_ptr<std::vector<std::pair<std::shared_ptr<Fault>, std::shared_ptr<Node>>>> top_fault_list) = 0;
};
