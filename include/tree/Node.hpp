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
 * @file Node.hpp
 * @brief Definition of the Node class, which serves as a base class for various types of nodes in a circuit.
 */

#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Fault.hpp"
#include "../utils/ANSI.hpp"

using namespace FaultModel;

class NodeVisitor;

/**
 * @class Node
 * @brief Represents a generic node in a circuit.
 *
 * This class serves as the base class for all types of nodes within a circuit. It contains common properties
 * and methods relevant to all nodes, such as identification, connections, and fault coverage.
 */
class Node {
public:
    /**
     * @brief Unique identifier for each node.
     */
    size_t Identifier;

    /**
     * @brief Children of the node.
     * 
     * Vector of pairs, each containing a shared pointer to a child node and an integer representing 
     * the input of the child to which this node is connected.
     */
    std::vector<std::pair<std::shared_ptr<Node>, int>> children;

    /**
     * @brief Parents of the node.
     * 
     * Vector of pairs, each containing a shared pointer to a parent node and an integer representing 
     * the input of this node to which the parent is connected.
     */
    std::vector<std::pair<std::shared_ptr<Node>, int>> parents;

    /**
     * @brief List of faults associated with the node for coverage analysis.
     */
    std::vector<std::shared_ptr<Fault>> fault_list;

    /**
     * @brief The output value of the node.
     */
    int value;

    /**
     * @brief Type of the node, typically indicating its functionality.
     */
    std::string type;

    /**
     * @brief The name of the node in the netlist
     * 
     * Replace by the name of the port if the node is an input or an output
     */
    std::string netlistName;

    /**
     * @brief Constructs a Node with a specified unique identifier.
     * @param identifier Unique identifier for the node.
     * @param netlistName The name of the gate in the netlist
     */
    Node(size_t identifier, std::string netlistName);

    /**
     * @brief Retrieves the unique identifier of the node.
     * @return The unique identifier of the node as an integer.
     */
    size_t getIdentifier();

    /**
     * @brief Adds a child node to this node's list of children.
     * @param child A pointer to the child `Node` to be added.
     * @param number The input number of the child node to which this node is connected.
     */
    void addChildren(std::shared_ptr<Node> child, int number);

    /**
     * @brief Adds a parent node to this node's list of parents.
     * @param parent A pointer to the parent `Node` to be added.
     * @param number The input number of the parent node to which this node is connected.
     */
    void addParent(std::shared_ptr<Node> parent, int number);

    /**
     * @brief Retrieves a node by its unique identifier within the tree structure.
     * @param identifier The unique identifier of the desired node.
     * @return A pointer to the `Node` with the corresponding identifier.
     */
    std::shared_ptr<Node> getNodeById(size_t identifier);

    /**
     * @brief Virtual function for printing the node. Must be implemented by derived classes.
     */
    virtual void printNodeRecursive() = 0;

    /**
     * @brief Destructor for the Node class.
     */
    virtual ~Node() {}

    /**
     * @brief Function to print some specificities of the node.
     */
    void printNodeAndNeighbor();

    /**
     * @brief Set the coverage flag to false, indicating that all faults have not already been covered.
     */
    void setUncovered();

    /**
     * @brief Set the coverage flag to true, indicating that all faults have already been covered.
     */
    void setCovered();

    /**
     * @brief Getter for the coverage flag.
     * @return The coverage flag.
     */
    bool getCoverageFlag();

    /**
     * @brief Virtual function for computing the input from the output. Must be implemented by derived classes.
     * @param value The value of the output of the node to compute.
     * @return A vector of pairs of nodes and integers. In the pair, the integers represent the value the node is supposed to have in order to get the expected value as an output.
     */
    virtual std::vector<std::pair<std::shared_ptr<Node>, int>> computeInputFromOutput(int value) = 0;

    /**
     * @brief Virtual function for computing the expected value. Must be implemented by derived classes.
     * @param value The value of the output of the node to compute.
     * @param port_number The port number for which to compute the expected value.
     * @return A vector of tuples of nodes, integers, and port numbers representing the expected values.
     */
    virtual std::vector<std::tuple<std::shared_ptr<Node>, int, int>> computeExpectedValue(int value, int port_number) = 0;

    /**
     * @brief Retrieves the parent node associated with a specific input number.
     * @param input_number The input number for which to find the parent node.
     * @return A shared pointer to the parent Node.
     */
    std::shared_ptr<Node> getParentFromInputNumber(int input_number);

    /**
     * @brief Virtual function to get a shared pointer to this node. Must be implemented by derived classes.
     * @return A shared pointer to this node.
     */
    virtual std::shared_ptr<Node> getSharedPtrToThis() = 0;

    /**
     * @brief Accepts a visitor to perform operations on this node.
     * @param visitor The visitor that performs operations on this node.
     * @param top_fault_list A list of faults and corresponding nodes to be used by the visitor.
     */
    void accept(NodeVisitor& visitor, std::shared_ptr<std::vector<std::pair<std::shared_ptr<Fault>, std::shared_ptr<Node>>>> top_fault_list);

    /**
     * @brief Get the Name of the Node
     * 
     * @return std::string 
     */
    virtual std::string getName() = 0;

    /**
     * @brief Get the Value From a port number
     * 
     * @param port_number the port number whom we want the value
     * @return the value of the port_number. If it is -1, it means that the port doesn't have any_value for the moment
     */
    int getValueFromPort(int port_number);

    std::shared_ptr<Node> getNodeFromPort(int port_number);

    void updatePort(int port_number, int value);

    /**
     * @brief Get the Value From a port number
     * 
     * @return the number of port that have been computed
     */
    int numberOfComputePort();
    /**
     * @param mandatory : vector of tuple of node, port_number, value, boolean
        * first member of the tuple : node where we must compute
        * second member of the tuple : the port number we must compute
        * third member of the tuple : the value we must compute
        * fourth member of the tuple : boolean that indicates if the value must be propagated or not
    
    */
    virtual bool computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) = 0;

    /**
     * @param mandatory : vector of tuple of node, port_number, value, boolean
        * first member of the tuple : node where we must compute
        * second member of the tuple : the port number we must compute
        * third member of the tuple : the value we must compute
        * fourth member of the tuple : boolean that indicates if the value must be propagated or not
    
    */
    bool computeOptional(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional);
    
    /**
     * @brief Reset the value of portValues to their origin
     * 
     */
    void resetPortValue();

    /**
     * @brief The value of each port of the node.
     * the first int represent the port number : if it is -1, it means that the value is the output of the node.
     * the second int represent the value at this port
     * The boolean value indicates if the value has already boon compute or not
     */
    std::vector<std::tuple<int, int, bool>> portValues;
    
private:
    /**
     * @brief Boolean flag indicating whether all the faults have been covered or not.
     */
    bool covered;

protected:
    /**
     * @brief Name of the Node
     * 
     */
    std::string name;

    void removeFromOptionial(std::shared_ptr<Node> node, int port_number, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional);

};
