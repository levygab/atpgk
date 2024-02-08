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
 * @file yosys_gate_level_cells.hpp
 * @brief Contains definitions of cell types.
*/

#pragma once

#include <unordered_set>
#include <string>
#include <cstdlib>

/**
 * @namespace YosysBasicGateLevelCells
 * @brief Define the different types of Yosys basic cells
*/
namespace YosysBasicGateLevelCells {
    /**
     * @brief Definition of unary cells.
    */
    const std::unordered_set<std::string> unaryCells = {
        "$_BUF_",
        "$_NOT_"
    };

    /**
     * @brief Definition of binary cells.
    */
    const std::unordered_set<std::string> binaryCells = {
        "$_AND_",
        "$_NAND_",
        "$_ANDNOT_",
        "$_OR_",
        "$_NOR_",
        "$_ORNOT_",
        "$_XOR_",
        "$_XNOR_"
    };

    /**
     * @brief Definition of more complex cells (multiplexers, tristate, combined AND/OR, etc.).
    */
    const std::unordered_set<std::string> complexCells = {
        "$_AOI3_",
        "$_OAI3_",
        "$_AOI4_",
        "$_OAI4_",
        "$_MUX_",
        "$_NMUX",
        "$_MUX4_",
        "$_MUX8_",
        "$_MUX16_",
        "$_TBUF_"
    };
} // namespace BasicGateLevelCells