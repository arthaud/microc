#include "ast.hpp"

#include <cassert>

namespace microc {
namespace ast {
    /*
     * Entities
     */
    Entity::~Entity() {}

    AssemblyEntity::AssemblyEntity(const std::string& s): assembly(s) {}

    void AssemblyEntity::accept(EntityVisitor& v) const {
        v.visit(*this);
    }

    GlobalEntity::GlobalEntity(std::unique_ptr<Type>&& t, const std::string& n):
        type(std::move(t)),
        name(n)
    {}

    void GlobalEntity::accept(EntityVisitor& v) const {
        v.visit(*this);
    }

    FunctionArgument::FunctionArgument(std::unique_ptr<Type>&& t, const std::string& n):
        type(std::move(t)),
        name(n)
    {}

    FunctionArgument::FunctionArgument(FunctionArgument&& f):
        type(std::move(f.type)),
        name(std::move(f.name))
    {}

    FunctionArgument& FunctionArgument::operator=(FunctionArgument&& f) {
        type = std::move(f.type);
        name = std::move(f.name);
    }

    FunctionEntity::FunctionEntity(std::unique_ptr<Type>&& t, const std::string& n):
        return_type(std::move(t)),
        name(n)
    {}

    void FunctionEntity::accept(EntityVisitor& v) const {
        v.visit(*this);
    }

    /*
     * Instructions
     */
    Instruction::~Instruction() {}

    BlockInstruction::BlockInstruction() {}

    BlockInstruction::BlockInstruction(std::vector<std::unique_ptr<Instruction>>&& instructions):
        instructions(std::move(instructions))
    {}

    void BlockInstruction::accept(InstructionVisitor& v) const {
        v.visit(*this);
    }

    DeclarationInstruction::DeclarationInstruction(std::unique_ptr<Type>&& t, const std::string& n, std::unique_ptr<Expression>&& e):
        type(std::move(t)),
        name(n),
        expression(std::move(e))
    {}

    void DeclarationInstruction::accept(InstructionVisitor& v) const {
        v.visit(*this);
    }

    ExpressionInstruction::ExpressionInstruction(std::unique_ptr<Expression>&& e):
        expression(std::move(e))
    {}

    void ExpressionInstruction::accept(InstructionVisitor& v) const {
        v.visit(*this);
    }

    IfInstruction::IfInstruction(std::unique_ptr<Expression>&& c): condition(std::move(c)) {}

    void IfInstruction::accept(InstructionVisitor& v) const {
        v.visit(*this);
    }

    WhileInstruction::WhileInstruction(std::unique_ptr<Expression>&& c): condition(std::move(c)) {}

    void WhileInstruction::accept(InstructionVisitor& v) const {
        v.visit(*this);
    }

    ReturnInstruction::ReturnInstruction(std::unique_ptr<Expression>&& e): expression(std::move(e)) {}

    void ReturnInstruction::accept(InstructionVisitor& v) const {
        v.visit(*this);
    }

    AssemblyInstruction::AssemblyInstruction(const std::string& a): assembly(a) {}

    void AssemblyInstruction::accept(InstructionVisitor& v) const {
        v.visit(*this);
    }

    /*
     * Expressions
     */
    Expression::~Expression() {}

    IdentExpression::IdentExpression(const std::string& n) : name(n) {}

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

    UnaryExpression::UnaryExpression(Operator o, std::unique_ptr<Expression>&& e):
        op(o),
        expression(std::move(e))
    {}

    void UnaryExpression::accept(ExpressionVisitor& v) const {
        v.visit(*this);
    }

    BinaryExpression::BinaryExpression(Operator o, std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r):
        op(o),
        left(std::move(l)),
        right(std::move(r))
    {}

    const char* UnaryExpression::operator_str(Operator op) {
        switch(op) {
            case UnaryExpression::Operator::Plus:  return "+";
            case UnaryExpression::Operator::Minus: return "-";
            case UnaryExpression::Operator::Not:   return "!";
            default: assert(false && "unknown unary operator");
        }
    }

    void BinaryExpression::accept(ExpressionVisitor& v) const {
        v.visit(*this);
    }

    const char* BinaryExpression::operator_str(Operator op) {
        switch(op) {
            case BinaryExpression::Operator::Add: return "+";
            case BinaryExpression::Operator::Sub: return "-";
            case BinaryExpression::Operator::Mul: return "*";
            default: assert(false && "unknown binary operator");
        }
    }

    AffectationExpression::AffectationExpression(std::unique_ptr<Expression>&& a, std::unique_ptr<Expression>&& v):
        affected(std::move(a)),
        value(std::move(v))
    {}

    void AffectationExpression::accept(ExpressionVisitor& v) const {
        v.visit(*this);
    }

    CastExpression::CastExpression(std::unique_ptr<Type>&& t, std::unique_ptr<Expression>&& e):
        type(std::move(t)),
        expression(std::move(e))
    {}

    void CastExpression::accept(ExpressionVisitor& v) const {
        v.visit(*this);
    }

    AccessExpression::AccessExpression(std::unique_ptr<Expression>&& e): expression(std::move(e)) {}

    void AccessExpression::accept(ExpressionVisitor& v) const {
        v.visit(*this);
    }

    CallExpression::CallExpression(const std::string& n): function_name(n) {}

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

    ScalarType::ScalarType(std::size_t size): size_(size) {}

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

    PointerType::PointerType(std::unique_ptr<Type>&& pointed_type, std::size_t size):
        ScalarType(size),
        pointed_type_(std::move(pointed_type))
    {}

    void PointerType::accept(TypeVisitor& v) const {
        v.visit(*this);
    }

    Type* PointerType::pointed_type() const {
        return pointed_type_.get();
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
        
        for(const FunctionArgument& arg : entity.arguments) {
            if(!first_argument) {
                o << ", ";
            }

            o << *arg.type << " " << arg.name;
            first_argument = false;
        }

        o << ") {" << std::endl;

        for(const std::unique_ptr<Instruction>& instr : entity.instructions) {
            o << *instr << std::endl;
        }

        o << "}";
    }

    PrintInstructionVisitor::PrintInstructionVisitor(std::ostream& o): o(o) {}

    void PrintInstructionVisitor::visit(const BlockInstruction& instr) {
        o << "{" << std::endl;

        for(const std::unique_ptr<Instruction>& ins : instr.instructions) {
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
        o << "if (" << *instr.condition << ") ";
        o << instr.true_block;
        o << std::endl << "else ";
        o << instr.false_block;
    }

    void PrintInstructionVisitor::visit(const WhileInstruction& instr) {
        o << "while (" << *instr.condition << ") ";
        o << instr.block;
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

        for(const std::unique_ptr<Expression>& e : expr.arguments) {
            if(!first_argument) {
                o << ", ";
            }

            o << *e << std::endl;
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

        for(const std::unique_ptr<Entity>& entity : prog.entities) {
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
}
}
