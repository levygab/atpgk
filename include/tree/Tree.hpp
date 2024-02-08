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
 * @file Tree.hpp
 * @brief Definition of the Tree class, representing a tree structure for a circuit at the gate level.
 */

#pragma once

#include <vector>
#include <memory>
#include "Node.hpp"

/**
 * @class Tree
 * @brief Represents a graph structure for a circuit at the gate level
 *
 * This class is used to construct and manage a graph representation of a circuit. 
 * It allows for the addition of nodes (representing gates or components) and 
 * facilitates operations on the circuit such as traversal and node retrieval.
 */
class Tree {
public:
    /**
     * @brief Name of the tree/circuit.
     */
    std::string name;

    /**
     * @brief Name of the source file
     * 
     */
    std::string srcName;

    /**
     * @brief List of all inputs to the circuit.
     */
    std::vector<std::shared_ptr<Node>> InputList;

    /**
     * @brief List of all nodes in the circuit.
     */
    std::vector<std::shared_ptr<Node>> NodeList;

    /**
     * @brief List of all outputs from the circuit.
     */
    std::vector<std::shared_ptr<Node>> OutputList;

    /**
     * @brief Constructor for Tree.
     * @param name Name of the tree/circuit
     */
    Tree(std::string name);

    /**
     * @brief Adds an input node to the circuit.
     * @param input Shared pointer to the Node representing an input.
     */
    void addInput(std::shared_ptr<Node> input);

    /**
     * @brief Adds a node to the circuit.
     * @param node Shared pointer to the Node to be added.
     */
    void addNode(std::shared_ptr<Node> node);

    /**
     * @brief Adds an output node to the circuit.
     * @param input Shared pointer to the Node representing an output.
     */
    void addOutput(std::shared_ptr<Node> input);

    /**
     * @brief Prints the identifier of each input node in the circuit.
     */
    void printInputId();

    /**
     * @brief Recursively prints the structure of the tree/circuit.
     */
    void printTree();

    /**
     * @brief Retrieves a node in the tree by its unique identifier.
     * @param identifier Unique identifier of the node to be retrieved.
     * @return Shared pointer to the requested Node.
     */
    std::shared_ptr<Node> getNodeByIdentifier(size_t identifier);

    /**
     * @brief Traverses the tree, visiting each node with the provided visitor.
     * @param visitor NodeVisitor object used to perform operations on each node.
     * @param top_fault_list Shared pointer to a vector of pairs, each containing a fault and the corresponding node.
     */
    void traverse(NodeVisitor& visitor, std::shared_ptr<std::vector<std::pair<std::shared_ptr<Fault>, std::shared_ptr<Node>>>> top_fault_list);

    void resetPortValue();

    void printPortValues();
};
