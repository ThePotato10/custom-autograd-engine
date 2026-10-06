#pragma once

#include<vector>

// Stores doubles independent of the Values they get wrapped in to preserve data between training passes
struct Dataset {
    // Every entry corresponds to a sample of n inputs
    std::vector<std::vector<double>> X; 
    // Every entry corresponds to the desired outputs for a given sample
    std::vector<std::vector<double>> Y;
};