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

#include "../../include/tree/Output.hpp"

// Constructor
Output::Output(size_t identifier, std::string _name, std::string netlistName) : Node(identifier, netlistName) {
    this->name = _name;
    this->type = "Output";
    std::vector<std::tuple<int, int, bool>> portValues;
    this -> portValues.emplace_back(1, -1, false);
}

void Output::printNodeRecursive() {
    std::cout << "node type: Output id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
}

std::vector<std::pair<std::shared_ptr<Node>, int>> Output::computeInputFromOutput(int value) {
    std::cout << "TODO" << std::endl;
    return std::vector<std::pair<std::shared_ptr<Node>, int>>();
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> Output::computeExpectedValue(int value, int port_number){
    //TODO
    std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": computeExpectedValue not implemented for an output" << std::endl;
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    return result;
}

std::shared_ptr<Node> Output::getSharedPtrToThis() {
    return std::make_shared<Output>(*this);
}

std::string Output::getName() {
    return this -> name;
}

bool Output::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    /*
    What to do :
        check port_number and value
        If there is no value :
            add the value
            send it to the parent
        if there is a value
            check it is the goodvalue
    */
    //check the port_number and value
    if(port_number != 1) std::cerr << "Error : port_number should be 1 for an output" << std::endl;
    if(value != 0 && value != 1) std::cerr << "Error : value should be 0 or 1 for an output" << std::endl;

    if(this -> getValueFromPort(1) == -1){ //no value
        //add the value
        this -> updatePort(1, value);
        //send it to the parent : it is supposed to have only one parent
        for (std::pair<std::shared_ptr<Node>, int> pair : parents) mandatory.emplace_back(pair.first, -1, value, false);
    }
    else { //value already here : not suppose to happen
        //check it is the good value
        if(this -> getValueFromPort(1) != value) {
            std::cout << "Error : the value of the output is not the same as the already here" << std::endl;
            return false;
        }
    }

    return true;
}