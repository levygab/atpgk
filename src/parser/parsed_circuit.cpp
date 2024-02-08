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

#include "../../include/parser/parsed_circuit.hpp"

ParsedCircuit::ParsedCircuit() {}

void ParsedCircuit::getCircuitInfos() {
    std::cout << "========= Input port =========" << std::endl;

    for (const uint input : input_vector) {
        std::cout << input << " ; ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "======== Output port =========" << std::endl;

    for (const uint output : output_vector) {
        std::cout << output << " ; ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "=========== Wires ============" << std::endl;

    for (const uint wire : wire_vector) {
        std::cout << wire << " ; ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "=========== Gates ============" << std::endl;

    for (auto gate = full_gate_vector.begin(); gate != full_gate_vector.end(); gate++) {
        std::cout << gate->second.name << " : " << gate->second.id << " -> inputs : ";
        for (const std::tuple<uint, std::string> input : gate->second.in) {
            std::cout << "(wire: " << std::get<0>(input) << ", port: " << std::get<1>(input) << ") ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "======== Unary Gates =========" << std::endl;

    for (const Gate gate : unary_gate_vector) {
        std::cout << "Unary cell : " << gate.name << " -> " << gate.id << std::endl;
    }
    std::cout << std::endl;

    std::cout << "======== Binary Gates ========" << std::endl;

    for (const Gate gate : binary_gate_vector) {
        std::cout << "Binary cell : " << gate.name << " -> " << gate.id << std::endl;
    }
    std::cout << std::endl;

    std::cout << "======= Complex Gates ========" << std::endl;

    for (const Gate gate : complex_gate_vector) {
        std::cout << "Complex cell : " << gate.name << " -> " << gate.id << std::endl;
    }
    std::cout << std::endl;

    std::cout << "======== Memory Gates ========" << std::endl;

    for (const Gate gate : memory_gate_vector) {
        std::cout << "Memory cell : " << gate.name << " -> " << gate.id << std::endl;
    }
    std::cout << std::endl;

    std::cout << "====== Input association =====" << std::endl;

    for (const auto& assoc : input_port_mapping) {
        std::cout << assoc.first << " ; " << assoc.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "===== Output association =====" << std::endl;

    for (const auto& assoc : output_port_mapping) {
        std::cout << assoc.first << " ; " << assoc.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "======= Direct mapping =======" << std::endl;

    for (const auto& assoc : direct_port_pair_mapping) {
        std::cout << std::get<0>(assoc) << " (" << full_gate_vector.find(std::get<0>(assoc))->second.name << ")  -->  ";
        std::cout << "wire: " << std::get<2>(assoc) << "  -->  ";
        std::cout << std::get<1>(assoc) << " (" << full_gate_vector.find(std::get<1>(assoc))->second.name << " , port: ";
        std::cout << std::get<3>(assoc) << ")" << std::endl;
    }
    std::cout << std::endl;
}