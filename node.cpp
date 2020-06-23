#include "node.h"

Node::Node(NodeType type, const string& value, Node* op1, Node* op2, Node* op3, Node* op4)
{
    this->type = type;
    this->value = value;
    this->op1 = op1;
    this->op2 = op2;
    this->op3 = op3;
    this->op4 = op4;
}

bool Node::isComparisonOperator()
{
    return type == NodeType::LESS ||
           type == NodeType::GREATER ||
           type == NodeType::LESS_EQUAL ||
           type == NodeType::GREATER_EQUAL ||
           type == NodeType::EQUAL ||
           type == NodeType::NOT_EQUAL;
}
