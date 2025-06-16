// File: datatypes.hpp

#pragma once

#include <stdexcept>

template <typename T>
T parse_value(const std::string& str);

// int
template <>
int parse_value<int>(const std::string& str) {
    return std::stoi(str);
}

// float
template <>
float parse_value<float>(const std::string& str) {
    return std::stof(str);
}

// double
template <>
double parse_value<double>(const std::string& str) {
    return std::stod(str);
}

// string
template <>
std::string parse_value<std::string>(const std::string& str) {
    return str;
}

// bool
template <>
bool parse_value<bool>(const std::string& str) {
    if (str == "true" || str == "1") return true;
    if (str == "false" || str == "0") return false;
    throw std::invalid_argument("Invalid bool value: " + str);
}
