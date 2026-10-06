#pragma once

#include<vector>

#include "../../engine/value.hpp"
#include "../../engine/owner.hpp"

// Converts from doubles to values, will be leveraged internally
std::vector<Value*> toValues(const std::vector<double>& row, Owner& owner);

void descend(std::vector<Value*>& params, double learningRate);