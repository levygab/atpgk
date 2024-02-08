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
 * @file builder_API.hpp
 * @brief Define a builder with an API that can be called to create a circuit model.
*/

#pragma once

#include <cstdlib>
#include <string>
#include <memory>

#include "../tree/Tree.hpp"
#include "../tree/Input.hpp"
#include "../tree/Output.hpp"
#include "../tree/Yosys/UnaryCell.hpp"
#include "../tree/Yosys/BinaryCell.hpp"
#include "../tree/Yosys/ComplexCell.hpp"

using namespace std;
using namespace Binarycell;
using namespace Complexcell;
using namespace Unarycell;

/**
 * @namespace BuilderAPI
 * @brief Module providing an API to create and manipulate a circuit model tree.
*/
namespace BuilderAPI {

    /**
     * @brief Creates a new cell with the specified identifier and type.
     * @param id Unique identifier for the new cell.
     * @param type Type of the cell to be created.
     * @param netlistName The name of the cell in the netlist.
     * @return A shared pointer to the newly created cell node.
    */
    shared_ptr<Node> createNewCell(size_t id, string type, std::string netlistName);

    /**
     * @brief Adds an input node to the specified tree.
     * @param tree The target circuit model tree.
     * @param node The input node to be added.
    */
    void addInputToTree(shared_ptr<Tree> tree, shared_ptr<Node> node);

    /**
     * @brief Adds a node to the specified tree.
     * @param tree The target circuit model tree.
     * @param node The node to be added.
    */
    void addNodeToTree(shared_ptr<Tree> tree, shared_ptr<Node> node);

    /**
     * @brief Creates and adds a node to the specified tree with the given identifier and type.
     * @param tree The target circuit model tree.
     * @param id Unique identifier for the new node.
     * @param type Type of the node to be created.
     * @param netlistName The name of the cell in the netlist
    */
    void createAndAddNodeToTree(shared_ptr<Tree> tree, size_t id, string type, std::string netlistName);

    /**
     * @brief Prints information about the nodes in the specified tree.
     * @param tree The target circuit model tree.
    */
    void print_nodes(shared_ptr<Tree> tree);

    /*[gate1, gate2, wire, gate2_input_port] */
    /**
     * @brief Binds a cell to the tree by specifying the parent and child nodes and the connecting port.
     * @param id_parent Unique identifier of the parent node.
     * @param id_children Unique identifier of the child node.
     * @param port Port through which the child node is connected to the parent node.
     * @param tree The target circuit model tree.
    */
    void bind_cell(size_t id_parent, size_t id_children, string port, shared_ptr<Tree> tree);

}
