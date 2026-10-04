# CUSTOM AUTOGRAD ENGINE

Autograd engine built from scratch in C++. It's bad, but it works. Kinda. Inspired by Karpathy's micrograd project.

## Functionality

This autograd engine supports basic operations (+, -, *, /) and raising variables to polynomial powers. Includes ReLU as an out-the-box activation function.

## Applications

I built this to learn how deep learning works from the ground up while also challenging myself to work in a more unfamiliar language. Has an API for computing forward passes and backpropagation on small MLP neural networks.

## Process

I used 3B1B's fantastic [YouTube series](https://www.youtube.com/watch?v=aircAruvnKk&list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi&index=2) to understand the math and process. All of this code was written the old-fashioned way, by hand. I used Claude Opus 5 to brainstorm, debug and outline some algorithmic stuff in pseudocode. 
