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

#include "../../include/reader/reader.hpp"

Reader::Reader(std::string filename, std::string extension) {
    // This allow definition of other kind of parsers
    if (extension == "json") {
        this->parser = make_shared<YosysJSONParser>(strings);
    } else {
        this->parser = make_shared<YosysJSONParser>(strings);
    }
}

void Reader::read(std::string filename, std::string extension, std::shared_ptr<Tree> tree) {
    
    std::cout << CYAN_TEXT << BOLD_TEXT << "\nInfo" << RESET_TEXT << ": " << strings["global"]["parsing"].get<std::string>() << std::endl;
    
    // Open the file
    std::ifstream file(filename);

    // Check if the file exists
    try {
        if (!file.is_open()) {
            throw std::runtime_error("");
        }
    } catch (std::runtime_error e) {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": file '" + filename + "' does not exist" << std::endl;
        exit(1);
    }

    // Read the json file and store it in a string
    const std::string fileString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Close the file
    file.close();
    
    // Give the file content to the parser
    parser->setInputFileContent(fileString);

    // Instanciate a ParsedCircuit
    ParsedCircuit netlist;

    // Parsing the string
    netlist = parser->parseCircuit();

    // TODO: Supprimer les print en prod
    // Getting infos from the parsed circuit
    //netlist.getCircuitInfos();
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["parsing_success"].get<std::string>() << RESET_TEXT << std::endl;

    std::cout << CYAN_TEXT << BOLD_TEXT << "\nInfo" << RESET_TEXT << ": " << strings["global"]["tree_building"].get<std::string>() << std::endl;

    // Create and add all the nodes to the circuit model
    for (auto gate = netlist.full_gate_vector.begin(); gate != netlist.full_gate_vector.end(); gate++) {
            size_t id = gate->second.id;
            std::string type = gate->second.name;
            std::string netlistName = gate->second.netlistName;
            createAndAddNodeToTree(tree, id, type, netlistName);
        }

    // Binding all the node
    for (const auto& assoc : netlist.direct_port_pair_mapping) {
        size_t id1 = std::get<0>(assoc);
        size_t id2 = std::get<1>(assoc);
        std::string port = std::get<3>(assoc);

        bind_cell(id1, id2, port, tree);
    }

    // TODO: Supprimer les print en prod
    // Getting infos from the circuit model tree
    //print_nodes(tree);
    std::cout << GREEN_TEXT << BOLD_TEXT << strings["global"]["tree_building_success"].get<std::string>() << RESET_TEXT << std::endl;
}