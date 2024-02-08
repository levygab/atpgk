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

#include "../../include/tree/Fault.hpp"

namespace FaultModel {

std::string getFaultAlias(FaultModelType fault) {
        auto it = faultAliases.find(fault);
    if (it != faultAliases.end()) {
        return it->second;
    } else {
        return "Unknown Fault";
    }
}

int Fault::stuck_at_0_counter = 0;
int Fault::stuck_at_1_counter = 0;

Fault::Fault(FaultModelType _type, int _port) : covered(false), type(_type), port(_port) {
    switch (this->type) {
        case FaultModelType::StuckAtZero:
            Fault::stuck_at_0_counter++;
            this->value = 1;
            break;
        case FaultModelType::StuckAtOne:
            Fault::stuck_at_1_counter++;
            this->value = 0;
            break;
    }

    this->covered = false;
};

Fault::~Fault() {
    switch (this->type) {
        case FaultModelType::StuckAtZero:
            Fault::stuck_at_0_counter--;
            break;
        case FaultModelType::StuckAtOne:
            Fault::stuck_at_1_counter--;
            break;
    }
};

void Fault::setCovered() {
    this->covered = true;
};

void Fault::setUncovered() {
    this->covered = false;
};

bool Fault::getCoverageFlag() {
    return this->covered;
};

int Fault::getPort() {
    return this->port;
};

FaultModelType Fault::getType() {
    return this->type;
};

void Fault::setValue(bool _value) {
    this->value = _value;
};

bool Fault::getValue() {
    return this->value;
};

} // namespace FaultModel