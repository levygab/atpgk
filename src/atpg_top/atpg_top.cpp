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

#include "../../include/atpg_top/atpg_top.hpp"

ATPGTop::ATPGTop() : reader(this->filename, this->extension_type), fault_decorator() {
    this->fault_list = make_shared<vector<pair<shared_ptr<Fault>, shared_ptr<Node>>>>();
    this->vectors_test = make_shared<vector<pair<vector<pair<shared_ptr<Node>, int>> , vector<pair<shared_ptr<Node>, int>>>>>();
    this->tree = make_shared<Tree>("tree");
    this->faultCount = make_shared<vector<array<int, 2>>>();
    for (int j = 0; j < static_cast<int>(FaultModelType::Count); ++j) {
        array<int, 2> new_array = {};
        faultCount->push_back(new_array);
    }

    this->failureFault = make_shared<vector<tuple<shared_ptr<Fault>, string, string>>>();
};

void ATPGTop::initialize() {

    // Check if the specified extension match the current filename
    if (this->filename.substr(this->filename.length()-(this->extension_type.length()+1)) != ("."+this->extension_type)) {
        cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": specified file extension '" + this->extension_type + "' does not match the current input file extension '" + this->filename + "'" << endl;
        exit(1);
    }

    this->tree->srcName = this->filename.substr(this->filename.find_last_of('/')+1);

    // Creating the output directory if it doesn't already exist
    try {
        if(!filesystem::exists(this->output_dir_path)) {
            filesystem::create_directory(this->output_dir_path);
        }
    } catch (const exception& ex) {
        cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": problem occurs when creating output directory: " << ex.what() << "\n";
    }

    // Find the last occurrence of the dot (.)
    size_t dotPos = this->cov_output_filename.find_last_of('.');

    // If there is an extension in the filename
    if (dotPos != string::npos) {
        // Extract the extension (substring after the dot)
        string cov_ext = this->cov_output_filename.substr(dotPos+1);

        if (this->cov_output_file_ext != "") {
            // Check if the specified extension match the current filename
            if (this->cov_output_file_ext != cov_ext) {
                cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": specified file extension '" + this->cov_output_file_ext + "' does not match the current output file extension '" + this->cov_output_filename + "'" << endl;
                exit(1);
            }
        } else {
            this->cov_output_file_ext = cov_ext;
        }

        // Extract the file name (substring before the dot) and place it in the ./out/ directory
        this->cov_output_filename = this->output_dir_path+this->cov_output_filename.substr(0, dotPos);

    } else {
        if (this->cov_output_file_ext == "") {
            this->cov_output_file_ext = "txt"; // Assuming the default ouptut format is .txt
        }
        // Place the output coverage file in the ./out/ directory
        this->cov_output_filename = this->output_dir_path+this->cov_output_filename;
    }

    // Find the last occurrence of the dot (.)
    dotPos = this->vect_output_filename.find_last_of('.');

    // If there is an extension in the filename
    if (dotPos != string::npos) {
        // Extract the extension (substring after the dot)
        string out_ext = this->vect_output_filename.substr(dotPos+1);

        if (this->vect_output_file_ext != "") {
            // Check if the specified extension match the current filename
            if (this->vect_output_file_ext != out_ext) {
                cerr << RED_TEXT << BOLD_TEXT << "Error" << RESET_TEXT << ": specified file extension '" + this->vect_output_file_ext + "' does not match the current output file extension '" + this->vect_output_filename + "'" << endl;
                exit(1);
            }
        } else {
            this->vect_output_file_ext = out_ext;
        }

        // Extract the file name (substring before the dot) and place it in the ./out/ directory
        this->vect_output_filename = this->output_dir_path+this->vect_output_filename.substr(0, dotPos);

    } else {
        if (this->vect_output_file_ext == "") {
            this->vect_output_file_ext = "txt"; // Assuming the default ouptut format is .txt
        }
        // Place the output vectors file in the ./out/ directory
        this->vect_output_filename = this->output_dir_path+this->vect_output_filename;
    }

    // Instanciate the coverage output file writer
    if (this->cov_output_file_ext == "txt") {
        this->covOutputFileWriter = make_shared<WriterTXT>(this->cov_output_filename, this->cov_output_file_ext);
    } else if (this->cov_output_file_ext == "json") {
        this->covOutputFileWriter = make_shared<WriterJSON>(this->cov_output_filename, this->cov_output_file_ext);
    } else { // Default coverage output file if the extension is unknown or unsupported is .txt
        cout << ORANGE_TEXT << BOLD_TEXT << "Warning" << RESET_TEXT  << ": output format '" + this->cov_output_file_ext + "' is not supported for coverage output file\n\t Using default '.txt' extension format instead" << endl;
        this->cov_output_file_ext = "txt";
        this->covOutputFileWriter = make_shared<WriterTXT>(this->cov_output_filename, this->cov_output_file_ext);
    }

    // Instanciate the vectors output file writer
    if (this->vect_output_file_ext == "txt") {
        this->vectOutputFileWriter = make_shared<WriterTXT>(this->vect_output_filename, this->vect_output_file_ext);
    } else if (this->vect_output_file_ext == "json") {
        this->vectOutputFileWriter = make_shared<WriterJSON>(this->vect_output_filename, this->vect_output_file_ext);
    } else { // Default vector output file if the extension is unknown or unsupported is .txt
        cout << ORANGE_TEXT << BOLD_TEXT << "Warning" << RESET_TEXT << ": output format '" + this->vect_output_file_ext + "' is not supported for test vectors output file\n\t Using default '.txt' extension format instead" << endl;
        this->vect_output_file_ext = "txt";
        this->vectOutputFileWriter = make_shared<WriterTXT>(this->vect_output_filename, this->vect_output_file_ext);
    }
};

void ATPGTop::read() {
    this->reader.read(this->filename, this->extension_type, this->tree);
};

void ATPGTop::generate_vector(){
    *(this->vectors_test) = FaultAPI::generateVectorError(this -> fault_list, this -> tree);
};

void ATPGTop::write_vector() {
    this->vectOutputFileWriter->formatFile(this->tree);
    this->vectOutputFileWriter->writeIOPort(this->tree);
    this->vectOutputFileWriter->writeVectors(this->vectors_test);
};

void ATPGTop::write_coverage() {
    this->covOutputFileWriter->formatFile(this->tree);
    this->covOutputFileWriter->writeCovStats(this->faultCount, this->failureFault);
};

void ATPGTop::decorate() {
    this->tree->traverse(this->fault_decorator, this->fault_list);
};

void ATPGTop::generate_cov_stats() {
    for (pair<shared_ptr<Fault>, shared_ptr<Node>> pair : *(this->fault_list)) {
        (*faultCount)[static_cast<int>(pair.first->getType())][0]++;
        if (pair.first->getFailure()) {
            // FIXME: replace "co" dynamically (it represents the reason of the failure -> "co" for controlability, "ob" for observability)
            // By now, only "co" is possible as we just manage combinatory circuits
            tuple<shared_ptr<Fault>, string, string> tuple = {pair.first, pair.second->netlistName, "co"};
            this->failureFault->push_back(tuple);
        }
    }

    for (auto &tuple : *(this->failureFault)) {
        (*faultCount)[static_cast<int>(get<0>(tuple)->getType())][1]++;
    }
};