#include "network.h"
#include <iostream>
#include <fstream>

network::network()
{
}
string network::getName() const{
    return Name;
}

void network::setName(const string &value){
    Name = value;
}
string network::getCommand() const{
    return Command;
}

void network::setCommand(const string &value){
    Command = value;
}

void network::setId(const int value){
    Id = value;
}

int network::getId() const {
    return Id;
}

string space_shift(string);

vector<network*> network::init_parse (string path){
    ifstream in (path.c_str());
    string line,newline;
    char data_in[4096];
    network *aux_net = new network();
    vector<network*> networks;
    FILE *arq;

        arq = fopen(path.c_str(), "r");
        if(arq == NULL)
                cout << "DEU RUIM XML";
            else{

                while (!feof(arq)){
                    fscanf(arq,"%s",data_in);
                    if (strcmp(data_in,"<name>")==0){
                        fscanf(arq,"%s",data_in);
                        aux_net->setName(data_in);
                    }
                    if (strcmp(data_in,"<command>")==0){
                        fscanf(arq,"%s",data_in);
                        aux_net->setCommand(data_in);
                    }
                    if (strcmp(data_in,"<id>")==0){
                        fscanf(arq,"%s",data_in);
                        aux_net->setId(atoi(data_in));
                        networks.push_back(aux_net);
                        aux_net = new network;
                    }

                }
                fclose(arq);
            }

    return networks;
}






