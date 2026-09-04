#include<vector>
#include<iostream>

#include "network.hpp"
#include "../engine/owner.hpp"
#include "../engine/value.hpp"

using namespace std;

int main() {
    Owner globalOwner = Owner();
    Network test = Network(3, vector<int>{4, 4, 1}, &globalOwner);

    return 0;
}