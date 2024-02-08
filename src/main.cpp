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

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <locale>

#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>

#include "../include/reader/reader.hpp"
#include "../include/atpg_top/atpg_top.hpp"
#include "../include/utils/ANSI.hpp"
#include "config.h"

// For convenience
namespace po = boost::program_options;
using json = nlohmann::json;

json strings;

// Main entrypoint of the kernel
int main(int argc, char* argv[]) {
    // Processing string management
    std::ifstream strings_file;

    std::locale current_locale("");
    std::string local_language = current_locale.name().substr(0,2);

    std::string root_path(PROJECT_ROOT_PATH);

    if (std::filesystem::exists(root_path+"/lang/"+local_language+".json")) {
        strings_file.open(root_path+"/lang/"+local_language+".json");
    } else {
        strings_file.open("../lang/en.json");
    }
    
    std::string stringify_file((std::istreambuf_iterator<char>(strings_file)), std::istreambuf_iterator<char>());
    strings = json::parse(stringify_file);

    // Check the correct usage of the command line
    if (argc < 2) {
        std::cout << strings["global"]["standard_usage"].get<std::string>() << "\n\n" << strings["global"]["option_message"].get<std::string>() << std::endl;
        return 1;
    }

    // Instantiate top level object
    ATPGTop top_level;

    // Processing cmd-line options management
    po::options_description desc("Allowed options");
    desc.add_options() 
        ("help,h", strings["options"]["help"].get<std::string>().c_str())
        ("version,v", strings["options"]["version"].get<std::string>().c_str())
        ("ext", po::value<std::string>(&top_level.extension_type)->default_value("json"), strings["options"]["ext"].get<std::string>().c_str())
        ("netlist,n", po::value<std::string>(&top_level.filename), strings["options"]["netlist"].get<std::string>().c_str())
        ("license", strings["options"]["license"].get<std::string>().c_str())
        ("output,o", po::value<std::string>(&top_level.vect_output_filename)->default_value("vectors"), strings["options"]["output"].get<std::string>().c_str())
        ("output-type,O", po::value<std::string>(&top_level.vect_output_file_ext)->default_value("txt"), strings["options"]["output_type"].get<std::string>().c_str())
        ("coverage,c", po::value<std::string>(&top_level.cov_output_filename)->default_value("coverage"), strings["options"]["coverage"].get<std::string>().c_str())
        ("cov-type,C", po::value<std::string>(&top_level.cov_output_file_ext)->default_value("txt"), strings["options"]["cov_type"].get<std::string>().c_str())
        ("out-path,p", po::value<std::string>(&top_level.output_dir_path)->default_value("./out/"), strings["options"]["output_dir_path"].get<std::string>().c_str())
    ;

    // To allow short './ATPG-Kernel <filename>' usage
    po::positional_options_description p;
    p.add("netlist", -1);

    // Reading cmd-line options
    try {
        std::cout << strings["global"]["header_message"].get<std::string>() << std::endl;
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        // Output help message and allowed options
        if (vm.count("help")) {
            std::cout << strings["global"]["standard_usage"].get<std::string>() << "\n\n" << desc << std::endl;
            return 0;
        } else if (vm.count("version")) {
            std::cout << strings["global"]["version"].get<std::string>() << std::endl;
            return 0;
        } else if (vm.count("license")) {
            std::ifstream license("../COPYING");
            if (!license.is_open()) {
                std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": " << strings["errors"]["license_file_opening"].get<std::string>() << std::endl;
                return 1;
            }
            std::string licenseString;
            while (std::getline(license, licenseString)){
                std::cout << licenseString << std::endl;
            }
            license.close();
            return 0;
        }
    } catch (boost::wrapexcept<boost::program_options::unknown_option> e) {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": " << strings["global"]["unknown_option"].get<std::string>() << "\n\n" << strings["global"]["option_message"].get<std::string>() << std::endl;
        return 1;
    } catch (boost::wrapexcept<boost::program_options::ambiguous_option> e) {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": " << strings["global"]["unknown_option"].get<std::string>() << "\n\n" << strings["global"]["option_message"].get<std::string>() << std::endl;
        return 1;
    }

    // TODO: Vérifier si chaque appel passe et adapter le texte en fonction (success ou echec) -> a faire aussi dans le reader

    std::cout << CYAN_TEXT << BOLD_TEXT << "\nInfo" << RESET_TEXT << ": " << strings["global"]["kernel_init"].get<std::string>() << std::endl;
    // Initialize the files' name and extensions
    top_level.initialize();
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["kernel_init_success"].get<std::string>() << RESET_TEXT << std::endl;

    // Call the reader
    top_level.read();

    std::cout << CYAN_TEXT << BOLD_TEXT << "\nInfo" << RESET_TEXT << ": " << strings["global"]["tree_decoration"].get<std::string>() << std::endl;
    // Decorate the circuit model tree with the fault to test
    top_level.decorate();
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["tree_decoration_success"].get<std::string>() << RESET_TEXT << std::endl;

    std::cout << CYAN_TEXT << BOLD_TEXT << "\nInfo" << RESET_TEXT << ": " << strings["global"]["vector_generation"].get<std::string>() << std::endl;
    // Generate the test vectors
    top_level.generate_vector();
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["vector_generation_success"].get<std::string>() << RESET_TEXT << std::endl;

    std::cout << CYAN_TEXT << BOLD_TEXT << "\nInfo" << RESET_TEXT << ": " << strings["global"]["vector_writing"].get<std::string>() << std::endl;
    // Write the vector output file
    top_level.write_vector();
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["vector_writing_success"].get<std::string>() << RESET_TEXT << std::endl;

    std::cout << CYAN_TEXT << BOLD_TEXT << "\nInfo" << RESET_TEXT << ": " << strings["global"]["cov_stat_generation"].get<std::string>() << std::endl;
    // Generate the coverage statistics
    top_level.generate_cov_stats();
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["cov_stat_generation_success"].get<std::string>() << RESET_TEXT << std::endl;

    std::cout << CYAN_TEXT << BOLD_TEXT << "\nInfo" << RESET_TEXT << ": " << strings["global"]["cov_writing"].get<std::string>() << std::endl;
    // Write the coverage output file
    top_level.write_coverage();
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["cov_writing_success"].get<std::string>() << RESET_TEXT << std::endl;

    // Program executed successfully
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["programm_success"].get<std::string>() << RESET_TEXT << std::endl;

    return 0;
}
