#include "ast.h"


Ast::Ast()
{
    this->root = nullptr;
}

void Ast::Analyze()
{
    FindVars(root);
}


void Ast::FindVars(Node* currentNode)
{
    if (currentNode == nullptr)
        return;

    if (currentNode->type == NodeType::VARIABLE_DECLARATION)
    {
        string variableName = currentNode->value;

        if (_vars.HasVar(variableName))
        {
            throw SemErr("The variable'" + variableName + "' has already been declared");
        }

        Var* newVariable = new Var(variableName);
        _vars.AddVar(newVariable);

        return;
    }
    else if (currentNode->type == NodeType::USING_VARIABLE)
    {
        string variableName = currentNode->value;

        if (!_vars.HasVar(variableName))
        {
            throw SemErr("The variable '" + variableName + "' is not declared");
        }

        return;
    }

    FindVars(currentNode->op1);
    FindVars(currentNode->op2);
    FindVars(currentNode->op3);
    FindVars(currentNode->op4);
}

void Ast::Print()
{
    cout << endl << endl << "AST:" << endl;
    Print(root, 0);

    cout << endl << endl << "Table of vars: " << endl << endl;
    _vars.Print();
    cout << endl << endl;
}

VarTable Ast::GetVars()
{
    return _vars;
}

void Ast::Print(Node* currentNode, size_t level)
{
    if (currentNode == nullptr)
        return;


    for (size_t i = 0; i < level; i++)
        cout << "  ";


    switch (currentNode->type)
    {
        case NodeType::VARIABLE_DECLARATION:
        {
            cout << "Declare variable '";
            cout << currentNode->value << "'";
            break;
        }
        case NodeType::USING_VARIABLE:
        {
            cout << "Using variable '";
            cout << currentNode->value << "'";
            break;
        }
        case NodeType::VARIABLE_TYPE:
        {
            cout << "Variable type '";
            cout << currentNode->value << "'";
            break;
        }
        case NodeType::INTEGER_CONST:
        {
            cout << "Number constant (";
            cout << currentNode->value << ")";
            break;
        }
        case NodeType::ADD:
        {
            cout << "Add (+)";
            break;
        }
        case NodeType::SUB:
        {
            cout << "Sub (-)";
            break;
        }
        case NodeType::MUL:
        {
            cout << "Mul (*)";
            break;
        }
        case NodeType::DIV:
        {
            cout << "Div (/)";
            break;
        }
        case NodeType::LESS:
        {
            cout << "Less (<)";
            break;
        }
        case NodeType::GREATER:
        {
            cout << "Greater (>)";
            break;
        }
        case NodeType::EQUAL:
        {
            cout << "Equal (=)";
            break;
        }
        case NodeType::NOT_EQUAL:
        {
            cout << "Not equal (<>)";
            break;
        }
        case NodeType::LESS_EQUAL:
        {
            cout << "Less equal (<=)";
            break;
        }
        case NodeType::GREATER_EQUAL:
        {
            cout << "Greater equal (>=)";
            break;
        }
        case NodeType::FOR:
        {
            cout << "for";
            break;
        }
        case NodeType::IF:
        {
            cout << "if ";
            break;
        }
        case NodeType::IF_ELSE:
        {
            cout << "if else ";
            break;
        }
        case NodeType::UNARY_MINUS:
        {
            cout << "unary - ";
            break;
        }
        case NodeType::SET:
        {
            cout << "set ";
            break;
        }
        case NodeType::EXPRESSION:
        {
            cout << "expr ";
            break;
        }
        case NodeType::STATEMENT:
        {
            cout << "stmt ";
            break;
        }
        case NodeType::STATEMENT_LIST:
        {
            cout << "seq ";
            break;
        }
        case NodeType::IDENTIFIER:
        {
            cout << "identifier '";
            cout << currentNode->value << "'";
            break;
        }
        case NodeType::WHILE:
        {
            cout << "while ";
            break;
        }
        case NodeType::REPEAT:
        {
            cout << "repeat ";
            break;
        }
        case NodeType::PRINT:
        {
            cout << "write ";
            break;
        }
    }

    cout << endl;

    Print(currentNode->op1, level + 1);
    Print(currentNode->op2, level + 1);
    Print(currentNode->op3, level + 1);
    Print(currentNode->op4, level + 1);
}
