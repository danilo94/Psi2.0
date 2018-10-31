#ifndef NETWORK_H
#define NETWORK_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <QTreeWidget>
#include <QList>
#include <QStandardItem>
#include <option.h>
using namespace std;
class network
{
public:
    network();
    string getName() const;
    void setName(const string &value);

    string getCommand() const;
    void setCommand(const string &value);

    int getId() const;
    void setId (const int value);

    vector<network*> init_parse (string path);

private:
    string Name;
    string Command;
    int Id;
};

#endif // NETWORK_H
