#include "varTable.h"

VarTable::VarTable()
{

}

VarTable::~VarTable()
{
    _variables.clear();
}

void VarTable::AddVar(Var* var)
{
    _variables.push_back(var);
}

bool VarTable::HasVar(const string& name)
{
    for (int i = 0; i < _variables.size(); ++i)
    {
        if (_variables[i]->GetName() == name)
        {
            return true;
        }
    }
    return false;
}

vector<Var*> VarTable::GetArray()
{
    return _variables;
}

void VarTable::Print()
{
    for (int i = 0; i < _variables.size(); ++i)
    {
        _variables[i]->Print();
    }
}
