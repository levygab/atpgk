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

#include "../../include/tree/FaultDecorator.hpp"
#include "../../include/tree/Node.hpp"

FaultDecorator::FaultDecorator() {};

void FaultDecorator::visit(std::shared_ptr<Node> node, std::shared_ptr<std::vector<std::pair<std::shared_ptr<Fault>, std::shared_ptr<Node>>>> top_fault_list) {
    // TODO: Filter fault model depending on the gate type (if needed)
    for (const auto& port : node->children) {
        for (int i = 0; i < static_cast<int>(FaultModelType::Count); ++i) {
            std::shared_ptr<Fault> fault = std::make_shared<Fault>(static_cast<FaultModelType>(i), -1); //FIXME port.second instead of -1 before. -1 is because it is an output
            node->fault_list.push_back(fault);
            top_fault_list->push_back({fault, node});}
    }
}