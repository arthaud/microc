#include "ast.hpp"

#include <cassert>

namespace microc {
namespace ast {

/*
 * Entities
 */
Entity::~Entity() {}

void AssemblyEntity::accept(EntityVisitor& v) const {
    v.visit(*this);
}

void GlobalEntity::accept(EntityVisitor& v) const {
    v.visit(*this);
}

void FunctionEntity::accept(EntityVisitor& v) const {
    v.visit(*this);
}

/*
 * Instructions
 */
Instruction::~Instruction() {}

void BlockInstruction::accept(InstructionVisitor& v) const {
    v.visit(*this);
}

void DeclarationInstruction::accept(InstructionVisitor& v) const {
    v.visit(*this);
}

void ExpressionInstruction::accept(InstructionVisitor& v) const {
    v.visit(*this);
}

void IfInstruction::accept(InstructionVisitor& v) const {
    v.visit(*this);
}

void WhileInstruction::accept(InstructionVisitor& v) const {
    v.visit(*this);
}

void ReturnInstruction::accept(InstructionVisitor& v) const {
    v.visit(*this);
}

void AssemblyInstruction::accept(InstructionVisitor& v) const {
    v.visit(*this);
}

/*
 * Expressions
 */
Expression::~Expression() {}

void IdentExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

template<>
void IntegerExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

template<>
void CharExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

template<>
void StringExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

void TrueExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

void FalseExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

void NullExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

void UnaryExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

const char* UnaryExpression::operator_str(Operator op) {
    switch(op) {
        case UnaryExpression::Operator::Plus:   return "+";
        case UnaryExpression::Operator::Minus:  return "-";
        case UnaryExpression::Operator::Not:    return "!";
        case UnaryExpression::Operator::BitNot: return "~";
        default: assert(false && "unknown unary operator");
    }
}

void BinaryExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

const char* BinaryExpression::operator_str(Operator op) {
    switch(op) {
        case BinaryExpression::Operator::Add:    return "+";
        case BinaryExpression::Operator::Sub:    return "-";
        case BinaryExpression::Operator::Mul:    return "*";
        case BinaryExpression::Operator::Div:    return "/";
        case BinaryExpression::Operator::Mod:    return "%";
        case BinaryExpression::Operator::Or:     return "||";
        case BinaryExpression::Operator::And:    return "&&";
        case BinaryExpression::Operator::BitOr:  return "|";
        case BinaryExpression::Operator::BitAnd: return "&";
        case BinaryExpression::Operator::BitXor: return "^";
        case BinaryExpression::Operator::Eq:     return "==";
        case BinaryExpression::Operator::Neq:    return "!=";
        case BinaryExpression::Operator::Inf:    return "<";
        case BinaryExpression::Operator::InfEq:  return "<=";
        case BinaryExpression::Operator::Sup:    return ">";
        case BinaryExpression::Operator::SupEq:  return ">=";
        case BinaryExpression::Operator::Lshift: return "<<";
        case BinaryExpression::Operator::Rshift: return ">>";
        default: assert(false && "unknown binary operator");
    }
}

void AffectationExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

void CastExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

void AccessExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

void CallExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

/*
 * Types
 */
Type::~Type() {}

std::size_t VoidType::size() const {
    return 0;
}

void VoidType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

std::size_t ScalarType::size() const {
    return size_;
}

void IntegerType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

void BooleanType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

void CharType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

void NullType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

void PointerType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

/*
 * operator<<
 */
PrintEntityVisitor::PrintEntityVisitor(std::ostream& o): o(o) {}

void PrintEntityVisitor::visit(const AssemblyEntity& entity) {
    o << "asm(\"" << entity.assembly << "\");";
}

void PrintEntityVisitor::visit(const GlobalEntity& entity) {
    o << *entity.type << " " << entity.name << ";";
}

void PrintEntityVisitor::visit(const FunctionEntity& entity) {
    bool first_argument = true;
    o << *entity.return_type << " " << entity.name << "(";

    for(const auto& arg : entity.arguments) {
        if(!first_argument) {
            o << ", ";
        }

        o << *arg.type << " " << arg.name;
        first_argument = false;
    }

    o << ") {" << std::endl;

    for(const auto& instr : entity.instructions) {
        o << *instr << std::endl;
    }

    o << "}";
}

PrintInstructionVisitor::PrintInstructionVisitor(std::ostream& o): o(o) {}

void PrintInstructionVisitor::visit(const BlockInstruction& instr) {
    o << "{" << std::endl;

    for(const auto& ins : instr.instructions) {
        o << *ins << std::endl;
    }

    o << "}";
}

void PrintInstructionVisitor::visit(const DeclarationInstruction& instr) {
    o << *instr.type << " " << instr.name;

    if(instr.expression != nullptr) {
        o << " = " << *instr.expression;
    }

    o << ";";
}

void PrintInstructionVisitor::visit(const ExpressionInstruction& instr) {
    o << *instr.expression << ";";
}

void PrintInstructionVisitor::visit(const IfInstruction& instr) {
    o << "if (" << *instr.condition << ") {" << std::endl;

    for(const auto& ins : instr.true_instrs) {
        o << *ins << std::endl;
    }

    o << "}" << std::endl << "else {" << std::endl;

    for(const auto& ins : instr.false_instrs) {
        o << *ins << std::endl;
    }

    o << "}";
}

void PrintInstructionVisitor::visit(const WhileInstruction& instr) {
    o << "while (" << *instr.condition << ") {" << std::endl;

    for(const auto& ins : instr.instructions) {
        o << *ins << std::endl;
    }

    o << "}";
}

void PrintInstructionVisitor::visit(const ReturnInstruction& instr) {
    o << "return " << *instr.expression << ";";
}

void PrintInstructionVisitor::visit(const AssemblyInstruction& instr) {
    o << "asm(\"" << instr.assembly << "\");";
}

PrintExpressionVisitor::PrintExpressionVisitor(std::ostream& o): o(o) {}

void PrintExpressionVisitor::visit(const IdentExpression& expr) {
    o << expr.name;
}

void PrintExpressionVisitor::visit(const IntegerExpression& expr) {
    o << expr.value;
}

void PrintExpressionVisitor::visit(const CharExpression& expr) {
    if(expr.value == '\0') {
        o << "'\\0'";
    }
    else if(expr.value == '\n') {
        o << "'\\n'";
    }
    else if(expr.value == '\r') {
        o << "'\\r'";
    }
    else if(expr.value == '\t') {
        o << "'\\t'";
    }
    else if(expr.value == '\'') {
        o << "'\\''";
    }
    else {
        o << "'" << expr.value << "'";
    }
}

void PrintExpressionVisitor::visit(const StringExpression& expr) {
    o << '"' << expr.value << '"';
}

void PrintExpressionVisitor::visit(const TrueExpression&) {
    o << "true";
}

void PrintExpressionVisitor::visit(const FalseExpression&) {
    o << "false";
}

void PrintExpressionVisitor::visit(const NullExpression&) {
    o << "NULL";
}

void PrintExpressionVisitor::visit(const UnaryExpression& expr) {
    o << UnaryExpression::operator_str(expr.op);
    o << '(' << *expr.expression << ')';
}

void PrintExpressionVisitor::visit(const BinaryExpression& expr) {
    o << '(' << *expr.left << ')';
    o << BinaryExpression::operator_str(expr.op);
    o << '(' << *expr.right << ')';
}

void PrintExpressionVisitor::visit(const AffectationExpression& expr) {
    o << *expr.affected << " = " << *expr.value;
}

void PrintExpressionVisitor::visit(const CastExpression& expr) {
    o << "(" << *expr.type << ") " << *expr.expression;
}

void PrintExpressionVisitor::visit(const AccessExpression& expr) {
    o << "*(" << *expr.expression << ")";
}

void PrintExpressionVisitor::visit(const CallExpression& expr) {
    bool first_argument = true;
    o << expr.function_name << "(";

    for(const auto& e : expr.arguments) {
        if(!first_argument) {
            o << ", ";
        }

        o << *e;
        first_argument = false;
    }

    o << ")";
}

PrintTypeVisitor::PrintTypeVisitor(std::ostream& o): o(o) {}

void PrintTypeVisitor::visit(const VoidType&) {
    o << "void";
}

void PrintTypeVisitor::visit(const IntegerType&) {
    o << "int";
}

void PrintTypeVisitor::visit(const BooleanType&) {
    o << "bool";
}

void PrintTypeVisitor::visit(const CharType&) {
    o << "char";
}

void PrintTypeVisitor::visit(const NullType&) {
    o << "null";
}

void PrintTypeVisitor::visit(const PointerType& type) {
    o << "pointer on " << *type.pointed_type();
}

std::ostream& operator<<(std::ostream& o, const Program& prog) {
    o << "program {" << std::endl;

    for(const auto& entity : prog.entities) {
        o << *entity << std::endl;
    }

    o << "}" << std::endl;
    return o;
}

std::ostream& operator<<(std::ostream& o, const Entity& entity) {
    PrintEntityVisitor v(o);
    entity.accept(v);
    return o;
}

std::ostream& operator<<(std::ostream& o, const Instruction& instr) {
    PrintInstructionVisitor v(o);
    instr.accept(v);
    return o;
}

std::ostream& operator<<(std::ostream& o, const Expression& expr) {
    PrintExpressionVisitor v(o);
    expr.accept(v);
    return o;
}

std::ostream& operator<<(std::ostream& o, const Type& type) {
    PrintTypeVisitor v(o);
    type.accept(v);
    return o;
}

} // namespace ast
} // namespace microc
