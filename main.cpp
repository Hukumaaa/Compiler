#include "lex.h"
#include "syn.h"
#include "codeGen.h"
#include <iostream>


using namespace std;


int main(void)
{

    Init();

    try
    {
        Lex lex("test.pas");
        lex.GetTokens();
        lex.PrintTokens();


        Syn syn(&lex);
        syn.Parse();


        Ast* tree = syn.GetAst();
        tree->Print();


        CodeGen gen("test.asm", tree);
        gen.GenerateAssembler();
    }
    catch (LexErr& err)
    {
        system("pause");
        exit(1);
    }
    catch (SynErr& err)
    {
        system("pause");
        exit(1);
    }
    catch (SemErr& err)
    {
        system("pause");
        exit(1);
    }

    system("pause");
    return 0;
}
