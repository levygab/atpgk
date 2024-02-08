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

#include "../../../include/tree/Yosys/UnaryCell.hpp"

namespace Unarycell {

// UnaryCell implementation ------------------------------------------
UnaryCell::UnaryCell(size_t identifier, std::string netlistName) : YosysCell(identifier, netlistName) {
    std::vector<std::tuple<int, int, bool>> portValues;
    this -> portValues.emplace_back(1, -1, false);
    this -> portValues.emplace_back(-1, -1, false);
}

// BufCell implementation --------------------------------------------

BufCell::BufCell(size_t identifier, std::string netlistName) : UnaryCell(identifier, netlistName) {
    this->type = "$_BUF_";
}

std::shared_ptr<Node> BufCell::getSharedPtrToThis() {
    return std::make_shared<BufCell>(*this);
}

void BufCell::printNodeRecursive() {
    std::cout << "node type: Buf Cell id = " << this->Identifier;
    std::cout << ", value parent1 = " << this->valueParent;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> BufCell::computeInputFromOutput(int value){
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 0));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 1));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for a buff cell" << std::endl;
    }
    return result;  
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> BufCell::computeExpectedValue(int value, int port_number){
    //here here are only output to write
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    if (port_number != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //write children
    for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);

    return result;
}

bool BufCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    if (port_number != 1 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for a buf cell" << std::endl;
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for a buf cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) return false; //it mean it can not be computed
            else return true;
        }

        if (this -> getValueFromPort(1) == -1 ) mandatory.emplace_back(node, 1, value, false); //case input not computed
        else { //just a check
            if (this -> getValueFromPort(1) != value) return false;
        }

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else { //port_number == 1
        if ( this -> getValueFromPort(2) != -1) { //it has already been computed
            if (this -> getValueFromPort(1) != value) return false; //it mean it can not be computed
            else return true;
        }

        //it has not been computed
        if (this -> getValueFromPort(-1) == -1) mandatory.emplace_back(node, 1, 1, propagate);
        else {
            if (this -> getValueFromPort(-1) != value) return false;
        }
        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }

    return true;
}

// NotCell implementation --------------------------------------------

NotCell::NotCell(size_t identifier, std::string netlistName) : UnaryCell(identifier, netlistName) {
    this->type = "$_NOT_";
}

std::shared_ptr<Node> NotCell::getSharedPtrToThis() {
    return std::make_shared<NotCell>(*this);
}

void NotCell::printNodeRecursive() {
    std::cout << "node type: Not Cell id = " << this->Identifier;
    std::cout << ", value parent1 = " << this->valueParent;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> NotCell::computeInputFromOutput(int value){
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for a not cell" << std::endl;
    }
    return result;
} 

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> NotCell::computeExpectedValue(int value, int port_number){
    //here here are only output to write
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    if (port_number != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //write children
    if (value == 0) {
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else {
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    return result;
}

bool NotCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    if (port_number != 1 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for a not cell" << std::endl;
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for a not cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) return false; //it mean it can not be computed
            else return true;
        }

        if (this -> getValueFromPort(1) == -1 ) {
            if (value == 1) mandatory.emplace_back(node, 1, 0, false);
            else mandatory.emplace_back(node, 1, 1, false); //value == 0
        }
        else { //just a check
            if (this -> getValueFromPort(1) == value) return false;
        }

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else { //port_number == 1
        if ( this -> getValueFromPort(1) != -1) { //it has already been computed
            if (this -> getValueFromPort(1) != value) return false; //it mean it can not be computed
            else return true;
        }

        if (this -> getValueFromPort(-1) == -1) {
            if (value == 0) mandatory.emplace_back(node, -1, 1, propagate);
            else mandatory.emplace_back(node, -1, 0, propagate); //value == 1
        }
        else {
            if (this -> getValueFromPort(-1) == value) return false;
        }
        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }

    return true;
}


} // namespace Unarycell