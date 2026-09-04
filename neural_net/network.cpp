#include<vector>

#include "network.hpp"
#include "layer.hpp"
#include "../engine/owner.hpp"
#include "../engine/value.hpp"

using namespace std;

Network::Network(int numInputs, vector<int> layerSizes, Owner* owner) {
    int prev = numInputs;

    for (size_t i = 0; i < layerSizes.size(); ++i) {
        int currSize = layerSizes.at(i);

        layers.emplace_back(
            Layer(prev, currSize, owner)
        );

        prev = currSize;
    }
}

vector<Value*> Network::forward() {
    
}

vector<Value*> Network::parameters() {
    vector<Value*> params;

    for (auto layer : layers) {
        vector<Value*> layerParams = layer.parameters();
        params.insert(params.end(), layerParams.begin(), layerParams.end());
    }

    return params;
}