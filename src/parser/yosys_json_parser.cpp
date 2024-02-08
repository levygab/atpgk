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

#include "../../include/parser/yosys_json_parser.hpp"

YosysJSONParser::YosysJSONParser(json _strings) : Parser(_strings) {};

void YosysJSONParser::setInputFileContent(std::string _inputFileContent) {
    this->inputFileContent = _inputFileContent;
}

ParsedCircuit YosysJSONParser::parseCircuit() {
    // Parsing string
    const json &json_netlist = json::parse(this->inputFileContent);

    ParsedCircuit parsedNetlist;

    uint gate_index = 0;

    if (json_netlist["modules"].size() > 1) {
        std::cout << ORANGE_TEXT << BOLD_TEXT << "Warning" << RESET_TEXT << " : " << this->strings["warnings"]["multi_module_def"].get<std::string>() << std::endl;
    }
    
    // Going through modules
    for (const json &module : json_netlist["modules"]) {

        // Iterating through ports
        for (auto port = module["ports"].begin(); port != module["ports"].end(); ++port) {
            try {
                const string &name = port.key();
                const string &direction = port.value().find("direction").value();
                const int &bit = port.value().find("bits").value()[0];

                if (direction == "input") {
                    parsedNetlist.input_vector.push_back(bit);
                    gate_index++;
                    Gate input("Input "+name, gate_index, name);
                    input.out = bit;
                    parsedNetlist.full_gate_vector.insert({input.id, input});
                    parsedNetlist.output_port_mapping.push_back({bit, input.id});
                } else if (direction == "output") {
                    parsedNetlist.output_vector.push_back(bit);
                    gate_index++;
                    Gate output("Output "+name, gate_index, name);
                    output.in.insert({bit, "A"});
                    parsedNetlist.full_gate_vector.insert({output.id, output});
                    parsedNetlist.input_port_mapping.push_back({bit, output.id});
                } else {
                    throw std::runtime_error("");
                }
            } catch (std::runtime_error e) {
                std::cerr << "Port definition error" << std::endl;
                exit(1);
            }
        }

        // Iterating through wires
        for (const json &wire : module["netnames"]) {
            parsedNetlist.wire_vector.push_back(wire["bits"][0]);
        }

        // Iterating through cells
        for (auto cell_json = module["cells"].begin(); cell_json != module["cells"].end(); ++cell_json) {
            try {
                // Define regex pattern for memory cells
                std::regex memoryPattern("\\$\\_(S?DFF|DLATCH|SR).*_$", std::regex::icase);

                // Creating new instance of gate
                Gate cell = Gate(cell_json.value().find("type").value(), gate_index++, cell_json.key());

                // Filling the input and output of the gate 
                for (auto &port : cell_json.value().find("port_directions").value().items()) {
                    if (port.value() == "output") {
                        cell.out = cell_json.value().find("connections").value()[port.key()][0];
                        parsedNetlist.output_port_mapping.push_back({cell_json.value().find("connections").value()[port.key()][0], cell.id});
                    } else if (port.value() == "input") {
                        cell.input_length ++;
                        cell.in.insert({cell_json.value().find("connections").value()[port.key()][0], port.key()});
                        parsedNetlist.input_port_mapping.push_back({cell_json.value().find("connections").value()[port.key()][0], cell.id});
                    }
                }

                parsedNetlist.full_gate_vector.insert({cell.id, cell});
                
                // Check if cell is a memory cell
                if (std::regex_match(cell.name, memoryPattern)) {
                    parsedNetlist.memory_gate_vector.push_back(cell);
                }
                
                // Check if cell is an unary gate
                else if (unaryCells.find(cell.name) != unaryCells.end()) {
                    parsedNetlist.unary_gate_vector.push_back(cell);
                }
                
                // Check if cell is a binary gate
                else if (binaryCells.find(cell.name) != binaryCells.end()) {
                    parsedNetlist.binary_gate_vector.push_back(cell);
                }
                
                // Check if cell is a complex gate
                else if (complexCells.find(cell.name) != complexCells.end()) {
                    parsedNetlist.complex_gate_vector.push_back(cell);
                }
                
                // Default if gate type is unknown
                else {
                    throw std::runtime_error("");
                }
            } catch (std::runtime_error e) {
                std::cerr << "Gate definition error" << std::endl;
                exit(1);
            }
        }
    }

    // Filling the direct gate mapping vector
    for (const auto& input : parsedNetlist.input_port_mapping) {
        for (const auto& output : parsedNetlist.output_port_mapping) {
            if (input.first == output.first) {
                parsedNetlist.direct_port_pair_mapping.push_back({output.second, input.second, input.first, parsedNetlist.full_gate_vector.find(input.second)->second.in.find(input.first)->second});
            }
        }
    }

    // for convinience
    std::sort(parsedNetlist.input_vector.begin(), parsedNetlist.input_vector.end());
    std::sort(parsedNetlist.output_vector.begin(), parsedNetlist.output_vector.end());
    std::sort(parsedNetlist.wire_vector.begin(), parsedNetlist.wire_vector.end());

    return parsedNetlist;
}