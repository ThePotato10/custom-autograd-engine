#pragma once

#include<vector>

#include "Dataset.hpp"
#include "../src/network.hpp"
#include "../../engine/owner.hpp"

Network createNetwork(int numInputs, std::vector<int> layerSizes, Owner &globalOwner);

Dataset createDataset(std::vector<std::vector<double>> inputs, std::vector<std::vector<double>> outputs);

void train(
    Network& network, 
    const Dataset& trainingData, 
    Owner& globalOwner, 
    int steps, 
    double learningRate
);

std::vector<double> predict(
    Network& network,
    std::vector<double> inputs,
    Owner& globalOwner
);