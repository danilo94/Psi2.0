#ifndef INTERCONNECTION_NETWORK_H
#define INTERCONNECTION_NETWORK_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

class interconnection_network
{
public:
    interconnection_network(int in, int out, string type);
    interconnection_network(int in, int out,int stages, string type);
    interconnection_network(int in, int out, int stages, int radix, string type);
private:
    int inputs;
    int outputs;
    int stages;
    int radix;
    std::string type_n;
    std::vector<int> ins;
    std::vector<int> outs;

};

#endif // INTERCONNECTION_NETWORK_H
