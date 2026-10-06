#include<vector>
#include<string>
#include<iostream>

#include "api.hpp"
#include "utils.hpp"
#include "Dataset.hpp"
#include "../src/network.hpp"
#include "../../engine/owner.hpp"
#include "../../engine/value.hpp"

using namespace std;

Network createNetwork(int numInputs, vector<int> layerSizes, Owner &globalOwner) {
    return Network(numInputs, layerSizes, &globalOwner);
}

Dataset createDataset(vector<vector<double>> inputs, vector<vector<double>> outputs) {
    return Dataset{inputs, outputs};
}

void train(
    Network& network, 
    const Dataset& trainingData, 
    Owner& globalOwner, 
    int steps, 
    double learningRate
) {
    cout << "Beginning training ..." << endl;

    vector<Value*> params = network.parameters();
    size_t checkpoint = globalOwner.size(); // Truncate to this after each pass to free unused Values

    for (int step = 0; step < steps; ++step) {
        Value* totalLoss = &globalOwner.create(0.0, "loss"); 

        for (size_t i = 0; i < trainingData.X.size(); ++i) { // compute sum of all losses for all training samples
            Value* sampleLoss = &globalOwner.create(0.0, "sample " + to_string(i) + " loss");
            vector<Value*> networkPredictions = network.forward(toValues(trainingData.X[i], globalOwner));

            for (size_t j = 0; j < networkPredictions.size(); ++j) {
                Value& diff = *networkPredictions[j] - globalOwner.create(trainingData.Y[i][j], "y");
                sampleLoss = &(*sampleLoss + diff.pow(2.0));
            }

            totalLoss = &(*totalLoss + *sampleLoss);
        }

        Value& averagedLoss = *totalLoss / globalOwner.create((double)trainingData.X.size(), "N");

        network.zeroGradients();
        averagedLoss.backprop();

        descend(params, learningRate);

        double lossVal = averagedLoss.value;
        globalOwner.truncate(checkpoint);

        if (step % 10 == 0) cout << "Step: " << step << " | Loss: " << lossVal << endl;
    }

    cout << "Training completed" << endl;
}

vector<double> predict(
    Network& network,
    vector<double> inputs,
    Owner& globalOwner
) {
    vector<Value*> networkPredictions = network.forward(toValues(inputs, globalOwner));
    vector<double> converted;

    for (Value* v : networkPredictions) converted.push_back(v->value);

    return converted;
}