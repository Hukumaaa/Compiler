#include "var.h"

Var::Var(const string& name)
{
    this->_name = name;
}

void Var::Print()
{
    cout << "Variable with name: '" << _name << "' and type: 'integer'\n";
}

string Var::GetName()
{
    return _name;
}
