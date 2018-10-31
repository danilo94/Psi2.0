#ifndef OPTION_H
#define OPTION_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <QTreeWidget>
#include <QList>
#include <QStandardItem>
using namespace std;
class option
{
public:
    option();
    string getOption_name() const;
    void setOption_name(const string &value);
    string getRelated_option() const;
    void setRelated_option(const string &value);
    string getData_type() const;
    void setData_type(const string &value);
    string getDescription() const;
    void setDescription(const string &value);
private:
    string option_name;
    string related_option;
    string data_type;
    string description;
};

#endif // OPTION_H
