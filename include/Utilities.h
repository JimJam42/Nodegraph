/*
  Copyright (C) 2014 Callum James

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <QPoint>
#include <QRect>
#include <QRectF>

/// @file Utilities.h
/// @brief A class for a single socket, this class knows which sockets and edges are connected
/// @author Callum James
/// @version 1.0
/// @date 24/03/2014
/// Revision History:
/// Initial Version 12/03/2014
/// @namespace GenUtils
/// @brief Custom namespace that contain utility functions that span a waide range of uses

#include "GraphNode.h"

/// @namespace GenUtils
/// @brief A custom namespace that contains simple generic utility functions
namespace GenUtils
{
    /// @brief Convert a node type to its string equivalent
    /// @param [in] _type NODE_TYPE - the type to convert
    /// @returns std::string
    std::string nodeTypeToString(NODE_TYPE _type);
    /// @brief Convert a string name to a NODE_TYPE
    /// @param [in] _type std::string - name to convert
    /// @returns NODE_TYPE
    NODE_TYPE nodeStringToType(std::string _type);
    /// @brief Convert top level type to string
    /// @param [in] _type VALUE_TYPE - type to convert
    /// @returns std::string
    std::string valueTypeToString(VALUE_TYPE _type);
}

#endif /* __UTILITIES_H__ */
