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

#include "../../../include/tree/Yosys/BinaryCell.hpp"

namespace Binarycell{

// BinaryCell implementation -----------------------------------------
BinaryCell::BinaryCell(size_t identifier, std::string netlistName) : YosysCell(identifier, netlistName){
    std::vector<std::tuple<int, int, bool>> portValues;
    this -> portValues.emplace_back(1, -1, false);
    this -> portValues.emplace_back(2, -1, false);
    this -> portValues.emplace_back(-1, -1, false);
}

// AndCell implementation --------------------------------------------

AndCell::AndCell(size_t identifier, std::string netlistName) : BinaryCell(identifier, netlistName) {
    this->type = "$_AND_";
    
}

void AndCell::printNodeRecursive() {
    std::cout << "node type: And Cell id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> AndCell::computeInputFromOutput(int value){
    /*
        No need to check the input's number
    */
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 0));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an and cell" << std::endl;
    }

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> AndCell::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //get p1, p2
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }

    if (port_number == 1){
        result.emplace_back(p2, 1, -1);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 1, -1);
    }
    
    if (value == 0){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (value == 1){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }

    return result; 
}

std::shared_ptr<Node> AndCell::getSharedPtrToThis() {
    return std::make_shared<AndCell>(*this);
}

bool AndCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for an and cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an and cell" << std::endl;

    if (port_number == -1) {
        //check : has it been already computed ?
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }
        //it has not been computed

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) { //inputs not computed
            if (value == 1) {
                mandatory.emplace_back(node, 1, 1, false);
                mandatory.emplace_back(node, 2, 1, false);
            }
            else if (value == 0){
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 0, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1) { //both input computed : just a check
            if (value == 1) {
                if (this -> getValueFromPort(1) != 1 || this -> getValueFromPort(2) != 1) return false; //it can not be
            }
            else {
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) return false;
            }
        }
        else if (this -> getValueFromPort(1) == -1 || this -> getValueFromPort(2) == -1) { //only one parent is computed
            //find the not computed input
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) == -1) {
                port_to_compute = 1;
                //parent_to_compute = this -> getNodeFromPort(1);
                computedvalue = this -> getValueFromPort(2);
            }
            else {
                port_to_compute = 2;
                computedvalue = this -> getValueFromPort(1);
            }
            if (value == 1){
                if (computedvalue == 0) return false; //it can not be computed
                else {
                    //add port_to_compute to mandatory
                    mandatory.emplace_back(node, port_to_compute, 1, false);
                }
            }
            else if (value == 0){
                if (computedvalue == 1) {
                    //add 0 to port_to_compute
                    mandatory.emplace_back(node, port_to_compute, 0, false);
                }
                else {
                    //add 0 to the other port in optional
                    optional.emplace_back(node, port_to_compute, 0, false);
                }
            }

        }
        

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1) {
        //check : has it been already computed ?
        if ( this -> getValueFromPort(1) != -1) { //it has already been computed
            if (this -> getValueFromPort(1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
        return true;
        }
        //it has not been computed
        if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 2, 1, false);
                mandatory.emplace_back(node, -1, value, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            //so we must compute the output
            if (this -> getValueFromPort(2) == 1 && value == 1) mandatory.emplace_back(node , -1, 1, propagate);
            else mandatory.emplace_back(node , -1, 0, propagate);

        }
        else if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            //we must compute the other input
            if (this -> getValueFromPort(-1) == 1) {
                if (value == 0) return false ; //problem
                if (value == 1) mandatory.emplace_back(node , 2, 1, propagate);
            }
            else {
                if (value == 1) mandatory.emplace_back(node , 2, 0, propagate);
                else optional.emplace_back(node , 2, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1) {
                if (this -> getValueFromPort(2) != 1 || value != 1) return false; //we are computing a 0
            }
            else {
                if (value == 1 && this -> getValueFromPort(2) == 1)  return false; //we are computing a one

            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }
    else { //port number =2

        //check : has it been already computed ?
        if ( this -> getValueFromPort(2) != -1) { //it has already been computed
            if (this -> getValueFromPort(2) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        //it has not been computed
        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 1, 1, false);
                mandatory.emplace_back(node, -1, value, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            //so we must compute the output
            if (this -> getValueFromPort(1) == 1 && value == 1) mandatory.emplace_back(node , -1, 1, propagate);
            else mandatory.emplace_back(node , -1, 0, propagate);

        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other inpu
            //we must compute the other input
            if (this -> getValueFromPort(-1) == 1) {
                if (value == 0) return false ; //problem
                if (value == 1) mandatory.emplace_back(node , 1, 1, propagate);
            }
            else {
                if (value == 1) mandatory.emplace_back(node , 1, 0, propagate);
                else optional.emplace_back(node , 2, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1) {
                if (this -> getValueFromPort(1) != 1 || value != 1) return false; //we are computing a 0
            }
            else {
                if (value == 1 && this -> getValueFromPort(1) == 1)  return false; //we are computing a one
            }
        }
        mandatory.emplace_back(this -> getNodeFromPort(2), -1, value, false); //send to the parent
        this -> updatePort(2, value);
    }

    return true;
}

// OrCell implementation ---------------------------------------------

OrCell::OrCell(size_t identifier, std::string netlistName) : BinaryCell(identifier, netlistName) {
    this->type = "$_OR_";
}

std::shared_ptr<Node> OrCell::getSharedPtrToThis() {
    return std::make_shared<OrCell>(*this);
}

void OrCell::printNodeRecursive() {
    std::cout << "node type: Or Cell id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> OrCell::computeInputFromOutput(int value){
    /*
        No need to check the input's number
    */
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 0));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an or cell" << std::endl;
    }

    return result; 
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> OrCell::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an or cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an or cell" << std::endl;

    //get p1, p2
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }

    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
    }

    if (value == 0){ //all the children are at 0 because both input are 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (value == 1){ //all the children are at 1 because one of the input is 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    
    return result;
}

bool OrCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) {
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for an or cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an or cell" << std::endl;

    if (port_number == -1) {
        //check : has it been already computed ?
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) { //input not computed
            if (value == 0){
                mandatory.emplace_back(node, 1, 0, false);
                mandatory.emplace_back(node, 2, 0, false);
            }
            else {
                optional.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1){ //both input compted : just a check
            if (value == 0) {
                if (this -> getValueFromPort(1) == 1 || this -> getValueFromPort(2) == 1) return false; //it can not be
            }
            else {
                if (this -> getValueFromPort(1) == 0 && this -> getValueFromPort(2) == 0) return false;
            }
        }
        else if (this -> getValueFromPort(1) == -1 || this -> getValueFromPort(2) == -1) { //only one parent is computed
            //find the not computed input
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) == -1) {
                port_to_compute = 1;
                //parent_to_compute = this -> getNodeFromPort(1);
                computedvalue = this -> getValueFromPort(2);
            }
            else {
                port_to_compute = 2;
                computedvalue = this -> getValueFromPort(1);
            }

            if (value == 0){
                if (computedvalue == 1) return false; //it can not be computed
                else  mandatory.emplace_back(node, port_to_compute, 0, false);
            }

            else { //value == 1
                if (computedvalue == 0) mandatory.emplace_back(node, port_to_compute, 1, false);
                else optional.emplace_back(node, port_to_compute, 1, false);
            }
        }
        
        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1){
        //check : has it been already computed ?
        if ( this -> getValueFromPort(1) != -1) { //it has already been computed
            if (this -> getValueFromPort(1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 2, 0, false);
                mandatory.emplace_back(node, -1, value, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(2) == 0 && value == 0) mandatory.emplace_back(node , -1, 0, propagate);
            else mandatory.emplace_back(node , -1, 1, propagate);
        }
        else if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 1) return false ; //problem
                else mandatory.emplace_back(node , 2, 0, propagate);
            }
            else {
                if (value == 0) mandatory.emplace_back(node , 2, 1, propagate);
                else optional.emplace_back(node , 2, 1, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 0){
                if (this -> getValueFromPort(2) == 1 || value == 1) return false; //we are computing a 1
            }
            else {
                if (value == 0 && this -> getValueFromPort(2) == 0)  return false; //we are computing a 0
            }
        }

        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }
    else { //port number 2

        if ( this -> getValueFromPort(2) != -1) { //check : has it been already computed ?
            if (this -> getValueFromPort(2) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 1, 0, false);
                mandatory.emplace_back(node, -1, value, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(1) == 0 && value == 0) mandatory.emplace_back(node , -1, 0, propagate);
            else mandatory.emplace_back(node , -1, 1, propagate);
        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 1) return false ; //problem
                if (value == 0) mandatory.emplace_back(node , 1, 0, propagate);
            }
            else {
                if (value == 0) mandatory.emplace_back(node , 1, 1, propagate);
                else optional.emplace_back(node , 1, 1, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 0){
                if (this -> getValueFromPort(1) == 1 || value == 1) return false; //we are computing a 1
            }
            else {
                if (value == 0 && this -> getValueFromPort(1) == 0)  return false; //we are computing a 0
            }
        }

        mandatory.emplace_back(this -> getNodeFromPort(2), -1, value, false); //send to the parent
        this -> updatePort(2, value);
    }

    return true;
}


// NorCell implementation --------------------------------------------

NorCell::NorCell(size_t identifier, std::string netlistName) : BinaryCell(identifier, netlistName) {
    this->type = "$_NOR_";
}

std::shared_ptr<Node> NorCell::getSharedPtrToThis() {
    return std::make_shared<NorCell>(*this);
}

void NorCell::printNodeRecursive() {
    std::cout << "node type: Nor Cell id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> NorCell::computeInputFromOutput(int value){
    /*
        No need to check the input's number
    */
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an nor cell" << std::endl;
    }
    return result;   
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> NorCell::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //get p1, p2
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }

    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
    }
    
    if (value == 0){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (value == 1){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }

    return result; 
}

bool NorCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) {
    //checking
    if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for a nor cell" << std::endl;
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for a nor cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) { //input not computed
            if (value == 1){
                mandatory.emplace_back(node, 1, 0, false);
                mandatory.emplace_back(node, 2, 0, false);
            }
            else {
                optional.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1) { //both input computed
            if (value == 1) {
                if (this -> getValueFromPort(1) == 1 || this -> getValueFromPort(2) == 1) return false; //it can not be
            }
            else {
                if (this -> getValueFromPort(1) == 0 && this -> getValueFromPort(2) == 0) return false;
            }
        }
        else if (this -> getValueFromPort(1) == -1 || this -> getValueFromPort(2) == -1) { //only one parent is computed
            //get input to compute
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) == -1) {
                port_to_compute = 1;
                //parent_to_compute = this -> getNodeFromPort(1);
                computedvalue = this -> getValueFromPort(2);
            }
            else {
                port_to_compute = 2;
                computedvalue = this -> getValueFromPort(1);
            }

            if (value == 1){
                if (computedvalue == 1) return false; //it can not be computed
                else  mandatory.emplace_back(node, port_to_compute, 0, false);
            }
            else { //value == 0
                if (computedvalue == 0) mandatory.emplace_back(node, port_to_compute, 1, false);
                else optional.emplace_back(node, port_to_compute, 1, false);
            }
        }
        

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1) {
        if ( this -> getValueFromPort(1) != -1) { //it has already been computed
            if (this -> getValueFromPort(1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 2, 0, false);
                if (value == 1) mandatory.emplace_back(node, -1, 0, true);
                else mandatory.emplace_back(node, -1, 1, true);
            }
        }
        else if (this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(2) == 0 && value == 0) mandatory.emplace_back(node , -1, 1, propagate);
            else mandatory.emplace_back(node , -1, 0, propagate);
        }
        else if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 1) {
                if (value == 1) return false ; //problem
                else mandatory.emplace_back(node , 2, 0, propagate);
            }
            else {
                if (value == 0) mandatory.emplace_back(node , 2, 1, propagate);
                else optional.emplace_back(node , 2, 1, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1){
                if (this -> getValueFromPort(2) == 1 || value == 1) return false; //we are computing a 1
            }
            else {
                if (value == 0 && this -> getValueFromPort(2) == 0)  return false; //we are computing a 0
            }
        }

        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }
    else { //port number 2
        if ( this -> getValueFromPort(2) != -1) { //it has already been computed
            if (this -> getValueFromPort(2) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 1, 0, false);
                if (value == 1) mandatory.emplace_back(node, -1, 0, propagate);
                else mandatory.emplace_back(node, -1, 1, propagate);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(1) == 0 && value == 0) mandatory.emplace_back(node , -1, 1, propagate);
            else mandatory.emplace_back(node , -1, 0, propagate);
        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 1) {
                if (value == 1) return false ; //problem
                else mandatory.emplace_back(node , 1, 0, propagate);
            }
            else {
                if (value == 0) mandatory.emplace_back(node , 1, 1, propagate);
                else optional.emplace_back(node , 1, 1, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1){
                if (this -> getValueFromPort(1) == 1 || value == 1) return false; //we are computing a 1
            }
            else {
                if (value == 0 && this -> getValueFromPort(1) == 0)  return false; //we are computing a 0
            }
        }

        mandatory.emplace_back(this -> getNodeFromPort(2), -1, value, false); //send to the parent
        this -> updatePort(2, value);
    }

    return true;
}

// NandCell implementation -------------------------------------------

NandCell::NandCell(size_t identifier, std::string netlistName) : BinaryCell(identifier, netlistName) {
    this->type = "$_NAND_";
}

std::shared_ptr<Node> NandCell::getSharedPtrToThis() {
    return std::make_shared<NandCell>(*this);
}

void NandCell::printNodeRecursive() {
    std::cout << "node type: Nand Cell id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> NandCell::computeInputFromOutput(int value){
    /*
        No need to check the input's number
    */
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an nand cell" << std::endl;
    }
    return result;   
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> NandCell::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //get p1, p2
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }

    if (port_number == 1){
        result.emplace_back(p2, 1, -1);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 1, -1);
    }
    
    if (value == 0){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (value == 1){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }

    return result;
}

bool NandCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for a nand cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for a nand cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) { //inputs not computed
            if (value == 1) {
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 0, false);
            }
            else if (value == 0){
                mandatory.emplace_back(node, 1, 1, false);
                mandatory.emplace_back(node, 2, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1) { //just a check
            if (value == 0) {
                if (this -> getValueFromPort(1) != 1 || this -> getValueFromPort(2) != 1) return false; //it can not be
            }
            else {
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) return false;
            }
        }
        else if (this -> getValueFromPort(1) == -1 || this -> getValueFromPort(2) == -1) { //only one parent is computed
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) == -1) {
                port_to_compute = 1;
                //parent_to_compute = this -> getNodeFromPort(1);
                computedvalue = this -> getValueFromPort(2);
            }
            else {
                port_to_compute = 2;
                computedvalue = this -> getValueFromPort(1);
            }
            if (value == 0){
                if (computedvalue == 0) return false;
                else {
                    mandatory.emplace_back(node, port_to_compute, 1, false);
                }
            }
            else{ //value == 1
                if (computedvalue == 1) {
                    mandatory.emplace_back(node, port_to_compute, 0, false);
                }
                else {
                    optional.emplace_back(node, port_to_compute, 0, false);
                }
            }

        }
        

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1) {
        if ( this -> getValueFromPort(1) != -1) { 
            if (this -> getValueFromPort(1) != value) { 
                return false; 
            }
        return true;
        }

        if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 2, 1, false);
                if (value == 1) mandatory.emplace_back(node, -1, 0, propagate);
                else mandatory.emplace_back(node, -1, 1, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(2) == 1 && value == 1) mandatory.emplace_back(node , -1, 0, propagate);
            else mandatory.emplace_back(node , -1, 1, propagate);

        }
        else if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0) return false ; //problem
                if (value == 1) mandatory.emplace_back(node , 2, 1, propagate);
            }
            else { //output is at 1
                if (value == 1) mandatory.emplace_back(node , 2, 0, propagate);
                else optional.emplace_back(node , 2, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(2) != 1 || value != 1) return false; //we are computing a 1
            }
            else {
                if (value == 1 && this -> getValueFromPort(2) == 1)  return false; //we are computing a 0

            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }
    else { //port number =2
        if ( this -> getValueFromPort(2) != -1) { //it has already been computed
            if (this -> getValueFromPort(1) != value) { 
                return false; 
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 1, 1, false);
                if (value == 1) mandatory.emplace_back(node, -1, 0, propagate);
                else mandatory.emplace_back(node, -1, 1, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(1) == 1 && value == 1) mandatory.emplace_back(node , -1, 0, propagate);
            else mandatory.emplace_back(node , -1, 1, propagate);

        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other inpu
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0) return false ; //problem
                if (value == 1) mandatory.emplace_back(node , 1, 1, propagate);
            }
            else {
                if (value == 1) mandatory.emplace_back(node , 1, 0, propagate);
                else optional.emplace_back(node , 2, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(1) != 1 && value != 1) return false; //we are computing a 1
            }
            else {
                if (value == 1 && this -> getValueFromPort(2) == 1)  return false; //we are computing a 0
            }
        }
        
        mandatory.emplace_back(this -> getNodeFromPort(2), -1, value, false); //send to the parent
        this -> updatePort(2, value);
    }

    return true;
}

// XorCell implementation --------------------------------------------

XorCell::XorCell(size_t identifier, std::string netlistName) : BinaryCell(identifier, netlistName) {
    this->type = "$_XOR_";
}

std::shared_ptr<Node> XorCell::getSharedPtrToThis() {
    return std::make_shared<XorCell>(*this);
}

void XorCell::printNodeRecursive() {
    std::cout << "node type : Xor Cell id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> XorCell::computeInputFromOutput(int value){
    //no need to check the input's number because we don't care which one is one or zero
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an xor cell" << std::endl;
    }
    return result;   
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> XorCell::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //get p1, p2
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }

    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
    }
    
    if (value == 0){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (value == 1){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }

    return result;
}

bool XorCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for a xor cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for a xor cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //check if it has already been computed
            if (this -> getValueFromPort(-1) != value) return false; //it have been computed and the value is different
            else return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) { //inputs not computed
            if (value == 1) {
                optional.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 0, false);
            }
            else if (value == 0){
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 0, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1) { //both input computed : just a check
            if (value == 0) {
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 0) return false;
                else if (this -> getValueFromPort(1) == 0 && this -> getValueFromPort(2) == 1) return false; 
            }
            else { //value == 1
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) return false;
                else if (this -> getValueFromPort(1) == 0 && this -> getValueFromPort(2) == 0) return false;
            }
        }
        else if (this -> getValueFromPort(1) == -1 || this -> getValueFromPort(2) == -1) { //only one parent is computed
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) == -1) {
                port_to_compute = 1;
                //parent_to_compute = this -> getNodeFromPort(1);
                computedvalue = this -> getValueFromPort(2);
            }
            else {
                port_to_compute = 2;
                computedvalue = this -> getValueFromPort(1);
            }
            
            if (value == 0){
                if (computedvalue == 0) mandatory.emplace_back(node, port_to_compute, 0, false);
                else mandatory.emplace_back(node, port_to_compute, 1, false);
            }
            else{ //value == 1
                if (computedvalue == 1) mandatory.emplace_back(node, port_to_compute, 0, false);                    
                else mandatory.emplace_back(node, port_to_compute, 1, false);
            }

        }
        

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1) {
        if ( this -> getValueFromPort(1) != -1) { 
            if (this -> getValueFromPort(1) != value) return false; 
            else return true;
        }

        if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                optional.emplace_back(node, 2, 0, false);
                if (value == 1) optional.emplace_back(node, -1, 1, propagate);
                else optional.emplace_back(node, -1, 0, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if ( (this -> getValueFromPort(2) == 1 && value == 1) || (this -> getValueFromPort(2) == 0 && value == 0) ) mandatory.emplace_back(node , -1, 0, propagate);
            else mandatory.emplace_back(node , -1, 1, propagate);

        }
        else if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0) mandatory.emplace_back(node , 2, 0, propagate);
                if (value == 1) mandatory.emplace_back(node , 2, 1, propagate);
            }
            else { //output is at 1
                if (value == 1) mandatory.emplace_back(node , 2, 0, propagate);
                else mandatory.emplace_back(node , 2, 1, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1) {
                if ( (this -> getValueFromPort(2) == 1 && value == 1) || (this -> getValueFromPort(2) == 0 && value == 0) ) return false; //we are computing a 0
            }
            else { //output == 0
                if ( (this -> getValueFromPort(2) == 1 && value == 0) || (this -> getValueFromPort(2) == 0 && value == 1) ) return false;
            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }
    else { //port_number == 2
        if ( this -> getValueFromPort(2) != -1) { 
            if (this -> getValueFromPort(2) != value) return false; 
            else return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                optional.emplace_back(node, 1, 0, false);
                if (value == 1) optional.emplace_back(node, -1, 1, propagate);
                else optional.emplace_back(node, -1, 0, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if ( (this -> getValueFromPort(1) == 1 && value == 1) || (this -> getValueFromPort(1) == 0 && value == 0) ) mandatory.emplace_back(node , -1, 0, propagate);
            else mandatory.emplace_back(node , -1, 1, propagate);

        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0) mandatory.emplace_back(node , 1, 0, propagate);
                if (value == 1) mandatory.emplace_back(node , 1, 1, propagate);
            }
            else { //output is at 1
                if (value == 1) mandatory.emplace_back(node , 1, 0, propagate);
                else            mandatory.emplace_back(node , 1, 1, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1) {
                if ( (this -> getValueFromPort(1) == 1 && value == 1) || (this -> getValueFromPort(1) == 0 && value == 0) ) return false; //we are computing a 0
            }
            else { //output == 0
                if ( (this -> getValueFromPort(1) == 1 && value == 0) || (this -> getValueFromPort(1) == 0 && value == 1) ) return false;
            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(2), -1, value, false); //send to the parent
        this -> updatePort(2, value);
    }

    return true;
}

// XnorCell implementation -------------------------------------------

XnorCell::XnorCell(size_t identifier, std::string netlistName) : BinaryCell(identifier, netlistName) {
    this->type = "$_XNOR_";
}

std::shared_ptr<Node> XnorCell::getSharedPtrToThis() {
    return std::make_shared<XnorCell>(*this);
}

void XnorCell::printNodeRecursive() {
    std::cout << "node type: XNor Cell id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> XnorCell::computeInputFromOutput(int value){
    // no need to check the input's number because we don't care which one is one or zero
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an xnor cell" << std::endl;
    }
    return result;   
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> XnorCell::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //get p1, p2
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }

    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
    }
    
    if (value == 0){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (value == 1){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }

    return result;
}

bool XnorCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for a xnor cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for a xnor cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //check if it has already been computed
            if (this -> getValueFromPort(-1) != value) return false; //it have been computed and the value is different
            else return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) { //inputs not computed
            if (value == 1) {
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 0, false);
            }
            else { //value == 0
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1) { //both input computed : just a check
            if (value == 0) {
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) return false;
                else if (this -> getValueFromPort(1) == 0 && this -> getValueFromPort(2) == 0) return false; 
            }
            else { //value == 1
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 0) return false;
                else if (this -> getValueFromPort(1) == 0 && this -> getValueFromPort(2) == 1) return false;
            }
        }
        else if (this -> getValueFromPort(1) == -1 || this -> getValueFromPort(2) == -1) { //only one parent is computed
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) == -1) {
                port_to_compute = 1;
                //parent_to_compute = this -> getNodeFromPort(1);
                computedvalue = this -> getValueFromPort(2);
            }
            else {
                port_to_compute = 2;
                computedvalue = this -> getValueFromPort(1);
            }
            
            if (value == 0){
                if (computedvalue == 0) mandatory.emplace_back(node, port_to_compute, 1, false);
                else mandatory.emplace_back(node, port_to_compute, 0, false);
            }
            else { //value == 1
                if (computedvalue == 1) mandatory.emplace_back(node, port_to_compute, 1, false);                    
                else mandatory.emplace_back(node, port_to_compute, 0, false);
            }

        }
        

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1) {
        if ( this -> getValueFromPort(1) != -1) { 
            if (this -> getValueFromPort(1) != value) return false; 
            else return true;
        }

        if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                optional.emplace_back(node, 2, 0, false);
                if (value == 1) optional.emplace_back(node, -1, 0, propagate);
                else optional.emplace_back(node, -1, 1, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if ( (this -> getValueFromPort(2) == 1 && value == 1) || (this -> getValueFromPort(2) == 0 && value == 0) ) mandatory.emplace_back(node , -1, 1, propagate);
            else mandatory.emplace_back(node , -1, 0, propagate);

        }
        else if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0) mandatory.emplace_back(node , 2, 1, propagate);
                if (value == 1) mandatory.emplace_back(node , 2, 0, propagate);
            }
            else { //output is at 1
                if (value == 1) mandatory.emplace_back(node , 2, 1, propagate);
                else mandatory.emplace_back(node , 2, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1) {
                if ( (this -> getValueFromPort(2) == 1 && value == 0) || (this -> getValueFromPort(2) == 0 && value == 1) ) return false; //we are computing a 0
            }
            else { //output == 0
                if ( (this -> getValueFromPort(2) == 1 && value == 1) || (this -> getValueFromPort(2) == 0 && value == 0) ) return false;
            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }
    else { //port_number == 2
        if ( this -> getValueFromPort(2) != -1) { 
            if (this -> getValueFromPort(2) != value) return false; 
            else return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                optional.emplace_back(node, 1, 0, false);
                if (value == 1) optional.emplace_back(node, -1, 0, propagate);
                else optional.emplace_back(node, -1, 1, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if ( (this -> getValueFromPort(1) == 1 && value == 1) || (this -> getValueFromPort(1) == 0 && value == 0) ) mandatory.emplace_back(node , -1, 1, propagate);
            else mandatory.emplace_back(node , -1, 0, propagate);
        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0) mandatory.emplace_back(node , 1, 1, propagate);
                else mandatory.emplace_back(node , 1, 0, propagate);
            }
            else { //output is at 1
                if (value == 1) mandatory.emplace_back(node , 1, 1, propagate);
                else            mandatory.emplace_back(node , 1, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1) {
                if ( (this -> getValueFromPort(1) == 1 && value == 0) || (this -> getValueFromPort(1) == 0 && value == 1) ) return false; //we are computing a 0
            }
            else { //output == 0
                if ( (this -> getValueFromPort(1) == 1 && value == 1) || (this -> getValueFromPort(1) == 0 && value == 0) ) return false;
            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(2), -1, value, false); //send to the parent
        this -> updatePort(2, value);
    }

    return true;
}

// OrnotCell implementation ------------------------------------------

OrnotCell::OrnotCell(size_t identifier, std::string netlistName) : BinaryCell(identifier, netlistName) {
    this->type = "$_ORNOT_";
}

std::shared_ptr<Node> OrnotCell::getSharedPtrToThis() {
    return std::make_shared<OrnotCell>(*this);
}

void OrnotCell::printNodeRecursive() {
    std::cout << "node type : ornot Cell id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    //call this function for each children
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> OrnotCell::computeInputFromOutput(int value){
    //need to check the input's number
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in the research of parent in computeInputFromOutput from OrnotCell with id"<< this-> Identifier << std::endl;
    }

    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(p1, 0));
        result.push_back(std::make_pair(p2, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(p1, 1));
        result.push_back(std::make_pair(p2, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an ornot cell" << std::endl;
    }
    return result;   
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> OrnotCell::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //get p1, p2
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }

    if (port_number == 1){
        result.emplace_back(p2, 1, -1);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
    }
    
    if (value == 0 && port_number == 1){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (value == 1 && port_number == 1){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (value == 0 && port_number == 2){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (value == 1 && port_number == 2){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }

    return result;
}

bool OrnotCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for an ornot cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an ornot cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) { //inputs not computed
            if (value == 1) {
                optional.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 0, false);
            }
            else if (value == 0){
                mandatory.emplace_back(node, 1, 0, false);
                mandatory.emplace_back(node, 2, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) != -1) { // only parent two is computed
            if (value == 0){
                if (this -> getValueFromPort(2) != 1) return false; //not compatible
                else mandatory.emplace_back(node, 1, 0, false);
            }
            else{ //value == 1
                if (this -> getValueFromPort(2) == 1) mandatory.emplace_back(node, 1, 1, false);
                else optional.emplace_back(node, 1, 1, false);
            }

        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) == -1) { //only parent one is computed
            if (value == 0){
                if (this -> getValueFromPort(1) != 0) return false; //not compatible
                else mandatory.emplace_back(node, 2, 1, false);
            }
            else{ //value == 1
                if (this -> getValueFromPort(1) == 0) mandatory.emplace_back(node, 2, 0, false);
                else optional.emplace_back(node, 2, 0, false);
            }

        }
        else { //just a check
            if (value == 0) {
                if (this -> getValueFromPort(1) != 0 || this -> getValueFromPort(2) != 1) return false; //it can not be
            }
            else {
                if (this -> getValueFromPort(1) == 0 && this -> getValueFromPort(2) == 1) return false;
            }
        }

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1) {
        if ( this -> getValueFromPort(1) != -1) { 
            if (this -> getValueFromPort(1) != value) { 
                return false; 
            }
        return true;
        }

        if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 2, 1, false);
                mandatory.emplace_back(node, -1, value, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(2) == 1 && value == 0) mandatory.emplace_back(node , -1, 0, propagate);
            else mandatory.emplace_back(node , -1, 1, propagate);

        }
        else if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 1) return false ; //problem
                else mandatory.emplace_back(node , 2, 1, propagate);
            }
            else { //output is at 1
                if (value == 0) mandatory.emplace_back(node , 2, 0, propagate);
                else optional.emplace_back(node , 2, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(2) != 1 || value != 0) return false; //we are computing a 1
            }
            else {
                if (value == 0 && this -> getValueFromPort(2) == 1)  return false; //we are computing a 0

            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }
    else { //port number =2
        if ( this -> getValueFromPort(2) != -1) { //it has already been computed
            if (this -> getValueFromPort(1) != value) { 
                return false; 
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 1, 0, false);
                if (value == 1) mandatory.emplace_back(node, -1, 0, propagate);
                else mandatory.emplace_back(node, -1, 1, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(1) == 0 && value == 1) mandatory.emplace_back(node , -1, 0, propagate);
            else mandatory.emplace_back(node , -1, 1, propagate);

        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other inpu
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0) return false ; //problem
                else mandatory.emplace_back(node , 1, 0, propagate);
            }
            else {
                if (value == 1) mandatory.emplace_back(node , 1, 0, propagate);
                else optional.emplace_back(node , 1, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(1) != 0 || value != 1) return false; //we are computing a 1
            }
            else {
                if (value == 1 && this -> getValueFromPort(2) == 0)  return false; //we are computing a 0
            }
        }
        
        mandatory.emplace_back(this -> getNodeFromPort(2), -1, value, false); //send to the parent
        this -> updatePort(2, value);
    }

    return true;
}

// AndnotCell implementation -----------------------------------------

AndnotCell::AndnotCell(size_t identifier, std::string netlistName) : BinaryCell(identifier, netlistName) {
    this->type = "$_ANDNOT_";
}

std::shared_ptr<Node> AndnotCell::getSharedPtrToThis() {
    return std::make_shared<AndnotCell>(*this);
}

void AndnotCell::printNodeRecursive() {
    std::cout << "node type : andnot Cell id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    //call this function for each children
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> AndnotCell::computeInputFromOutput(int value){
    //need to check the input's number
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in the research of parent in computeInputFromOutput from AndnotCell with id"<< this-> Identifier << std::endl;
    }

    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(p1, 0));
        result.push_back(std::make_pair(p2, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(p1, 1));
        result.push_back(std::make_pair(p2, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an andnot cell with id"<< this-> Identifier<< std::endl;
    }
    return result;   
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> AndnotCell::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an and cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an and cell" << std::endl;

    //get p1, p2
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    if (this->parents[0].second == 1){
        p1 = this->parents[0].first;
        p2 = this->parents[1].first;
    }
    else  if (this->parents[0].second == 2){
        p1 = this->parents[1].first;
        p2 = this->parents[0].first;
    }

    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 1, -1);
    }
    
    if (value == 0 && port_number == 1){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (value == 1 && port_number == 1){ //all the children are at 0 because we one of the input is 0
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (value == 0 && port_number == 2){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (value == 1 && port_number == 2){ //all the children are at 1 because both input are 1
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }

    return result;
}

bool AndnotCell::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for an andnot cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an andnot cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) { //inputs not computed
            if (value == 1) {
                mandatory.emplace_back(node, 1, 1, false);
                mandatory.emplace_back(node, 2, 0, false);
            }
            else if (value == 0){
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) != -1) { // only parent two is computed
            if (value == 0){
                if (this -> getValueFromPort(2) == 1) optional.emplace_back(node, 1, 0, false);
                else mandatory.emplace_back(node, 1, 1, false);
            }
            else{ //value == 1
                if (this -> getValueFromPort(2) == 1) return false;
                else mandatory.emplace_back(node, 1, 1, false);
            }

        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) == -1) { //only parent one is computed
            if (value == 0){
                if (this -> getValueFromPort(1) == 1) mandatory.emplace_back(node, 2, 1, false);
                else optional.emplace_back(node, 2, 0, false);
            }
            else{ //value == 1
                if (this -> getValueFromPort(1) == 0) return false;
                else mandatory.emplace_back(node, 2, 0, false);
            }

        }
        else { //just a check
            if (value == 0) {
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 0) return false; //it can not be
            }
            else {
                if (this -> getValueFromPort(1) != 1 || this -> getValueFromPort(2) != 0) return false;
            }
        }

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1) {
        if ( this -> getValueFromPort(1) != -1) { 
            if (this -> getValueFromPort(1) != value) { 
                return false; 
            }
        return true;
        }

        if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 2, 0, false);
                mandatory.emplace_back(node, -1, value, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(2) == 0 && value == 1) mandatory.emplace_back(node , -1, 1, propagate);
            else mandatory.emplace_back(node , -1, 0, propagate);

        }
        else if (this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other input
            if (this -> getValueFromPort(-1) == 1) {
                if (value == 0) return false ; //problem
                else mandatory.emplace_back(node , 2, 0, propagate);
            }
            else { //output is at 0
                if (value == 1) mandatory.emplace_back(node , 2, 1, propagate);
                else optional.emplace_back(node , 2, 1, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1) {
                if (value != 1 || this -> getValueFromPort(2) != 0 ) return false; //we are computing a 1
            }
            else {
                if (value == 1 && this -> getValueFromPort(2) == 0)  return false; //we are computing a 0

            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(1), -1, value, false); //send to the parent
        this -> updatePort(1, value);
    }
    else { //port_number == 2
        if ( this -> getValueFromPort(2) != -1) { //it has already been computed
            if (this -> getValueFromPort(2) != value) { 
                return false; 
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) == -1) { //no other port has been computed
            if (propagate) {
                mandatory.emplace_back(node, 1, 1, false);
                if (value == 1) mandatory.emplace_back(node, -1, 0, propagate);
                else mandatory.emplace_back(node, -1, 1, propagate);
            }
            //if no propagate : nothing to do
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(-1) == -1) { //other input compute but not the output
            if (this -> getValueFromPort(1) == 1 && value == 0) mandatory.emplace_back(node , -1, 1, propagate);
            else mandatory.emplace_back(node , -1, 0, propagate);

        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(-1) != -1) { //the output is computed but not the other inpu
            if (this -> getValueFromPort(-1) == 1) {
                if (value == 1) return false ; //problem
                else mandatory.emplace_back(node , 1, 1, propagate);
            }
            else {
                if (value == 0) mandatory.emplace_back(node , 1, 0, propagate);
                else optional.emplace_back(node , 1, 0, propagate);
            }
        }
        else { //both the other ports are computed
            if (this -> getValueFromPort(-1) == 1) {
                if (this -> getValueFromPort(1) != 1 || value != 0) return false; //we are computing a 1
            }
            else {
                if (value == 1 && this -> getValueFromPort(2) == 0)  return false; //we are computing a 0
            }
        }
        
        mandatory.emplace_back(this -> getNodeFromPort(2), -1, value, false); //send to the parent
        this -> updatePort(2, value);
    }

    return true;
}

} // namespace Binarycell