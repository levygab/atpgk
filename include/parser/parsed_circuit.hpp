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
 * @file parsed_circuit.hpp
 * @brief Definition of the ParsedCircuit class.
 */

#pragma once

#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <regex>
#include <cstdlib>
#include <iostream>

#include <nlohmann/json.hpp>

#include "gate.hpp"

// For convenience
using string = std::string;
using json = nlohmann::json;

/**
 * @class ParsedCircuit
 * @brief Holding informations about the whole circuit at parse time
*/
class ParsedCircuit {
public:
    /**
     * @brief Vector of all the input bits of the circuit
    */
    std::vector<uint> input_vector;

    /**
     * @brief Vector of all the output bits of the circuit
    */
    std::vector<uint> output_vector;

    /**
     * @brief Vector of all the wire (bits) of the circuit
     * @note Each wire represents a bit and have a unique integer index
    */
    std::vector<uint> wire_vector;

    /**
     * @brief Vector of all the gates of the circuit
    */
    std::map<size_t, Gate> full_gate_vector;

    /**
     * @brief Vector of all the memory gates of the circuit
     * @note This vector includes gates such as flip-flop and latches
    */
    std::vector<Gate> memory_gate_vector;

    /**
     * @brief Vector of all the unary gates of the circuit
    */
    std::vector<Gate> unary_gate_vector;

    /**
     * @brief Vector of all the binary gates of the circuit
    */
    std::vector<Gate> binary_gate_vector;

    /**
     * @brief Vector of all the complex gates of the circuit
     * @note This includes multiplexers, tristate and multiple input gates
    */
    std::vector<Gate> complex_gate_vector;

    /**
     * @brief Mapping between input bits and gates
    */
    std::vector<std::pair<uint, size_t>> input_port_mapping;

    /**
     * @brief Mapping between output bits and gates
    */
    std::vector<std::pair<uint, size_t>> output_port_mapping;

    /**
     * @brief Vector of association between two connected gates
     * @note Each tuple of the vector contains the two gates' id and a bit, that connects the first gate to the second one
     * @note {[gate1, gate2, wire, gate2_input_port], [gate1, gate2, wire, gate2_input_port], ... }
    */
    std::vector<std::tuple<size_t, size_t, uint, std::string>> direct_port_pair_mapping;

    /**
     * @brief Default constructor of the ParsedCircuit
    */
    ParsedCircuit();

    /**
     * @brief Get infos about the parsed circuit
    */
    void getCircuitInfos();
};