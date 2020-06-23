#include "codeGen.h"

CodeGen::CodeGen(const char* fname, Ast* ast)
{
    this->_outputFile.open(fname);
    this->_tree = ast;
    this->_assemblerCurrentPlace = &_assemblerPlaceData;
    this->_byteOnStack = 8;
    this->_countLabels = 0;
}

void CodeGen::GenerateAssembler()
{
    GenerateAssemblerCodeForDivOperand();
    GenerateAssemblerCodeForLocalVars();
    GenerateAssemblerCodeForPrintFunction();
    GenerateAssemblerCodeForBlocks();
    WriteInOutputFile(asmHeader);
    WriteInOutputFile(startData);
    WriteInOutputFile(_assemblerPlaceData);
    WriteInOutputFile(endData);
    WriteInOutputFile(textStart);
    WriteInOutputFile(_assemblerPlaceBeforeMain);
    WriteInOutputFile(labelStart);
    WriteInOutputFile(procProlog + to_string(_tree->GetVars().GetArray().size() * 4 + 4) + ", 0");
    WriteInOutputFile(_assemblerPlaceMain);
    WriteInOutputFile(procEpilogue);
    WriteInOutputFile(functionReturn);
    WriteInOutputFile(textEnd);
    WriteInOutputFile(labelEnd);

    _outputFile.close();
}

void CodeGen::GenerateAssemblerCodeForBlocks()
{
    GenerateAssemblerCodeForAllBlocks(_tree->root);
}

void CodeGen::GenerateAssemblerCodeForSetBlock(Node* currentNode)
{
    Node* op1 = currentNode->op1;
    Node* op2 = currentNode->op2;

    if (op1->type == NodeType::USING_VARIABLE || op1->type == NodeType::VARIABLE_DECLARATION)
    {
        string variableName = op1->value;

        GenerateAssemblerCodeForExpressions(op2);
        GenPop(eax);
        GenMov(GetAssemblerLocalVar(variableName), eax);
    }
}

void CodeGen::GenerateAssemblerCodeForIfBlock(Node* currentNode)
{
    Node* condition = currentNode->op1;
    Node* statement = currentNode->op2;
    Node* elseStatement = currentNode->op3;

    ++_countLabels;
    string startLabel = "LS" + to_string(_countLabels);
    string endLabel = "LE" + to_string(_countLabels);
    string elseLabel = "LEL" + to_string(_countLabels);

    GenerateAssemblerCodeForAllBlocks(condition);
    GenPop(eax);
    GenCmp(eax, null);

    string endOrElseLabel = endLabel;

    if (currentNode->type == NodeType::IF_ELSE)
    {
        endOrElseLabel = elseLabel;
    }

    GenJe(endOrElseLabel);

    if (currentNode->type == NodeType::IF_ELSE)
    {
        GenLabel(startLabel);
        GenerateAssemblerCodeForAllBlocks(statement);
        GenJmp(endLabel);

        GenLabel(elseLabel);
        GenerateAssemblerCodeForAllBlocks(elseStatement);
        GenLabel(endLabel);
    }
    else if (currentNode->type == NodeType::IF)
    {
        GenLabel(startLabel);
        GenerateAssemblerCodeForAllBlocks(statement);
        GenLabel(endLabel);
    }
}

void CodeGen::GenerateAssemblerCodeForWhileBlock(Node* currentNode)
{
    Node* condition = currentNode->op1;
    Node* statement = currentNode->op2;

    ++_countLabels;
    string startLabel = "LS" + to_string(_countLabels);
    string endLabel = "LE" + to_string(_countLabels);
    string aftereffectsLabel = "LA" + to_string(_countLabels);

    GenLabel(startLabel);
    GenLabel(aftereffectsLabel);
    GenerateAssemblerCodeForAllBlocks(condition);

    GenPop(eax);
    GenCmp(eax, null);
    GenJe(endLabel);
    GenerateAssemblerCodeForAllBlocks(statement);

    GenJmp(startLabel);
    GenLabel(endLabel);
}

void CodeGen::GenerateAssemblerCodeForForBlock(Node* currentNode)
{
    Node* prevention = currentNode->op1;
    Node* condition = currentNode->op2;
    Node* aftereffects = currentNode->op3;
    Node* statement = currentNode->op4;

    ++_countLabels;
    string startLabel = "LS" + to_string(_countLabels);
    string endLabel = "LE" + to_string(_countLabels);
    string aftereffectsLabel = "LA" + to_string(_countLabels);

    GenerateAssemblerCodeForAllBlocks(prevention);

    GenLabel(startLabel);
    GenerateAssemblerCodeForAllBlocks(condition);

    GenPop(eax);
    GenCmp(eax, null);
    GenJe(endLabel);
    GenerateAssemblerCodeForAllBlocks(statement);
    GenLabel(aftereffectsLabel);

    GenerateAssemblerCodeForAllBlocks(aftereffects);
    GenJmp(startLabel);

    GenLabel(endLabel);
}

void CodeGen::GenerateAssemblerCodeForPrintFunctionCall(Node* currentNode)
{
    GenerateAssemblerCodeForExpressions(currentNode->op1);
    GenPop(eax);
    GenPush(eax);
    GenRaw(tab + "call print\n");
}

void CodeGen::GenerateAssemblerCodeForAllBlocks(Node* currentNode)
{
    if (currentNode == nullptr)
        return;

    if (currentNode->type == NodeType::SET)
    {
        GenerateAssemblerCodeForSetBlock(currentNode);
        return;
    }
    else if (currentNode->isComparisonOperator())
    {
        GenerateAssemblerCodeForConditionExpressions(currentNode);
        return;
    }
    else if (currentNode->type == NodeType::IF || currentNode->type == NodeType::IF_ELSE)
    {
        GenerateAssemblerCodeForIfBlock(currentNode);
        return;
    }
    else if (currentNode->type == NodeType::WHILE)
    {
        GenerateAssemblerCodeForWhileBlock(currentNode);
        return;
    }
    else if (currentNode->type == NodeType::FOR)
    {
        GenerateAssemblerCodeForForBlock(currentNode);
        return;
    }
    else if (currentNode->type == NodeType::EXPRESSION)
    {
        GenerateAssemblerCodeForAllBlocks(currentNode->op1);
        return;
    }
    else if (currentNode->type == NodeType::PRINT)
    {
        GenerateAssemblerCodeForPrintFunctionCall(currentNode->op1);
        return;
    }

    GenerateAssemblerCodeForAllBlocks(currentNode->op1);
    GenerateAssemblerCodeForAllBlocks(currentNode->op2);
    GenerateAssemblerCodeForAllBlocks(currentNode->op3);
    GenerateAssemblerCodeForAllBlocks(currentNode->op4);
}

void CodeGen::GenerateAssemblerCodeForExpressions(Node* currentNode)
{
    if (currentNode == nullptr)
        return;

    if (currentNode->type == NodeType::ADD)
    {
        GenerateAssemblerCodeForExpressions(currentNode->op1);
        GenerateAssemblerCodeForExpressions(currentNode->op2);

        GenPop(eax);
        GenPop(ebx);
        GenAdd(eax, ebx);
        GenPush(eax);
    }
    else if (currentNode->type == NodeType::SUB)
    {
        GenerateAssemblerCodeForExpressions(currentNode->op1);
        GenerateAssemblerCodeForExpressions(currentNode->op2);

        GenPop(ebx);
        GenPop(eax);
        GenSub(eax, ebx);
        GenPush(eax);
    }
    else if (currentNode->type == NodeType::MUL)
    {
        GenerateAssemblerCodeForExpressions(currentNode->op1);
        GenerateAssemblerCodeForExpressions(currentNode->op2);

        GenPop(eax);
        GenPop(ebx);
        GenImul(eax, ebx);
        GenPush(eax);
    }
    else if (currentNode->type == NodeType::DIV)
    {
        GenerateAssemblerCodeForExpressions(currentNode->op1);
        GenerateAssemblerCodeForExpressions(currentNode->op2);

        GenPop(ebx);
        GenPop(eax);
        GenMov("division_1", eax);
        GenMov("division_2", ebx);
        GenFinit();
        GenFild("division_2");
        GenFild("division_1");
        GenFdiv("st(0)", "st(1)");
        GenFist("division_1");
        GenPush("division_1");
    }
    else if (currentNode->type == NodeType::UNARY_MINUS)
    {
        GenerateAssemblerCodeForExpressions(currentNode->op1);

        GenPop(eax);
        GenImul(eax, minus_one);
        GenPush(eax);
    }
    else if (currentNode->type == NodeType::INTEGER_CONST)
    {
        string numberValue = currentNode->value;
        GenPush(numberValue);
    }
    else if (currentNode->type == NodeType::USING_VARIABLE)
    {
        string variableName = currentNode->value;
        GenPush(GetAssemblerLocalVar(variableName));
    }
    else if (currentNode->isComparisonOperator())
    {
        GenerateAssemblerCodeForConditionExpressions(currentNode);
        return;
    }
    else if (currentNode->type == NodeType::EXPRESSION)
    {
        GenerateAssemblerCodeForExpressions(currentNode->op1);
    }

}

void CodeGen::GenerateAssemblerCodeForConditionExpressions(Node* currentNode)
{
    if (currentNode == nullptr)
        return;

    if (currentNode->type == NodeType::INTEGER_CONST)
    {
        int value = stoi(currentNode->value);
        if (value == 0)
        {
            GenPush(null);
        }
        else
        {
            GenPush(one);
        }
    }
    else if (currentNode->type == NodeType::USING_VARIABLE)
    {
        ++_countLabels;
        string labelIfEqual = "LE" + to_string(_countLabels);
        string labelIfNotEqual = "LN" + to_string(_countLabels);
        string labelCompareEnd = "LCE" + to_string(_countLabels);

        string variableName = currentNode->value;

        GenCmp(GetAssemblerLocalVar(variableName), null);
        GenJne(labelIfNotEqual);
        GenPush(null);
        GenJmp(labelCompareEnd);
        GenLabel(labelIfNotEqual);
        GenPush(one);
        GenLabel(labelCompareEnd);
    }
    else if (currentNode->type == NodeType::EXPRESSION)
    {
        GenerateAssemblerCodeForConditionExpressions(currentNode->op1);
        return;
    }
    else
    {
        Node* op1 = currentNode->op1;
        Node* op2 = currentNode->op2;

        GenerateAssemblerCodeForExpressions(op1);
        GenPop(ecx);
        GenerateAssemblerCodeForExpressions(op2);
        GenPop(edx);
        GenCmp(ecx, edx);

        ++_countLabels;
        string labelIfEqual = "LE" + to_string(_countLabels);
        string labelIfNotEqual = "LN" + to_string(_countLabels);
        string labelCompareEnd = "LCE" + to_string(_countLabels);

        if (currentNode->type == NodeType::LESS)
        {
            GenJge(labelIfNotEqual);
        }
        else if (currentNode->type == NodeType::LESS_EQUAL)
        {
            GenJg(labelIfNotEqual);
        }
        else if (currentNode->type == NodeType::GREATER)
        {
            GenJle(labelIfNotEqual);
        }
        else if (currentNode->type == NodeType::GREATER_EQUAL)
        {
            GenJl(labelIfNotEqual);
        }
        else if (currentNode->type == NodeType::EQUAL)
        {
            GenJne(labelIfNotEqual);
        }
        else if (currentNode->type == NodeType::NOT_EQUAL)
        {
            GenJe(labelIfNotEqual);
        }

        GenPush(one);
        GenJmp(labelCompareEnd);
        GenLabel(labelIfNotEqual);
        GenPush(null);
        GenLabel(labelCompareEnd);

        return;
    }
}

void CodeGen::WriteInOutputFile(const string& text)
{
    _outputFile << text << "\n";
}

void CodeGen::SetPlaceInAssemblerForWrite(AssemblerPart place)
{
    switch (place)
    {
        case AssemblerPart::DATA:
        {
            _assemblerCurrentPlace = &_assemblerPlaceData;
            break;
        }
        case AssemblerPart::BEFORE_MAIN:
        {
            _assemblerCurrentPlace = &_assemblerPlaceBeforeMain;
            break;
        }
        case AssemblerPart::MAIN:
        {
            _assemblerCurrentPlace = &_assemblerPlaceMain;
            break;
        }
    }
}

void CodeGen::GenerateAssemblerCodeForLocalVars()
{
    vector<Var*> variables = _tree->GetVars().GetArray();

    for (int i = 0; i < variables.size(); ++i)
    {
        GenerateAssemblerCodeForStackVariable(variables[i]);
    }

    _byteOnStack = 4;
}

void CodeGen::GenerateAssemblerCodeForDivOperand()
{
    SetPlaceInAssemblerForWrite(AssemblerPart::DATA);
    GenRaw(tab + "division_1 dd 0\n");
    GenRaw(tab + "division_2 dd 0\n");
    SetPlaceInAssemblerForWrite(AssemblerPart::MAIN);
}

void CodeGen::GenerateAssemblerCodeForPrintFunction()
{
    SetPlaceInAssemblerForWrite(AssemblerPart::DATA);
    GenRaw(tab + "print_format db \"%d \", 0\n");
    SetPlaceInAssemblerForWrite(AssemblerPart::BEFORE_MAIN);

    GenRaw("\nprint PROC\n");
    GenRaw(tab + "enter 0, 0\n");

    GenMov(eax, "[ebp + 8]");
    GenRaw(tab + "invoke crt_printf, offset print_format, eax\n");

    GenRaw(tab + "leave\n");
    GenRaw(tab + "ret 4\n");
    GenRaw("print ENDP\n");

    SetPlaceInAssemblerForWrite(AssemblerPart::MAIN);
}

string CodeGen::GetAssemblerLocalVar(const string& value)
{
    return value + "[ebp]";
}

void CodeGen::GenerateAssemblerCodeForStackVariable(Var* variable)
{
    string variableName = variable->GetName();
    size_t variableSize = 4;
    _assemblerPlaceBeforeMain.append(variableName + " = " + "-" + to_string(_byteOnStack) + "\n");
    _byteOnStack += variableSize;
}

void CodeGen::GenPush(const string& value)
{
    _assemblerCurrentPlace->append(tab + "push " + value + "\n");
}

void CodeGen::GenPop(const string& value)
{
    _assemblerCurrentPlace->append(tab + "pop " + value + "\n");
}

void CodeGen::GenAdd(const string& value1, const string& value2)
{
    _assemblerCurrentPlace->append(tab + "add " + value1 + ", " + value2 + "\n");
}

void CodeGen::GenSub(const string& value1, const string& value2)
{
    _assemblerCurrentPlace->append(tab + "sub " + value1 + ", " + value2 + "\n");
}

void CodeGen::GenImul(const string& value1, const string& value2)
{
    _assemblerCurrentPlace->append(tab + "imul " + value1 + ", " + value2 + "\n");
}

void CodeGen::GenMov(const string& value1, const string& value2)
{
    _assemblerCurrentPlace->append(tab + "mov " + value1 + ", " + value2 + "\n");
}

void CodeGen::GenRaw(const string& value)
{
    _assemblerCurrentPlace->append(value);
}

void CodeGen::GenCmp(const string& value1, const string& value2)
{
    _assemblerCurrentPlace->append(tab + "cmp " + value1 + ", " + value2 + "\n");
}

void CodeGen::GenJmp(const string& value)
{
    _assemblerCurrentPlace->append(tab + "jmp " + value + "\n");
}

void CodeGen::GenJe(const string& value)
{
    _assemblerCurrentPlace->append(tab + "je " + value + "\n");
}

void CodeGen::GenJne(const string& value)
{
    _assemblerCurrentPlace->append(tab + "jne " + value + "\n");
}

void CodeGen::GenJl(const string& value)
{
    _assemblerCurrentPlace->append(tab + "jl " + value + "\n");
}

void CodeGen::GenJle(const string& value)
{
    _assemblerCurrentPlace->append(tab + "jle " + value + "\n");
}

void CodeGen::GenJg(const string& value)
{
    _assemblerCurrentPlace->append(tab + "jg " + value + "\n");
}

void CodeGen::GenJge(const string& value)
{
    _assemblerCurrentPlace->append(tab + "jge " + value + "\n");
}

void CodeGen::GenLabel(const string& value)
{
    _assemblerCurrentPlace->append(value + ":\n");
}

void CodeGen::GenFinit()
{
    _assemblerCurrentPlace->append(tab + "finit\n");
}

void CodeGen::GenFild(const string& value)
{
    _assemblerCurrentPlace->append(tab + "fild " + value + "\n");
}

void CodeGen::GenFdiv(const string& value1, const string& value2)
{
    _assemblerCurrentPlace->append(tab + "fdiv " + value1 + ", " + value2 + "\n");
}

void CodeGen::GenFist(const string& value)
{
    _assemblerCurrentPlace->append(tab + "fist " + value + "\n");
}

