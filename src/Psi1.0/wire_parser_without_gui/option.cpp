#include "option.h"

option::option()
{
}
string option::getOption_name() const
{
    return option_name;
}

void option::setOption_name(const string &value)
{
    option_name = value;
}
string option::getRelated_option() const
{
    return related_option;
}

void option::setRelated_option(const string &value)
{
    related_option = value;
}
string option::getData_type() const
{
    return data_type;
}

void option::setData_type(const string &value)
{
    data_type = value;
}
string option::getDescription() const
{
    return description;
}

void option::setDescription(const string &value)
{
    description = value;
}




