#pragma once

#include<vector>

#include "layer.hpp"
#include "../engine/owner.hpp"
#include "../engine/value.hpp"


class Network {
    std::vector<Layer> layers;

    public:
        Network(int numInputs, std::vector<int> layerSizes, Owner* owner);

        void zeroGradients();
        std::vector<Value*> parameters();
        std::vector<Value*> forward();
};