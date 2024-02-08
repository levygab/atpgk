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

#include "../../include/writer/writer.hpp"

Writer::Writer(std::string _filename, std::string _filetype) {
    this->fileName = _filename;
    this->fileType = _filetype;
    this->file.open((this->fileName+"."+this->fileType).c_str());
};

void Writer::addLineToFile(std::string lineContent) {
    this->file << lineContent << std::endl;
};

Writer::~Writer() {
    this->file.close();
};