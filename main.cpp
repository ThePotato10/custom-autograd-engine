#include<vector>
#include<iostream>

#include "api/api.hpp"

using namespace std;

int main() {
    /* As a proof of concept, the network learns to emulate a XOR gate. 
     * This is a nonlinear function, so the loss will never reach zero, 
     * but it can get pretty close.
     * In this example, -1 is used instead of 0 to give the model more room to learn
     * So for example, the second sample in the training data represents 1 XOR 0
     */

    vector<vector<double>> inputs;
    vector<vector<double>> outputs;

    inputs.push_back(vector<double>{1.0, 1.0});
    inputs.push_back(vector<double>{1.0, -1.0});
    inputs.push_back(vector<double>{-1.0, 1.0});
    inputs.push_back(vector<double>{-1.0, -1.0});

    outputs.push_back(vector<double>{-1.0});
    outputs.push_back(vector<double>{1.0});
    outputs.push_back(vector<double>{1.0});
    outputs.push_back(vector<double>{-1.0});

    Dataset trainingData = createDataset(inputs, outputs);
    Owner globalOwner = Owner();
    Network network = createNetwork(2, vector<int>{8, 1}, globalOwner);

    train(
        network,
        trainingData,
        globalOwner,
        2000,
        0.05
    );

    cout << "\n";

    cout << "NETWORK TESTING" << endl;
    cout << "Inputs: {1, 1}" << endl;
    cout << "Network should predict -1" << endl;
    cout << "Network prediction: " << predict(network, vector<double>{1, 1}, globalOwner)[0] << endl;
    cout << "\n";

    cout << "Inputs: {-1, 1}" << endl;
    cout << "Network should predict 1" << endl;
    cout << "Network prediction: " << predict(network, vector<double>{-1, 1}, globalOwner)[0] << endl;
    cout << "\n";

    cout << "Inputs: {1, -1}" << endl;
    cout << "Network should predict 1" << endl;
    cout << "Network prediction: " << predict(network, vector<double>{1, -1}, globalOwner)[0] << endl;
    cout << "\n";

    cout << "Inputs: {-1, -1}" << endl;
    cout << "Network should predict -1" << endl;
    cout << "Network prediction: " << predict(network, vector<double>{-1, -1}, globalOwner)[0] << endl;
    cout << "\n";

    return 0;
}