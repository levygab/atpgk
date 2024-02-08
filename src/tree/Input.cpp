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

#include "../../include/tree/Input.hpp"

// Constructor
Input::Input(size_t identifier, std::string _name, std::string netlistName) : Node(identifier, netlistName) {
    this->name = _name;
    this->type = "Input";
    std::vector<std::tuple<int, int, bool>> portValues;
    this -> portValues.emplace_back(-1, -1, false);
}

void Input::printNodeRecursive() {
    std::cout << "node type: Input id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    // Call this function for each child
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> Input::computeInputFromOutput(int value) {
    std::cout << "TODO" << std::endl;
    return std::vector<std::pair<std::shared_ptr<Node>, int>>();
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> Input::computeExpectedValue(int value, int port_number){
    //TODO
    std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": computeExpectedValue not implemented for an input" << std::endl;
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    return result;
}

std::shared_ptr<Node> Input::getSharedPtrToThis() {
    return std::make_shared<Input>(*this);
}

std::string Input::getName() {
    return this -> name;
}

bool Input::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    /*
    what do do ?
        check port_number and value
        If there is no value :
            add the value
            send it to the children
        if there is a value
            check it is the goodvalue
    */
    //check the port_number and value
    if(port_number != -1) std::cout << "Error : port_number should be -1 for an input" << std::endl;
    if(value != 0 && value != 1) std::cout << "Error : value should be 0 or 1 for an input" << std::endl;

    if(this -> getValueFromPort(port_number) == -1){
        //add the value
        this -> updatePort(port_number, value);
        //send it to the children
        for (std::pair<std::shared_ptr<Node>, int> pair : children) mandatory.emplace_back(pair.first, pair.second, value, propagate);
    }
    else {
        //check it is the goodvalue
        if(this -> getValueFromPort(port_number) != value) return false;
    }

    return true;
}