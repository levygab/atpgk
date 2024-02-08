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

#include "../../../include/tree/Yosys/ComplexCell.hpp"

namespace Complexcell{

// ComplexCell implementation ----------------------------------------

ComplexCell::ComplexCell(size_t identifier, std::string netlistName) : YosysCell(identifier, netlistName) {}

// AOI3 implementation -----------------------------------------------

AOI3::AOI3(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_AOI3_";
    std::vector<std::tuple<int, int, bool>> portValues;
    this -> portValues.emplace_back( 1, -1, false);
    this -> portValues.emplace_back( 2, -1, false);
    this -> portValues.emplace_back( 3, -1, false);
    this -> portValues.emplace_back(-1, -1, false);
}

std::shared_ptr<Node> AOI3::getSharedPtrToThis() {
    return std::make_shared<AOI3>(*this);
}

void AOI3::printNodeRecursive() {
    std::cout << "node type: AOI3 id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> AOI3::computeInputFromOutput(int value){
    //no need of parent's number because we are writing the same number for each parent
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
        result.push_back(std::make_pair(this->parents[2].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 0));
        result.push_back(std::make_pair(this->parents[2].first, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an AOI3 cell" << std::endl;
    }

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> AOI3::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port
    if (port_number != 1 && port_number != 2 && port_number != 3) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an AOI3" << std::endl;

    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an AOI3" << std::endl;

    //get p1, p2 and p3
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;

    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this->parents[0].second == 3) p3 = this->parents[0].first;
    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this->parents[1].second == 3) p3 = this->parents[1].first;
    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this->parents[2].second == 3) p3 = this->parents[2].first;

    if (port_number == 1){
        result.emplace_back(p2, 1, -1);
        result.emplace_back(p3, 0, -1);
        if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
        else if (value == 0)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 1, -1);
        result.emplace_back(p3, 0, -1);
        if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
        else if (value == 0)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
    else if (port_number == 3) {
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p2, 0, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    return result;
}

bool AOI3::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != 3 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for an AOI3 cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an AOI3 cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1 && this -> getValueFromPort(3) == -1) { //inputs not computed
            if (value == 1) {
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 0, false);
                optional.emplace_back(node, 3, 0, false);
            }
            else if (value == 0){
                optional.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 1, false);
                optional.emplace_back(node, 3, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(3) == -1) { // parent 1 and 2 are computed
            if (value == 0){
                if (this -> getValueFromPort(1) != 1 || this -> getValueFromPort(2) != 1) mandatory.emplace_back(node, 3, 1, false); //if A&B = 0, we need C = 1
                else optional.emplace_back(node, 3, 1, false); //if A&B = 1, we can have C = 0 or 1
                
            }
            else{ //value == 1
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) return false; //we are computing a 0<
                else mandatory.emplace_back(node, 3, 0, false);
            }

        }
        else if ( (this -> getValueFromPort(1) != -1 || this -> getValueFromPort(2) != -1) && this -> getValueFromPort(3) == -1) { //parent 1 or 2 is computed
            //find the input to compute (between 1 and 2)
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
                if (computedvalue == 0) {
                    mandatory.emplace_back(node, 3, 1, false);
                    optional.emplace_back(node, port_to_compute, 1, false);
                }
            }
            else { //computedvalue == 1
                    optional.emplace_back(node, 3, 1, false);
                    optional.emplace_back(node, port_to_compute, 1, false);
            }
            
                
        }
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1 && this -> getValueFromPort(3) != -1) { //only input 3 computed
            if (value == 0) {
                if (this -> getValueFromPort(3) == 1) {
                    optional.emplace_back(node, 1, 1, false);
                    optional.emplace_back(node, 2, 1, false);
                }
                else {
                    mandatory.emplace_back(node, 1, 1, false);
                    mandatory.emplace_back(node, 2, 1, false);
                }
            }
            else { //value == 1
                if (this -> getValueFromPort(3) == 1) return false; //we are computing a 0
                else {
                    optional.emplace_back(node, 1, 0, false);
                    optional.emplace_back(node, 2, 0, false);
                }
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(3) != -1) { //all input computed
            if (value == 0) {
                if (this -> getValueFromPort(3) == 0 && (this -> getValueFromPort(1) != 1 || this -> getValueFromPort(2) != 1) ) return false; //we are computing a 1
            }
            else { //value == 1
                if (this -> getValueFromPort(3) == 1) return false; //we are computing a 0
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) return false; //we are computing a 0
            }
        }
        else { //parent 3 computed and parent 1 or 2 is computed
            //find the input to compute (between 1 and 2)
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

            if (value == 0) {
                if (this -> getValueFromPort(3) == 1 ) optional.emplace_back(node, port_to_compute, 1, false);
                else {
                    if (computedvalue == 0) return false; //problem but not supposed to happen
                    else mandatory.emplace_back(node, port_to_compute, 1, false);
                }
            }
            else { //value == 1
                if (this -> getValueFromPort(3) == 1 ) return false; //not supposed to happen
                else {
                    if (computedvalue == 1) mandatory.emplace_back(node, port_to_compute, 0, false);
                    else optional.emplace_back(node, port_to_compute, 0, false);
                }
            }
        }
        
        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else if (port_number == 1 || port_number == 2) {
        int other_port;
        int current_port;
        if (port_number == 1) {
            other_port = 2;
            current_port = 1;
        } 
        else {
            other_port = 1;
            current_port = 2;
        }

        if ( this -> getValueFromPort(current_port) != -1) { 
            if (this -> getValueFromPort(current_port) != value) { 
                return false; 
            }
        return true;
        }

        if (propagate) { //no other port is supposed to be computed
                mandatory.emplace_back(node, other_port, 1, false);
                mandatory.emplace_back(node, 3, 0, false);
                if (value == 0) mandatory.emplace_back(node, -1, 1, true);
                else mandatory.emplace_back(node, -1, 0, true);
            }
        else if (this -> getValueFromPort(other_port) != -1 && this -> getValueFromPort(3) != -1 && this -> getValueFromPort(-1) != -1 ){ //all other port computed
            //only checking
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(3) == 0 && (this -> getValueFromPort(other_port) != 1 || value != 1)) return false; 
            }
            else { //out = 1
                if (this -> getValueFromPort(3) == 1 || (this -> getValueFromPort(other_port) == 1 && value == 1)) return false; 
            }
        }
        else if (this -> getValueFromPort(other_port) != -1 && this -> getValueFromPort(3) == -1 && this -> getValueFromPort(-1) == -1 ){ //only other_port computed
            if (this -> getValueFromPort(other_port) == 1 && value == 1) mandatory.emplace_back(node, -1, 0, propagate);
            //FIXME Do I need to propagate ?
        }
        else if (this -> getValueFromPort(other_port) == -1 && this -> getValueFromPort(3) != -1 && this -> getValueFromPort(-1) == -1 ){ //only port 3 computed
            //nothing to do
        }
        else if (this -> getValueFromPort(other_port) == -1 && this -> getValueFromPort(3) == -1 && this -> getValueFromPort(-1) != -1 ){ //only out computed
            if (this -> getValueFromPort(-1) == 1) {
                mandatory.emplace_back(node, 3, 0, propagate);
                if (value == 1) mandatory.emplace_back(node, other_port, 0, propagate);
                else optional.emplace_back(node, other_port, 0, propagate);
            }
            //if out == 0, nothing to do
            //don't add it to opti because they are already there
        }
        else if (this -> getValueFromPort(other_port) != -1 && this -> getValueFromPort(3) != -1 && this -> getValueFromPort(-1) == -1 ){ //input other_port and 3 computed
            if (this -> getValueFromPort(3) == 1 || (this -> getValueFromPort(other_port) == 1 && value == 1)) mandatory.emplace_back(node, -1, 0, propagate);
            else mandatory.emplace_back(node, -1, 1, propagate);
        }
        else if (this -> getValueFromPort(other_port) != -1 && this -> getValueFromPort(3) == -1 && this -> getValueFromPort(-1) != -1 ){ //output and other_port computed
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(other_port) != 1 || value != 1) mandatory.emplace_back(node, 3, 1, propagate);
            }
            else { //out == 1
                if (this -> getValueFromPort(other_port) == 1 && value == 1) return false;
                else mandatory.emplace_back(node, 3, 0, propagate);
            }
        }
        else { //input 3 and output computed
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(3) == 0) {
                    if (value == 0) return false;
                    else mandatory.emplace_back(node, other_port, 1, propagate);
                }
            }
            else { //out == 1
                if (this -> getValueFromPort(3) == 1) return false;
                else {
                    if (value == 1) mandatory.emplace_back(node, other_port, 0, propagate);
                    else optional.emplace_back(node, other_port, 0, propagate);
                }
            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(current_port), -1, value, false); //send to the parent
        this -> updatePort(current_port, value);
    }
    else  { //port_number == 3
        if ( this -> getValueFromPort(3) != -1) { 
            if (this -> getValueFromPort(3) != value) return false; 
        return true;
        }
        
        if (propagate) {
                mandatory.emplace_back(node, 1, 0, false); //I block one of them
                optional.emplace_back(node, 2, 0, false);
                if (value == 0) mandatory.emplace_back(node, -1, 1, true);
                else mandatory.emplace_back(node, -1, 0, true);
            }
        else if ( this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) != -1) { //all computed
            if (this -> getValueFromPort(-1) == 1) {
                if (value == 1 || (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) ) return false;
            }
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0 && (this -> getValueFromPort(1) != 1 || this -> getValueFromPort(2) != 1) ) return false;
            }
        }
        else if ( this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //we have input 1 and input 2
            if (value == 1 || (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) ) mandatory.emplace_back(node, -1, 0, propagate);
            else mandatory.emplace_back(node, -1, 1, propagate);    
        }
        else if ( (this -> getValueFromPort(1) != -1 || this -> getValueFromPort(2) != -1) && this -> getValueFromPort(-1) == -1) { //we have input 1 or input 2
            if (value == 1) {
                mandatory.emplace_back(node, -1, 0, propagate);
            }
        }
        else { //out is computed
            if (this -> getValueFromPort(-1) == 0) {
                if (value == 0) {
                    mandatory.emplace_back(node, 1, 1, propagate);
                    mandatory.emplace_back(node, 2, 1, propagate);
                }
            }
            else {
                if (value == 1) return false;
            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(3), -1, value, false); //send to the parent
        this -> updatePort(3, value);
    }

    return true;
}

// OAI3 implementation -----------------------------------------------

OAI3::OAI3(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_OAI3_";
    std::vector<std::tuple<int, int, bool>> portValues;
    this -> portValues.emplace_back( 1, -1, false);
    this -> portValues.emplace_back( 2, -1, false);
    this -> portValues.emplace_back( 3, -1, false);
    this -> portValues.emplace_back(-1, -1, false);
}

std::shared_ptr<Node> OAI3::getSharedPtrToThis() {
    return std::make_shared<OAI3>(*this);
}

void OAI3::printNodeRecursive() {
    std::cout << "node type: OAI3 id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> OAI3::computeInputFromOutput(int value){
    //no need of parent's number because we are writing the same number for each parent
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
        result.push_back(std::make_pair(this->parents[2].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 0));
        result.push_back(std::make_pair(this->parents[2].first, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an OAI3 cell" << std::endl;
    }

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> OAI3::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port
    if (port_number != 1 && port_number != 2 && port_number != 3) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an AOI3" << std::endl;

    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an AOI3" << std::endl;

    //get p1, p2 and p3
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;

    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this->parents[0].second == 3) p3 = this->parents[0].first;
    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this->parents[1].second == 3) p3 = this->parents[1].first;
    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this->parents[2].second == 3) p3 = this->parents[2].first;

    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p3, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p3, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (port_number == 3) {
        result.emplace_back(p1, 1, -1);
        result.emplace_back(p2, 1, -1);
        if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
        else if (value == 0)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
    }
        
    return result;
}

bool OAI3::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != 3 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for an OAI3 cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an OAI3 cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1 && this -> getValueFromPort(3) == -1) { //inputs not computed
            if (value == 1) {
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 0, false);
                optional.emplace_back(node, 3, 0, false);
            }
            else if (value == 0){
                optional.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 1, false);
                optional.emplace_back(node, 3, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(3) != -1) { //all input computed
            if (value == 0) {
                if (this -> getValueFromPort(3) != 1 || (this -> getValueFromPort(2) != 1 && this -> getValueFromPort(1) != 1) ) return false; //we are computing a 1
            }
            else { //value == 1
                if (this -> getValueFromPort(3) == 1 && (this -> getValueFromPort(2) == 1 || this -> getValueFromPort(1) == 1) ) return false; //we are computing a 0
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(3) == -1) { // parent 1 and 2 are computed
            if (value == 0){
                if (this -> getValueFromPort(1) == 0 && this -> getValueFromPort(2) == 0) return false; //we are computing a 1
                else mandatory.emplace_back(node, 3, 1, false); //if A||B = 1, we must have C = 1
                
            }
            else{ //value == 1
                if (this -> getValueFromPort(1) == 1 || this -> getValueFromPort(2) == 1) mandatory.emplace_back(node, 3, 0, false); // A|B = 1, we must have C = 0
                else optional.emplace_back(node, 3, 0, false);
            }

        }        
        else if ( (this -> getValueFromPort(1) != -1 || this -> getValueFromPort(2) != -1) && this -> getValueFromPort(3) == -1) { //parent 1 or 2 is computed
            //find the input to compute (between 1 and 2)
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
                if (computedvalue == 0) {
                    mandatory.emplace_back(node, 3, 1, false);
                    mandatory.emplace_back(node, port_to_compute, 1, false);
                }
                else { //computedValue = 1
                    mandatory.emplace_back(node, 3, 1, false);
                    optional.emplace_back(node, port_to_compute, 1, false);
                }
            }
            else { //value == 1
                if (computedvalue == 0) {
                    optional.emplace_back(node, 3, 0, false);
                    optional.emplace_back(node, port_to_compute, 0, false);
                }
                else { //computedValue = 1
                    mandatory.emplace_back(node, 3, 0, false);
                    optional.emplace_back(node, port_to_compute, 0, false);
                }
            }
            
                
        }    
        else if (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1 && this -> getValueFromPort(3) != -1) { //only input 3 computed
            if (value == 0) {
                if (this -> getValueFromPort(3) == 0) return false; //we are computing a 1
                else { // C=1
                    optional.emplace_back(node, 1, 1, false);
                    optional.emplace_back(node, 2, 1, false);
                }
            }
            else { //value == 1
                if (this -> getValueFromPort(3) == 0) {
                    optional.emplace_back(node, 1, 0, false);
                    optional.emplace_back(node, 2, 0, false);
                }
                else {
                    mandatory.emplace_back(node, 1, 0, false);
                    mandatory.emplace_back(node, 2, 0, false);
                }
            }
        }        
        else { //parent 3 computed and parent 1 or 2 is computed
            //find the input to compute (between 1 and 2)
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

            if (value == 0) {
                if (this -> getValueFromPort(3) == 0 ) return false; //we are computing a 1
                else {
                    if (computedvalue == 1) optional.emplace_back(node, port_to_compute, 1, false);
                    else mandatory.emplace_back(node, port_to_compute, 1, false);
                }
            }
            else { //value == 1
                if (this -> getValueFromPort(3) == 0 ) optional.emplace_back(node, port_to_compute, 0, false);
                else { //C=1
                    if (computedvalue == 1) return false; //we are computing a 0
                    else mandatory.emplace_back(node, port_to_compute, 0, false);
                }
            }
        }
        
        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }  
    else if (port_number == 1 || port_number == 2) {
        int other_port;
        int current_port;
        if (port_number == 1) {
            other_port = 2;
            current_port = 1;
        } 
        else {
            other_port = 1;
            current_port = 2;
        }

        if ( this -> getValueFromPort(current_port) != -1) { 
            if (this -> getValueFromPort(current_port) != value) { 
                return false; 
            }
        return true;
        }

        if (propagate) {
                mandatory.emplace_back(node, other_port, 0, false);
                mandatory.emplace_back(node, 3, 1, false);
                if (value == 0) mandatory.emplace_back(node, -1, 1, true);
                else mandatory.emplace_back(node, -1, 0, true);
            }
        else if (this -> getValueFromPort(other_port) != -1 && this -> getValueFromPort(3) != -1 && this -> getValueFromPort(-1) != -1 ){ //all other port computed
            //only checking
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(3) != 1 || (this -> getValueFromPort(other_port) != 1 && value != 1)) return false; 
            }
            else { //out = 1
                if (this -> getValueFromPort(3) == 1 && (this -> getValueFromPort(other_port) == 1 || value == 1)) return false; 
            }
        }   
        else if (this -> getValueFromPort(other_port) != -1 && this -> getValueFromPort(3) == -1 && this -> getValueFromPort(-1) == -1 ){ //only other_port computed
            if (this -> getValueFromPort(other_port) == 0 && value == 0) mandatory.emplace_back(node, -1, 1, propagate);
            //FIXME Do I need to propagate ?
        }    
        else if (this -> getValueFromPort(other_port) == -1 && this -> getValueFromPort(3) != -1 && this -> getValueFromPort(-1) == -1 ){ //only port 3 computed
            if (value == 1 && this -> getValueFromPort(3) == 1) mandatory.emplace_back(node, -1, 0, propagate);
        }     
        else if (this -> getValueFromPort(other_port) == -1 && this -> getValueFromPort(3) == -1 && this -> getValueFromPort(-1) != -1 ){ //only out computed
            if (this -> getValueFromPort(-1) == 1) {
                if (value == 1) mandatory.emplace_back(node, 3, 0, propagate);
            }
            else { //out=0
                if (value == 0) mandatory.emplace_back(node, 2, 1, propagate); //no need to add port 3 because it is already there
            }
        }    
        else if (this -> getValueFromPort(other_port) != -1 && this -> getValueFromPort(3) != -1 && this -> getValueFromPort(-1) == -1 ){ //input other_port and 3 computed
            if (this -> getValueFromPort(3) == 1 && (this -> getValueFromPort(other_port) == 1 || value == 1)) mandatory.emplace_back(node, -1, 0, propagate);
            else mandatory.emplace_back(node, -1, 1, propagate);
        }        
        else if (this -> getValueFromPort(other_port) != -1 && this -> getValueFromPort(3) == -1 && this -> getValueFromPort(-1) != -1 ){ //output and other_port computed
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(other_port) != 1 && value != 1) return false;
            }
            else { //out == 1
                if (this -> getValueFromPort(other_port) == 1 || value == 1) mandatory.emplace_back(node, 3, 0, false);
            }
        }    
        else { //input 3 and output computed
            if (this -> getValueFromPort(-1) == 0) {
                if (this -> getValueFromPort(3) == 0) return false;
                else { //C=1
                    if (value == 0) mandatory.emplace_back(node, 2, 1, propagate);
                }
            }
            else { //out == 1
                if (this -> getValueFromPort(3) == 1){
                    if (value == 1) return false; //we are computing a one
                    else mandatory.emplace_back(node, 2, 0, propagate);
                }
                //c=0, nothing to do
            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(current_port), -1, value, false); //send to the parent
        this -> updatePort(current_port, value);
    }
    else  { //port_number == 3
        if ( this -> getValueFromPort(3) != -1) { 
            if (this -> getValueFromPort(3) != value) return false; 
        return true;
        }

        if (propagate) {
                mandatory.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 1, false);
                if (value == 0) mandatory.emplace_back(node, -1, 1, true);
                else mandatory.emplace_back(node, -1, 0, true);
            }
        else if ( this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) != -1) { //all computed
            if (this -> getValueFromPort(-1) == 0) {
                if (value != 1 || (this -> getValueFromPort(1) != 1 && this -> getValueFromPort(2) != 1) ) return false;
            }
            else { //out=1
                if (value == 1 && (this -> getValueFromPort(1) == 1 || this -> getValueFromPort(2) == 1) ) return false;
            }
        }      
        else if ( this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(-1) == -1) { //we have input 1 and input 2
            if (value == 1 && (this -> getValueFromPort(1) == 1 || this -> getValueFromPort(2) == 1) ) mandatory.emplace_back(node, -1, 0, propagate);
            else mandatory.emplace_back(node, -1, 1, propagate);    
        }       
        else if ( (this -> getValueFromPort(1) != -1 || this -> getValueFromPort(2) != -1) && this -> getValueFromPort(-1) == -1) { //we have input 1 or input 2
            //get computed value and port to compute
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) == -1) {
                port_to_compute = 1;
                computedvalue = this -> getValueFromPort(2);
            }
            else {
                port_to_compute = 2;
                computedvalue = this -> getValueFromPort(1);
            }
            
            if (value == 0) mandatory.emplace_back(node, -1, 1, propagate);
            else { //value = 1
                if (computedvalue == 1) mandatory.emplace_back(node, -1, 0, propagate);
            }
        }
        else if ( this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1 && this -> getValueFromPort(-1) != -1){//only out
            if (this -> getValueFromPort(-1) == 0){//out = 0
                if (value == 0) return false; //we are computing a 1
            }
            else {//out = 1
                if (value == 1){
                    mandatory.emplace_back(node, 1, 0, propagate);
                    mandatory.emplace_back(node, 2, 0, propagate);
                }
            }
        }
        else { //out is computed and A or B is computed
            //get computed value and port to compute
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) == -1) {
                port_to_compute = 1;
                computedvalue = this -> getValueFromPort(2);
            }
            else {
                port_to_compute = 2;
                computedvalue = this -> getValueFromPort(1);
            }

            if (this -> getValueFromPort(-1) == 0) { //out = 0
                if (value == 0) return false;
            }
            else { //out = 1
                if (value == 1) {
                    if (computedvalue == 1) return false;
                    else mandatory.emplace_back(node, port_to_compute, 0, propagate);
                }
                 
            }
        }

        //send to parent
        mandatory.emplace_back(this -> getNodeFromPort(3), -1, value, false); //send to the parent
        this -> updatePort(3, value);
    }

    return true;
}

// AOI4 implementation -----------------------------------------------

AOI4::AOI4(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_AOI4_";
    this -> portValues.emplace_back( 1, -1, false);
    this -> portValues.emplace_back( 2, -1, false);
    this -> portValues.emplace_back( 3, -1, false);
    this -> portValues.emplace_back( 4, -1, false);
    this -> portValues.emplace_back(-1, -1, false);
}

std::shared_ptr<Node> AOI4::getSharedPtrToThis() {
    return std::make_shared<AOI4>(*this);
}

void AOI4::printNodeRecursive() {
    std::cout << "node type: AOI4 id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> AOI4::computeInputFromOutput(int value){
    //no need of parent's number because we are writing the same number for each parent
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
        result.push_back(std::make_pair(this->parents[2].first, 1));
        result.push_back(std::make_pair(this->parents[3].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 0));
        result.push_back(std::make_pair(this->parents[2].first, 0));
        result.push_back(std::make_pair(this->parents[3].first, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an AOI4 cell" << std::endl;
    }

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> AOI4::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port
    if (port_number != 1 && port_number != 2 && port_number != 3 && port_number != 4) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an AOI3" << std::endl;

    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an AOI3" << std::endl;

    //get p1, p2 and p3
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;
    std::shared_ptr<Node> p4;

    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this->parents[0].second == 3) p3 = this->parents[0].first;
    else if (this->parents[0].second == 4) p4 = this->parents[0].first;
    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this->parents[1].second == 3) p3 = this->parents[1].first;
    else if (this->parents[1].second == 4) p4 = this->parents[1].first;
    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this->parents[2].second == 3) p3 = this->parents[2].first;
    else if (this->parents[2].second == 4) p4 = this->parents[2].first;
    if (this->parents[3].second == 1) p1 = this->parents[3].first;
    else if (this->parents[3].second == 2) p2 = this->parents[3].first;
    else if (this->parents[3].second == 3) p3 = this->parents[3].first;
    else if (this->parents[3].second == 4) p4 = this->parents[3].first;

    if (port_number == 1) {
        result.emplace_back(p2, 1, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p4, 0, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); 
    }
    else if (port_number == 2){
        result.emplace_back(p1, 1, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p4, 0, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); 
    }
    else if (port_number == 3){
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p4, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (port_number == 4){
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p3, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1)for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    return result;
}

bool AOI4::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != 3 && port_number != 4 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for an AOI4 cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an AOI4 cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if      (this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1 && this -> getValueFromPort(3) == -1 && this -> getValueFromPort(4) == -1) { //inputs not computed
            if (value == 1) {
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 0, false);
                optional.emplace_back(node, 3, 0, false);
                optional.emplace_back(node, 4, 0, false);
            }
            else if (value == 0){
                optional.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 1, false);
                optional.emplace_back(node, 3, 1, false);
                optional.emplace_back(node, 4, 1, false);
            }
        }
        else if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1 && this -> getValueFromPort(3) != -1 && this -> getValueFromPort(4) != -1) { //all input computed
            if (value == 0){
                if ( (this -> getValueFromPort(1) == 0 || this -> getValueFromPort(2) == 0) && (this -> getValueFromPort(3) == 0 || this -> getValueFromPort(4) == 0) ) return false;
            }
            else { //value=1
                if ( (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) || (this -> getValueFromPort(3) == 1 && this -> getValueFromPort(4) == 1) ) return false;
            }
        }      
        else if ( ((this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1) && (this -> getValueFromPort(3) != -1 || this -> getValueFromPort(4) != -1)) 
               || ((this -> getValueFromPort(1) != -1 || this -> getValueFromPort(2) != -1) && (this -> getValueFromPort(3) != -1 && this -> getValueFromPort(4) != -1)) ) { //3 inputs computed 
            
            //get the value from the computed pair and the computed value from the not computed pair
            int computedpair;
            int computedvalue;
            int port_to_compute;
            if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1){//the first pair is the computed pair
                //get computed pair
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) computedpair = 1;
                else computedpair = 0;

                //get computed value and port to compute
                if (this -> getValueFromPort(3) == -1) {
                    port_to_compute = 3;
                    computedvalue = this -> getValueFromPort(4);
                }
                else {
                    port_to_compute = 4;
                    computedvalue = this -> getValueFromPort(3);
                }
            }
            else { //the second pair is the computed pair
                //get computed pair
                if (this -> getValueFromPort(3) == 1 && this -> getValueFromPort(4) == 1) computedpair = 1;
                else computedpair = 0;

                //get computed value and port to compute
                if (this -> getValueFromPort(1) == -1) {
                    port_to_compute = 1;
                    computedvalue = this -> getValueFromPort(2);
                }
                else {
                    port_to_compute = 2;
                    computedvalue = this -> getValueFromPort(1);
                }

            }

            if (value == 0){
                if (computedpair == 0){
                    if (computedvalue == 0) return false;
                    else mandatory.emplace_back(node, port_to_compute, 1, false);
                }
                else optional.emplace_back(node, port_to_compute, 1, false); 
                }
            else { //value == 1
                if (computedpair == 0){
                    if (computedvalue == 0) optional.emplace_back(node, port_to_compute, 0, false);
                    else mandatory.emplace_back(node, port_to_compute, 0, false);
                }
                else return false;
            }
        }
        else if ( ((this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1) && (this -> getValueFromPort(3) == -1 && this -> getValueFromPort(4) == -1)) 
               || ((this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) && (this -> getValueFromPort(3) != -1 && this -> getValueFromPort(4) != -1)) ) { //1 pair of input computed 
            
            //get the value from the computed pair and the computed value from the not computed pair
            int computedpair;
            int port_to_compute1;
            int port_to_compute2;
            if (this -> getValueFromPort(1) != -1 && this -> getValueFromPort(2) != -1){//the first pair is the computed pair
                //get computed pair
                if (this -> getValueFromPort(1) == 1 && this -> getValueFromPort(2) == 1) computedpair = 1;
                else computedpair = 0;

                port_to_compute1 = 3;
                port_to_compute2 = 4;
                
            }
            else { //the second pair is the computed pair
                //get computed pair
                if (this -> getValueFromPort(3) == 1 && this -> getValueFromPort(4) == 1) computedpair = 1;
                else computedpair = 0;

                port_to_compute1 = 1;
                port_to_compute2 = 2;

            }

            if (value == 0){
                if (computedpair == 0){
                    mandatory.emplace_back(node, port_to_compute1, 1, false);
                    mandatory.emplace_back(node, port_to_compute2, 1, false);
                }
                else {
                    optional.emplace_back(node, port_to_compute1, 1, false);
                    optional.emplace_back(node, port_to_compute2, 1, false);
                }
                }
            else { //value == 1
                if (computedpair == 0){
                    optional.emplace_back(node, port_to_compute1, 0, false);
                    optional.emplace_back(node, port_to_compute2, 0, false);
                }
                else return false;
            }
        }
        else if ( ((this -> getValueFromPort(1) != -1 || this -> getValueFromPort(2) != -1) && (this -> getValueFromPort(3) == -1 && this -> getValueFromPort(4) == -1)) 
               || ((this -> getValueFromPort(1) == -1 && this -> getValueFromPort(2) == -1) && (this -> getValueFromPort(3) != -1 || this -> getValueFromPort(4) != -1)) ) { //1 input computed 
            
            //get the value from the computed pair and the computed value from the not computed pair
            int computedvalue;
            int port_to_compute_alone;
            int port_to_compute2;
            int port_to_compute3;
            if (this -> getValueFromPort(1) != -1 || this -> getValueFromPort(2) != -1){//the computed input is in the first pair
                //get computed pair
                if (this -> getValueFromPort(1) != -1){ //input number 1 computed
                    computedvalue = this -> getValueFromPort(1);
                    port_to_compute_alone = 2;
                } 
                else{ //input number 2 computed
                    computedvalue = this -> getValueFromPort(2);
                    port_to_compute_alone = 1;
                }

                port_to_compute2 = 3;
                port_to_compute3 = 4;
                
            }
            else { //the computed input is in the second pair
                if (this -> getValueFromPort(3) != -1){//input number 3 computed
                    computedvalue = this -> getValueFromPort(3);
                    port_to_compute_alone = 4;
                } 
                else{ //input number 4 computed
                    computedvalue = this -> getValueFromPort(4);
                    port_to_compute_alone = 3;
                }

                port_to_compute2 = 1;
                port_to_compute3 = 2;

            }

            if (value == 0){
                if (computedvalue == 0){
                    mandatory.emplace_back(node, port_to_compute2, 1, false);
                    mandatory.emplace_back(node, port_to_compute3, 1, false);
                    optional.emplace_back(node, port_to_compute_alone, 1, false);
                }
                else {
                    optional.emplace_back(node, port_to_compute2, 1, false);
                    optional.emplace_back(node, port_to_compute3, 1, false);
                    optional.emplace_back(node, port_to_compute_alone, 1, false);
                }
            }
            else { //value == 1
                if (computedvalue == 0){
                    optional.emplace_back(node, port_to_compute2, 0, false);
                    optional.emplace_back(node, port_to_compute3, 0, false);
                    optional.emplace_back(node, port_to_compute_alone, 0, false);
                }
                else {
                    optional.emplace_back(node, port_to_compute2, 0, false);
                    optional.emplace_back(node, port_to_compute3, 0, false);
                    mandatory.emplace_back(node, port_to_compute_alone, 0, false);
                }
            }
        }        
        else { //1 in each pair computed 
            
            //get the two computed values and the two ports to compute
            int computedvalue1;
            int computedvalue2;
            int port_to_compute1;
            int port_to_compute2;         
            if (this -> getValueFromPort(1) != -1){ //input number 1 computed
                computedvalue1 = this -> getValueFromPort(1);
                port_to_compute1 = 2;
            } 
            else{ //input number 2 computed
                computedvalue1 = this -> getValueFromPort(2);
                port_to_compute1 = 1;
            }
            if (this -> getValueFromPort(3) != -1){ //input number 3 computed
                computedvalue2 = this -> getValueFromPort(3);
                port_to_compute2 = 4;
            } 
            else{ //input number 4 computed
                computedvalue2 = this -> getValueFromPort(4);
                port_to_compute2 = 3;
            }

            if (value == 0){
                if (computedvalue1 == 0){
                    if (computedvalue2 == 0) return false;
                    else{
                        mandatory.emplace_back(node, port_to_compute2, 1, false);
                        optional.emplace_back(node, port_to_compute1, 1, false);
                    }
                }
                else{//computedvalue1 == 1
                    if (computedvalue2 == 0){
                        mandatory.emplace_back(node, port_to_compute1, 1, false);
                        optional.emplace_back(node, port_to_compute2, 1, false);
                    }
                    else{
                        optional.emplace_back(node, port_to_compute1, 1, false);
                        optional.emplace_back(node, port_to_compute2, 1, false);
                    }
                }
            }
            else { //value == 1
                if (computedvalue1 == 0) optional.emplace_back(node, port_to_compute1, 0, false);
                else mandatory.emplace_back(node, port_to_compute1, 0, false);
                if (computedvalue2 == 0) optional.emplace_back(node, port_to_compute2, 0, false);
                else mandatory.emplace_back(node, port_to_compute2, 0, false);
            }
        }

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }  
    else {//we are computing one of the input
        //get the other port
        int port_to_compute_alone;
        int port_to_compute1;
        int port_to_compute2;
        if (port_number == 1){
            port_to_compute_alone = 2;
            port_to_compute1 = 3;
            port_to_compute2 = 4;
        }
        else if (port_number == 2){
            port_to_compute_alone = 1;
            port_to_compute1 = 3;
            port_to_compute2 = 4;
        }
        else if (port_number == 3){
            port_to_compute_alone = 4;
            port_to_compute1 = 1;
            port_to_compute2 = 2;
        }
        else{
            port_to_compute_alone = 3;
            port_to_compute1 = 1;
            port_to_compute2 = 2;
        }

        if ( this -> getValueFromPort(port_number) != -1) { 
            if (this -> getValueFromPort(port_number) != value) { 
                return false; 
            }
        return true;
        }

        if (propagate) { //propagate done
            mandatory.emplace_back(node, port_to_compute_alone, 1, false);
            optional.emplace_back(node, port_to_compute2, 0, false);
            mandatory.emplace_back(node, port_to_compute1, 0, false);
            if (value == 0) mandatory.emplace_back(node, -1, 1, true);
            else mandatory.emplace_back(node, -1, 0, true);
        }
        else if (this -> getValueFromPort(-1) == -1){ //out not computed
            if (this -> getValueFromPort(port_to_compute_alone) != -1 &&  this -> getValueFromPort(port_to_compute1) != -1 && this -> getValueFromPort(port_to_compute2) != -1){ //all input computed
                if ( (this -> getValueFromPort(port_to_compute_alone) == 1 && value == 1) || (this -> getValueFromPort(port_to_compute1) == 1 && this -> getValueFromPort(port_to_compute2) == 1) ) mandatory.emplace_back(node, -1, 0, false);
                else mandatory.emplace_back(node, -1, 1, false);
            }
            else if (this -> getValueFromPort(port_to_compute_alone) == -1 &&  this -> getValueFromPort(port_to_compute1) != -1 && this -> getValueFromPort(port_to_compute2) != -1){ //only port_to_compute_alone not computed
                if ( this -> getValueFromPort(port_to_compute1) != 1 || this -> getValueFromPort(port_to_compute2) != 1) {
                    if (value == 0) mandatory.emplace_back(node, -1, 1, false);
                }
            }
            else if (this -> getValueFromPort(port_to_compute_alone) != -1 &&  this -> getValueFromPort(port_to_compute1) == -1 && this -> getValueFromPort(port_to_compute2) == -1){//only port_to_compute_alone computed
                if (this -> getValueFromPort(port_to_compute_alone) == 1 && value == 1) mandatory.emplace_back(node, -1, 0, false);
            }
            else if (this -> getValueFromPort(port_to_compute_alone) != -1 && (this -> getValueFromPort(port_to_compute1) != -1 || this -> getValueFromPort(port_to_compute2) != -1)){//port_to_compute_alone and one of the other computed
                //get the computed value and the port to compute
                int computedvalue;
                int port_to_compute;
                if (this -> getValueFromPort(port_to_compute1) == -1) {
                    port_to_compute = port_to_compute1;
                    computedvalue = this -> getValueFromPort(port_to_compute2);
                }
                else {
                    port_to_compute = port_to_compute2;
                    computedvalue = this -> getValueFromPort(port_to_compute1);
                }
            
                if ( (this -> getValueFromPort(port_to_compute_alone) == 0 || value == 0) && computedvalue == 0) mandatory.emplace_back(node, -1, 1, false);
                else if (this -> getValueFromPort(port_to_compute_alone) == 1 && value == 1)  mandatory.emplace_back(node, -1, 0, false);
            }
            else if (this -> getValueFromPort(port_to_compute_alone) == -1 && (this -> getValueFromPort(port_to_compute1) != -1 || this -> getValueFromPort(port_to_compute2) != -1)){
                //get the computed value and the port to compute
                int computedvalue;
                int port_to_compute;
                if (this -> getValueFromPort(port_to_compute1) == -1) {
                    port_to_compute = port_to_compute1;
                    computedvalue = this -> getValueFromPort(port_to_compute2);
                }
                else {
                    port_to_compute = port_to_compute2;
                    computedvalue = this -> getValueFromPort(port_to_compute1);
                }
            
                if (computedvalue == 0 && value == 0) mandatory.emplace_back(node, -1, 1, false);
            }
            else std::cerr << "Error: a problem occured when out not computed in AOI4" << std::endl;
        }
        else{ //out computed
            int out = this -> getValueFromPort(-1);
            if      (this -> getValueFromPort(port_to_compute_alone) != -1 &&  this -> getValueFromPort(port_to_compute1) != -1 && this -> getValueFromPort(port_to_compute2) != -1){ //all input computed
                if (out == 0){
                    if ( (this -> getValueFromPort(port_to_compute_alone) != 1 || value != 1) && (this -> getValueFromPort(port_to_compute1) != 1 || this -> getValueFromPort(port_to_compute2) != 1) ) return false;
                }
                else { //out=1
                    if ( (this -> getValueFromPort(port_to_compute_alone) == 1 && value == 1) || (this -> getValueFromPort(port_to_compute1) == 1 && this -> getValueFromPort(port_to_compute2) == 1) ) return false;
                }
            }       
            else if (this -> getValueFromPort(port_to_compute_alone) == -1 &&  this -> getValueFromPort(port_to_compute1) != -1 && this -> getValueFromPort(port_to_compute2) != -1){ //only port_to_compute_alone not computed
                if ( out == 0) {
                    if (this -> getValueFromPort(port_to_compute1) != 1 || this -> getValueFromPort(port_to_compute2) != -1){ //C&&D=0
                        if (value == 0) return false;
                        else mandatory.emplace_back(node, port_to_compute_alone, 1, false);
                    }
                }
                else { //out=1
                    if (this -> getValueFromPort(port_to_compute1) != 1 || this -> getValueFromPort(port_to_compute2) != -1){ //C&&D=0
                        if (value == 1) mandatory.emplace_back(node, port_to_compute_alone, 0, false);
                    }
                }
            }           
            else if (this -> getValueFromPort(port_to_compute_alone) != -1 &&  this -> getValueFromPort(port_to_compute1) == -1 && this -> getValueFromPort(port_to_compute2) == -1){//only port_to_compute_alone computed
                if (out == 0){
                    if (this -> getValueFromPort(port_to_compute_alone) == 0 || value == 0) {
                        mandatory.emplace_back(node, port_to_compute1, 1, false);
                        mandatory.emplace_back(node, port_to_compute2, 1, false);
                    }
                }
                else {//out=1
                    if (this -> getValueFromPort(port_to_compute_alone) == 1 && value == 1) return false;
                }
            }            
            else if (this -> getValueFromPort(port_to_compute_alone) != -1 && (this -> getValueFromPort(port_to_compute1) != -1 || this -> getValueFromPort(port_to_compute2) != -1)){//port_to_compute_alone and one of the other computed
                //get the computed value and the port to compute
                int computedvalue;
                int port_to_compute;
                if (this -> getValueFromPort(port_to_compute1) == -1) {
                    port_to_compute = port_to_compute1;
                    computedvalue = this -> getValueFromPort(port_to_compute2);
                }
                else {
                    port_to_compute = port_to_compute2;
                    computedvalue = this -> getValueFromPort(port_to_compute1);
                }
            
                if (out == 0) {
                    if (this -> getValueFromPort(port_to_compute_alone) == 0 || value == 0){
                        if (computedvalue == 0) return false;
                        else mandatory.emplace_back(node, port_to_compute, 1, false);
                    }
                }
                else { //out=1
                    if (this -> getValueFromPort(port_to_compute_alone) == 1 && value == 1) return false;
                    else {
                        if (computedvalue == 1) mandatory.emplace_back(node, port_to_compute, 0, false);
                    }
                }
            }            
            else if (this -> getValueFromPort(port_to_compute_alone) == -1 && (this -> getValueFromPort(port_to_compute1) != -1 || this -> getValueFromPort(port_to_compute2) != -1)){//onlyy one inout of the other pair computed
                //get the computed value and the port to compute
                int computedvalue;
                int port_to_compute;
                if (this -> getValueFromPort(port_to_compute1) == -1) {
                    port_to_compute = port_to_compute1;
                    computedvalue = this -> getValueFromPort(port_to_compute2);
                }
                else {
                    port_to_compute = port_to_compute2;
                    computedvalue = this -> getValueFromPort(port_to_compute1);
                }
            
                if (out == 0){
                    if (computedvalue == 0){
                        if (value == 0) return false;
                        else mandatory.emplace_back(node, port_to_compute_alone, 1, false);
                    }
                    else{
                        if (value == 0) mandatory.emplace_back(node, port_to_compute, 1, false);
                    }
                }
                else{//out=1
                    if (value == 1) mandatory.emplace_back(node, port_to_compute_alone, 0, false);
                }
            }
            else if (this -> getValueFromPort(port_to_compute_alone) == -1 &&  this -> getValueFromPort(port_to_compute1) == -1 && this -> getValueFromPort(port_to_compute2) == -1) { //nothing else computed
                if (out == 0){
                    if (value == 0) {
                        mandatory.emplace_back(node, port_to_compute1, 1, false);
                        mandatory.emplace_back(node, port_to_compute2, 1, false);
                    }
                }
            }
            else std::cerr << "Error: a problem occured when out computed in AOI4" << std::endl;
        }
        mandatory.emplace_back(this -> getNodeFromPort(port_number), -1, value, false); //send to the parent
        this -> updatePort(port_number, value);
    }

    return true;
}

// OAI4 implementation -----------------------------------------------

OAI4::OAI4(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_OAI4_";
    this -> portValues.emplace_back( 1, -1, false);
    this -> portValues.emplace_back( 2, -1, false);
    this -> portValues.emplace_back( 3, -1, false);
    this -> portValues.emplace_back( 4, -1, false);
    this -> portValues.emplace_back(-1, -1, false);
}

std::shared_ptr<Node> OAI4::getSharedPtrToThis() {
    return std::make_shared<OAI4>(*this);
}

void OAI4::printNodeRecursive() {
    std::cout << "node type: OAI4 id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> OAI4::computeInputFromOutput(int value){
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(this->parents[0].first, 1));
        result.push_back(std::make_pair(this->parents[1].first, 1));
        result.push_back(std::make_pair(this->parents[2].first, 1));
        result.push_back(std::make_pair(this->parents[3].first, 1));
    }
    else if (value == 1){
        result.push_back(std::make_pair(this->parents[0].first, 0));
        result.push_back(std::make_pair(this->parents[1].first, 0));
        result.push_back(std::make_pair(this->parents[2].first, 0));
        result.push_back(std::make_pair(this->parents[3].first, 0));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for an OAI4 cell" << std::endl;
    }

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> OAI4::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port
    if (port_number != 1 && port_number != 2 && port_number != 3 && port_number != 4) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for an AOI3" << std::endl;

    //check the value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for an AOI3" << std::endl;

    //get p1, p2 and p3
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;
    std::shared_ptr<Node> p4;

    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this->parents[0].second == 3) p3 = this->parents[0].first;
    else if (this->parents[0].second == 4) p4 = this->parents[0].first;
    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this->parents[1].second == 3) p3 = this->parents[1].first;
    else if (this->parents[1].second == 4) p4 = this->parents[1].first;
    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this->parents[2].second == 3) p3 = this->parents[2].first;
    else if (this->parents[2].second == 4) p4 = this->parents[2].first;
    if (this->parents[3].second == 1) p1 = this->parents[3].first;
    else if (this->parents[3].second == 2) p2 = this->parents[3].first;
    else if (this->parents[3].second == 3) p3 = this->parents[3].first;
    else if (this->parents[3].second == 4) p4 = this->parents[3].first;

    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p3, 1, -1);
        result.emplace_back(p4, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); 
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p3, 1, -1);
        result.emplace_back(p4, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); 
    }
    else if (port_number == 3){
        result.emplace_back(p1, 1, -1);
        result.emplace_back(p2, 1, -1);
        result.emplace_back(p4, 0, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); 
    }
    else if (port_number == 4){
        result.emplace_back(p1, 1, -1);
        result.emplace_back(p2, 1, -1);
        result.emplace_back(p3, 0, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); 
    }
    
    return result;
}

bool OAI4::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != 3 && port_number != 4 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for an OAI4 cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for an OAI4 cell" << std::endl;

    if (port_number == -1) {
        if ( this -> getValueFromPort(-1) != -1) { //it has already been computed
            if (this -> getValueFromPort(-1) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        //get value from the port
        int valuePort1 = this -> getValueFromPort(1);
        int valuePort2 = this -> getValueFromPort(2);
        int valuePort3 = this -> getValueFromPort(3);
        int valuePort4 = this -> getValueFromPort(4);

        if      (   valuePort1 == -1 && valuePort2 == -1  &&  valuePort3 == -1 && valuePort4 == -1   ) { //inputs not computed
            if (value == 0) {
                optional.emplace_back(node, 1, 1, false);
                optional.emplace_back(node, 2, 1, false);
                optional.emplace_back(node, 3, 1, false);
                optional.emplace_back(node, 4, 1, false);
            }
            else { //value=1
                optional.emplace_back(node, 1, 0, false);
                optional.emplace_back(node, 2, 0, false);
                optional.emplace_back(node, 3, 0, false);
                optional.emplace_back(node, 4, 0, false);
            }
        }
        else if (   valuePort1 != -1 && valuePort2 != -1  &&  valuePort3 != -1 && valuePort4 != -1   ) { //all input computed
            if (value == 0){
                if ( (valuePort1 != 1 && valuePort2 != 1) || (valuePort3 != 1 && valuePort4 != 1) ) return false;
            }
            else { //value=1
                if ( (valuePort1 == 1 || valuePort2 == 1) && (valuePort3 == 1 || valuePort4 == 1) ) return false;
            }
        }              
        else if ( ((valuePort1 != -1 || valuePort2 != -1) &&  valuePort3 != -1 && valuePort4 != -1 ) 
               || ( valuePort1 != -1 && valuePort2 != -1  && (valuePort3 != -1 || valuePort4 != -1)) ) { //3 inputs computed 
            
            //get the value from the computed pair and the computed value from the not computed pair
            int computedpair;
            int computedvalue;
            int port_to_compute;
            if (valuePort1 != -1 && valuePort2 != -1){//the first pair is the computed pair
                //get computed pair
                if (valuePort1 == 1 || valuePort2 == 1) computedpair = 1;
                else computedpair = 0;

                //get computed value and port to compute
                if (valuePort3 == -1) {
                    port_to_compute = 3;
                    computedvalue = valuePort4;
                }
                else {
                    port_to_compute = 4;
                    computedvalue = valuePort3;
                }
            }
            else { //the second pair is the computed pair
                //get computed pair
                if (valuePort3 == 1 || valuePort4 == 1) computedpair = 1;
                else computedpair = 0;

                //get computed value and port to compute
                if (valuePort1 == -1) {
                    port_to_compute = 1;
                    computedvalue = valuePort2;
                }
                else {
                    port_to_compute = 2;
                    computedvalue = valuePort1;
                }

            }

            if (value == 0){
                if (computedpair == 0) return false;
                else { //computedpair == 1
                    if (computedvalue == 0) mandatory.emplace_back(node, port_to_compute, 1, false);
                    else optional.emplace_back(node, port_to_compute, 1, false);
                }
            }
            else { //value == 1
                if (computedpair == 0) optional.emplace_back(node, port_to_compute, 0, false);
                else { //computedPair = 1
                    if (computedvalue == 0) mandatory.emplace_back(node, port_to_compute, 0, false);
                    else return false;
                }
            }
        }
        else if (  (valuePort1 != -1 && valuePort2 != -1) || (valuePort3 != -1 && valuePort4 != -1)  ) { //1 pair of input computed 
            
            //get the value from the computed pair and the computed value from the not computed pair
            int computedpair;
            int port_to_compute1;
            int port_to_compute2;
            if (valuePort1 != -1 && valuePort2 != -1){//the first pair is the computed pair
                //get computed pair
                if (valuePort1 == 1 || valuePort2 == 1) computedpair = 1;
                else computedpair = 0;

                port_to_compute1 = 3;
                port_to_compute2 = 4;
                
            }
            else { //the second pair is the computed pair
                //get computed pair
                if (valuePort3 == 1 || valuePort4 == 1) computedpair = 1;
                else computedpair = 0;

                port_to_compute1 = 1;
                port_to_compute2 = 2;

            }

            if (value == 0){
                if (computedpair == 0) return false;
                else {
                    optional.emplace_back(node, port_to_compute1, 1, false);
                    optional.emplace_back(node, port_to_compute2, 1, false);
                }
                }
            else { //value == 1
                if (computedpair == 0){
                    optional.emplace_back(node, port_to_compute1, 0, false);
                    optional.emplace_back(node, port_to_compute2, 0, false);
                }
                else {
                    mandatory.emplace_back(node, port_to_compute1, 0, false);
                    mandatory.emplace_back(node, port_to_compute2, 0, false);
                }
            }
        }
        else if (  (valuePort1 != -1 || valuePort2 != -1) && (valuePort3 != -1 || valuePort4 != -1)  ) { //1 port in each pair computed 
            
            //get the two computed values and the two ports to compute
            int computedvalue1;
            int computedvalue2;
            int port_to_compute1;
            int port_to_compute2;         
            if (valuePort1 != -1){ //input number 1 computed
                computedvalue1 = valuePort1;
                port_to_compute1 = 2;
            } 
            else{ //input number 2 computed
                computedvalue1 = valuePort2;
                port_to_compute1 = 1;
            }
            if (valuePort3 != -1){ //input number 3 computed
                computedvalue2 = valuePort3;
                port_to_compute2 = 4;
            } 
            else{ //input number 4 computed
                computedvalue2 = valuePort4;
                port_to_compute2 = 3;
            }

            if (value == 0){
                if (computedvalue1 == 0) mandatory.emplace_back(node, port_to_compute1, 1, false);
                else optional.emplace_back(node, port_to_compute1, 1, false);
                if (computedvalue2 == 0) mandatory.emplace_back(node, port_to_compute2, 1, false);
                else optional.emplace_back(node, port_to_compute2, 1, false);
            }
            else { //value == 1
                if (computedvalue1 == 1 && computedvalue2 == 1) return false;
            }
        }
        else if (   valuePort1 != -1 || valuePort2 != -1  ||  valuePort3 != -1 || valuePort4 != -1   ) { //1 input computed 
            
            //get the value from the computed pair and the computed value from the not computed pair
            int computedvalue;
            int port_to_compute_alone;
            int port_to_compute2;
            int port_to_compute3;
            if (valuePort1 != -1 || valuePort2 != -1){//the computed input is in the first pair
                //get computed pair
                if (valuePort1 != -1){ //input number 1 computed
                    computedvalue = valuePort1;
                    port_to_compute_alone = 2;
                } 
                else{ //input number 2 computed
                    computedvalue = valuePort2;
                    port_to_compute_alone = 1;
                }

                port_to_compute2 = 3;
                port_to_compute3 = 4;
                
            }
            else { //the computed input is in the second pair
                if (valuePort3 != -1){//input number 3 computed
                    computedvalue = valuePort3;
                    port_to_compute_alone = 4;
                } 
                else{ //input number 4 computed
                    computedvalue = valuePort4;
                    port_to_compute_alone = 3;
                }
                port_to_compute2 = 1;
                port_to_compute3 = 2;

            }

            if (value == 0){
                if (computedvalue == 0){
                    optional.emplace_back(node, port_to_compute2, 1, false);
                    optional.emplace_back(node, port_to_compute3, 1, false);
                    mandatory.emplace_back(node, port_to_compute_alone, 1, false);
                }
                else {
                    optional.emplace_back(node, port_to_compute2, 1, false);
                    optional.emplace_back(node, port_to_compute3, 1, false);
                    optional.emplace_back(node, port_to_compute_alone, 1, false);
                }
            }
            else { //value == 1
                if (computedvalue == 0){
                    optional.emplace_back(node, port_to_compute2, 0, false);
                    optional.emplace_back(node, port_to_compute3, 0, false);
                    optional.emplace_back(node, port_to_compute_alone, 0, false);
                }
                else {
                    mandatory.emplace_back(node, port_to_compute2, 0, false);
                    mandatory.emplace_back(node, port_to_compute3, 0, false);
                    optional.emplace_back(node, port_to_compute_alone, 0, false);
                }
            }
        }        
        else std::cerr << "Error: a problem occured when we are computing out in OAI4" << std::endl;

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);
    }
    else {//we are computing one of the input
        //get the other ports
        int port_to_compute_alone;
        int port_to_compute1;
        int port_to_compute2;
        int valuePortAlone;
        int valuePort1;
        int valuePort2;
        int valueOut = this -> getValueFromPort(-1);
        if (port_number == 1){
            port_to_compute_alone = 2;
            valuePortAlone = this -> getValueFromPort(2);
            port_to_compute1 = 3;
            valuePort1 = this -> getValueFromPort(3);
            port_to_compute2 = 4;
            valuePort2 = this -> getValueFromPort(4);
        }
        else if (port_number == 2){
            port_to_compute_alone = 1;
            valuePortAlone = this -> getValueFromPort(1);
            port_to_compute1 = 3;
            valuePort1 = this -> getValueFromPort(3);
            port_to_compute2 = 4;
            valuePort2 = this -> getValueFromPort(4);
        }
        else if (port_number == 3){
            port_to_compute_alone = 4;
            valuePortAlone = this -> getValueFromPort(4);
            port_to_compute1 = 1;
            valuePort1 = this -> getValueFromPort(1);
            port_to_compute2 = 2;
            valuePort2 = this -> getValueFromPort(2);
        }
        else{
            port_to_compute_alone = 3;
            valuePortAlone = this -> getValueFromPort(3);
            port_to_compute1 = 1;
            valuePort1 = this -> getValueFromPort(1);
            port_to_compute2 = 2;
            valuePort2 = this -> getValueFromPort(2);
        }

        if (propagate) { //propagate done
            mandatory.emplace_back(node, port_to_compute_alone, 0, false);
            mandatory.emplace_back(node, port_to_compute2, 1, false);
            optional.emplace_back(node, port_to_compute1, 1, false);
            if (value == 0) mandatory.emplace_back(node, -1, 1, true);
            else mandatory.emplace_back(node, -1, 0, true);
        }
        else if (valueOut == -1){ //out not computed
            if      (valuePortAlone != -1 &&  valuePort1 != -1 && valuePort2 != -1) { //all input computed
                if ( (valuePortAlone == 1 || value == 1) && (valuePort1 == 1 || valuePort2 == 1) ) mandatory.emplace_back(node, -1, 0, false);
                else mandatory.emplace_back(node, -1, 1, false);
            }
            else if (valuePortAlone == -1 &&  valuePort1 != -1 && valuePort2 != -1) { //only port_to_compute_alone not computed
                if ( valuePort1 == 0 && valuePort2 == 0) mandatory.emplace_back(node, -1, 1, false);
                else {
                    if (value == 1) mandatory.emplace_back(node, -1, 0, false);
                }
            }
            else if (valuePortAlone != -1 &&  valuePort1 == -1 && valuePort2 == -1) {//only port_to_compute_alone computed
                if (valuePortAlone == 0 && value == 0) mandatory.emplace_back(node, -1, 1, false);
            }   
            else if (valuePortAlone != -1 && (valuePort1 != -1 || valuePort2 != -1)){//port_to_compute_alone and one of the other computed
                //get the computed value and the port to compute
                int computedvalue;
                int port_to_compute;
                if (valuePort1 == -1) {
                    port_to_compute = port_to_compute1;
                    computedvalue = valuePort2;
                }
                else {
                    port_to_compute = port_to_compute2;
                    computedvalue = valuePort1;
                }
            
                if (valuePortAlone == 0 && value == 0) mandatory.emplace_back(node, -1, 1, false);
                else{
                    if (computedvalue == 1) mandatory.emplace_back(node, -1, 0, false);
                }
            }
            else if (valuePortAlone == -1 && (valuePort1 != -1 || valuePort2 != -1)){
                //get the computed value and the port to compute
                int computedvalue;
                int port_to_compute;
                if (valuePort1 == -1) {
                    port_to_compute = port_to_compute1;
                    computedvalue = valuePort2;
                }
                else {
                    port_to_compute = port_to_compute2;
                    computedvalue = valuePort1;
                }
            
                if (value == 1 && computedvalue == 1) mandatory.emplace_back(node, -1, 0, false);
            }
            else std::cerr << "Error: a problem occured when out not computed in OAI4" << std::endl;
        }
        else{ //out computed
            if      (valuePortAlone != -1 &&  valuePort1 != -1 && valuePort2 != -1){ //all input computed
                if (valueOut == 0){
                    if ( (valuePortAlone == 0 && value == 0) || (valuePort1 == 0 && valuePort2 == 0) ) return false;
                }
                else { //out=1
                    if ( (valuePortAlone == 1 || value == 1) && (valuePort1 == 1 || valuePort2 == 1) ) return false;
                }
            }          
            else if (valuePortAlone == -1 &&  valuePort1 != -1 && valuePort2 != -1){ //only port_to_compute_alone not computed
                if ( valueOut == 0) {
                    if (valuePort1 == 0 && valuePort2 == 0) return false;
                    else {
                        if (value == 0) mandatory.emplace_back(node, port_to_compute_alone, 1, false); 
                    }
                }
                else { //out=1
                    if (valuePort1 == 1 || valuePort2 == 1){ //C||D=1
                        if (value == 1) return false;
                        else mandatory.emplace_back(node, port_to_compute_alone, 0, false);
                    }
                }
            }                      
            else if (valuePortAlone != -1 &&  valuePort1 == -1 && valuePort2 == -1){//only port_to_compute_alone computed
                if (valueOut == 0){
                    if (value == 0 && valuePortAlone == 0) return false;
                }
                else {
                    if (value == 1 || valuePortAlone == 1) {
                        mandatory.emplace_back(node, port_to_compute1, 0, false);
                        mandatory.emplace_back(node, port_to_compute2, 0, false);
                    }
                }
            }                  
            else if (valuePortAlone != -1 && (valuePort1 != -1 || valuePort2 != -1)){//port_to_compute_alone and one of the other computed
                //get the computed value and the port to compute
                int computedvalue;
                int port_to_compute;
                if (valuePort1 == -1) {
                    port_to_compute = port_to_compute1;
                    computedvalue = valuePort2;
                }
                else {
                    port_to_compute = port_to_compute2;
                    computedvalue = valuePort2;
                }
            
                if (valueOut == 0) {
                    if (valuePortAlone == 0 && value == 0) return false;
                }
                else { //out=1
                    if (valuePortAlone == 1 || value == 1) {
                        if (computedvalue == 1) return false;
                        else mandatory.emplace_back(node, port_to_compute, 0, false);
                    }
                }
            }                     
            else if (valuePortAlone == -1 && (valuePort1 != -1 || valuePort2 != -1) ){//onlyy one inout of the other pair computed
                //get the computed value and the port to compute
                int computedvalue;
                int port_to_compute;
                if (valuePort1 == -1) {
                    port_to_compute = port_to_compute1;
                    computedvalue = valuePort2;
                }
                else {
                    port_to_compute = port_to_compute2;
                    computedvalue = valuePort1;
                }
            
                if (valueOut == 0){
                    if (value == 0) mandatory.emplace_back(node, port_to_compute_alone, 1, false);
                }
                else{
                        if (computedvalue == 1) {
                            if (value == 1) return false;
                            else mandatory.emplace_back(node, port_to_compute_alone, 0, false);
                        }
                }
            }
            
            else if (this -> getValueFromPort(port_to_compute_alone) == -1 &&  this -> getValueFromPort(port_to_compute1) == -1 && this -> getValueFromPort(port_to_compute2) == -1) { //nothing else computed
                if (valueOut == 0){
                    if (value == 0) mandatory.emplace_back(node, port_to_compute_alone, 1, false);
                }
            }
            
            else std::cerr << "Error: a problem occured when out computed in OAI4" << std::endl;
        }
        
        mandatory.emplace_back(this -> getNodeFromPort(port_number), -1, value, false); //send to the parent
        this -> updatePort(port_number, value);
    }

    return true;
}

// Mux implementation ------------------------------------------------

Mux::Mux(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_MUX_";
    this -> portValues.emplace_back(   1, -1, false);
    this -> portValues.emplace_back(   2, -1, false);
    this -> portValues.emplace_back( 100, -1, false); //signal
    this -> portValues.emplace_back(  -1, -1, false);
}

std::shared_ptr<Node> Mux::getSharedPtrToThis() {
    return std::make_shared<Mux>(*this);
}

int Mux::findValueSignal(int input_number){
    if (input_number == 1) return 0; //input_number = 1 mean input 1 so signal = 0
    else return 1;
    
}

int Mux::findPortNumber(int value_signal){
    if (value_signal == 0) return 1;
    else return 2;
}

void Mux::printNodeRecursive() {
    std::cout << "node type: Mux id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> Mux::computeInputFromOutput(int value){
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> s1;
    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this->parents[0].second == 100) s1 = this->parents[0].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this->parents[1].second == 100) s1 = this->parents[1].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this->parents[2].second == 100) s1 = this->parents[2].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    // find the result
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(p1, 0));
        result.push_back(std::make_pair(p2, 0));
        result.push_back(std::make_pair(s1, 0));
    }
    else if (value == 1){
        result.push_back(std::make_pair(p1, 1));
        result.push_back(std::make_pair(p2, 1));
        result.push_back(std::make_pair(s1, 1));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for a mux cell" << std::endl;
    }

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> Mux::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != 100) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for a mux" << std::endl;
    //check value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for a mux" << std::endl;

    //get the necessary pointer
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> s1;
    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this->parents[0].second == 100) s1 = this->parents[0].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this->parents[1].second == 100) s1 = this->parents[1].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this->parents[2].second == 100) s1 = this->parents[2].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    //if the port number is one, we need valueSignal at 0 and if it is 2, we need valueSignal at 1
    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
        result.emplace_back(s1, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
        result.emplace_back(s1, 1, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    if (port_number == 100){ 
        //we need one output at 0 and the other at 1 so that we know if there is a problem
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p2, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //because 0 is sent in p1 (on port1)
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //because 1 is sent in p1 (on port1)
    }

    return result;
}

bool Mux::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != 100 && port_number != -1) std::cerr << "Error: port number doesn't have a good value for a mux cell" << std::endl;
    //check the value
    if (value != 0 && value != 1) std::cerr <<  "Error: value doesn't have a good value for a mux cell" << std::endl;

    if (port_number == -1) {
        int valuePort1 = this -> getValueFromPort(1);
        int valuePort2 = this -> getValueFromPort(2);
        int valueSignal = this -> getValueFromPort(100);

        if ( this -> getValueFromPort(port_number) != -1) { //it has already been computed
            if (this -> getValueFromPort(port_number) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if        (valuePort1 == -1 && valuePort2 == -1  && valueSignal == -1){ //no input computed
            if (value == 1) {
                mandatory.emplace_back(node, 1, 1, false);
                mandatory.emplace_back(node, 2, 1, false);
                mandatory.emplace_back(node, 100, 1, false);
            }
            else { //value = 0
                mandatory.emplace_back(node, 1, 0, false);
                mandatory.emplace_back(node, 2, 0, false);
                mandatory.emplace_back(node, 100, 0, false);
            }
            
        }
        else if   (valuePort1 != -1 && valuePort2 != -1  && valueSignal != -1){ //all input computed
            if (valueSignal == 0) { //A is linked to ouput
                if (valuePort1 != value) return false;
            }
            else { //B is linked to output
                if (valuePort2 != value) return false;
            }
                
        }
        else if   (valuePort1 != -1 && valuePort2 != -1  && valueSignal == -1){ //only input 1 and 2 computed
            if (value == valuePort1 && value == valuePort2) optional.emplace_back(node, 100, 0, false); //val = A and B
            else if (value != valuePort1 && value != valuePort2) return false; //value different from 1 and B -> error
            else { //value = A or B
                if (value == valuePort1) mandatory.emplace_back(node, 100, 0, false); //val = 1
                else mandatory.emplace_back(node, 100, 1, false); //val = B
            }
        }
        else if ( (valuePort1 != -1 || valuePort2 != -1) && valueSignal == -1){ //only A or B
            int computedValue;
            int port_to_compute;
            if (valuePort1 != -1) {
                computedValue = this -> getValueFromPort(1);
                port_to_compute = 2;
            }
            else {
                computedValue = this -> getValueFromPort(2);
                port_to_compute = 1;
            }

            if (value == computedValue){
                optional.emplace_back(node, port_to_compute, value, false);
                optional.emplace_back(node, 100, 0, false);
            }
            else{
                mandatory.emplace_back(node, port_to_compute, value, false);
                mandatory.emplace_back(node, 100, this -> findValueSignal(port_to_compute), false);
            } 
        }
        else if ( (valuePort1 != -1 || valuePort2 != -1) && valueSignal != -1){ //A or B and signal
            int valueUseful;
            int valueUseless;
            int portUseful;
            int portUseless;
            if (valueSignal == 0) {
                valueUseful = valuePort1;
                portUseful = 1;
                valueUseless = valuePort2;
                portUseless = 2;
            }
            else {
                valueUseful = valuePort2;
                portUseful = 2;
                valueUseless = valuePort1;
                portUseless = 1;
            }

            if (valueUseful == -1)mandatory.emplace_back(node, portUseful, value, false);
            else {
                if (valueUseful == value) optional.emplace_back(node, portUseless, 0, false);
                else return false;
            }
        }
        else if   (valuePort1 == -1 && valuePort2 == -1  && valueSignal != -1){ //only S computed
            if (valueSignal == 0){
                mandatory.emplace_back(node, 1, value, false);
                optional.emplace_back(node, 2, 0, false);
            }
            else {
                mandatory.emplace_back(node, 2, value, false);
                optional.emplace_back(node, 1, 0, false);
            }
        }
        else std::cerr << "Error: a problem occured when we are computing an out in mux" << std::endl;

        for (std::pair<std::shared_ptr<Node>, int> pair : this -> children) mandatory.emplace_back(pair.first, pair.second, value, propagate); //send to children
        this -> updatePort(-1, value);

    }
    else if (port_number == 100){
        int valuePort1 = this -> getValueFromPort(1);
        int valuePort2 = this -> getValueFromPort(2);
        int valueOut = this -> getValueFromPort(-1);

        if ( this -> getValueFromPort(port_number) != -1) { //it has already been computed
            if (this -> getValueFromPort(port_number) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }


        if        (valuePort1 != -1 && valuePort2 != -1  && valueOut != -1){ // all the other ports computed
            int valueUseful;
            if (value == 0) valueUseful = valuePort1;
            else valueUseful = valuePort2;

            if (valueUseful != valueOut) return false;
        }
        else if   (valuePort1 != -1 && valuePort2 != -1  && valueOut == -1){ // A and B
            int valueUseful;
            if (value == 0) valueUseful = valuePort1;
            else valueUseful = valuePort2;

            mandatory.emplace_back(node, -1, valueUseful, propagate);  
        }
        else if   (valuePort1 == -1 && valuePort2 == -1  && valueOut != -1){ //only out 
            int port_to_compute;
            int portUseless;
            if (value = 0) {
                port_to_compute = 1;
                portUseless = 2;
            }
            else {
                port_to_compute = 2;
                portUseless = 1;
            }

            if (valueOut == 0) {
                mandatory.emplace_back(node, port_to_compute, 0, false);
                optional.emplace_back(node, portUseless, 1, false);
            }
            else {
                mandatory.emplace_back(node, port_to_compute, 1, false);
                optional.emplace_back(node, portUseless, 0, false);
            }
        }
        else if ( (valuePort1 != -1 || valuePort2 != -1) && valueOut != -1){ //out and A or B
            int valueUseful;
            int portUseful;
            int portUseless;
            if (value == 0) {
                valueUseful = valuePort1;
                portUseful = 1;
                portUseless = 2;
            }
            else {
                valueUseful = valuePort2;
                portUseful = 2;
                portUseless = 1;
            }

            if (valueUseful == -1) mandatory.emplace_back(node, portUseful, valueOut, false);
            else {
                if (valueUseful != valueOut) return false;
                else optional.emplace_back(node, portUseless, 0, false);
            }
        }
        else if ( (valuePort1 != -1 || valuePort2 != -1) && valueOut == -1){ //only A or B
            int valueUseful;
            int portUseful;
            if (value == 0) {
                valueUseful = valuePort1;
                portUseful = 1;
            }
            else {
                valueUseful = valuePort2;
                portUseful = 2;
            }

            if (valueUseful != -1) mandatory.emplace_back(node, -1, valueUseful, propagate);
        }
        else if   (valuePort1 == -1 && valuePort2 == -1  && valueOut == -1){ //nothing computed
            if (propagate){
                //let's choose port 1
                mandatory.emplace_back(node, 1, 0, false);
                mandatory.emplace_back(node, -1, 0, true);
                optional.emplace_back(node, 2, 0, false);
            }
    }
   
        mandatory.emplace_back(this -> getNodeFromPort(port_number), -1, value, false); //send to the parent
        this -> updatePort(port_number, value);
    }
    else { //A or B
        int current_port;
        int other_port;
        if (port_number == 1) {
            current_port = 1;
            other_port = 2;
        }
        else {
            current_port = 2;
            other_port = 1;
        }

        int valueOtherPort = this -> getValueFromPort(other_port);
        int valueOut = this -> getValueFromPort(-1);
        int valueSignal = this -> getValueFromPort(100);

        if ( this -> getValueFromPort(port_number) != -1) { //it has already been computed
            if (this -> getValueFromPort(port_number) != value) { //it have been computed and the value is different
                return false; //it mean it can not be computed
            }
            return true;
        }

        if (propagate){
            mandatory.emplace_back(node, -1, value, true);
            if (current_port == 1) mandatory.emplace_back(node, 100, 0, false);
            else mandatory.emplace_back(node, 100, 1, false);
            optional.emplace_back(node, other_port, 0, false);
        }

        else if      (valueOtherPort != -1 && valueOut != -1 && valueSignal != -1){ //all the other ports computed
            if (findPortNumber(valueSignal) == current_port){
                if (value != valueOut) return false;
            } 
            else {
                if (value != valueOtherPort) return false;
            }
        }
        else if (valueOtherPort != -1 && valueOut == -1 && valueSignal == -1){ //only other port computed
            if (valueOtherPort == value) mandatory.emplace_back(node, -1, value, false);
        }
        else if (valueOtherPort == -1 && valueOut == -1 && valueSignal != -1){ //only signal computed
            if ( findPortNumber(valueSignal) == current_port){
                mandatory.emplace_back(node, -1, value, false);
            }
        }
        else if (valueOtherPort != -1 && valueOut != -1 && valueSignal == -1){ //other port and out computed
            if (value != valueOut && valueOtherPort != valueOut) return false;
            else if (value == valueOut && valueOtherPort == valueOut) optional.emplace_back(node, 100, 0, false);
            else {
                if (value == valueOut) mandatory.emplace_back(node, 100, findValueSignal(current_port), false);
                else mandatory.emplace_back(node, 100, findValueSignal(other_port), false);
            }
        }
        else if (valueOtherPort == -1 && valueOut != -1 && valueSignal != -1){ //signal and out computed
            if (findPortNumber(valueSignal) == current_port){
                if (value != valueOut) return false;
            }
        }
        else if (valueOtherPort != -1 && valueOut == -1 && valueSignal != -1){
            if (findPortNumber(valueSignal) == current_port) mandatory.emplace_back(node, -1, value, propagate);
            else mandatory.emplace_back(node, -1, valueOtherPort, propagate);
        }
    
        mandatory.emplace_back(this -> getNodeFromPort(port_number), -1, value, false); //send to the parent
        this -> updatePort(port_number, value);
    }
    return true;

}

// Nmux implementation -----------------------------------------------

Nmux::Nmux(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_NMUX_";
}

std::shared_ptr<Node> Nmux::getSharedPtrToThis() {
    return std::make_shared<Nmux>(*this);
}

void Nmux::printNodeRecursive() {
    std::cout << "node type: Nmux id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> Nmux::computeInputFromOutput(int value){
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> s1;
    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this->parents[0].second == 100) s1 = this->parents[0].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this->parents[1].second == 100) s1 = this->parents[1].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this->parents[2].second == 100) s1 = this->parents[2].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    
    // find the result
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(p1, 0));
        result.push_back(std::make_pair(p2, 0));
        result.push_back(std::make_pair(s1, 0));
    }
    else if (value == 1){
        result.push_back(std::make_pair(p1, 1));
        result.push_back(std::make_pair(p2, 1));
        result.push_back(std::make_pair(s1, 1));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for a mux cell" << std::endl;
    }

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> Nmux::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != 100) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for a mux" << std::endl;
    //check value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for a mux" << std::endl;

    //get the necessary pointer
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> s1;
    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this->parents[0].second == 100) s1 = this->parents[0].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this->parents[1].second == 100) s1 = this->parents[1].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;
    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this->parents[2].second == 100) s1 = this->parents[2].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    //if the port number is 1, we need valueSignal at 0 and if it is 2, we need valueSignal at 1
    if (port_number == 1){
        result.emplace_back(p2, 0, -1);
        result.emplace_back(s1, 0, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    else if (port_number == 2){
        result.emplace_back(p1, 0, -1);
        result.emplace_back(s1, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
    }
    if (port_number == 100){
        //we need one output at 0 and the other at 1 so that we know if there is a problem
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p2, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //because 0 is sent in p1 (on port1)
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //because 1 is sent in p1 (on port1)
    }
    return result;
}

bool Nmux::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //TODO
    std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": computeMandatory not implemented for Nmux" << std::endl;
    return false;
}

// Mux4 implementation -----------------------------------------------

Mux4::Mux4(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_MUX4_";
}

std::shared_ptr<Node> Mux4::getSharedPtrToThis() {
    return std::make_shared<Mux4>(*this);
}

void Mux4::printNodeRecursive(){
    std::cout << "node type: Mux4 id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> Mux4::computeInputFromOutput(int value){
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;
    std::shared_ptr<Node> p4;
    std::shared_ptr<Node> s1;
    std::shared_ptr<Node> s2;
    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this -> parents[0].second == 3) p3 = this->parents[0].first;
    else if (this -> parents[0].second == 4) p4 = this->parents[0].first;
    else if (this->parents[0].second == 100) s1 = this->parents[0].first;
    else if (this->parents[0].second == 101) s2 = this->parents[0].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this -> parents[1].second == 3) p3 = this->parents[1].first;
    else if (this -> parents[1].second == 4) p4 = this->parents[1].first;
    else if (this->parents[1].second == 100) s1 = this->parents[1].first;
    else if (this->parents[1].second == 101) s2 = this->parents[1].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this -> parents[2].second == 3) p3 = this->parents[2].first;
    else if (this -> parents[2].second == 4) p4 = this->parents[2].first;
    else if (this->parents[2].second == 100) s1 = this->parents[2].first;
    else if (this->parents[2].second == 101) s2 = this->parents[2].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[3].second == 1) p1 = this->parents[3].first;
    else if (this->parents[3].second == 2) p2 = this->parents[3].first;
    else if (this -> parents[3].second == 3) p3 = this->parents[3].first;
    else if (this -> parents[3].second == 4) p4 = this->parents[3].first;
    else if (this->parents[3].second == 100) s1 = this->parents[3].first;
    else if (this->parents[3].second == 101) s2 = this->parents[3].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this -> parents[4].second == 1) p1 = this->parents[4].first;
    else if (this -> parents[4].second == 2) p2 = this->parents[4].first;
    else if (this -> parents[4].second == 3) p3 = this->parents[4].first;
    else if (this -> parents[4].second == 4) p4 = this->parents[4].first;
    else if (this -> parents[4].second == 100) s1 = this->parents[4].first;
    else if (this -> parents[4].second == 101) s2 = this->parents[4].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this -> parents[5].second == 1) p1 = this->parents[5].first;
    else if (this -> parents[5].second == 2) p2 = this->parents[5].first;
    else if (this -> parents[5].second == 3) p3 = this->parents[5].first;
    else if (this -> parents[5].second == 4) p4 = this->parents[5].first;
    else if (this -> parents[5].second == 100) s1 = this->parents[5].first;
    else if (this -> parents[5].second == 101) s2 = this->parents[5].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    //compute
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    if (value == 0){
        result.push_back(std::make_pair(p1, 0));
        result.push_back(std::make_pair(p2, 0));
        result.push_back(std::make_pair(p3, 0));
        result.push_back(std::make_pair(p4, 0));
        result.push_back(std::make_pair(s1, 0));
        result.push_back(std::make_pair(s2, 0));
    }
    else if (value == 1) {
        result.push_back(std::make_pair(p1, 1));
        result.push_back(std::make_pair(p2, 1));
        result.push_back(std::make_pair(p3, 1));
        result.push_back(std::make_pair(p4, 1));
        result.push_back(std::make_pair(s1, 1));
        result.push_back(std::make_pair(s2, 1));
    }
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": trying to compute input from output that does not exist for a mux4 cell" << std::endl;
    }

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> Mux4::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //check the port number
    if (port_number != 1 && port_number != 2 && port_number != 3 && port_number != 4 && port_number != 100 && port_number != 101) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number doesn't have a good value for a mux" << std::endl;
    //check value
    if (value != 0 && value != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value doesn't have a good value for a mux" << std::endl;
    
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;
    std::shared_ptr<Node> p4;
    std::shared_ptr<Node> s1;
    std::shared_ptr<Node> s2;
    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this -> parents[0].second == 3) p3 = this->parents[0].first;
    else if (this -> parents[0].second == 4) p4 = this->parents[0].first;
    else if (this->parents[0].second == 100) s1 = this->parents[0].first;
    else if (this->parents[0].second == 101) s2 = this->parents[0].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this -> parents[1].second == 3) p3 = this->parents[1].first;
    else if (this -> parents[1].second == 4) p4 = this->parents[1].first;
    else if (this->parents[1].second == 100) s1 = this->parents[1].first;
    else if (this->parents[1].second == 101) s2 = this->parents[1].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this -> parents[2].second == 3) p3 = this->parents[2].first;
    else if (this -> parents[2].second == 4) p4 = this->parents[2].first;
    else if (this->parents[2].second == 100) s1 = this->parents[2].first;
    else if (this->parents[2].second == 101) s2 = this->parents[2].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[3].second == 1) p1 = this->parents[3].first;
    else if (this->parents[3].second == 2) p2 = this->parents[3].first;
    else if (this -> parents[3].second == 3) p3 = this->parents[3].first;
    else if (this -> parents[3].second == 4) p4 = this->parents[3].first;
    else if (this->parents[3].second == 100) s1 = this->parents[3].first;
    else if (this->parents[3].second == 101) s2 = this->parents[3].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this -> parents[4].second == 1) p1 = this->parents[4].first;
    else if (this -> parents[4].second == 2) p2 = this->parents[4].first;
    else if (this -> parents[4].second == 3) p3 = this->parents[4].first;
    else if (this -> parents[4].second == 4) p4 = this->parents[4].first;
    else if (this -> parents[4].second == 100) s1 = this->parents[4].first;
    else if (this -> parents[4].second == 101) s2 = this->parents[4].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this -> parents[5].second == 1) p1 = this->parents[5].first;
    else if (this -> parents[5].second == 2) p2 = this->parents[5].first;
    else if (this -> parents[5].second == 3) p3 = this->parents[5].first;
    else if (this -> parents[5].second == 4) p4 = this->parents[5].first;
    else if (this -> parents[5].second == 100) s1 = this->parents[5].first;
    else if (this -> parents[5].second == 101) s2 = this->parents[5].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    //input
    if (port_number == 1){
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);        
    }
    else if (port_number == 2){
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p1, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second); 
    }
    else if (port_number == 3){
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second); 
    }
    else if (port_number == 4){
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second); 
    }
    //signal
    else if (port_number == 100){
        result.emplace_back(s2, 1, -1); //s2 is at 1, so we are working on p3 and p4
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 0, -1);
        if (value == 0) { //p3 is the output
            result.emplace_back(p3, 0, -1);
            result.emplace_back(p4, 1, -1);
            //output is at p3 so at 0
            for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
        }
        else if (value == 1) { //p4 is the output
            result.emplace_back(p3, 0, -1);
            result.emplace_back(p4, 1, -1);
            //output is p4 so at 1
            for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        }
    }
    else if (port_number == 101) {
        result.emplace_back(s1, 1, -1); //s1 is at 1, so we are working on p2 and p4
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p1, 0, -1);
        if (value == 0){ //p2 is the output
            result.emplace_back(p2, 0, -1);
            result.emplace_back(p4, 1, -1);
            //output is p2 so at 0
            for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second);
        }
        else if (value == 1){
            result.emplace_back(p2, 0, -1);
            result.emplace_back(p4, 1, -1);
            //output is p4 so at 1
            for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second);
        }
    }
    return result;
}

std::pair<int, int> Mux4::findValueSignals(int input_number){
    if      (input_number == 1) return std::make_pair(0, 0);
    else if (input_number == 2) return std::make_pair(0, 1);
    else if (input_number == 3) return std::make_pair(1, 0);
    else if (input_number == 4) return std::make_pair(1, 1);
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value of signal can not be computed for a Mux4" << std::endl;
        exit(1);
    }
}

int Mux4::findPortNumber(int value_signal1, int value_signal2){
    if      (value_signal1 == 0 && value_signal2 == 0) return 1;
    else if (value_signal1 == 0 && value_signal2 == 1) return 2;
    else if (value_signal1 == 1 && value_signal2 == 0) return 3;
    else if (value_signal1 == 1 && value_signal2 == 1) return 4;
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": port number can not be computed for a Mux4" << std::endl;
        exit(1);
    }
}

std::pair<int, int> Mux4::findSafeCombination(int input_wrong1, int input_wrong2){
    if      ( (input_wrong1 == 1 && input_wrong2 == 2) || (input_wrong1 == 2 && input_wrong2 == 1) ) return std::make_pair(1, 1);
    else if ( (input_wrong1 == 1 && input_wrong2 == 3) || (input_wrong1 == 3 && input_wrong2 == 1) ) return std::make_pair(1, 1);
    else if ( (input_wrong1 == 1 && input_wrong2 == 4) || (input_wrong1 == 4 && input_wrong2 == 1) ) return std::make_pair(1, 0);
    else if ( (input_wrong1 == 2 && input_wrong2 == 3) || (input_wrong1 == 3 && input_wrong2 == 2) ) return std::make_pair(0, 0);
    else if ( (input_wrong1 == 2 && input_wrong2 == 4) || (input_wrong1 == 4 && input_wrong2 == 2) ) return std::make_pair(0, 0);
    else if ( (input_wrong1 == 3 && input_wrong2 == 4) || (input_wrong1 == 4 && input_wrong2 == 3) ) return std::make_pair(0, 0);
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": safe combination can not be computed for a Mux4" << std::endl;
        exit(1);
    }
}

std::pair<int, int> Mux4::findSafeCombination(int input_wrong){
    if      (input_wrong == 1) return std::make_pair(1, 1);
    else if (input_wrong == 2) return std::make_pair(1, 1);
    else if (input_wrong == 3) return std::make_pair(0, 0);
    else if (input_wrong == 4) return std::make_pair(0, 0);
    else {
        std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": safe combination can not be computed for a Mux4" << std::endl;
        exit(1);
    }
}

bool Mux4::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //TODO
    std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": computeMandatory not implemented for Mux4" << std::endl;
    return false;
}


// Mux8 implementation -----------------------------------------------

Mux8::Mux8(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_MUX8_";
}

std::shared_ptr<Node> Mux8::getSharedPtrToThis() {
    return std::make_shared<Mux8>(*this);
}


void Mux8::printNodeRecursive(){
    std::cout << "node type: Mux8 id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> Mux8::computeInputFromOutput(int value){
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;
    std::shared_ptr<Node> p4;
    std::shared_ptr<Node> p5;
    std::shared_ptr<Node> p6;
    std::shared_ptr<Node> p7;
    std::shared_ptr<Node> p8;
    std::shared_ptr<Node> s1;
    std::shared_ptr<Node> s2;
    std::shared_ptr<Node> s3;
    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this -> parents[0].second == 3) p3 = this->parents[0].first;
    else if (this -> parents[0].second == 4) p4 = this->parents[0].first;
    else if (this -> parents[0].second == 5) p5 = this->parents[0].first;
    else if (this -> parents[0].second == 6) p6 = this->parents[0].first;
    else if (this -> parents[0].second == 7) p7 = this->parents[0].first;
    else if (this -> parents[0].second == 8) p8 = this->parents[0].first;
    else if (this -> parents[0].second == 100) s1 = this->parents[0].first;
    else if (this -> parents[0].second == 101) s2 = this->parents[0].first;
    else if (this -> parents[0].second == 102) s3 = this->parents[0].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this -> parents[1].second == 3) p3 = this->parents[1].first;
    else if (this -> parents[1].second == 4) p4 = this->parents[1].first;
    else if (this -> parents[1].second == 5) p5 = this->parents[1].first;
    else if (this -> parents[1].second == 6) p6 = this->parents[1].first;
    else if (this -> parents[1].second == 7) p7 = this->parents[1].first;
    else if (this -> parents[1].second == 8) p8 = this->parents[1].first;
    else if (this -> parents[1].second == 100) s1 = this->parents[1].first;
    else if (this -> parents[1].second == 101) s2 = this->parents[1].first;
    else if (this -> parents[1].second == 4) s3 = this->parents[1].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this -> parents[2].second == 3) p3 = this->parents[2].first;
    else if (this -> parents[2].second == 4) p4 = this->parents[2].first;
    else if (this -> parents[2].second == 5) p5 = this->parents[2].first;
    else if (this -> parents[2].second == 6) p6 = this->parents[2].first;
    else if (this -> parents[2].second == 7) p7 = this->parents[2].first;
    else if (this -> parents[2].second == 8) p8 = this->parents[2].first;
    else if (this -> parents[2].second == 100) s1 = this->parents[2].first;
    else if (this -> parents[2].second == 101) s2 = this->parents[2].first;
    else if (this -> parents[2].second == 4) s3 = this->parents[2].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[3].second == 1) p1 = this->parents[3].first;
    else if (this->parents[3].second == 2) p2 = this->parents[3].first;
    else if (this -> parents[3].second == 3) p3 = this->parents[3].first;
    else if (this -> parents[3].second == 4) p4 = this->parents[3].first;
    else if (this -> parents[3].second == 5) p5 = this->parents[3].first;
    else if (this -> parents[3].second == 6) p6 = this->parents[3].first;
    else if (this -> parents[3].second == 7) p7 = this->parents[3].first;
    else if (this -> parents[3].second == 8) p8 = this->parents[3].first;
    else if (this -> parents[3].second == 100) s1 = this->parents[3].first;
    else if (this -> parents[3].second == 101) s2 = this->parents[3].first;
    else if (this -> parents[3].second == 4) s3 = this->parents[3].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this -> parents[4].second == 1) p1 = this->parents[4].first;
    else if (this -> parents[4].second == 2) p2 = this->parents[4].first;
    else if (this -> parents[4].second == 3) p3 = this->parents[4].first;
    else if (this -> parents[4].second == 4) p4 = this->parents[4].first;
    else if (this -> parents[4].second == 5) p5 = this->parents[4].first;
    else if (this -> parents[4].second == 6) p6 = this->parents[4].first;
    else if (this -> parents[4].second == 7) p7 = this->parents[4].first;
    else if (this -> parents[4].second == 8) p8 = this->parents[4].first;
    else if (this -> parents[4].second == 100) s1 = this->parents[4].first;
    else if (this -> parents[4].second == 101) s2 = this->parents[4].first;
    else if (this -> parents[4].second == 102) s3 = this->parents[4].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this -> parents[5].second == 1) p1 = this->parents[5].first;
    else if (this -> parents[5].second == 2) p2 = this->parents[5].first;
    else if (this -> parents[5].second == 3) p3 = this->parents[5].first;
    else if (this -> parents[5].second == 4) p4 = this->parents[5].first;
    else if (this -> parents[5].second == 5) p5 = this->parents[5].first;
    else if (this -> parents[5].second == 6) p6 = this->parents[5].first;
    else if (this -> parents[5].second == 7) p7 = this->parents[5].first;
    else if (this -> parents[5].second == 8) p8 = this->parents[5].first;
    else if (this -> parents[5].second == 100) s1 = this->parents[5].first;
    else if (this -> parents[5].second == 101) s2 = this->parents[5].first;
    else if (this -> parents[5].second == 102) s3 = this->parents[5].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[6].second == 1) p1 = this->parents[6].first;
    else if (this->parents[6].second == 2) p2 = this->parents[6].first;
    else if (this -> parents[6].second == 3) p3 = this->parents[6].first;
    else if (this -> parents[6].second == 4) p4 = this->parents[6].first;
    else if (this -> parents[6].second == 5) p5 = this->parents[6].first;
    else if (this -> parents[6].second == 6) p6 = this->parents[6].first;
    else if (this -> parents[6].second == 7) p7 = this->parents[6].first;
    else if (this -> parents[6].second == 8) p8 = this->parents[6].first;
    else if (this -> parents[6].second == 100) s1 = this->parents[6].first;
    else if (this -> parents[6].second == 101) s2 = this->parents[6].first;
    else if (this -> parents[6].second == 102) s3 = this->parents[6].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[7].second == 1) p1 = this->parents[7].first;
    else if (this->parents[7].second == 2) p2 = this->parents[7].first;
    else if (this -> parents[7].second == 3) p3 = this->parents[7].first;
    else if (this -> parents[7].second == 4) p4 = this->parents[7].first;
    else if (this -> parents[7].second == 5) p5 = this->parents[7].first;
    else if (this -> parents[7].second == 6) p6 = this->parents[7].first;
    else if (this -> parents[7].second == 7) p7 = this->parents[7].first;
    else if (this -> parents[7].second == 8) p8 = this->parents[7].first;
    else if (this -> parents[7].second == 100) s1 = this->parents[7].first;
    else if (this -> parents[7].second == 101) s2 = this->parents[7].first;
    else if (this -> parents[7].second == 102) s3 = this->parents[7].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[8].second == 1) p1 = this->parents[8].first;
    else if (this->parents[8].second == 2) p2 = this->parents[8].first;
    else if (this -> parents[8].second == 3) p3 = this->parents[8].first;
    else if (this -> parents[8].second == 4) p4 = this->parents[8].first;
    else if (this -> parents[8].second == 5) p5 = this->parents[8].first;
    else if (this -> parents[8].second == 6) p6 = this->parents[8].first;
    else if (this -> parents[8].second == 7) p7 = this->parents[8].first;
    else if (this -> parents[8].second == 8) p8 = this->parents[8].first;
    else if (this -> parents[8].second == 100) s1 = this->parents[8].first;
    else if (this -> parents[8].second == 101) s2 = this->parents[8].first;
    else if (this -> parents[8].second == 102) s3 = this->parents[8].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    //parent 9,10 and 11
    if (this->parents[9].second == 1) p1 = this->parents[9].first;
    else if (this->parents[9].second == 2) p2 = this->parents[9].first;
    else if (this -> parents[9].second == 3) p3 = this->parents[9].first;
    else if (this -> parents[9].second == 4) p4 = this->parents[9].first;
    else if (this -> parents[9].second == 5) p5 = this->parents[9].first;
    else if (this -> parents[9].second == 6) p6 = this->parents[9].first;
    else if (this -> parents[9].second == 7) p7 = this->parents[9].first;
    else if (this -> parents[9].second == 8) p8 = this->parents[9].first;
    else if (this -> parents[9].second == 100) s1 = this->parents[9].first;
    else if (this -> parents[9].second == 101) s2 = this->parents[9].first;
    else if (this -> parents[9].second == 102) s3 = this->parents[9].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[10].second == 1) p1 = this->parents[10].first;
    else if (this->parents[10].second == 2) p2 = this->parents[10].first;
    else if (this -> parents[10].second == 3) p3 = this->parents[10].first;
    else if (this -> parents[10].second == 4) p4 = this->parents[10].first;
    else if (this -> parents[10].second == 5) p5 = this->parents[10].first;
    else if (this -> parents[10].second == 6) p6 = this->parents[10].first;
    else if (this -> parents[10].second == 7) p7 = this->parents[10].first;
    else if (this -> parents[10].second == 8) p8 = this->parents[10].first;
    else if (this -> parents[10].second == 100) s1 = this->parents[10].first;
    else if (this -> parents[10].second == 101) s2 = this->parents[10].first;
    else if (this -> parents[10].second == 102) s3 = this->parents[10].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[11].second == 1) p1 = this->parents[11].first;
    else if (this->parents[11].second == 2) p2 = this->parents[11].first;
    else if (this -> parents[11].second == 3) p3 = this->parents[11].first;
    else if (this -> parents[11].second == 4) p4 = this->parents[11].first;
    else if (this -> parents[11].second == 5) p5 = this->parents[11].first;
    else if (this -> parents[11].second == 6) p6 = this->parents[11].first;
    else if (this -> parents[11].second == 7) p7 = this->parents[11].first;
    else if (this -> parents[11].second == 8) p8 = this->parents[11].first;
    else if (this -> parents[11].second == 100) s1 = this->parents[11].first;
    else if (this -> parents[11].second == 101) s2 = this->parents[11].first;
    else if (this -> parents[11].second == 102) s3 = this->parents[11].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (value == 0){
        result.push_back(std::make_pair(p1, 0));
        result.push_back(std::make_pair(p2, 0));
        result.push_back(std::make_pair(p3, 0));
        result.push_back(std::make_pair(p4, 0));
        result.push_back(std::make_pair(p5, 0));
        result.push_back(std::make_pair(p6, 0));
        result.push_back(std::make_pair(p7, 0));
        result.push_back(std::make_pair(p8, 0));
        result.push_back(std::make_pair(s1, 0));
        result.push_back(std::make_pair(s2, 0));
        result.push_back(std::make_pair(s3, 0));
    }
    else if (value == 1){
        result.push_back(std::make_pair(p1, 1));
        result.push_back(std::make_pair(p2, 1));
        result.push_back(std::make_pair(p3, 1));
        result.push_back(std::make_pair(p4, 1));
        result.push_back(std::make_pair(p5, 1));
        result.push_back(std::make_pair(p6, 1));
        result.push_back(std::make_pair(p7, 1));
        result.push_back(std::make_pair(p8, 1));
        result.push_back(std::make_pair(s1, 1));
        result.push_back(std::make_pair(s2, 1));
        result.push_back(std::make_pair(s3, 1));
    }   
    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> Mux8::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;
    std::shared_ptr<Node> p4;
    std::shared_ptr<Node> p5;
    std::shared_ptr<Node> p6;
    std::shared_ptr<Node> p7;
    std::shared_ptr<Node> p8;
    std::shared_ptr<Node> s1;
    std::shared_ptr<Node> s2;
    std::shared_ptr<Node> s3;
    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this -> parents[0].second == 3) p3 = this->parents[0].first;
    else if (this -> parents[0].second == 4) p4 = this->parents[0].first;
    else if (this -> parents[0].second == 5) p5 = this->parents[0].first;
    else if (this -> parents[0].second == 6) p6 = this->parents[0].first;
    else if (this -> parents[0].second == 7) p7 = this->parents[0].first;
    else if (this -> parents[0].second == 8) p8 = this->parents[0].first;
    else if (this -> parents[0].second == 100) s1 = this->parents[0].first;
    else if (this -> parents[0].second == 101) s2 = this->parents[0].first;
    else if (this -> parents[0].second == 102) s3 = this->parents[0].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this -> parents[1].second == 3) p3 = this->parents[1].first;
    else if (this -> parents[1].second == 4) p4 = this->parents[1].first;
    else if (this -> parents[1].second == 5) p5 = this->parents[1].first;
    else if (this -> parents[1].second == 6) p6 = this->parents[1].first;
    else if (this -> parents[1].second == 7) p7 = this->parents[1].first;
    else if (this -> parents[1].second == 8) p8 = this->parents[1].first;
    else if (this -> parents[1].second == 100) s1 = this->parents[1].first;
    else if (this -> parents[1].second == 101) s2 = this->parents[1].first;
    else if (this -> parents[1].second == 4) s3 = this->parents[1].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this -> parents[2].second == 3) p3 = this->parents[2].first;
    else if (this -> parents[2].second == 4) p4 = this->parents[2].first;
    else if (this -> parents[2].second == 5) p5 = this->parents[2].first;
    else if (this -> parents[2].second == 6) p6 = this->parents[2].first;
    else if (this -> parents[2].second == 7) p7 = this->parents[2].first;
    else if (this -> parents[2].second == 8) p8 = this->parents[2].first;
    else if (this -> parents[2].second == 100) s1 = this->parents[2].first;
    else if (this -> parents[2].second == 101) s2 = this->parents[2].first;
    else if (this -> parents[2].second == 4) s3 = this->parents[2].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[3].second == 1) p1 = this->parents[3].first;
    else if (this->parents[3].second == 2) p2 = this->parents[3].first;
    else if (this -> parents[3].second == 3) p3 = this->parents[3].first;
    else if (this -> parents[3].second == 4) p4 = this->parents[3].first;
    else if (this -> parents[3].second == 5) p5 = this->parents[3].first;
    else if (this -> parents[3].second == 6) p6 = this->parents[3].first;
    else if (this -> parents[3].second == 7) p7 = this->parents[3].first;
    else if (this -> parents[3].second == 8) p8 = this->parents[3].first;
    else if (this -> parents[3].second == 100) s1 = this->parents[3].first;
    else if (this -> parents[3].second == 101) s2 = this->parents[3].first;
    else if (this -> parents[3].second == 4) s3 = this->parents[3].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this -> parents[4].second == 1) p1 = this->parents[4].first;
    else if (this -> parents[4].second == 2) p2 = this->parents[4].first;
    else if (this -> parents[4].second == 3) p3 = this->parents[4].first;
    else if (this -> parents[4].second == 4) p4 = this->parents[4].first;
    else if (this -> parents[4].second == 5) p5 = this->parents[4].first;
    else if (this -> parents[4].second == 6) p6 = this->parents[4].first;
    else if (this -> parents[4].second == 7) p7 = this->parents[4].first;
    else if (this -> parents[4].second == 8) p8 = this->parents[4].first;
    else if (this -> parents[4].second == 100) s1 = this->parents[4].first;
    else if (this -> parents[4].second == 101) s2 = this->parents[4].first;
    else if (this -> parents[4].second == 102) s3 = this->parents[4].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this -> parents[5].second == 1) p1 = this->parents[5].first;
    else if (this -> parents[5].second == 2) p2 = this->parents[5].first;
    else if (this -> parents[5].second == 3) p3 = this->parents[5].first;
    else if (this -> parents[5].second == 4) p4 = this->parents[5].first;
    else if (this -> parents[5].second == 5) p5 = this->parents[5].first;
    else if (this -> parents[5].second == 6) p6 = this->parents[5].first;
    else if (this -> parents[5].second == 7) p7 = this->parents[5].first;
    else if (this -> parents[5].second == 8) p8 = this->parents[5].first;
    else if (this -> parents[5].second == 100) s1 = this->parents[5].first;
    else if (this -> parents[5].second == 101) s2 = this->parents[5].first;
    else if (this -> parents[5].second == 102) s3 = this->parents[5].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[6].second == 1) p1 = this->parents[6].first;
    else if (this->parents[6].second == 2) p2 = this->parents[6].first;
    else if (this -> parents[6].second == 3) p3 = this->parents[6].first;
    else if (this -> parents[6].second == 4) p4 = this->parents[6].first;
    else if (this -> parents[6].second == 5) p5 = this->parents[6].first;
    else if (this -> parents[6].second == 6) p6 = this->parents[6].first;
    else if (this -> parents[6].second == 7) p7 = this->parents[6].first;
    else if (this -> parents[6].second == 8) p8 = this->parents[6].first;
    else if (this -> parents[6].second == 100) s1 = this->parents[6].first;
    else if (this -> parents[6].second == 101) s2 = this->parents[6].first;
    else if (this -> parents[6].second == 102) s3 = this->parents[6].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[7].second == 1) p1 = this->parents[7].first;
    else if (this->parents[7].second == 2) p2 = this->parents[7].first;
    else if (this -> parents[7].second == 3) p3 = this->parents[7].first;
    else if (this -> parents[7].second == 4) p4 = this->parents[7].first;
    else if (this -> parents[7].second == 5) p5 = this->parents[7].first;
    else if (this -> parents[7].second == 6) p6 = this->parents[7].first;
    else if (this -> parents[7].second == 7) p7 = this->parents[7].first;
    else if (this -> parents[7].second == 8) p8 = this->parents[7].first;
    else if (this -> parents[7].second == 100) s1 = this->parents[7].first;
    else if (this -> parents[7].second == 101) s2 = this->parents[7].first;
    else if (this -> parents[7].second == 102) s3 = this->parents[7].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[8].second == 1) p1 = this->parents[8].first;
    else if (this->parents[8].second == 2) p2 = this->parents[8].first;
    else if (this -> parents[8].second == 3) p3 = this->parents[8].first;
    else if (this -> parents[8].second == 4) p4 = this->parents[8].first;
    else if (this -> parents[8].second == 5) p5 = this->parents[8].first;
    else if (this -> parents[8].second == 6) p6 = this->parents[8].first;
    else if (this -> parents[8].second == 7) p7 = this->parents[8].first;
    else if (this -> parents[8].second == 8) p8 = this->parents[8].first;
    else if (this -> parents[8].second == 100) s1 = this->parents[8].first;
    else if (this -> parents[8].second == 101) s2 = this->parents[8].first;
    else if (this -> parents[8].second == 102) s3 = this->parents[8].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    //parent 9,10 and 11
    if (this->parents[9].second == 1) p1 = this->parents[9].first;
    else if (this->parents[9].second == 2) p2 = this->parents[9].first;
    else if (this -> parents[9].second == 3) p3 = this->parents[9].first;
    else if (this -> parents[9].second == 4) p4 = this->parents[9].first;
    else if (this -> parents[9].second == 5) p5 = this->parents[9].first;
    else if (this -> parents[9].second == 6) p6 = this->parents[9].first;
    else if (this -> parents[9].second == 7) p7 = this->parents[9].first;
    else if (this -> parents[9].second == 8) p8 = this->parents[9].first;
    else if (this -> parents[9].second == 100) s1 = this->parents[9].first;
    else if (this -> parents[9].second == 101) s2 = this->parents[9].first;
    else if (this -> parents[9].second == 102) s3 = this->parents[9].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[10].second == 1) p1 = this->parents[10].first;
    else if (this->parents[10].second == 2) p2 = this->parents[10].first;
    else if (this -> parents[10].second == 3) p3 = this->parents[10].first;
    else if (this -> parents[10].second == 4) p4 = this->parents[10].first;
    else if (this -> parents[10].second == 5) p5 = this->parents[10].first;
    else if (this -> parents[10].second == 6) p6 = this->parents[10].first;
    else if (this -> parents[10].second == 7) p7 = this->parents[10].first;
    else if (this -> parents[10].second == 8) p8 = this->parents[10].first;
    else if (this -> parents[10].second == 100) s1 = this->parents[10].first;
    else if (this -> parents[10].second == 101) s2 = this->parents[10].first;
    else if (this -> parents[10].second == 102) s3 = this->parents[10].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (this->parents[11].second == 1) p1 = this->parents[11].first;
    else if (this->parents[11].second == 2) p2 = this->parents[11].first;
    else if (this -> parents[11].second == 3) p3 = this->parents[11].first;
    else if (this -> parents[11].second == 4) p4 = this->parents[11].first;
    else if (this -> parents[11].second == 5) p5 = this->parents[11].first;
    else if (this -> parents[11].second == 6) p6 = this->parents[11].first;
    else if (this -> parents[11].second == 7) p7 = this->parents[11].first;
    else if (this -> parents[11].second == 8) p8 = this->parents[11].first;
    else if (this -> parents[11].second == 100) s1 = this->parents[11].first;
    else if (this -> parents[11].second == 101) s2 = this->parents[11].first;
    else if (this -> parents[11].second == 102) s3 = this->parents[11].first;
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem in parent number for mux" << std::endl;

    if (port_number == 1){
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);   
    }
    else if (port_number == 2){
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p1, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);   
    }
    else if (port_number == 3){
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);   
    }
    else if (port_number == 4){
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);   
    }
    else if (port_number == 5){
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);   
    }
    else if (port_number == 6){
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);   
    }
    else if (port_number == 7){
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);   
    }
    else if (port_number == 8){
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);   
    }
    else if (port_number = 100){
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        //s3 and s2 are at 0 so we are working on p2 and p1
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //p1 is the output
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //p2 is the output
    }

    else if (port_number = 101){
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s1, 0, -1);
        //s3 and s2 are at 0 so we are working on p3 and p1
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //p1 is the output
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //p3 is the output
    }

    else if (port_number = 102){
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 0, -1);
        //s3 and s2 are at 0 so we are working on p5 and p1
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //p1 is the output
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //p5 is the output
    }

    return result;
}

bool Mux8::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //TODO
    std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": computeMandatory not implemented for mux8" << std::endl;
    return false;
}

// Mux16 implementation ----------------------------------------------

Mux16::Mux16(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName){
    this->type = "$_MUX16_";
}

std::shared_ptr<Node> Mux16::getSharedPtrToThis() {
    return std::make_shared<Mux16>(*this);
}

void Mux16::printNodeRecursive(){
    std::cout << "node type: Mux16 id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive();
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> Mux16::computeInputFromOutput(int value){
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;
    std::shared_ptr<Node> p4;
    std::shared_ptr<Node> p5;
    std::shared_ptr<Node> p6;
    std::shared_ptr<Node> p7;
    std::shared_ptr<Node> p8;
    std::shared_ptr<Node> p9;
    std::shared_ptr<Node> p10;
    std::shared_ptr<Node> p11;
    std::shared_ptr<Node> p12;
    std::shared_ptr<Node> p13;
    std::shared_ptr<Node> p14;
    std::shared_ptr<Node> p15;
    std::shared_ptr<Node> p16;
    std::shared_ptr<Node> s1;
    std::shared_ptr<Node> s2;
    std::shared_ptr<Node> s3;
    std::shared_ptr<Node> s4;

    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this -> parents[0].second == 3) p3 = this->parents[0].first;
    else if (this -> parents[0].second == 4) p4 = this->parents[0].first;
    else if (this -> parents[0].second == 5) p5 = this->parents[0].first;
    else if (this -> parents[0].second == 6) p6 = this->parents[0].first;
    else if (this -> parents[0].second == 7) p7 = this->parents[0].first;
    else if (this -> parents[0].second == 8) p8 = this->parents[0].first;
    else if (this -> parents[0].second == 9) p9 = this->parents[0].first;
    else if (this -> parents[0].second == 10) p10 = this->parents[0].first;
    else if (this -> parents[0].second == 11) p11 = this->parents[0].first;
    else if (this -> parents[0].second == 12) p12 = this->parents[0].first;
    else if (this -> parents[0].second == 13) p13 = this->parents[0].first;
    else if (this -> parents[0].second == 14) p14 = this->parents[0].first;
    else if (this -> parents[0].second == 15) p15 = this->parents[0].first;
    else if (this -> parents[0].second == 16) p16 = this->parents[0].first;
    else if (this -> parents[0].second == 100) s1 = this->parents[0].first;
    else if (this -> parents[0].second == 101) s2 = this->parents[0].first;
    else if (this -> parents[0].second == 102) s3 = this->parents[0].first;
    else if (this -> parents[0].second == 103) s4 = this->parents[0].first;

    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this -> parents[1].second == 3) p3 = this->parents[1].first;
    else if (this -> parents[1].second == 4) p4 = this->parents[1].first;
    else if (this -> parents[1].second == 5) p5 = this->parents[1].first;
    else if (this -> parents[1].second == 6) p6 = this->parents[1].first;
    else if (this -> parents[1].second == 7) p7 = this->parents[1].first;
    else if (this -> parents[1].second == 8) p8 = this->parents[1].first;
    else if (this -> parents[1].second == 9) p9 = this->parents[1].first;
    else if (this -> parents[1].second == 10) p10 = this->parents[1].first;
    else if (this -> parents[1].second == 11) p11 = this->parents[1].first;
    else if (this -> parents[1].second == 12) p12 = this->parents[1].first;
    else if (this -> parents[1].second == 13) p13 = this->parents[1].first;
    else if (this -> parents[1].second == 14) p14 = this->parents[1].first;
    else if (this -> parents[1].second == 15) p15 = this->parents[1].first;
    else if (this -> parents[1].second == 16) p16 = this->parents[1].first;
    else if (this -> parents[1].second == 100) s1 = this->parents[1].first;
    else if (this -> parents[1].second == 101) s2 = this->parents[1].first;
    else if (this -> parents[1].second == 102) s3 = this->parents[1].first;
    else if (this -> parents[1].second == 103) s4 = this->parents[1].first;

    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this -> parents[2].second == 3) p3 = this->parents[2].first;
    else if (this -> parents[2].second == 4) p4 = this->parents[2].first;
    else if (this -> parents[2].second == 5) p5 = this->parents[2].first;
    else if (this -> parents[2].second == 6) p6 = this->parents[2].first;
    else if (this -> parents[2].second == 7) p7 = this->parents[2].first;
    else if (this -> parents[2].second == 8) p8 = this->parents[2].first;
    else if (this -> parents[2].second == 9) p9 = this->parents[2].first;
    else if (this -> parents[2].second == 10) p10 = this->parents[2].first;
    else if (this -> parents[2].second == 11) p11 = this->parents[2].first;
    else if (this -> parents[2].second == 12) p12 = this->parents[2].first;
    else if (this -> parents[2].second == 13) p13 = this->parents[2].first;
    else if (this -> parents[2].second == 14) p14 = this->parents[2].first;
    else if (this -> parents[2].second == 15) p15 = this->parents[2].first;
    else if (this -> parents[2].second == 16) p16 = this->parents[2].first;
    else if (this -> parents[2].second == 100) s1 = this->parents[2].first;
    else if (this -> parents[2].second == 101) s2 = this->parents[2].first;
    else if (this -> parents[2].second == 102) s3 = this->parents[2].first;
    else if (this -> parents[2].second == 103) s4 = this->parents[2].first;

    if (this->parents[3].second == 1) p1 = this->parents[3].first;
    else if (this->parents[3].second == 2) p2 = this->parents[3].first;
    else if (this -> parents[3].second == 3) p3 = this->parents[3].first;
    else if (this -> parents[3].second == 4) p4 = this->parents[3].first;
    else if (this -> parents[3].second == 5) p5 = this->parents[3].first;
    else if (this -> parents[3].second == 6) p6 = this->parents[3].first;
    else if (this -> parents[3].second == 7) p7 = this->parents[3].first;
    else if (this -> parents[3].second == 8) p8 = this->parents[3].first;
    else if (this -> parents[3].second == 9) p9 = this->parents[3].first;
    else if (this -> parents[3].second == 10) p10 = this->parents[3].first;
    else if (this -> parents[3].second == 11) p11 = this->parents[3].first;
    else if (this -> parents[3].second == 12) p12 = this->parents[3].first;
    else if (this -> parents[3].second == 13) p13 = this->parents[3].first;
    else if (this -> parents[3].second == 14) p14 = this->parents[3].first;
    else if (this -> parents[3].second == 15) p15 = this->parents[3].first;
    else if (this -> parents[3].second == 16) p16 = this->parents[3].first;
    else if (this -> parents[3].second == 100) s1 = this->parents[3].first;
    else if (this -> parents[3].second == 101) s2 = this->parents[3].first;
    else if (this -> parents[3].second == 102) s3 = this->parents[3].first;
    else if (this -> parents[3].second == 103) s4 = this->parents[3].first;

    if (this->parents[4].second == 1) p1 = this->parents[4].first;
    else if (this->parents[4].second == 2) p2 = this->parents[4].first;
    else if (this -> parents[4].second == 3) p3 = this->parents[4].first;
    else if (this -> parents[4].second == 4) p4 = this->parents[4].first;
    else if (this -> parents[4].second == 5) p5 = this->parents[4].first;
    else if (this -> parents[4].second == 6) p6 = this->parents[4].first;
    else if (this -> parents[4].second == 7) p7 = this->parents[4].first;
    else if (this -> parents[4].second == 8) p8 = this->parents[4].first;
    else if (this -> parents[4].second == 9) p9 = this->parents[4].first;
    else if (this -> parents[4].second == 10) p10 = this->parents[4].first;
    else if (this -> parents[4].second == 11) p11 = this->parents[4].first;
    else if (this -> parents[4].second == 12) p12 = this->parents[4].first;
    else if (this -> parents[4].second == 13) p13 = this->parents[4].first;
    else if (this -> parents[4].second == 14) p14 = this->parents[4].first;
    else if (this -> parents[4].second == 15) p15 = this->parents[4].first;
    else if (this -> parents[4].second == 16) p16 = this->parents[4].first;
    else if (this -> parents[4].second == 100) s1 = this->parents[4].first;
    else if (this -> parents[4].second == 101) s2 = this->parents[4].first;
    else if (this -> parents[4].second == 102) s3 = this->parents[4].first;
    else if (this -> parents[4].second == 103) s4 = this->parents[4].first;

    if (this->parents[5].second == 1) p1 = this->parents[5].first;
    else if (this->parents[5].second == 2) p2 = this->parents[5].first;
    else if (this -> parents[5].second == 3) p3 = this->parents[5].first;
    else if (this -> parents[5].second == 4) p4 = this->parents[5].first;
    else if (this -> parents[5].second == 5) p5 = this->parents[5].first;
    else if (this -> parents[5].second == 6) p6 = this->parents[5].first;
    else if (this -> parents[5].second == 7) p7 = this->parents[5].first;
    else if (this -> parents[5].second == 8) p8 = this->parents[5].first;
    else if (this -> parents[5].second == 9) p9 = this->parents[5].first;
    else if (this -> parents[5].second == 10) p10 = this->parents[5].first;
    else if (this -> parents[5].second == 11) p11 = this->parents[5].first;
    else if (this -> parents[5].second == 12) p12 = this->parents[5].first;
    else if (this -> parents[5].second == 13) p13 = this->parents[5].first;
    else if (this -> parents[5].second == 14) p14 = this->parents[5].first;
    else if (this -> parents[5].second == 15) p15 = this->parents[5].first;
    else if (this -> parents[5].second == 16) p16 = this->parents[5].first;
    else if (this -> parents[5].second == 100) s1 = this->parents[5].first;
    else if (this -> parents[5].second == 101) s2 = this->parents[5].first;
    else if (this -> parents[5].second == 102) s3 = this->parents[5].first;
    else if (this -> parents[5].second == 103) s4 = this->parents[5].first;

    if (this->parents[6].second == 1) p1 = this->parents[6].first;
    else if (this->parents[6].second == 2) p2 = this->parents[6].first;
    else if (this -> parents[6].second == 3) p3 = this->parents[6].first;
    else if (this -> parents[6].second == 4) p4 = this->parents[6].first;
    else if (this -> parents[6].second == 5) p5 = this->parents[6].first;
    else if (this -> parents[6].second == 6) p6 = this->parents[6].first;
    else if (this -> parents[6].second == 7) p7 = this->parents[6].first;
    else if (this -> parents[6].second == 8) p8 = this->parents[6].first;
    else if (this -> parents[6].second == 9) p9 = this->parents[6].first;
    else if (this -> parents[6].second == 10) p10 = this->parents[6].first;
    else if (this -> parents[6].second == 11) p11 = this->parents[6].first;
    else if (this -> parents[6].second == 12) p12 = this->parents[6].first;
    else if (this -> parents[6].second == 13) p13 = this->parents[6].first;
    else if (this -> parents[6].second == 14) p14 = this->parents[6].first;
    else if (this -> parents[6].second == 15) p15 = this->parents[6].first;
    else if (this -> parents[6].second == 16) p16 = this->parents[6].first;
    else if (this -> parents[6].second == 100) s1 = this->parents[6].first;
    else if (this -> parents[6].second == 101) s2 = this->parents[6].first ;
    else if (this -> parents[6].second == 102) s3 = this->parents[6].first;
    else if (this -> parents[6].second == 103) s4 = this->parents[6].first;

    if (this->parents[7].second == 1) p1 = this->parents[7].first;
    else if (this->parents[7].second == 2) p2 = this->parents[7].first;
    else if (this -> parents[7].second == 3) p3 = this->parents[7].first;
    else if (this -> parents[7].second == 4) p4 = this->parents[7].first;
    else if (this -> parents[7].second == 5) p5 = this->parents[7].first;
    else if (this -> parents[7].second == 6) p6 = this->parents[7].first;
    else if (this -> parents[7].second == 7) p7 = this->parents[7].first;
    else if (this -> parents[7].second == 8) p8 = this->parents[7].first;
    else if (this -> parents[7].second == 9) p9 = this->parents[7].first;
    else if (this -> parents[7].second == 10) p10 = this->parents[7].first;
    else if (this -> parents[7].second == 11) p11 = this->parents[7].first;
    else if (this -> parents[7].second == 12) p12 = this->parents[7].first;
    else if (this -> parents[7].second == 13) p13 = this->parents[7].first;
    else if (this -> parents[7].second == 14) p14 = this->parents[7].first;
    else if (this -> parents[7].second == 15) p15 = this->parents[7].first;
    else if (this -> parents[7].second == 16) p16 = this->parents[7].first;
    else if (this -> parents[7].second == 100) s1 = this->parents[7].first;
    else if (this -> parents[7].second == 101) s2 = this->parents[7].first;
    else if (this -> parents[7].second == 102) s3 = this->parents[7].first;
    else if (this -> parents[7].second == 103) s4 = this->parents[7].first;

    if (this->parents[8].second == 1) p1 = this->parents[8].first;
    else if (this->parents[8].second == 2) p2 = this->parents[8].first;
    else if (this -> parents[8].second == 3) p3 = this->parents[8].first;
    else if (this -> parents[8].second == 4) p4 = this->parents[8].first;
    else if (this -> parents[8].second == 5) p5 = this->parents[8].first;
    else if (this -> parents[8].second == 6) p6 = this->parents[8].first;
    else if (this -> parents[8].second == 7) p7 = this->parents[8].first;
    else if (this -> parents[8].second == 8) p8 = this->parents[8].first;
    else if (this -> parents[8].second == 9) p9 = this->parents[8].first;
    else if (this -> parents[8].second == 10) p10 = this->parents[8].first;
    else if (this -> parents[8].second == 11) p11 = this->parents[8].first;
    else if (this -> parents[8].second == 12) p12 = this->parents[8].first;
    else if (this -> parents[8].second == 13) p13 = this->parents[8].first;
    else if (this -> parents[8].second == 14) p14 = this->parents[8].first;
    else if (this -> parents[8].second == 15) p15 = this->parents[8].first;
    else if (this -> parents[8].second == 16) p16 = this->parents[8].first;
    else if (this -> parents[8].second == 100) s1 = this->parents[8].first;
    else if (this -> parents[8].second == 101) s2 = this->parents[8].first;
    else if (this -> parents[8].second == 102) s3 = this->parents[8].first;
    else if (this -> parents[8].second == 103) s4 = this->parents[8].first;

    if (this->parents[9].second == 1) p1 = this->parents[9].first;
    else if (this->parents[9].second == 2) p2 = this->parents[9].first;
    else if (this -> parents[9].second == 3) p3 = this->parents[9].first;
    else if (this -> parents[9].second == 4) p4 = this->parents[9].first;
    else if (this -> parents[9].second == 5) p5 = this->parents[9].first;
    else if (this -> parents[9].second == 6) p6 = this->parents[9].first;
    else if (this -> parents[9].second == 7) p7 = this->parents[9].first;
    else if (this -> parents[9].second == 8) p8 = this->parents[9].first;
    else if (this -> parents[9].second == 9) p9 = this->parents[9].first;
    else if (this -> parents[9].second == 10) p10 = this->parents[9].first;
    else if (this -> parents[9].second == 11) p11 = this->parents[9].first;
    else if (this -> parents[9].second == 12) p12 = this->parents[9].first;
    else if (this -> parents[9].second == 13) p13 = this->parents[9].first;
    else if (this -> parents[9].second == 14) p14 = this->parents[9].first;
    else if (this -> parents[9].second == 15) p15 = this->parents[9].first;
    else if (this -> parents[9].second == 16) p16 = this->parents[9].first;
    else if (this -> parents[9].second == 100) s1 = this->parents[9].first;
    else if (this -> parents[9].second == 101) s2 = this->parents[9].first;
    else if (this -> parents[9].second == 102) s3 = this->parents[9].first;
    else if (this -> parents[9].second == 103) s4 = this->parents[9].first;

    if (this->parents[10].second == 1) p1 = this->parents[10].first;
    else if (this->parents[10].second == 2) p2 = this->parents[10].first;
    else if (this -> parents[10].second == 3) p3 = this->parents[10].first;
    else if (this -> parents[10].second == 4) p4 = this->parents[10].first;
    else if (this -> parents[10].second == 5) p5 = this->parents[10].first;
    else if (this -> parents[10].second == 6) p6 = this->parents[10].first;
    else if (this -> parents[10].second == 7) p7 = this->parents[10].first;
    else if (this -> parents[10].second == 8) p8 = this->parents[10].first;
    else if (this -> parents[10].second == 9) p9 = this->parents[10].first;
    else if (this -> parents[10].second == 10) p10 = this->parents[10].first;
    else if (this -> parents[10].second == 11) p11 = this->parents[10].first;
    else if (this -> parents[10].second == 12) p12 = this->parents[10].first;
    else if (this -> parents[10].second == 13) p13 = this->parents[10].first;
    else if (this -> parents[10].second == 14) p14 = this->parents[10].first;
    else if (this -> parents[10].second == 15) p15 = this->parents[10].first;
    else if (this -> parents[10].second == 16) p16 = this->parents[10].first;
    else if (this -> parents[10].second == 100) s1 = this->parents[10].first;
    else if (this -> parents[10].second == 101) s2 = this->parents[10].first;
    else if (this -> parents[10].second == 102) s3 = this->parents[10].first;
    else if (this -> parents[10].second == 103) s4 = this->parents[10].first;

    if (this->parents[11].second == 1) p1 = this->parents[11].first;
    else if (this->parents[11].second == 2) p2 = this->parents[11].first;
    else if (this -> parents[11].second == 3) p3 = this->parents[11].first;
    else if (this -> parents[11].second == 4) p4 = this->parents[11].first;
    else if (this -> parents[11].second == 5) p5 = this->parents[11].first;
    else if (this -> parents[11].second == 6) p6 = this->parents[11].first;
    else if (this -> parents[11].second == 7) p7 = this->parents[11].first;
    else if (this -> parents[11].second == 8) p8 = this->parents[11].first;
    else if (this -> parents[11].second == 9) p9 = this->parents[11].first;
    else if (this -> parents[11].second == 10) p10 = this->parents[11].first;
    else if (this -> parents[11].second == 11) p11 = this->parents[11].first;
    else if (this -> parents[11].second == 12) p12 = this->parents[11].first;
    else if (this -> parents[11].second == 13) p13 = this->parents[11].first;
    else if (this -> parents[11].second == 14) p14 = this->parents[11].first;
    else if (this -> parents[11].second == 15) p15 = this->parents[11].first;
    else if (this -> parents[11].second == 16) p16 = this->parents[11].first;
    else if (this -> parents[11].second == 100) s1 = this->parents[11].first;
    else if (this -> parents[11].second == 101) s2 = this->parents[11].first;
    else if (this -> parents[11].second == 102) s3 = this->parents[11].first;
    else if (this -> parents[11].second == 103) s4 = this->parents[11].first;

    if (this->parents[12].second == 1) p1 = this->parents[12].first;
    else if (this->parents[12].second == 2) p2 = this->parents[12].first;
    else if (this -> parents[12].second == 3) p3 = this->parents[12].first;
    else if (this -> parents[12].second == 4) p4 = this->parents[12].first;
    else if (this -> parents[12].second == 5) p5 = this->parents[12].first;
    else if (this -> parents[12].second == 6) p6 = this->parents[12].first;
    else if (this -> parents[12].second == 7) p7 = this->parents[12].first;
    else if (this -> parents[12].second == 8) p8 = this->parents[12].first;
    else if (this -> parents[12].second == 9) p9 = this->parents[12].first;
    else if (this -> parents[12].second == 10) p10 = this->parents[12].first;
    else if (this -> parents[12].second == 11) p11 = this->parents[12].first;
    else if (this -> parents[12].second == 12) p12 = this->parents[12].first;
    else if (this -> parents[12].second == 13) p13 = this->parents[12].first;
    else if (this -> parents[12].second == 14) p14 = this->parents[12].first;
    else if (this -> parents[12].second == 15) p15 = this->parents[12].first;
    else if (this -> parents[12].second == 16) p16 = this->parents[12].first;
    else if (this -> parents[12].second == 100) s1 = this->parents[12].first;
    else if (this -> parents[12].second == 101) s2 = this->parents[12].first;
    else if (this -> parents[12].second == 102) s3 = this->parents[12].first;
    else if (this -> parents[12].second == 103) s4 = this->parents[12].first;

    if (this->parents[13].second == 1) p1 = this->parents[13].first;
    else if (this->parents[13].second == 2) p2 = this->parents[13].first;
    else if (this -> parents[13].second == 3) p3 = this->parents[13].first;
    else if (this -> parents[13].second == 4) p4 = this->parents[13].first;
    else if (this -> parents[13].second == 5) p5 = this->parents[13].first;
    else if (this -> parents[13].second == 6) p6 = this->parents[13].first;
    else if (this -> parents[13].second == 7) p7 = this->parents[13].first;
    else if (this -> parents[13].second == 8) p8 = this->parents[13].first;
    else if (this -> parents[13].second == 9) p9 = this->parents[13].first;
    else if (this -> parents[13].second == 10) p10 = this->parents[13].first;
    else if (this -> parents[13].second == 11) p11 = this->parents[13].first;
    else if (this -> parents[13].second == 12) p12 = this->parents[13].first;
    else if (this -> parents[13].second == 13) p13 = this->parents[13].first;
    else if (this -> parents[13].second == 14) p14 = this->parents[13].first;
    else if (this -> parents[13].second == 15) p15 = this->parents[13].first;
    else if (this -> parents[13].second == 16) p16 = this->parents[13].first;
    else if (this -> parents[13].second == 100) s1 = this->parents[13].first;
    else if (this -> parents[13].second == 101) s2 = this->parents[13].first;
    else if (this -> parents[13].second == 102) s3 = this->parents[13].first;
    else if (this -> parents[13].second == 103) s4 = this->parents[13].first;

    if (this->parents[14].second == 1) p1 = this->parents[14].first;
    else if (this->parents[14].second == 2) p2 = this->parents[14].first;
    else if (this -> parents[14].second == 3) p3 = this->parents[14].first;
    else if (this -> parents[14].second == 4) p4 = this->parents[14].first;
    else if (this -> parents[14].second == 5) p5 = this->parents[14].first;
    else if (this -> parents[14].second == 6) p6 = this->parents[14].first;
    else if (this -> parents[14].second == 7) p7 = this->parents[14].first;
    else if (this -> parents[14].second == 8) p8 = this->parents[14].first;
    else if (this -> parents[14].second == 9) p9 = this->parents[14].first;
    else if (this -> parents[14].second == 10) p10 = this->parents[14].first;
    else if (this -> parents[14].second == 11) p11 = this->parents[14].first;
    else if (this -> parents[14].second == 12) p12 = this->parents[14].first;
    else if (this -> parents[14].second == 13) p13 = this->parents[14].first;
    else if (this -> parents[14].second == 14) p14 = this->parents[14].first;
    else if (this -> parents[14].second == 15) p15 = this->parents[14].first;
    else if (this -> parents[14].second == 16) p16 = this->parents[14].first;
    else if (this -> parents[14].second == 100) s1 = this->parents[14].first;
    else if (this -> parents[14].second == 101) s2 = this->parents[14].first;
    else if (this -> parents[14].second == 102) s3 = this->parents[14].first;
    else if (this -> parents[14].second == 103) s4 = this->parents[14].first;

    if (this->parents[15].second == 1) p1 = this->parents[15].first;
    else if (this->parents[15].second == 2) p2 = this->parents[15].first;
    else if (this -> parents[15].second == 3) p3 = this->parents[15].first;
    else if (this -> parents[15].second == 4) p4 = this->parents[15].first;
    else if (this -> parents[15].second == 5) p5 = this->parents[15].first;
    else if (this -> parents[15].second == 6) p6 = this->parents[15].first;
    else if (this -> parents[15].second == 7) p7 = this->parents[15].first;
    else if (this -> parents[15].second == 8) p8 = this->parents[15].first;
    else if (this -> parents[15].second == 9) p9 = this->parents[15].first;
    else if (this -> parents[15].second == 10) p10 = this->parents[15].first;
    else if (this -> parents[15].second == 11) p11 = this->parents[15].first;
    else if (this -> parents[15].second == 12) p12 = this->parents[15].first;
    else if (this -> parents[15].second == 13) p13 = this->parents[15].first;
    else if (this -> parents[15].second == 14) p14 = this->parents[15].first;
    else if (this -> parents[15].second == 15) p15 = this->parents[15].first;
    else if (this -> parents[15].second == 16) p16 = this->parents[15].first;
    else if (this -> parents[15].second == 100) s1 = this->parents[15].first;
    else if (this -> parents[15].second == 101) s2 = this->parents[15].first;
    else if (this -> parents[15].second == 102) s3 = this->parents[15].first;
    else if (this -> parents[15].second == 103) s4 = this->parents[15].first;

    if (this->parents[16].second == 1) p1 = this->parents[16].first;
    else if (this->parents[16].second == 2) p2 = this->parents[16].first;
    else if (this -> parents[16].second == 3) p3 = this->parents[16].first;
    else if (this -> parents[16].second == 4) p4 = this->parents[16].first;
    else if (this -> parents[16].second == 5) p5 = this->parents[16].first;
    else if (this -> parents[16].second == 6) p6 = this->parents[16].first;
    else if (this -> parents[16].second == 7) p7 = this->parents[16].first;
    else if (this -> parents[16].second == 8) p8 = this->parents[16].first;
    else if (this -> parents[16].second == 9) p9 = this->parents[16].first;
    else if (this -> parents[16].second == 10) p10 = this->parents[16].first;
    else if (this -> parents[16].second == 11) p11 = this->parents[16].first;
    else if (this -> parents[16].second == 12) p12 = this->parents[16].first;
    else if (this -> parents[16].second == 13) p13 = this->parents[16].first;
    else if (this -> parents[16].second == 14) p14 = this->parents[16].first;
    else if (this -> parents[16].second == 15) p15 = this->parents[16].first;
    else if (this -> parents[16].second == 16) p16 = this->parents[16].first;
    else if (this -> parents[16].second == 100) s1 = this->parents[16].first;
    else if (this -> parents[16].second == 101) s2 = this->parents[16].first;
    else if (this -> parents[16].second == 102) s3 = this->parents[16].first;
    else if (this -> parents[16].second == 103) s4 = this->parents[16].first;

    if (this->parents[17].second == 1) p1 = this->parents[17].first;
    else if (this->parents[17].second == 2) p2 = this->parents[17].first;
    else if (this -> parents[17].second == 3) p3 = this->parents[17].first;
    else if (this -> parents[17].second == 4) p4 = this->parents[17].first;
    else if (this -> parents[17].second == 5) p5 = this->parents[17].first;
    else if (this -> parents[17].second == 6) p6 = this->parents[17].first;
    else if (this -> parents[17].second == 7) p7 = this->parents[17].first;
    else if (this -> parents[17].second == 8) p8 = this->parents[17].first;
    else if (this -> parents[17].second == 9) p9 = this->parents[17].first;
    else if (this -> parents[17].second == 10) p10 = this->parents[17].first;
    else if (this -> parents[17].second == 11) p11 = this->parents[17].first;
    else if (this -> parents[17].second == 12) p12 = this->parents[17].first;
    else if (this -> parents[17].second == 13) p13 = this->parents[17].first;
    else if (this -> parents[17].second == 14) p14 = this->parents[17].first;
    else if (this -> parents[17].second == 15) p15 = this->parents[17].first;
    else if (this -> parents[17].second == 16) p16 = this->parents[17].first;
    else if (this -> parents[17].second == 100) s1 = this->parents[17].first;
    else if (this -> parents[17].second == 101) s2 = this->parents[17].first;
    else if (this -> parents[17].second == 102) s3 = this->parents[17].first;
    else if (this -> parents[17].second == 103) s4 = this->parents[17].first;

    if (this->parents[18].second == 1) p1 = this->parents[18].first;
    else if (this->parents[18].second == 2) p2 = this->parents[18].first;
    else if (this -> parents[18].second == 3) p3 = this->parents[18].first;
    else if (this -> parents[18].second == 4) p4 = this->parents[18].first;
    else if (this -> parents[18].second == 5) p5 = this->parents[18].first;
    else if (this -> parents[18].second == 6) p6 = this->parents[18].first;
    else if (this -> parents[18].second == 7) p7 = this->parents[18].first;
    else if (this -> parents[18].second == 8) p8 = this->parents[18].first;
    else if (this -> parents[18].second == 9) p9 = this->parents[18].first;
    else if (this -> parents[18].second == 10) p10 = this->parents[18].first;
    else if (this -> parents[18].second == 11) p11 = this->parents[18].first;
    else if (this -> parents[18].second == 12) p12 = this->parents[18].first;
    else if (this -> parents[18].second == 13) p13 = this->parents[18].first;
    else if (this -> parents[18].second == 14) p14 = this->parents[18].first;
    else if (this -> parents[18].second == 15) p15 = this->parents[18].first;
    else if (this -> parents[18].second == 16) p16 = this->parents[18].first;
    else if (this -> parents[18].second == 100) s1 = this->parents[18].first;
    else if (this -> parents[18].second == 101) s2 = this->parents[18].first;
    else if (this -> parents[18].second == 102) s3 = this->parents[18].first;
    else if (this -> parents[18].second == 103) s4 = this->parents[18].first;

    if (this->parents[19].second == 1) p1 = this->parents[19].first;
    else if (this->parents[19].second == 2) p2 = this->parents[19].first;
    else if (this -> parents[19].second == 3) p3 = this->parents[19].first;
    else if (this -> parents[19].second == 4) p4 = this->parents[19].first;
    else if (this -> parents[19].second == 5) p5 = this->parents[19].first;
    else if (this -> parents[19].second == 6) p6 = this->parents[19].first;
    else if (this -> parents[19].second == 7) p7 = this->parents[19].first;
    else if (this -> parents[19].second == 8) p8 = this->parents[19].first;
    else if (this -> parents[19].second == 9) p9 = this->parents[19].first;
    else if (this -> parents[19].second == 10) p10 = this->parents[19].first;
    else if (this -> parents[19].second == 11) p11 = this->parents[19].first;
    else if (this -> parents[19].second == 12) p12 = this->parents[19].first;
    else if (this -> parents[19].second == 13) p13 = this->parents[19].first;
    else if (this -> parents[19].second == 14) p14 = this->parents[19].first;
    else if (this -> parents[19].second == 15) p15 = this->parents[19].first;
    else if (this -> parents[19].second == 16) p16 = this->parents[19].first;
    else if (this -> parents[19].second == 100) s1 = this->parents[19].first;
    else if (this -> parents[19].second == 101) s2 = this->parents[19].first;
    else if (this -> parents[19].second == 102) s3 = this->parents[19].first;
    else if (this -> parents[19].second == 103) s4 = this->parents[19].first;

    if (this->parents[20].second == 1) p1 = this->parents[20].first;
    else if (this->parents[20].second == 2) p2 = this->parents[20].first;
    else if (this -> parents[20].second == 3) p3 = this->parents[20].first;
    else if (this -> parents[20].second == 4) p4 = this->parents[20].first;
    else if (this -> parents[20].second == 5) p5 = this->parents[20].first;
    else if (this -> parents[20].second == 6) p6 = this->parents[20].first;
    else if (this -> parents[20].second == 7) p7 = this->parents[20].first;
    else if (this -> parents[20].second == 8) p8 = this->parents[20].first;
    else if (this -> parents[20].second == 9) p9 = this->parents[20].first;
    else if (this -> parents[20].second == 10) p10 = this->parents[20].first;
    else if (this -> parents[20].second == 11) p11 = this->parents[20].first;
    else if (this -> parents[20].second == 12) p12 = this->parents[20].first;
    else if (this -> parents[20].second == 13) p13 = this->parents[20].first;
    else if (this -> parents[20].second == 14) p14 = this->parents[20].first;
    else if (this -> parents[20].second == 15) p15 = this->parents[20].first;
    else if (this -> parents[20].second == 16) p16 = this->parents[20].first;
    else if (this -> parents[20].second == 100) s1 = this->parents[20].first;
    else if (this -> parents[20].second == 101) s2 = this->parents[20].first;
    else if (this -> parents[20].second == 102) s3 = this->parents[20].first;
    else if (this -> parents[20].second == 103) s4 = this->parents[20].first;

    //compute
    if (value == 0) {
        result.push_back(std::make_pair(p1, 0));
        result.push_back(std::make_pair(p2, 0));
        result.push_back(std::make_pair(p3, 0));
        result.push_back(std::make_pair(p4, 0));
        result.push_back(std::make_pair(p5, 0));
        result.push_back(std::make_pair(p6, 0));
        result.push_back(std::make_pair(p7, 0));
        result.push_back(std::make_pair(p8, 0));
        result.push_back(std::make_pair(p9, 0));
        result.push_back(std::make_pair(p10, 0));
        result.push_back(std::make_pair(p11, 0));
        result.push_back(std::make_pair(p12, 0));
        result.push_back(std::make_pair(p13, 0));
        result.push_back(std::make_pair(p14, 0));
        result.push_back(std::make_pair(p15, 0));
        result.push_back(std::make_pair(p16, 0));
        result.push_back(std::make_pair(s1, 0));
        result.push_back(std::make_pair(s2, 0));
        result.push_back(std::make_pair(s3, 0));
        result.push_back(std::make_pair(s4, 0));
    }
    else if (value == 1){
        result.push_back(std::make_pair(p1, 1));
        result.push_back(std::make_pair(p2, 1));
        result.push_back(std::make_pair(p3, 1));
        result.push_back(std::make_pair(p4, 1));
        result.push_back(std::make_pair(p5, 1));
        result.push_back(std::make_pair(p6, 1));
        result.push_back(std::make_pair(p7, 1));
        result.push_back(std::make_pair(p8, 1));
        result.push_back(std::make_pair(p9, 1));
        result.push_back(std::make_pair(p10, 1));
        result.push_back(std::make_pair(p11, 1));
        result.push_back(std::make_pair(p12, 1));
        result.push_back(std::make_pair(p13, 1));
        result.push_back(std::make_pair(p14, 1));
        result.push_back(std::make_pair(p15, 1));
        result.push_back(std::make_pair(p16, 1));
        result.push_back(std::make_pair(s1, 1));
        result.push_back(std::make_pair(s2, 1));
        result.push_back(std::make_pair(s3, 1));
        result.push_back(std::make_pair(s4, 1));
    }
    else std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": value not correct for a Mux16" << std::endl;

    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> Mux16::computeExpectedValue(int value, int port_number){
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    //need to check the input's number (or signal)
    std::shared_ptr<Node> p1;
    std::shared_ptr<Node> p2;
    std::shared_ptr<Node> p3;
    std::shared_ptr<Node> p4;
    std::shared_ptr<Node> p5;
    std::shared_ptr<Node> p6;
    std::shared_ptr<Node> p7;
    std::shared_ptr<Node> p8;
    std::shared_ptr<Node> p9;
    std::shared_ptr<Node> p10;
    std::shared_ptr<Node> p11;
    std::shared_ptr<Node> p12;
    std::shared_ptr<Node> p13;
    std::shared_ptr<Node> p14;
    std::shared_ptr<Node> p15;
    std::shared_ptr<Node> p16;
    std::shared_ptr<Node> s1;
    std::shared_ptr<Node> s2;
    std::shared_ptr<Node> s3;
    std::shared_ptr<Node> s4;

    if (this->parents[0].second == 1) p1 = this->parents[0].first;
    else if (this->parents[0].second == 2) p2 = this->parents[0].first;
    else if (this -> parents[0].second == 3) p3 = this->parents[0].first;
    else if (this -> parents[0].second == 4) p4 = this->parents[0].first;
    else if (this -> parents[0].second == 5) p5 = this->parents[0].first;
    else if (this -> parents[0].second == 6) p6 = this->parents[0].first;
    else if (this -> parents[0].second == 7) p7 = this->parents[0].first;
    else if (this -> parents[0].second == 8) p8 = this->parents[0].first;
    else if (this -> parents[0].second == 9) p9 = this->parents[0].first;
    else if (this -> parents[0].second == 10) p10 = this->parents[0].first;
    else if (this -> parents[0].second == 11) p11 = this->parents[0].first;
    else if (this -> parents[0].second == 12) p12 = this->parents[0].first;
    else if (this -> parents[0].second == 13) p13 = this->parents[0].first;
    else if (this -> parents[0].second == 14) p14 = this->parents[0].first;
    else if (this -> parents[0].second == 15) p15 = this->parents[0].first;
    else if (this -> parents[0].second == 16) p16 = this->parents[0].first;
    else if (this -> parents[0].second == 100) s1 = this->parents[0].first;
    else if (this -> parents[0].second == 101) s2 = this->parents[0].first;
    else if (this -> parents[0].second == 102) s3 = this->parents[0].first;
    else if (this -> parents[0].second == 103) s4 = this->parents[0].first;

    if (this->parents[1].second == 1) p1 = this->parents[1].first;
    else if (this->parents[1].second == 2) p2 = this->parents[1].first;
    else if (this -> parents[1].second == 3) p3 = this->parents[1].first;
    else if (this -> parents[1].second == 4) p4 = this->parents[1].first;
    else if (this -> parents[1].second == 5) p5 = this->parents[1].first;
    else if (this -> parents[1].second == 6) p6 = this->parents[1].first;
    else if (this -> parents[1].second == 7) p7 = this->parents[1].first;
    else if (this -> parents[1].second == 8) p8 = this->parents[1].first;
    else if (this -> parents[1].second == 9) p9 = this->parents[1].first;
    else if (this -> parents[1].second == 10) p10 = this->parents[1].first;
    else if (this -> parents[1].second == 11) p11 = this->parents[1].first;
    else if (this -> parents[1].second == 12) p12 = this->parents[1].first;
    else if (this -> parents[1].second == 13) p13 = this->parents[1].first;
    else if (this -> parents[1].second == 14) p14 = this->parents[1].first;
    else if (this -> parents[1].second == 15) p15 = this->parents[1].first;
    else if (this -> parents[1].second == 16) p16 = this->parents[1].first;
    else if (this -> parents[1].second == 100) s1 = this->parents[1].first;
    else if (this -> parents[1].second == 101) s2 = this->parents[1].first;
    else if (this -> parents[1].second == 102) s3 = this->parents[1].first;
    else if (this -> parents[1].second == 103) s4 = this->parents[1].first;

    if (this->parents[2].second == 1) p1 = this->parents[2].first;
    else if (this->parents[2].second == 2) p2 = this->parents[2].first;
    else if (this -> parents[2].second == 3) p3 = this->parents[2].first;
    else if (this -> parents[2].second == 4) p4 = this->parents[2].first;
    else if (this -> parents[2].second == 5) p5 = this->parents[2].first;
    else if (this -> parents[2].second == 6) p6 = this->parents[2].first;
    else if (this -> parents[2].second == 7) p7 = this->parents[2].first;
    else if (this -> parents[2].second == 8) p8 = this->parents[2].first;
    else if (this -> parents[2].second == 9) p9 = this->parents[2].first;
    else if (this -> parents[2].second == 10) p10 = this->parents[2].first;
    else if (this -> parents[2].second == 11) p11 = this->parents[2].first;
    else if (this -> parents[2].second == 12) p12 = this->parents[2].first;
    else if (this -> parents[2].second == 13) p13 = this->parents[2].first;
    else if (this -> parents[2].second == 14) p14 = this->parents[2].first;
    else if (this -> parents[2].second == 15) p15 = this->parents[2].first;
    else if (this -> parents[2].second == 16) p16 = this->parents[2].first;
    else if (this -> parents[2].second == 100) s1 = this->parents[2].first;
    else if (this -> parents[2].second == 101) s2 = this->parents[2].first;
    else if (this -> parents[2].second == 102) s3 = this->parents[2].first;
    else if (this -> parents[2].second == 103) s4 = this->parents[2].first;

    if (this->parents[3].second == 1) p1 = this->parents[3].first;
    else if (this->parents[3].second == 2) p2 = this->parents[3].first;
    else if (this -> parents[3].second == 3) p3 = this->parents[3].first;
    else if (this -> parents[3].second == 4) p4 = this->parents[3].first;
    else if (this -> parents[3].second == 5) p5 = this->parents[3].first;
    else if (this -> parents[3].second == 6) p6 = this->parents[3].first;
    else if (this -> parents[3].second == 7) p7 = this->parents[3].first;
    else if (this -> parents[3].second == 8) p8 = this->parents[3].first;
    else if (this -> parents[3].second == 9) p9 = this->parents[3].first;
    else if (this -> parents[3].second == 10) p10 = this->parents[3].first;
    else if (this -> parents[3].second == 11) p11 = this->parents[3].first;
    else if (this -> parents[3].second == 12) p12 = this->parents[3].first;
    else if (this -> parents[3].second == 13) p13 = this->parents[3].first;
    else if (this -> parents[3].second == 14) p14 = this->parents[3].first;
    else if (this -> parents[3].second == 15) p15 = this->parents[3].first;
    else if (this -> parents[3].second == 16) p16 = this->parents[3].first;
    else if (this -> parents[3].second == 100) s1 = this->parents[3].first;
    else if (this -> parents[3].second == 101) s2 = this->parents[3].first;
    else if (this -> parents[3].second == 102) s3 = this->parents[3].first;
    else if (this -> parents[3].second == 103) s4 = this->parents[3].first;

    if (this->parents[4].second == 1) p1 = this->parents[4].first;
    else if (this->parents[4].second == 2) p2 = this->parents[4].first;
    else if (this -> parents[4].second == 3) p3 = this->parents[4].first;
    else if (this -> parents[4].second == 4) p4 = this->parents[4].first;
    else if (this -> parents[4].second == 5) p5 = this->parents[4].first;
    else if (this -> parents[4].second == 6) p6 = this->parents[4].first;
    else if (this -> parents[4].second == 7) p7 = this->parents[4].first;
    else if (this -> parents[4].second == 8) p8 = this->parents[4].first;
    else if (this -> parents[4].second == 9) p9 = this->parents[4].first;
    else if (this -> parents[4].second == 10) p10 = this->parents[4].first;
    else if (this -> parents[4].second == 11) p11 = this->parents[4].first;
    else if (this -> parents[4].second == 12) p12 = this->parents[4].first;
    else if (this -> parents[4].second == 13) p13 = this->parents[4].first;
    else if (this -> parents[4].second == 14) p14 = this->parents[4].first;
    else if (this -> parents[4].second == 15) p15 = this->parents[4].first;
    else if (this -> parents[4].second == 16) p16 = this->parents[4].first;
    else if (this -> parents[4].second == 100) s1 = this->parents[4].first;
    else if (this -> parents[4].second == 101) s2 = this->parents[4].first;
    else if (this -> parents[4].second == 102) s3 = this->parents[4].first;
    else if (this -> parents[4].second == 103) s4 = this->parents[4].first;

    if (this->parents[5].second == 1) p1 = this->parents[5].first;
    else if (this->parents[5].second == 2) p2 = this->parents[5].first;
    else if (this -> parents[5].second == 3) p3 = this->parents[5].first;
    else if (this -> parents[5].second == 4) p4 = this->parents[5].first;
    else if (this -> parents[5].second == 5) p5 = this->parents[5].first;
    else if (this -> parents[5].second == 6) p6 = this->parents[5].first;
    else if (this -> parents[5].second == 7) p7 = this->parents[5].first;
    else if (this -> parents[5].second == 8) p8 = this->parents[5].first;
    else if (this -> parents[5].second == 9) p9 = this->parents[5].first;
    else if (this -> parents[5].second == 10) p10 = this->parents[5].first;
    else if (this -> parents[5].second == 11) p11 = this->parents[5].first;
    else if (this -> parents[5].second == 12) p12 = this->parents[5].first;
    else if (this -> parents[5].second == 13) p13 = this->parents[5].first;
    else if (this -> parents[5].second == 14) p14 = this->parents[5].first;
    else if (this -> parents[5].second == 15) p15 = this->parents[5].first;
    else if (this -> parents[5].second == 16) p16 = this->parents[5].first;
    else if (this -> parents[5].second == 100) s1 = this->parents[5].first;
    else if (this -> parents[5].second == 101) s2 = this->parents[5].first;
    else if (this -> parents[5].second == 102) s3 = this->parents[5].first;
    else if (this -> parents[5].second == 103) s4 = this->parents[5].first;

    if (this->parents[6].second == 1) p1 = this->parents[6].first;
    else if (this->parents[6].second == 2) p2 = this->parents[6].first;
    else if (this -> parents[6].second == 3) p3 = this->parents[6].first;
    else if (this -> parents[6].second == 4) p4 = this->parents[6].first;
    else if (this -> parents[6].second == 5) p5 = this->parents[6].first;
    else if (this -> parents[6].second == 6) p6 = this->parents[6].first;
    else if (this -> parents[6].second == 7) p7 = this->parents[6].first;
    else if (this -> parents[6].second == 8) p8 = this->parents[6].first;
    else if (this -> parents[6].second == 9) p9 = this->parents[6].first;
    else if (this -> parents[6].second == 10) p10 = this->parents[6].first;
    else if (this -> parents[6].second == 11) p11 = this->parents[6].first;
    else if (this -> parents[6].second == 12) p12 = this->parents[6].first;
    else if (this -> parents[6].second == 13) p13 = this->parents[6].first;
    else if (this -> parents[6].second == 14) p14 = this->parents[6].first;
    else if (this -> parents[6].second == 15) p15 = this->parents[6].first;
    else if (this -> parents[6].second == 16) p16 = this->parents[6].first;
    else if (this -> parents[6].second == 100) s1 = this->parents[6].first;
    else if (this -> parents[6].second == 101) s2 = this->parents[6].first ;
    else if (this -> parents[6].second == 102) s3 = this->parents[6].first;
    else if (this -> parents[6].second == 103) s4 = this->parents[6].first;

    if (this->parents[7].second == 1) p1 = this->parents[7].first;
    else if (this->parents[7].second == 2) p2 = this->parents[7].first;
    else if (this -> parents[7].second == 3) p3 = this->parents[7].first;
    else if (this -> parents[7].second == 4) p4 = this->parents[7].first;
    else if (this -> parents[7].second == 5) p5 = this->parents[7].first;
    else if (this -> parents[7].second == 6) p6 = this->parents[7].first;
    else if (this -> parents[7].second == 7) p7 = this->parents[7].first;
    else if (this -> parents[7].second == 8) p8 = this->parents[7].first;
    else if (this -> parents[7].second == 9) p9 = this->parents[7].first;
    else if (this -> parents[7].second == 10) p10 = this->parents[7].first;
    else if (this -> parents[7].second == 11) p11 = this->parents[7].first;
    else if (this -> parents[7].second == 12) p12 = this->parents[7].first;
    else if (this -> parents[7].second == 13) p13 = this->parents[7].first;
    else if (this -> parents[7].second == 14) p14 = this->parents[7].first;
    else if (this -> parents[7].second == 15) p15 = this->parents[7].first;
    else if (this -> parents[7].second == 16) p16 = this->parents[7].first;
    else if (this -> parents[7].second == 100) s1 = this->parents[7].first;
    else if (this -> parents[7].second == 101) s2 = this->parents[7].first;
    else if (this -> parents[7].second == 102) s3 = this->parents[7].first;
    else if (this -> parents[7].second == 103) s4 = this->parents[7].first;

    if (this->parents[8].second == 1) p1 = this->parents[8].first;
    else if (this->parents[8].second == 2) p2 = this->parents[8].first;
    else if (this -> parents[8].second == 3) p3 = this->parents[8].first;
    else if (this -> parents[8].second == 4) p4 = this->parents[8].first;
    else if (this -> parents[8].second == 5) p5 = this->parents[8].first;
    else if (this -> parents[8].second == 6) p6 = this->parents[8].first;
    else if (this -> parents[8].second == 7) p7 = this->parents[8].first;
    else if (this -> parents[8].second == 8) p8 = this->parents[8].first;
    else if (this -> parents[8].second == 9) p9 = this->parents[8].first;
    else if (this -> parents[8].second == 10) p10 = this->parents[8].first;
    else if (this -> parents[8].second == 11) p11 = this->parents[8].first;
    else if (this -> parents[8].second == 12) p12 = this->parents[8].first;
    else if (this -> parents[8].second == 13) p13 = this->parents[8].first;
    else if (this -> parents[8].second == 14) p14 = this->parents[8].first;
    else if (this -> parents[8].second == 15) p15 = this->parents[8].first;
    else if (this -> parents[8].second == 16) p16 = this->parents[8].first;
    else if (this -> parents[8].second == 100) s1 = this->parents[8].first;
    else if (this -> parents[8].second == 101) s2 = this->parents[8].first;
    else if (this -> parents[8].second == 102) s3 = this->parents[8].first;
    else if (this -> parents[8].second == 103) s4 = this->parents[8].first;

    if (this->parents[9].second == 1) p1 = this->parents[9].first;
    else if (this->parents[9].second == 2) p2 = this->parents[9].first;
    else if (this -> parents[9].second == 3) p3 = this->parents[9].first;
    else if (this -> parents[9].second == 4) p4 = this->parents[9].first;
    else if (this -> parents[9].second == 5) p5 = this->parents[9].first;
    else if (this -> parents[9].second == 6) p6 = this->parents[9].first;
    else if (this -> parents[9].second == 7) p7 = this->parents[9].first;
    else if (this -> parents[9].second == 8) p8 = this->parents[9].first;
    else if (this -> parents[9].second == 9) p9 = this->parents[9].first;
    else if (this -> parents[9].second == 10) p10 = this->parents[9].first;
    else if (this -> parents[9].second == 11) p11 = this->parents[9].first;
    else if (this -> parents[9].second == 12) p12 = this->parents[9].first;
    else if (this -> parents[9].second == 13) p13 = this->parents[9].first;
    else if (this -> parents[9].second == 14) p14 = this->parents[9].first;
    else if (this -> parents[9].second == 15) p15 = this->parents[9].first;
    else if (this -> parents[9].second == 16) p16 = this->parents[9].first;
    else if (this -> parents[9].second == 100) s1 = this->parents[9].first;
    else if (this -> parents[9].second == 101) s2 = this->parents[9].first;
    else if (this -> parents[9].second == 102) s3 = this->parents[9].first;
    else if (this -> parents[9].second == 103) s4 = this->parents[9].first;

    if (this->parents[10].second == 1) p1 = this->parents[10].first;
    else if (this->parents[10].second == 2) p2 = this->parents[10].first;
    else if (this -> parents[10].second == 3) p3 = this->parents[10].first;
    else if (this -> parents[10].second == 4) p4 = this->parents[10].first;
    else if (this -> parents[10].second == 5) p5 = this->parents[10].first;
    else if (this -> parents[10].second == 6) p6 = this->parents[10].first;
    else if (this -> parents[10].second == 7) p7 = this->parents[10].first;
    else if (this -> parents[10].second == 8) p8 = this->parents[10].first;
    else if (this -> parents[10].second == 9) p9 = this->parents[10].first;
    else if (this -> parents[10].second == 10) p10 = this->parents[10].first;
    else if (this -> parents[10].second == 11) p11 = this->parents[10].first;
    else if (this -> parents[10].second == 12) p12 = this->parents[10].first;
    else if (this -> parents[10].second == 13) p13 = this->parents[10].first;
    else if (this -> parents[10].second == 14) p14 = this->parents[10].first;
    else if (this -> parents[10].second == 15) p15 = this->parents[10].first;
    else if (this -> parents[10].second == 16) p16 = this->parents[10].first;
    else if (this -> parents[10].second == 100) s1 = this->parents[10].first;
    else if (this -> parents[10].second == 101) s2 = this->parents[10].first;
    else if (this -> parents[10].second == 102) s3 = this->parents[10].first;
    else if (this -> parents[10].second == 103) s4 = this->parents[10].first;

    if (this->parents[11].second == 1) p1 = this->parents[11].first;
    else if (this->parents[11].second == 2) p2 = this->parents[11].first;
    else if (this -> parents[11].second == 3) p3 = this->parents[11].first;
    else if (this -> parents[11].second == 4) p4 = this->parents[11].first;
    else if (this -> parents[11].second == 5) p5 = this->parents[11].first;
    else if (this -> parents[11].second == 6) p6 = this->parents[11].first;
    else if (this -> parents[11].second == 7) p7 = this->parents[11].first;
    else if (this -> parents[11].second == 8) p8 = this->parents[11].first;
    else if (this -> parents[11].second == 9) p9 = this->parents[11].first;
    else if (this -> parents[11].second == 10) p10 = this->parents[11].first;
    else if (this -> parents[11].second == 11) p11 = this->parents[11].first;
    else if (this -> parents[11].second == 12) p12 = this->parents[11].first;
    else if (this -> parents[11].second == 13) p13 = this->parents[11].first;
    else if (this -> parents[11].second == 14) p14 = this->parents[11].first;
    else if (this -> parents[11].second == 15) p15 = this->parents[11].first;
    else if (this -> parents[11].second == 16) p16 = this->parents[11].first;
    else if (this -> parents[11].second == 100) s1 = this->parents[11].first;
    else if (this -> parents[11].second == 101) s2 = this->parents[11].first;
    else if (this -> parents[11].second == 102) s3 = this->parents[11].first;
    else if (this -> parents[11].second == 103) s4 = this->parents[11].first;

    if (this->parents[12].second == 1) p1 = this->parents[12].first;
    else if (this->parents[12].second == 2) p2 = this->parents[12].first;
    else if (this -> parents[12].second == 3) p3 = this->parents[12].first;
    else if (this -> parents[12].second == 4) p4 = this->parents[12].first;
    else if (this -> parents[12].second == 5) p5 = this->parents[12].first;
    else if (this -> parents[12].second == 6) p6 = this->parents[12].first;
    else if (this -> parents[12].second == 7) p7 = this->parents[12].first;
    else if (this -> parents[12].second == 8) p8 = this->parents[12].first;
    else if (this -> parents[12].second == 9) p9 = this->parents[12].first;
    else if (this -> parents[12].second == 10) p10 = this->parents[12].first;
    else if (this -> parents[12].second == 11) p11 = this->parents[12].first;
    else if (this -> parents[12].second == 12) p12 = this->parents[12].first;
    else if (this -> parents[12].second == 13) p13 = this->parents[12].first;
    else if (this -> parents[12].second == 14) p14 = this->parents[12].first;
    else if (this -> parents[12].second == 15) p15 = this->parents[12].first;
    else if (this -> parents[12].second == 16) p16 = this->parents[12].first;
    else if (this -> parents[12].second == 100) s1 = this->parents[12].first;
    else if (this -> parents[12].second == 101) s2 = this->parents[12].first;
    else if (this -> parents[12].second == 102) s3 = this->parents[12].first;
    else if (this -> parents[12].second == 103) s4 = this->parents[12].first;

    if (this->parents[13].second == 1) p1 = this->parents[13].first;
    else if (this->parents[13].second == 2) p2 = this->parents[13].first;
    else if (this -> parents[13].second == 3) p3 = this->parents[13].first;
    else if (this -> parents[13].second == 4) p4 = this->parents[13].first;
    else if (this -> parents[13].second == 5) p5 = this->parents[13].first;
    else if (this -> parents[13].second == 6) p6 = this->parents[13].first;
    else if (this -> parents[13].second == 7) p7 = this->parents[13].first;
    else if (this -> parents[13].second == 8) p8 = this->parents[13].first;
    else if (this -> parents[13].second == 9) p9 = this->parents[13].first;
    else if (this -> parents[13].second == 10) p10 = this->parents[13].first;
    else if (this -> parents[13].second == 11) p11 = this->parents[13].first;
    else if (this -> parents[13].second == 12) p12 = this->parents[13].first;
    else if (this -> parents[13].second == 13) p13 = this->parents[13].first;
    else if (this -> parents[13].second == 14) p14 = this->parents[13].first;
    else if (this -> parents[13].second == 15) p15 = this->parents[13].first;
    else if (this -> parents[13].second == 16) p16 = this->parents[13].first;
    else if (this -> parents[13].second == 100) s1 = this->parents[13].first;
    else if (this -> parents[13].second == 101) s2 = this->parents[13].first;
    else if (this -> parents[13].second == 102) s3 = this->parents[13].first;
    else if (this -> parents[13].second == 103) s4 = this->parents[13].first;

    if (this->parents[14].second == 1) p1 = this->parents[14].first;
    else if (this->parents[14].second == 2) p2 = this->parents[14].first;
    else if (this -> parents[14].second == 3) p3 = this->parents[14].first;
    else if (this -> parents[14].second == 4) p4 = this->parents[14].first;
    else if (this -> parents[14].second == 5) p5 = this->parents[14].first;
    else if (this -> parents[14].second == 6) p6 = this->parents[14].first;
    else if (this -> parents[14].second == 7) p7 = this->parents[14].first;
    else if (this -> parents[14].second == 8) p8 = this->parents[14].first;
    else if (this -> parents[14].second == 9) p9 = this->parents[14].first;
    else if (this -> parents[14].second == 10) p10 = this->parents[14].first;
    else if (this -> parents[14].second == 11) p11 = this->parents[14].first;
    else if (this -> parents[14].second == 12) p12 = this->parents[14].first;
    else if (this -> parents[14].second == 13) p13 = this->parents[14].first;
    else if (this -> parents[14].second == 14) p14 = this->parents[14].first;
    else if (this -> parents[14].second == 15) p15 = this->parents[14].first;
    else if (this -> parents[14].second == 16) p16 = this->parents[14].first;
    else if (this -> parents[14].second == 100) s1 = this->parents[14].first;
    else if (this -> parents[14].second == 101) s2 = this->parents[14].first;
    else if (this -> parents[14].second == 102) s3 = this->parents[14].first;
    else if (this -> parents[14].second == 103) s4 = this->parents[14].first;

    if (this->parents[15].second == 1) p1 = this->parents[15].first;
    else if (this->parents[15].second == 2) p2 = this->parents[15].first;
    else if (this -> parents[15].second == 3) p3 = this->parents[15].first;
    else if (this -> parents[15].second == 4) p4 = this->parents[15].first;
    else if (this -> parents[15].second == 5) p5 = this->parents[15].first;
    else if (this -> parents[15].second == 6) p6 = this->parents[15].first;
    else if (this -> parents[15].second == 7) p7 = this->parents[15].first;
    else if (this -> parents[15].second == 8) p8 = this->parents[15].first;
    else if (this -> parents[15].second == 9) p9 = this->parents[15].first;
    else if (this -> parents[15].second == 10) p10 = this->parents[15].first;
    else if (this -> parents[15].second == 11) p11 = this->parents[15].first;
    else if (this -> parents[15].second == 12) p12 = this->parents[15].first;
    else if (this -> parents[15].second == 13) p13 = this->parents[15].first;
    else if (this -> parents[15].second == 14) p14 = this->parents[15].first;
    else if (this -> parents[15].second == 15) p15 = this->parents[15].first;
    else if (this -> parents[15].second == 16) p16 = this->parents[15].first;
    else if (this -> parents[15].second == 100) s1 = this->parents[15].first;
    else if (this -> parents[15].second == 101) s2 = this->parents[15].first;
    else if (this -> parents[15].second == 102) s3 = this->parents[15].first;
    else if (this -> parents[15].second == 103) s4 = this->parents[15].first;

    if (this->parents[16].second == 1) p1 = this->parents[16].first;
    else if (this->parents[16].second == 2) p2 = this->parents[16].first;
    else if (this -> parents[16].second == 3) p3 = this->parents[16].first;
    else if (this -> parents[16].second == 4) p4 = this->parents[16].first;
    else if (this -> parents[16].second == 5) p5 = this->parents[16].first;
    else if (this -> parents[16].second == 6) p6 = this->parents[16].first;
    else if (this -> parents[16].second == 7) p7 = this->parents[16].first;
    else if (this -> parents[16].second == 8) p8 = this->parents[16].first;
    else if (this -> parents[16].second == 9) p9 = this->parents[16].first;
    else if (this -> parents[16].second == 10) p10 = this->parents[16].first;
    else if (this -> parents[16].second == 11) p11 = this->parents[16].first;
    else if (this -> parents[16].second == 12) p12 = this->parents[16].first;
    else if (this -> parents[16].second == 13) p13 = this->parents[16].first;
    else if (this -> parents[16].second == 14) p14 = this->parents[16].first;
    else if (this -> parents[16].second == 15) p15 = this->parents[16].first;
    else if (this -> parents[16].second == 16) p16 = this->parents[16].first;
    else if (this -> parents[16].second == 100) s1 = this->parents[16].first;
    else if (this -> parents[16].second == 101) s2 = this->parents[16].first;
    else if (this -> parents[16].second == 102) s3 = this->parents[16].first;
    else if (this -> parents[16].second == 103) s4 = this->parents[16].first;

    if (this->parents[17].second == 1) p1 = this->parents[17].first;
    else if (this->parents[17].second == 2) p2 = this->parents[17].first;
    else if (this -> parents[17].second == 3) p3 = this->parents[17].first;
    else if (this -> parents[17].second == 4) p4 = this->parents[17].first;
    else if (this -> parents[17].second == 5) p5 = this->parents[17].first;
    else if (this -> parents[17].second == 6) p6 = this->parents[17].first;
    else if (this -> parents[17].second == 7) p7 = this->parents[17].first;
    else if (this -> parents[17].second == 8) p8 = this->parents[17].first;
    else if (this -> parents[17].second == 9) p9 = this->parents[17].first;
    else if (this -> parents[17].second == 10) p10 = this->parents[17].first;
    else if (this -> parents[17].second == 11) p11 = this->parents[17].first;
    else if (this -> parents[17].second == 12) p12 = this->parents[17].first;
    else if (this -> parents[17].second == 13) p13 = this->parents[17].first;
    else if (this -> parents[17].second == 14) p14 = this->parents[17].first;
    else if (this -> parents[17].second == 15) p15 = this->parents[17].first;
    else if (this -> parents[17].second == 16) p16 = this->parents[17].first;
    else if (this -> parents[17].second == 100) s1 = this->parents[17].first;
    else if (this -> parents[17].second == 101) s2 = this->parents[17].first;
    else if (this -> parents[17].second == 102) s3 = this->parents[17].first;
    else if (this -> parents[17].second == 103) s4 = this->parents[17].first;

    if (this->parents[18].second == 1) p1 = this->parents[18].first;
    else if (this->parents[18].second == 2) p2 = this->parents[18].first;
    else if (this -> parents[18].second == 3) p3 = this->parents[18].first;
    else if (this -> parents[18].second == 4) p4 = this->parents[18].first;
    else if (this -> parents[18].second == 5) p5 = this->parents[18].first;
    else if (this -> parents[18].second == 6) p6 = this->parents[18].first;
    else if (this -> parents[18].second == 7) p7 = this->parents[18].first;
    else if (this -> parents[18].second == 8) p8 = this->parents[18].first;
    else if (this -> parents[18].second == 9) p9 = this->parents[18].first;
    else if (this -> parents[18].second == 10) p10 = this->parents[18].first;
    else if (this -> parents[18].second == 11) p11 = this->parents[18].first;
    else if (this -> parents[18].second == 12) p12 = this->parents[18].first;
    else if (this -> parents[18].second == 13) p13 = this->parents[18].first;
    else if (this -> parents[18].second == 14) p14 = this->parents[18].first;
    else if (this -> parents[18].second == 15) p15 = this->parents[18].first;
    else if (this -> parents[18].second == 16) p16 = this->parents[18].first;
    else if (this -> parents[18].second == 100) s1 = this->parents[18].first;
    else if (this -> parents[18].second == 101) s2 = this->parents[18].first;
    else if (this -> parents[18].second == 102) s3 = this->parents[18].first;
    else if (this -> parents[18].second == 103) s4 = this->parents[18].first;

    if (this->parents[19].second == 1) p1 = this->parents[19].first;
    else if (this->parents[19].second == 2) p2 = this->parents[19].first;
    else if (this -> parents[19].second == 3) p3 = this->parents[19].first;
    else if (this -> parents[19].second == 4) p4 = this->parents[19].first;
    else if (this -> parents[19].second == 5) p5 = this->parents[19].first;
    else if (this -> parents[19].second == 6) p6 = this->parents[19].first;
    else if (this -> parents[19].second == 7) p7 = this->parents[19].first;
    else if (this -> parents[19].second == 8) p8 = this->parents[19].first;
    else if (this -> parents[19].second == 9) p9 = this->parents[19].first;
    else if (this -> parents[19].second == 10) p10 = this->parents[19].first;
    else if (this -> parents[19].second == 11) p11 = this->parents[19].first;
    else if (this -> parents[19].second == 12) p12 = this->parents[19].first;
    else if (this -> parents[19].second == 13) p13 = this->parents[19].first;
    else if (this -> parents[19].second == 14) p14 = this->parents[19].first;
    else if (this -> parents[19].second == 15) p15 = this->parents[19].first;
    else if (this -> parents[19].second == 16) p16 = this->parents[19].first;
    else if (this -> parents[19].second == 100) s1 = this->parents[19].first;
    else if (this -> parents[19].second == 101) s2 = this->parents[19].first;
    else if (this -> parents[19].second == 102) s3 = this->parents[19].first;
    else if (this -> parents[19].second == 103) s4 = this->parents[19].first;

    if (this->parents[20].second == 1) p1 = this->parents[20].first;
    else if (this->parents[20].second == 2) p2 = this->parents[20].first;
    else if (this -> parents[20].second == 3) p3 = this->parents[20].first;
    else if (this -> parents[20].second == 4) p4 = this->parents[20].first;
    else if (this -> parents[20].second == 5) p5 = this->parents[20].first;
    else if (this -> parents[20].second == 6) p6 = this->parents[20].first;
    else if (this -> parents[20].second == 7) p7 = this->parents[20].first;
    else if (this -> parents[20].second == 8) p8 = this->parents[20].first;
    else if (this -> parents[20].second == 9) p9 = this->parents[20].first;
    else if (this -> parents[20].second == 10) p10 = this->parents[20].first;
    else if (this -> parents[20].second == 11) p11 = this->parents[20].first;
    else if (this -> parents[20].second == 12) p12 = this->parents[20].first;
    else if (this -> parents[20].second == 13) p13 = this->parents[20].first;
    else if (this -> parents[20].second == 14) p14 = this->parents[20].first;
    else if (this -> parents[20].second == 15) p15 = this->parents[20].first;
    else if (this -> parents[20].second == 16) p16 = this->parents[20].first;
    else if (this -> parents[20].second == 100) s1 = this->parents[20].first;
    else if (this -> parents[20].second == 101) s2 = this->parents[20].first;
    else if (this -> parents[20].second == 102) s3 = this->parents[20].first;
    else if (this -> parents[20].second == 103) s4 = this->parents[20].first;

    if (port_number == 1){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);

    }
    else if ( port_number == 2){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p1, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 3){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 4){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 5){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 6){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 7){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 8){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 9){
        result.emplace_back(s4, 1, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 10){
        result.emplace_back(s4, 1, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 11){
        result.emplace_back(s4, 1, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 12){
        result.emplace_back(s4, 1, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 13){
        result.emplace_back(s4, 1, -1);
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 14){
        result.emplace_back(s4, 1, -1);
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 0, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 15){
        result.emplace_back(s4, 1, -1);
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 0, -1);
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    else if ( port_number == 16){
        result.emplace_back(s4, 1, -1);
        result.emplace_back(s3, 1, -1);
        result.emplace_back(s2, 1, -1);
        result.emplace_back(s1, 1, -1);
        result.emplace_back(p1, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, value, pair.second);
    }
    if (port_number == 100){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        //we are working on p1 and p2
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //p1 is the output
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //p2 is the output
    }
    if (port_number == 101){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s1, 0, -1);
        //we are working on p1 and p3
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //p1 is the output
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //p3 is the output
    }
    if (port_number == 102){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        //we are working on p1 and p5
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //p1 is the output
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //p5 is the output
    }
    if (port_number == 103){
        result.emplace_back(s4, 0, -1);
        result.emplace_back(s3, 0, -1);
        result.emplace_back(s2, 0, -1);
        //we are working on p1 and p9
        result.emplace_back(p16, 0, -1);
        result.emplace_back(p15, 0, -1);
        result.emplace_back(p14, 0, -1);
        result.emplace_back(p13, 0, -1);
        result.emplace_back(p12, 0, -1);
        result.emplace_back(p11, 0, -1);
        result.emplace_back(p10, 0, -1);
        result.emplace_back(p9, 0, -1);
        result.emplace_back(p8, 0, -1);
        result.emplace_back(p7, 0, -1);
        result.emplace_back(p6, 0, -1);
        result.emplace_back(p5, 0, -1);
        result.emplace_back(p4, 0, -1);
        result.emplace_back(p3, 0, -1);
        result.emplace_back(p2, 0, -1);
        result.emplace_back(p1, 1, -1);
        if (value == 0) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 1, pair.second); //p1 is the output
        else if (value == 1) for (std::pair<std::shared_ptr<Node>, int> pair : children) result.emplace_back(pair.first, 0, pair.second); //p9 is the output
    }

    return result;
}

bool Mux16::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //TODO
    std::cerr << "Mux16::computeMandatory not implemented yet" << std::endl;
    return false;
}

// Tbuf implementation -----------------------------------------------

Tbuf::Tbuf(size_t identifier, std::string netlistName) : ComplexCell(identifier, netlistName) {
    this->type = "$_TBUF_";
}

std::shared_ptr<Node> Tbuf::getSharedPtrToThis() {
    return std::make_shared<Tbuf>(*this);
}

void Tbuf::printNodeRecursive(){
    std::cout << "node type: Tbuf id = " << this->Identifier;
    std::cout << ", value = " << this->value << std::endl;
    
    int size = this->children.size();
    for (int i = 0; i < size; i++) {
        children[i].first->printNodeRecursive(); //print the children
    }
}

std::vector<std::pair<std::shared_ptr<Node>, int>> Tbuf::computeInputFromOutput(int value){
    //TODO I don't know the error of a Tbuf
    std::cerr << "Tbuf::computeInputFromOutput not implemented yet" << std::endl;
    std::vector<std::pair<std::shared_ptr<Node>, int>> result;
    return result;
}

std::vector<std::tuple<std::shared_ptr<Node>, int, int>> Tbuf::computeExpectedValue(int value, int port_number){
    //TODO
    std::cerr << "Tbuf::computeExpectedValue not implemented yet" << std::endl;
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result;
    return result;
}

bool Tbuf::computeMandatory(std::shared_ptr<Node> node, int port_number, int value, bool propagate, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    //TODO
    std::cerr << "Tbuf::computeMandatory not implemented yet" << std::endl;
    return false;
}

} // namespace Complexcell