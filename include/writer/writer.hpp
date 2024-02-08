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
 * @file writer.hpp
 * @brief Definition of the Writer class.
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <chrono>
#include <ctime>
#include <map>

#include "../tree/Tree.hpp"

using namespace std;

/**
 * @class Writer
 * @brief Abstract base class representing a generic file writer.
 */
class Writer {
    protected:
         /**
         * @brief The extension type of the file to write. 
         */
         std::string fileType;

         /**
         * @brief The name of the file to write.
         */
         std::string fileName;

         /**
         * @brief Output file stream
         */
         std::ofstream file;
      

    public:
        /**
         * @brief Construct a new Writer object
         * 
         * @param _filename the name of the file
         * @param _filetype the extension type of the output file
         */
        Writer(std::string _filename, std::string _filetype);

        /**
         * @brief Destroy the Writer object
         * 
         */
        ~Writer();

        /**
         * @brief Method to add a line to the file
         * 
         * @param _lineContent The content of the line to add
        */
        void addLineToFile(std::string _lineContent);

        /**
         * @brief Pure virtual method to format the output file
         * 
         * Add the source file name and the timestanp of the generation
         * 
         * @param tree The circuit model tree
         */
        virtual void formatFile(std::shared_ptr<Tree> tree) = 0;

        /**
         * @brief Pure virtual method to write the input and output ports in the file
         * 
         * @param tree The circuit model tree
         */
        virtual void writeIOPort(std::shared_ptr<Tree> tree) = 0;

        /**
         * @brief Pure virtual method to write the vectors in the output file
         * 
         * @param vectors_test A shared pointer to the vector containing all the pair of input/output vectors
         */
        virtual void writeVectors(std::shared_ptr<std::vector<std::pair<std::vector<std::pair<std::shared_ptr<Node>, int>> , std::vector<std::pair<std::shared_ptr<Node>, int>>>>> vectors_test) = 0;

        /**
         * @brief Write the coverage statistics in the dedicated file
         * 
         * @param faultCount A list that contains the number of each different fault and the number of Fault in failure
         * @param failureFault A list of all the fault that can't be tested
         */
        virtual void writeCovStats(shared_ptr<vector<array<int, 2>>> faultCount, shared_ptr<vector<tuple<shared_ptr<Fault>, string, string>>> failureFault) = 0;
};