#pragma once

#include<vector>

#include "neuron.hpp"
#include "../engine/owner.hpp"
#include "../engine/value.hpp"

class Layer {
    std::vector<Neuron> neurons;

    public:
        Layer(int numInputs, int numOutputs, Owner* owner);

        std::vector<Value*> parameters();
        std::vector<Value*> forward(std::vector<Value*>& inputs);
};