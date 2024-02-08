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

#include "../../include/fault_API/fault_API.hpp"

namespace FaultAPI {

std::pair< std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>> get_vector_error(int value, shared_ptr<Tree> tree, shared_ptr<Node> node, int port_number){
    std::vector<std::pair<shared_ptr<Node>, int>> input_vector;
    std::vector<std::pair<shared_ptr<Node>, int>> output_vector;

    std::vector<std::pair<std::shared_ptr<Node>, int>> result_ComputeInputFromOutput;
    std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result_ComputeExpectedValue;

    if (node -> type == "Input") {
        if (port_number != -1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": input only have one port" << std::endl; //the only port they get is their output port
        input_vector.push_back(std::make_pair(node, value));
        //propagate de the value to the children
        //propagate the value to the children
        for (std::pair<std::shared_ptr<Node>, int> pair : node -> children){
            computeValue(input_vector, output_vector, pair.first ,value, pair.second);
        }
    }

    else if (node -> type == "Output") {
        if (port_number != 1) std::cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": output only have one port" << std::endl;
        output_vector.push_back(std::make_pair(node, value));
        //propagate de the value to the children
        //propagate the value to the parent
        for (std::pair<std::shared_ptr<Node>, int> pair : node -> parents){
            computeValue(input_vector, output_vector, pair.first ,value, -1);
        }
    }

    //case we test an output of a classic node
    else if (port_number == -1){
        //we are going to test the output so we need the function computeInputFromOutput
        result_ComputeInputFromOutput = node -> computeInputFromOutput(value); //get the value for the input
        for (std::pair<std::shared_ptr<Node>, int> pair : result_ComputeInputFromOutput){ //for each input
            computeValue(input_vector, output_vector, pair.first, pair.second, -1); //-1 because we need to say it is will be an output
        }
        //propagate the value to the children
        for (std::pair<std::shared_ptr<Node>, int> pair : node -> children){
            computeValue(input_vector, output_vector, pair.first ,value, pair.second);
        }
    }
    //case we test an Input of a classic node
    else {
        result_ComputeExpectedValue = node -> computeExpectedValue(value, port_number);
        for (std::tuple<std::shared_ptr<Node>, int, int> tuple : result_ComputeExpectedValue){
            computeValue(input_vector, output_vector, std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple)); //I need to make get2 beiing the input_number
        }
        //propagate the value in the input it come from
        // get the vector 
        std::shared_ptr<Node> parent =  node -> getParentFromInputNumber(port_number);
        computeValue(input_vector, output_vector, parent, value, -1);
    }
    std::pair< std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>> vector_error = std::make_pair(input_vector, output_vector);
    return vector_error;
}

void computeValue(std::vector<std::pair<shared_ptr<Node>, int>> &input_vector, std::vector<std::pair<shared_ptr<Node>, int>> &output_vector, shared_ptr<Node> node, int value, int input_number){
    //check if the node is an input or an output
    if (node -> type == "Input") {
        //if it is an input : add it to the return list 
        input_vector.push_back(std::make_pair(node, value));
    }
    else if (node -> type == "Output"){
        output_vector.push_back(std::make_pair(node, value));
    }
    
    else {
        //not output or input, so we have to compute it 
        if (input_number == -1){
            //case it is an output, so we have to compute the input value 
            std::vector<std::pair<std::shared_ptr<Node>, int>> result_ComputeInputFromOutput = node -> computeInputFromOutput(value); 
            //propagate the value
            for (std::pair<std::shared_ptr<Node>, int> pair : result_ComputeInputFromOutput){ //for each input
                computeValue(input_vector, output_vector, pair.first, pair.second, -1); //-1 because we need to say it is will be an output
            }
        }
        else {
            //case it is an input
            std::vector<std::tuple<std::shared_ptr<Node>, int, int>> result_ComputeExpectedValue = node -> computeExpectedValue(value, input_number);
            //propagate the compute
            for (std::tuple<std::shared_ptr<Node>, int, int> tuple : result_ComputeExpectedValue){
                computeValue(input_vector, output_vector, std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple)); //I need to make get2 beiing the input_number
            }
        }
    }
}

std::vector<std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>>> generate_vector_error(std::shared_ptr<std::vector<std::pair<shared_ptr<Fault>, shared_ptr<Node>>>> fault_list, shared_ptr<Tree> tree){
    //this is a vector with the value for the inputs and the value for the outputs for these value of the inputs
    std::vector<std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>>> list_error_vect;

    for (std::pair<shared_ptr<Fault>, shared_ptr<Node>> pair : *fault_list) {
        std::pair< std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>> vector_error;
        vector_error = get_vector_error( (int) pair.first -> getValue(),  tree, pair.second, pair.first -> getPort()); //get the vector for a specific error
        list_error_vect.push_back(vector_error);
    }
    return list_error_vect;
}


//for computeMandatory
bool computeMandatoryList(std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional) {
    bool result;
    while(!mandatory.empty()){
        std::tuple<std::shared_ptr<Node>, int, int, bool> first_tuple = mandatory[0];
        
        result = std::get<0>(first_tuple) -> computeMandatory(std::get<0>(first_tuple), std::get<1>(first_tuple), std::get<2>(first_tuple), std::get<3>(first_tuple), mandatory, optional);
        if (result == false) return false;
        mandatory.erase(mandatory.begin());
    }
    return true;
}

bool computeOptionalFirstElem(std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& mandatory, std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>>& optional){
    bool result;
    if (!optional.empty()){
        std::tuple<std::shared_ptr<Node>, int, int, bool> first_tuple = optional[0];
        result  = std::get<0>(first_tuple) -> computeOptional(std::get<0>(first_tuple), std::get<1>(first_tuple), std::get<2>(first_tuple), std::get<3>(first_tuple), mandatory, optional);
        optional.erase(optional.begin());
        if (result == false) return false;
       
    }
    return true;
}

std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>> generateOneVector(std::pair<shared_ptr<Fault>, shared_ptr<Node>>& fault, shared_ptr<Tree> tree, bool& success){

    //variable used for the function
    std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>> mandatory;
    std::vector<std::tuple<std::shared_ptr<Node>, int, int, bool>> optional; 
    bool result; 

    std::shared_ptr<Node> nodeToWorkOn = tree -> getNodeByIdentifier(fault.second -> getIdentifier());

    //make the first compute
    result = nodeToWorkOn -> computeMandatory(nodeToWorkOn, fault.first -> getPort(), fault.first -> getValue(), true, mandatory, optional);
    if (result == false) success = false;
    //make the other compute
    result = computeMandatoryList(mandatory, optional);
    if (result == false) success = false;
    while (!optional.empty()){
        
        result = computeOptionalFirstElem(mandatory, optional);
        if (result == false) success = false;
        result = computeMandatoryList(mandatory, optional);
        if (result == false) success = false;
    }

    //get input and output vector
    std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>> vector_error;
    for (shared_ptr<Node> node : tree -> InputList){
        vector_error.first.push_back(std::make_pair(node, node -> getValueFromPort(-1)));
    }
    for (shared_ptr<Node> node : tree -> OutputList){
        vector_error.second.push_back(std::make_pair(node, node -> getValueFromPort(1)));
    }
    return vector_error;

}

std::vector<std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>>> generateVectorError(std::shared_ptr<std::vector<std::pair<shared_ptr<Fault>, shared_ptr<Node>>>> fault_list, shared_ptr<Tree> tree){

    //this is a vector with the value for the inputs and the value for the outputs for these value of the inputs
    std::vector<std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>>> list_error_vect;
    bool success = true;

    for (std::pair<shared_ptr<Fault>, shared_ptr<Node>>& pair : *fault_list) {

        std::pair<std::vector<std::pair<shared_ptr<Node>, int>> , std::vector<std::pair<shared_ptr<Node>, int>>> vector_error = generateOneVector(pair, tree, success);
        if (success) list_error_vect.push_back(vector_error);
        else {
            pair.first -> setFailure();
        }
        success = true;
        tree -> resetPortValue();
        

        
    }
    
    return list_error_vect;
}

} // namespace FaultAPI
