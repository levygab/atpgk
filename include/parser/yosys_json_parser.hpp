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
 * @file yosys_json_parser.hpp
 * @brief Definition of the YosysJSONParser class.
 */

#pragma once

#include <nlohmann/json.hpp>

#include "parser.hpp"
#include "yosys_gate_level_cells.hpp"

using namespace YosysBasicGateLevelCells;

/**
 * @class YosysJSONParser
 * @brief Concrete class representing a parser for Yosys JSON format, inheriting from @link Parser @endlink.
 */
class YosysJSONParser : public Parser {
    public:
        /**
         * @brief Default constructor for the YosysJSONParser class.
         */
        YosysJSONParser(json _strings);

        /**
         * @brief Overriden method to set the content of the input file inside the inputFileContent member
         * 
         * @param _inputFileContent The content of the input JSON file (from Yosys).
        */
        void setInputFileContent(std::string _inputFileContent) override;
    
        /**
         * @brief Overridden method to parse an Yosys JSON netlist.
         *
         * This function parses the content of the Yosys JSON file contained in the @link inputFileContent @endlink member and returns a ParsedCircuit object.
         * 
         * @return ParsedCircuit object containing parsed information about the circuit.
         */
        ParsedCircuit parseCircuit() override;
};