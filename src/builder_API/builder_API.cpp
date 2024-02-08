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

#include "../../include/builder_API/builder_API.hpp"


namespace BuilderAPI{

    shared_ptr<Node> createNewCell(size_t id, string type, string netlistName){
        if (type == "$_AND_"){
            shared_ptr<Node> node = make_shared<AndCell>(id, netlistName);
            return node;
        }
        
        else if (type == "$_OR_"){
            shared_ptr<Node> node = make_shared<OrCell>(id, netlistName);
            return node;
        }
        else if (type == "$_XOR_"){
            shared_ptr<Node> node = make_shared<XorCell>(id, netlistName);
            return node;
        }
        else if (type == "$_XNOR_"){
            shared_ptr<Node> node = make_shared<XnorCell>(id, netlistName);
            return node;
        }
        else if (type == "$_NAND_"){
            shared_ptr<Node> node = make_shared<NandCell>(id, netlistName);
            return node;
        }
        else if (type == "$_NOR_"){
            shared_ptr<Node> node = make_shared<NorCell>(id, netlistName);
            return node;
        }
        else if (type == "$_NOT_"){
            shared_ptr<Node> node = make_shared<NotCell>(id, netlistName);
            return node;
        }
        else if (type == "$_BUF_"){
            shared_ptr<Node> node = make_shared<BufCell>(id, netlistName);
            return node;
        }
        else if (type == "$_ORNOT_"){
            shared_ptr<Node> node = make_shared<OrnotCell>(id, netlistName);
            return node;
        }
        else if (type == "$_ANDNOT_"){
            shared_ptr<Node> node = make_shared<AndnotCell>(id, netlistName);
            return node;
        }
        else if (type == "$_AOI3_"){
            shared_ptr<Node> node = make_shared<AOI3>(id, netlistName);
            return node;
        }
        else if (type == "$_AOI4_"){
            shared_ptr<Node> node = make_shared<AOI4>(id, netlistName);
            return node;
        }
        else if (type == "$_OAI3_"){
            shared_ptr<Node> node = make_shared<OAI3>(id, netlistName);
            return node;
        }
        else if (type == "$_OAI4_"){
            shared_ptr<Node> node = make_shared<OAI4>(id, netlistName);
            return node;
        }
        else if (type == "$_MUX_"){
            shared_ptr<Node> node = make_shared<Mux>(id, netlistName);
            return node;
        }
        else if (type == "$_NMUX_"){
            shared_ptr<Node> node = make_shared<Nmux>(id, netlistName);
            return node;
        }
        else if (type == "$_MUX4_"){
            shared_ptr<Node> node = make_shared<Mux4>(id, netlistName);
            return node;
        }
        else if (type == "$_MUX8_"){
            shared_ptr<Node> node = make_shared<Mux8>(id, netlistName);
            return node;
        }
        else if (type == "$_MUX16_"){
            shared_ptr<Node> node = make_shared<Mux16>(id, netlistName);
            return node;
        }
        else if (type == "$_TBUF_"){
            shared_ptr<Node> node = make_shared<Tbuf>(id, netlistName);
            return node;
        }
        else if (type.substr(0,5) == "Input"){
            shared_ptr<Node> node = make_shared<Input>(id, type.substr(6), type.substr(6));
            return node;
        }
        else if (type.substr(0,6) == "Output"){
            shared_ptr<Node> node = make_shared<Output>(id, type.substr(7), type.substr(7));
            return node;
        }
        else{
            cerr << "Error : type of cell "<< type <<" not recognized" << endl;
            return nullptr;
        }
    }

    void addInputToTree(shared_ptr<Tree> tree, shared_ptr<Node> node){
        tree->addInput(node);
    }

    void addOutputToTree(shared_ptr<Tree> tree, shared_ptr<Node> node){
        tree->addOutput(node);
    }

    void addNodeToTree(shared_ptr<Tree> tree, shared_ptr<Node> node){
        //check if the node belong to Input or Output list
        if (node -> type == "Input") addInputToTree(tree, node);
        else if (node -> type == "Output") addOutputToTree(tree, node);
        tree->addNode(node);
    }

    void createAndAddNodeToTree(shared_ptr<Tree> tree, size_t id, string type, string netlistName){
        shared_ptr<Node> node = createNewCell(id, type, netlistName);
        addNodeToTree(tree, node);
    }

    void print_nodes(shared_ptr<Tree> tree){
        for (shared_ptr<Node> node : tree->NodeList){
            node->printNodeAndNeighbor();
        }
    }

    void bind_cell(size_t id_parent, size_t id_children, string port, shared_ptr<Tree> tree){
        shared_ptr<Node> parent = tree->getNodeByIdentifier(id_parent);
        shared_ptr<Node> children = tree->getNodeByIdentifier(id_children);

        int string_len = port.length();
        int port_number = -1;
        if (string_len == 0){
            cerr << "Error : port not found : there is no input" << endl;
        }
        else if(string_len == 1){
            port_number = int(port[0]) - (int('A')-1);
            if (port_number < 1 || port_number > 26){
                cerr << "Error : port not found : input is not a capital letter" << endl;
            }
            else if (port_number == 19){ //case S
                port_number = 100;
            }
            else if (port_number == 20){ //case T
                port_number = 101;
            }
            else if (port_number == 21){ //case U
                port_number = 102;
            }
            else if (port_number == 22){ //case V
                port_number = 103;
            }
        }
        else if(string_len==2){
            if (port == "EN"){
                port_number = 200;
            }
            else{
                cerr << "Error : port not found : input is not EN" << endl;
            }
        }
        

        if (parent == nullptr || children == nullptr){
            cerr << "Error : parent or children not found" << endl;
        }
        else{
            parent->addChildren(children, port_number);
            children->addParent(parent, port_number);
        }
    }
}
