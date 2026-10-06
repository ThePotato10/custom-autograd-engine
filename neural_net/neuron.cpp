#include<vector>
#include<random>
#include<cassert>

#include "neuron.hpp"

#include "../engine/value.hpp"
#include "../engine/owner.hpp"

using namespace std;

double generateRandomWeight() {
    // Straight from ChatGPT
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<double> dist(-1.0, 1.0);

    return dist(gen);
}

Neuron::Neuron(int numInputs, Owner* owner, bool nl) : globalOwner(owner) {
    // numInputs is the number of neurons in the previous layer (or input for the first layer)
    // Each neuron stores the weights for the previous layer's activations within itself
    // Then the process for computing the out value of this neuron is multiplying it's stored weights by the inputs, and summing that
    // (Plus a bias and being applied to an activation function)
    // This value is then returned by the forward() method 
    // (Note that the Neuron doesn't have to store an internal value, just internal weights used to create that value)

    for (int i = 0; i < numInputs; ++i) {
        weights.push_back(&owner->create(generateRandomWeight(), "generic_weight"));
    }

    bias = &owner->create(generateRandomWeight(), "generic_bias");

    nonlinear = nl;
}

// Extracts the weights and bias from an individual neuron, base case for extracting all parameters from network
// Used for zeroGradients and updating weights/biases
vector<Value*> Neuron::parameters() {
    vector<Value*> params = weights;
    params.push_back(bias);

    return params;
}

Value& Neuron::forward(vector<Value*> prev) {
    // If this triggers, then the network is in some way misconfigured
    // Meaning that we should trigger a crash before wasting compute on meaningless number-crunching
    assert(prev.size() == weights.size());

    Value* out = bias;

    for (size_t i = 0; i < prev.size(); ++i) {
        // Lotta pointer symbols here, so let's go through it
        // Out is a pointer to a Value
        // For each pass of the loop, we set out to be equal to 
        // 1. the address (&(...)) of the result of adding 
        // 2. the Value referenced by out (*out) and
        // 3. the product of the Values referenced in the vectors of addresses prev and weight (*prev.at(i) and *weight.at(i))
        out = &(*out + (*prev.at(i) * *weights.at(i)));
    }

    if (nonlinear) return out->relu();
    else return *out;
}