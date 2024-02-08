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
 * @file reader.hpp
 * @brief Define a reader that call a parser and then build a circuit-model using a builder API
*/

#pragma once

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "../parser/yosys_json_parser.hpp"
#include "../builder_API/builder_API.hpp"
#include "../utils/ANSI.hpp"

using namespace BuilderAPI;

extern json strings;

/**
 * @class Reader
 * @brief Reads a netlist file and organizes the call to the parser and the builder API to create the circuit model
*/
class Reader {
    public:
        /**
         * @brief Constructor of the Reader
        */
        Reader(std::string filename, std::string extension);

        void read(std::string filename, std::string extension, std::shared_ptr<Tree> tree);
    
    private:
        shared_ptr<Parser> parser;
};