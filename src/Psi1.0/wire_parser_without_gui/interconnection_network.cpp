#include "interconnection_network.h"


interconnection_network::interconnection_network(int in, int out, string type)
{
    ins.resize(in);
    outs.resize(out);
    this->type_n = type;
}

interconnection_network::interconnection_network(int in, int out,int stages, string type)
{
    ins.resize(in);
    outs.resize(out);
    this->type_n = type;
    this->stages = stages;
}

interconnection_network::interconnection_network(int in, int out,int stages, int radix, string type)
{
    ins.resize(in);
    outs.resize(out);
    this->type_n = type;
    this->stages = stages;
    this->radix = stages;
}
