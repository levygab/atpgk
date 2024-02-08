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

#include "../../include/tree/Tree.hpp"

Tree::Tree(std::string name) {
    this->name = name;
    std::vector<std::shared_ptr<Node>> InputList;
    std::vector<std::shared_ptr<Node>> NodeList;
    std::vector<std::shared_ptr<Node>> OutputList;
}


void Tree::addInput(std::shared_ptr<Node> input){
    this->InputList.push_back(input);  
}

void Tree::addOutput(std::shared_ptr<Node> input){
    this->OutputList.push_back(input);  
}

void Tree::addNode(std::shared_ptr<Node> node){
    this->NodeList.push_back(node);
}

void Tree::printInputId(){
    int size = this->InputList.size();
    for (int i = 0; i < size ; i++){
        std::cout << "i = " << i ;
        std::cout << ", id = " << this->InputList[i] -> getIdentifier() << std::endl;
    }
}

void Tree::printTree(){
    int size = this->InputList.size();
    std::cout << "tree named " << this -> name << " contains " << size << " input and the following value" << std::endl;

    for (int i = 0; i < size ; i++){
        //std::cout << "intput number " << i << " has id = " << this->Input[i] -> getIdentifier();
        // std::cout << " and value = " << this->Input[i] -> getValue() << std::endl;
        this->InputList[i] -> printNodeAndNeighbor();
    }
        
};

std::shared_ptr<Node> Tree::getNodeByIdentifier(size_t identifier){
    for (std::shared_ptr<Node> node : NodeList){
        if (node -> getIdentifier() == identifier){
            return node;
        }
    }
    std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": no node with this identifier, id = " << identifier << std::endl;
    return nullptr;
};

void Tree::traverse(NodeVisitor& visitor, std::shared_ptr<std::vector<std::pair<std::shared_ptr<Fault>, std::shared_ptr<Node>>>> top_fault_list) {
    for (std::shared_ptr<Node> node : this->NodeList) {
        node->accept(visitor, top_fault_list);
    }
};

void Tree::resetPortValue(){
    for (std::shared_ptr<Node>& node : this->NodeList) {
        node->resetPortValue();
    }
};



