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
 * @file fault_API.hpp
 * @brief Define functions related to fault computation and error generation.
 */

#pragma once

#include <cstdlib>
#include <string>
#include "../tree/Node.hpp"
#include "../tree/Tree.hpp"
#include "../tree/Yosys/BinaryCell.hpp"
#include "../tree/Cell.hpp"
#include "../tree/Yosys/ComplexCell.hpp"
#include "../tree/Yosys/UnaryCell.hpp"
#include "../tree/Input.hpp"
#include "../tree/Output.hpp"
#include "../builder_API/builder_API.hpp"
#include "../utils/ANSI.hpp"

using namespace BuilderAPI;
using namespace Binarycell;
using namespace Complexcell;
using namespace Unarycell;

/**
 * @namespace FaultAPI
 * @brief Namespace providing functions related to fault computation and error generation.
 */
namespace FaultAPI {
    
    /**
     * @brief Computes the value of a node based on input vectors.
     * @param input_vector A vector of input nodes and their corresponding values.
     * @param output_vector A vector to store output nodes and their computed values.
     * @param node The node for which to compute the value.
     * @param value The value to compute for the node.
     * @param input_number The input number for which to compute the value.
     */
    void computeValue(std::vector<std::pair<shared_ptr<Node>, int>> &input_vector, std::vector<std::pair<shared_ptr<Node>, int>> &output_vector, shared_ptr<Node> node, int value, int input_number);

    /**
     * @brief Gets error vectors for a node based on a specific fault.
     * @param value The value to generate error vectors for.
     * @param tree The circuit model tree.
     * @param node The node for which to generate error vectors.
     * @param port_number The port number for which to generate error vectors.
     * @return A pair of vectors representing input and output nodes with error values.
     */
    std::pair<std::vector<std::pair<shared_ptr<Node>, int>>, std::vector<std::pair<shared_ptr<Node>, int>>> get_vector_error(int value, shared_ptr<Tree> tree, shared_ptr<Node> node, int port_number);

    /**
     * @brief Generates error vectors for a list of faults in the circuit model.
     * @param fault_list A list of faults in the circuit model.
     * @param tree The circuit model tree.
     * @return A vector of pairs, each containing input and output nodes with error values for a specific fault.
     */
    std::vector<std::pair<std::vector<std::pair<shared_ptr<Node>, int>>, std::vector<std::pair<shared_ptr<Node>, int>>>> generate_vector_error(std::shared_ptr<std::vector<std::pair<shared_ptr<Fault>, shared_ptr<Node>>>> fault_list, shared_ptr<Tree> tree);

    /**
     * @brief Computes all the mandatory list by calling the function compteMandatory for each node.
     * @param mandatory A vector that contains the node that must be compute.
     * @param optional A vector that contains the node that will be compute once the mandatory list is computed.
     * @return True if the mandatory list is computed without creating a conflict. False otherwise : means the error can not be tested
     */
    bool computeMandatoryList(std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional);

    /**
     * @brief Computes all the mandatory list by calling the function compteMandatory for each node.
     * @param mandatory A vector that contains the node that must be compute.
     * @param optional A vector that contains the node that will be compute once the mandatory list is computed.
     * @return True if it is working. False otherwise. 
     */
    bool computeOptionalFirstElem(std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional);

    /**
     * @brief Computes one test vector.
     * @param fault A pair containing a fault and the node where he fault must be tested.
     * @param tree the tree representing the circuit.
     * @param success a boolean that indicates if the error has been generated or if there is a conflict.
     * @return a pair of vector. One is representinf the value for the input, the other the value for the output 
     */
    std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>> generateOneVector(std::pair<shared_ptr<Fault>, shared_ptr<Node>>& fault, shared_ptr<Tree> tree, bool& success);

    /**
     * @brief function that generate all the test vector for a fault model.
     * @param fault_list a shared_ptr on a vector of tuple of the fault and the node where the fault must be tested
     * @param tree the tree representing the circuit.
     * @return the vector contains the tests vectors. 
     */
    std::vector<std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>>> generateVectorError(std::shared_ptr<std::vector<std::pair<shared_ptr<Fault>, shared_ptr<Node>>>> fault_list, shared_ptr<Tree> tree);
}
