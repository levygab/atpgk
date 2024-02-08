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
 * @file writer_json.hpp
 * @brief Definition of a Writer class for .json output files.
 */

#pragma once

#include "writer.hpp"

/**
 * @class WriterJSON
 * @brief Class representing a file writer for json format.
 */
class WriterJSON : public Writer {
    public:
        /**
         * @brief Default constructor for the WriterTXT class.
         */
        WriterJSON(std::string _filename, std::string _filetype);

        /**
         * @brief Add end of file '}' char
         * 
         */
        void endFile();

        /**
         * @brief Format the output file
         * 
         * Add the source file name and the timestanp of the generation
         * 
         * @param tree The circuit model tree
         */
        void formatFile(std::shared_ptr<Tree> tree) override;

        /**
         * @brief Write the input and output ports in the file
         * 
         * @param tree The circuit model tree
         */
        void writeIOPort(std::shared_ptr<Tree> tree) override;

        /**
         * @brief Write the vectors in the output file
         * 
         * @param vectors_test A shared pointer to the vector containing all the pair of input/output vectors
         */
        void writeVectors(std::shared_ptr<std::vector<std::pair<std::vector<std::pair<std::shared_ptr<Node>, int>> , std::vector<std::pair<std::shared_ptr<Node>, int>>>>> vectors_test) override;

        /**
         * @brief Write the coverage statistics in the dedicated file
         * 
         * @param faultCount A list that contains the number of each different fault and the number of Fault in failure
         * @param failureFault A list of all the fault that can't be tested
         */
        void writeCovStats(shared_ptr<vector<array<int, 2>>> faultCount, shared_ptr<vector<tuple<shared_ptr<Fault>, string, string>>> failureFault) override;
};