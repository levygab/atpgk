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
 * @file parser.hpp
 * @brief Definition of the Parser class.
 */

#pragma once

#include "parsed_circuit.hpp"
#include "../utils/ANSI.hpp"

/**
 * @class Parser
 * @brief Abstract base class representing a generic parser for circuit data.
 */
class Parser {
    protected:
        /**
         * @brief The content of the input file. 
         */
        std::string inputFileContent;

        /**
         * @brief Json structure for string management
         */
        json strings;

    public:
        /**
         * @brief Default constructor for Parser
         */
        Parser(json _strings);

        /**
         * @brief Pure virtual method to parse a circuit.
         *
         * This function is pure virtual and must be implemented by derived classes.
         * 
         * @return ParsedCircuit object containing parsed information about the circuit.
         */
        virtual ParsedCircuit parseCircuit() = 0;

        /**
         * @brief Pure virtual method to set the content of the input file inside the inputFileContent member
         * 
         * @param _inputFileContent The content of the input JSON file (from Yosys).
        */
        virtual void setInputFileContent(std::string _inputFileContent) = 0;
};