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
 * @file atpg_top.hpp
 * @brief Define the top class that contains the circuit model and call method to process it and generate test vectors
*/

#pragma once

#include <string>
#include <cstdlib>
#include <memory>
#include <filesystem>
#include <vector>
#include <tuple>
#include <array>

#include "../tree/Tree.hpp"
#include "../tree/Fault.hpp"
#include "../tree/FaultDecorator.hpp"
#include "../reader/reader.hpp"
#include "../writer/writer_txt.hpp"
#include "../writer/writer_json.hpp"
#include "../fault_API/fault_API.hpp"
#include "../utils/ANSI.hpp"

using namespace std;

/**
 * @class ATPGTop
 * @brief Top level class of the ATPG
 * 
 * It embbeds the circuit model and define all the method to process the model and generate vectors
*/
class ATPGTop {
    public:
        /**
         * @brief Shared pointer to the circuit model tree
        */
        shared_ptr<Tree> tree;

        /**
         * @brief Name of the input file
        */
        string filename;

        /**
         * @brief Extension of the input file
        */
        string extension_type;

        /**
         * @brief Name of the output file that constains the test vectors
        */
        string vect_output_filename;

        /**
         * @brief Name of the output coverage file
        */
        string cov_output_filename;

        /**
         * @brief Extension type of the output file that contains the test vectors
        */
        string vect_output_file_ext;

        /**
         * @brief Extension type of the output coverage file
        */
        string cov_output_file_ext;

        /**
         * @brief The list of fault to test in the circuit
         */
        shared_ptr<vector<pair<shared_ptr<Fault>, shared_ptr<Node>>>> fault_list;

        /**
         * @brief The list of vectors to test the circuit
         * @brief it is a pair of vectors. The first elem of the pair is a vector that contains pair of input node and the value to test. The second elem of the pair is a vectpr that contains pair of the output node and the expected value
         */
        shared_ptr<vector<pair<vector<pair<shared_ptr<Node>, int>> , vector<pair<shared_ptr<Node>, int>>>>> vectors_test;

        /**
         * @brief Path of the output directory
         * The default value is ./out/
         */
        string output_dir_path;

        /**
         * @brief The number of instances for each type of fault
         */
        shared_ptr<vector<array<int, 2>>> faultCount;

        /**
         * @brief The list of Fault that can't be tested (because of controlability or observability issues)
         */
        shared_ptr<vector<tuple<shared_ptr<Fault>, string, string>>> failureFault;

        /**
         * @brief Constructor of the ATPGTop class
        */
        ATPGTop();

        /**
         * @brief Method to initialize the output files name
        */
        void initialize();

        /**
         * @brief Call the reader to create the circuit model from the input file
        */
        void read();

        /**
         * @brief Decorate the circuit model tree with the fault to test
         * 
         */
        void decorate();
        
        /**
         * @brief Method to generate the test vectors
        */
        void generate_vector();

        /**
         * @brief Write the generated test vectors into the output file and format it
         * 
         */
        void write_vector();

        /**
         * @brief Generate the coverage statistics
         * 
         */
        void generate_cov_stats();

        /**
         * @brief Write the coverage statistics in the dedicated coverage file
         * 
         */
        void write_coverage();

    private:
        /**
         * @brief Instance of the Reader class to read the input file
         * 
         */
        Reader reader;

        /**
         * @brief Instance of the FaultDecorator class to add faults to the circuit model tree
         * 
         */
        FaultDecorator fault_decorator;

        /**
         * @brief Shared pointer to the Writer for the test vectors output file
         * 
         */
        shared_ptr<Writer> vectOutputFileWriter;

        /**
         * @brief Shared pointer to the Writer for the coverage output file
         * 
         */
        shared_ptr<Writer> covOutputFileWriter;

};