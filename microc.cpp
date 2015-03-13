#include "ast.hpp"

#include <iostream>

using namespace microc::ast;

int main(int argc, char* argv[]) {
    Program prog;

    prog.entities.push_back(std::make_unique<AssemblyEntity>("mov 0, %rax"));
    prog.entities.push_back(std::make_unique<GlobalEntity>(
        std::make_unique<VoidType>(), "x"
    ));

    std::unique_ptr<FunctionEntity> fun = std::make_unique<FunctionEntity>(
        std::make_unique<IntegerType>(4), "f"
    );
    std::vector< std::unique_ptr<Instruction> >& instrs = fun->instructions;

    instrs.push_back(std::make_unique<DeclarationInstruction>(
        std::make_unique<PointerType>(std::make_unique<CharType>(1), 4),
        "toto",
        std::make_unique<IntegerExpression>(42)
    ));

    instrs.push_back(std::make_unique<ExpressionInstruction>(
        std::make_unique<AffectationExpression>(
            std::make_unique<IdentExpression>("toto"),
            std::make_unique<BinaryExpression>(BinaryExpression::Operator::Add,
                std::make_unique<IdentExpression>("x"),
                std::make_unique<IntegerExpression>(1)
            )
        )
    ));

    prog.entities.push_back(std::move(fun));

    std::cout << prog;
    return 0;
}
