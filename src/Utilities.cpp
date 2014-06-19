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

#include "Utilities.h"

std::string GenUtils::nodeTypeToString(NODE_TYPE _type)
{
    switch (_type)
    {
        case(NT_BOOLEAN): return "Boolean"; break;
        case(NT_DOUBLE): return "Double"; break;
        case(NT_FLOAT): return "Float"; break;
        case(NT_INT): return "Integer"; break;
        case(NT_STRING): return "String"; break;
        case(NT_CHAR): return "Character"; break;
        case(NT_VECTOR): return "Vector"; break;
        case(NT_MATRIX): return "Matrix"; break;
        case(NT_OBJ_ANGLE): return "Angle Unit"; break;
        case(NT_OBJ_DISTANCE): return "Distance Unit"; break;
        case(NT_OBJ_TIME): return "Time Unit"; break;
        case(NT_OBJ_LAST): return "Last Value Unit"; break;
        case(NT_OBJ_COMPOUND): return "Compound Attribute"; break;
        case(NT_OBJ_ENUM): return "Enum Attribute"; break;
        case(NT_OBJ_GENERIC): return "Generic Attribute"; break;
        case(NT_OBJ_MATRIX): return "Matrix Attribute"; break;
        case(NT_OBJ_MESSAGE): return "Message Attribute"; break;
        case(NT_OBJ_BOOLEAN): return "Boolean Numeric"; break;
        case(NT_OBJ_ONEBYTE): return "One Byte Numeric"; break;
        case(NT_OBJ_ONECHAR): return "One Char Numeric"; break;
        case(NT_OBJ_ONESHORT): return "One Short Numeric"; break;
        case(NT_OBJ_TWOSHORT): return "Two Shorts Numeric"; break;
        case(NT_OBJ_THREESHORT): return "Three Shorts Numeric"; break;
        case(NT_OBJ_ONELONG): return "One Long Numeric"; break;
        case(NT_OBJ_ONEINT): return "One Int Numeric"; break;
        case(NT_OBJ_TWOLONG): return "Two Longs Numeric"; break;
        case(NT_OBJ_TWOINT): return "Two Ints Numeric"; break;
        case(NT_OBJ_THREELONG): return "Three Longs Numeric"; break;
        case(NT_OBJ_THREEINT): return "Three Ints Numeric"; break;
        case(NT_OBJ_ONEFLOAT): return "One Float Numeric"; break;
        case(NT_OBJ_TWOFLOAT): return "Two Floats Numeric"; break;
        case(NT_OBJ_THREEFLOAT): return "Three Floats Numeric"; break;
        case(NT_OBJ_ONEDOUBLE): return "One Double Numeric"; break;
        case(NT_OBJ_TWODOUBLE): return "Two Doubles Numeric"; break;
        case(NT_OBJ_THREEDOUBLE): return "Three Doubles Numeric"; break;
        case(NT_OBJ_FOURDOUBLE): return "Four Doubles Numeric"; break;
        case(NT_OBJ_ADDRESS): return "Address Numeric"; break;
        case(NT_OBJ_NUM_LAST): return "Last Value Numeric"; break;
        case(NT_ENDNODE): return "__end_node__"; break;
        default: return "__invalid_type__"; break;
    }
}

NODE_TYPE GenUtils::nodeStringToType(std::string _type)
{
    if (_type == "Boolean") return NT_BOOLEAN;
    else if (_type == "Double") return NT_DOUBLE;
    else if (_type == "Float") return NT_FLOAT;
    else if (_type == "Integer") return NT_INT;
    else if (_type == "String") return NT_STRING;
    else if (_type == "Character") return NT_CHAR;
    else if (_type == "Vector") return NT_VECTOR;
    else if (_type == "Matrix") return NT_MATRIX;
    else if (_type == "Angle Unit") return NT_OBJ_ANGLE;
    else if (_type == "Distance Unit") return NT_OBJ_DISTANCE;
    else if (_type == "Time Unit") return NT_OBJ_TIME;
    else if (_type == "Last Value Unit") return NT_OBJ_LAST;
    else if (_type == "Compound Attribute") return NT_OBJ_COMPOUND;
    else if (_type == "Enum Attribute") return NT_OBJ_ENUM;
    else if (_type == "Generic Attribute") return NT_OBJ_GENERIC;
    else if (_type == "Matrix Attribute") return NT_OBJ_MATRIX;
    else if (_type == "Message Attribute") return NT_OBJ_MESSAGE;
    else if (_type == "Boolean Numeric") return NT_OBJ_BOOLEAN;
    else if (_type == "One Byte Numeric") return NT_OBJ_ONEBYTE;
    else if (_type == "One Char Numeric") return NT_OBJ_ONECHAR;
    else if (_type == "One Short Numeric") return NT_OBJ_ONESHORT;
    else if (_type == "Two Shorts Numeric") return NT_OBJ_TWOSHORT;
    else if (_type == "Three Shorts Numeric") return NT_OBJ_THREESHORT;
    else if (_type == "One Long Numeric") return NT_OBJ_ONELONG;
    else if (_type == "One Int Numeric") return NT_OBJ_ONEINT;
    else if (_type == "Two Longs Numeric") return NT_OBJ_TWOLONG;
    else if (_type == "Two Ints Numeric") return NT_OBJ_TWOINT;
    else if (_type == "Three Longs Numeric") return NT_OBJ_THREELONG;
    else if (_type == "Three Ints Numeric") return NT_OBJ_THREEINT;
    else if (_type == "One Float Numeric") return NT_OBJ_ONEFLOAT;
    else if (_type == "Two Floats Numeric") return NT_OBJ_TWOFLOAT;
    else if (_type == "Three Floats Numeric") return NT_OBJ_THREEFLOAT;
    else if (_type == "One Double Numeric") return NT_OBJ_ONEDOUBLE;
    else if (_type == "Two Doubles Numeric") return NT_OBJ_TWODOUBLE;
    else if (_type == "Three Doubles Numeric") return NT_OBJ_THREEDOUBLE;
    else if (_type == "Four Doubles Numeric") return NT_OBJ_FOURDOUBLE;
    else if (_type == "Address Numeric") return NT_OBJ_ADDRESS;
    else if (_type == "Last Value Numeric") return NT_OBJ_NUM_LAST;
    else if (_type == "__end_node__") return NT_ENDNODE;
    else return NT_NOTYPE;
}

std::string GenUtils::valueTypeToString(VALUE_TYPE _type)
{
    switch(_type)
    {
        case(VT_OBJECT): return "OBJECT"; break;
        case(VT_ARGUMENTS): return "ARGUMENT"; break;
        case(VT_MEMBER): return "MEMBER"; break;
        case(VT_END): return "END"; break;
        default: return "__no_type__"; break;
    }
}
