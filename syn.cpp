#include "syn.h"

Syn::Syn(Lex* lex)
{
    this->_lex = lex;
    this->_tree = new Ast();
}

Syn::~Syn()
{
    delete _tree;
}

void Syn::Parse()
{
    _tree->root = Program();
	if (_lex->LastToken() != TokenType::DOT) Error("Not found .");
    _tree->Analyze();
}

Ast* Syn::GetAst()
{
    return _tree;
}

Node* Syn::Program(void)
{
       return StatementList();
}

Node* Syn::Number(void)
{
    string numberValue = _lex->CurrentTokenValue();

    Node* tempNode = new Node(NodeType::INTEGER_CONST, numberValue);
    _lex->NextToken();

    return tempNode;
}

Node* Syn::Identifier(void)
{
    string identifierValue = _lex->CurrentTokenValue();

    Node* tempNode  = new Node(NodeType::USING_VARIABLE, identifierValue);
    _lex->NextToken();

    return tempNode;
}

Node* Syn::ParenthesizedExpression(void)
{
    if (_lex->CurrentTokenType() != TokenType::LPAR)
        Error("'('");
    _lex->NextToken();

    Node* tempNode = Expression();

    if (_lex->CurrentTokenType() != TokenType::RPAR)
        Error("')'");
    _lex->NextToken();

    return tempNode;
}

Node* Syn::VariableDeclaration(void)
{
    if (flag<1)
        Error("Not found VAR");
    if (_lex->CurrentTokenType() != TokenType::IDENT)
        Error("Name of variable");

    string variableName = _lex->CurrentTokenValue();
    _lex->NextToken();


    if (_lex->CurrentTokenType() != TokenType::COLON)
        Error(":");
    _lex->NextToken();


    if (_lex->CurrentTokenType() != TokenType::KW_INTEGER)
    {
        Error("Type of variable");
    }
    string variableType = _lex->CurrentTokenValue();
    _lex->NextToken();

    Node* variableTypeNode = new Node(NodeType::VARIABLE_TYPE, variableType);

    return new Node(NodeType::VARIABLE_DECLARATION, variableName, variableTypeNode);
}

Node* Syn::PrimaryExpression(void)
{
    if (_lex->CurrentTokenType() == TokenType::DEC)
    {
        return Number();
    }
    else if (_lex->CurrentTokenType() == TokenType::IDENT)
    {
        string value = _lex->CurrentTokenValue();
        if (value == "write")
        {
            _lex->NextToken();

            if (_lex->CurrentTokenType() != TokenType::LPAR)
                Error("(");
            _lex->NextToken();

            Node* expressionNode = Expression();

            if (_lex->CurrentTokenType() != TokenType::RPAR)
                Error(")");
            _lex->NextToken();

            return new Node(NodeType::PRINT, "", expressionNode);
        }

        _lex->NextToken();
        if (_lex->CurrentTokenType() == TokenType::COLON)
        {
            _lex->PrevToken();
            return VariableDeclaration();
        }
        else
        {
            _lex->PrevToken();
            return Identifier();
        }
    }
    else if (_lex->CurrentTokenType() == TokenType::LPAR)
    {
        return ParenthesizedExpression();
    }

    return nullptr;
}

Node* Syn::UnaryExpression(void)
{
    if (_lex->CurrentTokenType() == TokenType::MINUS)
    {
        _lex->NextToken();

        Node* unaryExpressionNode = UnaryExpression();
        return new Node(NodeType::UNARY_MINUS, "", unaryExpressionNode);
    }

    return PrimaryExpression();
}

Node* Syn::MultiplicativeExpression(void)
{
    Node* tempNode = UnaryExpression();

    if (_lex->CurrentTokenType() == TokenType::MUL ||
            _lex->CurrentTokenType() == TokenType::DIV)
    {
        NodeType tempType = NodeType::MUL;

        if (_lex->CurrentTokenType() == TokenType::DIV)
            tempType = NodeType::DIV;

        _lex->NextToken();

        Node* tempMultiplicativeExpression = MultiplicativeExpression();

        tempNode = new Node(tempType, "", tempNode, tempMultiplicativeExpression);
    }

    return tempNode;
}

Node* Syn::AdditiveExpression(void)
{
    Node* tempNode = MultiplicativeExpression();

    if (_lex->CurrentTokenType() == TokenType::PLUS ||
            _lex->CurrentTokenType() == TokenType::MINUS)
    {
        NodeType tempType = NodeType::ADD;

        if (_lex->CurrentTokenType() == TokenType::MINUS)
            tempType = NodeType::SUB;

        _lex->NextToken();

        Node* tempAdditiveExpression = AdditiveExpression();

        tempNode = new Node(tempType, "", tempNode, tempAdditiveExpression);
    }

    return tempNode;
}

Node* Syn::RelationalExpression(void)
{
    Node* tempNode = AdditiveExpression();

    if (_lex->CurrentTokenType() == TokenType::LT)
    {
        _lex->NextToken();
        if (_lex->CurrentTokenType() == TokenType::GT)
     {
        _lex->NextToken();
        Node* tempRelationalExpression = RelationalExpression();
        tempNode = new Node(NodeType::NOT_EQUAL, "", tempNode, tempRelationalExpression);
     } else
     {
        Node* tempAdditiveExpression = AdditiveExpression();

        tempNode = new Node(NodeType::LESS, "", tempNode, tempAdditiveExpression);
     }
    }
    else if (_lex->CurrentTokenType() == TokenType::GT)
    {
        _lex->NextToken();
        Node* tempAdditiveExpression = AdditiveExpression();

        tempNode = new Node(NodeType::GREATER, "", tempNode, tempAdditiveExpression);
    }
    else if (_lex->CurrentTokenType() == TokenType::GTE)
    {
        _lex->NextToken();
        Node* tempAdditiveExpression = AdditiveExpression();

        tempNode = new Node(NodeType::GREATER_EQUAL, "", tempNode, tempAdditiveExpression);
    }
    else if (_lex->CurrentTokenType() == TokenType::LTE)
    {
        _lex->NextToken();
        Node* tempAdditiveExpression = AdditiveExpression();

        tempNode = new Node(NodeType::LESS_EQUAL, "", tempNode, tempAdditiveExpression);
    }

    return tempNode;
}

Node* Syn::EqualityExpression(void)
{
    Node* tempNode = RelationalExpression();

    if (_lex->CurrentTokenType() == TokenType::EQ)
    {
        _lex->NextToken();
        Node* tempRelationalExpression = RelationalExpression();

        tempNode = new Node(NodeType::EQUAL, "", tempNode, tempRelationalExpression);
    }
    else if (_lex->CurrentTokenType() == TokenType::NEQ)
    {
        _lex->NextToken();
        Node* tempRelationalExpression = RelationalExpression();

        tempNode = new Node(NodeType::NOT_EQUAL, "", tempNode, tempRelationalExpression);
    }

    return tempNode;
}

Node* Syn::AssignmentExpression(void)
{
    Node* tempNode = EqualityExpression();

    if (_lex->CurrentTokenType() == TokenType::ASSIGN)
    {
        _lex->NextToken();
        Node* tempAssignmentExpression = AssignmentExpression();

        tempNode = new Node(NodeType::SET, "", tempNode, tempAssignmentExpression);
    }

    return tempNode;
}

Node* Syn::Expression(void)
{
    Node* tempNode = AssignmentExpression();

    tempNode = new Node(NodeType::EXPRESSION, "", tempNode);

    return tempNode;
}

Node* Syn::SelectionStatement(void)
{
    Node* tempNode = new Node(NodeType::IF);

    _lex->NextToken();

    tempNode->op1 = Expression();

    if (_lex->CurrentTokenType() != TokenType::KW_THEN)
        Error("then");
    _lex->NextToken();


    tempNode->op2 = StatementList();


    if (_lex->CurrentTokenType() == TokenType::KW_ELSE)
    {
        tempNode->type = NodeType::IF_ELSE;
        _lex->NextToken();

        tempNode->op3 = StatementList();
    }

    return tempNode;
}

Node* Syn::IterationStatement(void)
{
    if (_lex->CurrentTokenType() == TokenType::KW_FOR)
    {
        _lex->NextToken();

        Node* initNode = AssignmentExpression();

        if (_lex->CurrentTokenType() != TokenType::KW_TO)
            Error("to");
        _lex->NextToken();

        Node* endValueNode = AssignmentExpression();

        if (_lex->CurrentTokenType() != TokenType::KW_DO)
            Error("do");
        _lex->NextToken();

        Node* bodyNode = StatementList();

        Node* oneConstantNode = new Node(NodeType::INTEGER_CONST, "1");
        Node* addOneNode = new Node(NodeType::ADD, "", initNode->op1, oneConstantNode);
        Node* afterEffectNode = new Node(NodeType::SET, "", initNode->op1, addOneNode);

        Node* condition = new Node(NodeType::LESS, "", initNode->op1, endValueNode);

        return new Node(NodeType::FOR, "", initNode, condition, afterEffectNode, bodyNode);
    }
    else if (_lex->CurrentTokenType() == TokenType::KW_WHILE)
    {
        _lex->NextToken();

        Node* conditionNode = EqualityExpression();

        if (_lex->CurrentTokenType() != TokenType::KW_DO)
            Error("do");
        _lex->NextToken();

        Node* bodyNode = StatementList();

        return new Node(NodeType::WHILE, "", conditionNode, bodyNode);
    }


    return nullptr;
}

Node* Syn::ExpressionStatement(void)
{
    Node* tempNode = Expression();
	
	if (_lex->CurrentTokenType() != TokenType::SEMICOLON)
	{
		
		if (_lex->CurrentTokenType() != TokenType::KW_VAR && _lex->CurrentTokenType() != TokenType::KW_ELSE && _lex->CurrentTokenType() != TokenType::DOT)
			Error(";");
		
	}
    _lex->NextToken();

    return tempNode;
}


Node* Syn::StatementList(void)
{
    Node* tempNode = nullptr;

    while (_lex->CurrentTokenType() != TokenType::KW_END &&
            _lex->CurrentTokenType() != TokenType::KW_ELSE && _lex->CurrentTokenType()!=TokenType::DOT)
    {
        Node* tempStatement = Statement();
        tempNode = new Node(NodeType::STATEMENT_LIST, "", tempNode, tempStatement);
    }

    if (_lex->CurrentTokenType() != TokenType::KW_ELSE)
    {
        _lex->NextToken();
    }

    return tempNode;
}

Node* Syn::Statement(void)
{
	/*if (_lex->CurrentTokenType() == TokenType::KW_BEGIN)
		temp = temp+1;
	if (_lex->CurrentTokenType() == TokenType::KW_END)
		temp = temp - 1;*/
	
    if (_lex->CurrentTokenType()==TokenType::KW_VAR)
        flag=1;
     if (_lex->CurrentTokenType() == TokenType::KW_IF)
    {
        return SelectionStatement();
    }
    else if (_lex->CurrentTokenType() == TokenType::KW_WHILE ||
            _lex->CurrentTokenType() == TokenType::KW_FOR)
    {
        return IterationStatement();
    }
    else
    {
            return ExpressionStatement();
    }
}
