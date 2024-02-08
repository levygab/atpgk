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

/**
 * @file FlipFlop.hpp
 * @brief Definition of the FlipFlop class
*/

#pragma once

#include "YosysCell.hpp"

/**
 * @brief A basic class reprensenting a FlipFlop : TO BE IMPLEMENTED
 * 
 */
class FlipFlop : public YosysCell {
public:
    int clkEdge;

    // Constructor
    FlipFlop(int identifier, std::string netlistName);
};

