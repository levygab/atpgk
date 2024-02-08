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

#include "../../include/tree/Node.hpp"
#include "../../include/tree/NodeVisitor.hpp"

Node::Node(size_t identifier, std::string netlistName) {
    this->Identifier = identifier;
    this->netlistName = netlistName;
    std::vector<std::pair<std::shared_ptr<Node>, int>> children;
    std::vector<std::pair<std::shared_ptr<Node>, int>> parents;
    this -> value = -1;
    this->covered = false;
};

size_t Node::getIdentifier() {
    return this->Identifier;
}

void Node::addChildren(std::shared_ptr<Node> child, int number) {
    this->children.push_back(std::make_pair(child, number));
}

void Node::addParent(std::shared_ptr<Node> parent, int number) {
    this->parents.push_back(std::make_pair(parent, number));
}

std::shared_ptr<Node> Node::getParentFromInputNumber(int input_number){
    for (std::pair<std::shared_ptr<Node>, int> pair : parents) {
        if (pair.second == input_number) {
            return pair.first;
        }
    }

    return nullptr;
}

std::shared_ptr<Node> Node::getNodeById(size_t identifier){
    if (this -> Identifier == identifier) {
        return (std::shared_ptr<Node>) this;
    }

    for (std::pair<std::shared_ptr<Node>, int> pair : children) {
        std::shared_ptr<Node> child = pair.first;
        std::shared_ptr<Node> result = child->getNodeById(identifier);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

void Node::printNodeAndNeighbor(){
    std::cout << "======================" << std::endl;
    std::cout << "Node id = " << this->Identifier << " ; node type: "<< this -> type << std::endl;
    std::cout << "children = " << std::endl;
    for (std::pair<std::shared_ptr<Node>, int> pair : children) {
        std::shared_ptr<Node> child = pair.first;
        std::cout << "          " ;
        std::cout << child -> getIdentifier() << ", linked to number: " << pair.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "parents = "<< std::endl;
    for (std::pair<std::shared_ptr<Node>, int> pair : parents) {
        std::shared_ptr<Node> parent = pair.first;
        std::cout << "          " ;
        std::cout << parent -> getIdentifier() << ", linked to number: " << pair.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
};

void Node::setCovered() {
    this->covered = true;
};

void Node::setUncovered() {
    this->covered = false;
};

bool Node::getCoverageFlag() {
    return this->covered;
};

void Node::accept(NodeVisitor& visitor, std::shared_ptr<std::vector<std::pair<std::shared_ptr<Fault>, std::shared_ptr<Node>>>> top_fault_list) {
    visitor.visit(this->getSharedPtrToThis(), top_fault_list);
};

int Node::getValueFromPort(int port_number){
    for (std::tuple<int, int, bool> tuple : this -> portValues){
        if (std::get<0>(tuple) == port_number){
            return std::get<1>(tuple); //case there is a computed value
            //else return -1; //case there is no computed value
        }
    }
    std::cout << "Error in getValueFromPort: the port number " << port_number << " has not been found in cell type " << this -> type << " ID is " << this -> Identifier << std::endl; 
    return -2;
}

int Node::numberOfComputePort(){
    int result = 0;
    for (std::tuple<int, int, bool> tuple : this -> portValues){
        if (std::get<2>(tuple)) result++;
    }
    return result;
}

std::shared_ptr<Node> Node::getNodeFromPort(int port_number){
    if (port_number <= 0) std::cerr << "Error in getNodeFromPort: you can only search for parent (port > 0)" << std::endl;
    for (std::pair<std::shared_ptr<Node>, int> pair : this -> parents){
        if (pair.second == port_number) return pair.first;
    }
    std::cerr << "Error in getNodeFromPort: the port number " << port_number << " has not been found" << std::endl;
    return nullptr;
}

void Node::updatePort(int port_number, int value){
    bool find = false;
    for (std::tuple<int, int, bool>& tuple : this -> portValues){
        if (std::get<0>(tuple) == port_number){
            std::get<1>(tuple) = value;
            std::get<2>(tuple) = true;
            find = true;
        }
    }
    if (!find) std::cerr << "Error in updatePort: the port number " << port_number << " has not been found" << std::endl;
}
    
bool Node::computeOptional(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) {
    //checking
    //if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port value must be 1, 2 or -1 for computeOptional" << std::endl;
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an and cell" << std::endl;
    
    if (this -> getValueFromPort(port_number) != -1) return true; //already computed

    if (port_number == -1) { //sending to the children
        this -> updatePort(port_number, value);
        for (std::pair<std::shared_ptr<Node>, int> pair : this -> parents) mandatory.emplace_back(pair.first, pair.second, value, false);
    }

    else { // sending to the parent
        this -> updatePort(port_number, value);
        mandatory.emplace_back(this -> getNodeFromPort(port_number), -1, value, false);
    }
    
    
    return true;
}

void Node::removeFromOptionial(std::shared_ptr<Node> node, int port_number, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    for (auto iter = optional.begin(); iter != optional.end();){
        if (std::get<0>(*iter) == node && std::get<1>(*iter) == port_number) {
            iter = optional.erase(iter);
        } else {
            ++iter;
        }
    }
}

void Node::resetPortValue(){
    for (std::tuple<int, int, bool>& tuple : this -> portValues){
        std::get<2>(tuple) = false;
        std::get<1>(tuple) = -1;

    }
}