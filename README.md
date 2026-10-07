# CUSTOM AUTOGRAD ENGINE

Autograd engine built from scratch in C++. It's bad, but it works. Kinda. Inspired by Karpathy's micrograd project.

## Functionality

This autograd engine supports basic operations (+, -, *, /) and raising variables to polynomial powers. Includes ReLU as an out-the-box activation function.

## Applications

I built this to learn how deep learning works from the ground up while also challenging myself to work in a more unfamiliar language. Exposes an API for creating MLPs with two functions, `train()` and `predict()`, which do exactly what you'd expect them to. `train()` uses internal forward and backpropagation methods to train the network by gradient descent, while `predict` uses forward propagation to generate a prediction based on the data provided

## Process

I used 3B1B's fantastic [YouTube series](https://www.youtube.com/watch?v=aircAruvnKk&list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi&index=2) to understand the math and process. All of this code was written the old-fashioned way, by hand. I used Claude Opus 5 to brainstorm, debug and outline some algorithmic stuff in pseudocode. 

## API

Everything necessary to use this engine is exposed through `./neural_net/api/api.hpp`. The training data for the network should be formatted as two `std::vector<std::vector<double>>`s, where the first is a vector of samples, and each sample is a vector of doubles, and the second is a vector of outputs, and each output is a vector of doubles corresponding to the desired activations of the neurons in the output layer of the network.

```
#include "./{engine}/neural_net/api/api.hpp"

int main() {
    std::vector<std::vector<double>> inputs;
    std::vector<std::vector<double>> outputs;
    ...
}
```

Then, call `Dataset trainingData = createDataset(inputs, outputs)`. This wraps your training data in a `Dataset` struct that the network can work with.

To build the network, the API exposes a class and a factory function. The first is the `Owner()` class, which takes no parameters and creates a store that the network uses to manage pointers. The second is the `createNetwork(int numInputs, std::vector<int> layerSizes, Owner &globalOwner)` factory function. The first parameter of this function is the number of inputs the network receives. The second is a vector of integers representing the number of neurons in each layer (note that this includes the final output layer). The third is an `Owner` object. So, for example,
```
int main() {
    ...
    Owner globalOwner = Owner();
    Network network = createNetwork(4, vector<int>{8, 8, 1}, globalOwner);
    ...
}
```
creates a network that takes in 4 inputs, then has two hidden layers with 8 neurons each, and an output layer that has one nueron. 

Once the network is created, use `train(Network& network, const Dataset& trainingData, Owner& globalOwner, int steps, double learningRate)` to train it. The first parameter of this function is the network to train, and the second is the dataset to train it with. The third is the `Owner` object that was previously created. The fourth parameter is the number of iterations to train the network for, and the fifth and last is the rate at which the network learns. 
```
int main() {
    ...
    // In this example, the network trains for 2000 iterations at a learning rate of 0.05
    train(
        network,
        trainingData,
        globalOwner,
        2000,
        0.05
    );
    ...
}
```
Now the network is trained and hopefully the loss function is close to zero. Now, we call use the `predict(Network& network, std::vector<double> inputs, Owner& globalOwner)` function to generate predictions with the network. The predict function returns an `std::vector<double>`, where each double in the vector is the activation of a neuron in the output layer. 

In the following example, we read `predict(...)[0]` to get the activation of the only neuron in the output layer. If instead your network had more than one neuron in the output layer, the `nth` output activation would be accessed with `predict(...)[n - 1]`.

Putting it all together:

```
#include<vector>
#include<iostream>

#include "api/api.hpp"

using namespace std;

int main() {
    /* As an example, the network learns to emulate a XOR gate. 
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

    cout << "NETWORK PREDICTIONS" << endl;
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
```