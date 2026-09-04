#include<vector>

#include "layer.hpp"
#include "neuron.hpp"
#include "../engine/owner.hpp"
#include "../engine/value.hpp"

using namespace std;

Layer::Layer(int numInputs, int numOutputs, Owner* owner) {
    for (int i = 0; i < numOutputs; ++i) {
        neurons.emplace_back(numInputs, owner);
    }
}

vector<Value*> Layer::parameters() {
    vector<Value*> params;

    for (auto neuron : neurons) {
        vector<Value*> neuronParams = neuron.parameters();
        params.insert(params.end(), neuronParams.begin(), neuronParams.end());
    }

    return params;
}

vector<Value*> Layer::forward(vector<Value*> inputs) {
    vector<Value*> layerOut;

    for (auto& neuron : neurons) layerOut.push_back(&neuron.forward(inputs));

    return layerOut;
}