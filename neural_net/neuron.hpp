#include<vector>

#include "../engine/value.hpp"
#include "../engine/owner.hpp"

class Neuron {
    // Each Neuron controls its own weights and biases, so these should be private
    std::vector<Value*> weights;
    Value* bias;
    Owner* globalOwner;
    
    public:
        // For a later version, I will add the ability to pass in a custom activation function, but for now I'm just gonna hardcode ReLU

        Neuron(int numInputs, Owner* owner);

        Value& forward(std::vector<Value*> prev);
};