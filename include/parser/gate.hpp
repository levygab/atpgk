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
 * @file gate.hpp
 * @brief Definition of the Gate class.
 */

#pragma once

#include <string>
#include <map>

/**
 * @class Gate
 * @brief Holding information of each gate at parse time
*/
class Gate {
public:
    /**
     * @brief Name type of the gate
    */
    std::string name;

    /**
     * @brief Name of the gate in the netlist
     */
    std::string netlistName;

    /**
     * @brief Unique ID of the gate
    */
    size_t id;

    /**
     * @brief Number of gate inputs
    */
    uint input_length = 0;

    /**
     * @brief Vector of all the gate input bits and their corresponding input port
    */
    std::map<uint, std::string> in;

    /**
     * @brief Gate output bit
     * @note Assuming there is only one output per gate
    */
    uint out;

    /**
     * @brief Default constructor of the GateStruct
     * 
     * @param _name The name of the gate
     * @param index Index of the gate
    */
    Gate(const std::string& _name, uint index, std::string _netlistName);
};