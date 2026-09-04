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

// Resets gradients between training passes to prevent accumulation bugs
void Network::zeroGradients() {
    vector<Value*> params = parameters();

    for (auto p : params) p->gradient = 0.0;
}

vector<Value*> Network::forward(vector<Value*> entry) {
    // The core of the forward propagation logic:
    // Each layer receives a vector of values, passes that vector to the next layer. 
    // Shape logic (number of values returned) is handled internally, not addressed here
    for (auto& l : layers) {
        entry = l.forward(entry);
    }

    return entry;
}

vector<Value*> Network::parameters() {
    vector<Value*> params;

    for (auto layer : layers) {
        vector<Value*> layerParams = layer.parameters();
        params.insert(params.end(), layerParams.begin(), layerParams.end());
    }

    return params;
}