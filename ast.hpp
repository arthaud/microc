#ifndef MICROC_AST_HPP
#define MICROC_AST_HPP

#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace microc {
namespace ast {

class Entity;
class Instruction;
class Expression;
class Type;

class EntityVisitor;
class InstructionVisitor;
class ExpressionVisitor;
class TypeVisitor;

class Program {
    public:
        std::vector<std::unique_ptr<Entity>> entities;
};

class Entity {
    public:
        virtual ~Entity() = 0;
        virtual void accept(EntityVisitor&) const = 0;
};

class AssemblyEntity : public Entity {
    public:
        explicit AssemblyEntity(const std::string& s): assembly(s) {}
        virtual void accept(EntityVisitor&) const;

    public:
        std::string assembly;
};

class GlobalEntity : public Entity {
    public:
        GlobalEntity(std::unique_ptr<Type>&& type, const std::string& name):
            type(std::move(type)),
            name(name)
        {}

        virtual void accept(EntityVisitor&) const;

    public:
        std::unique_ptr<Type> type;
        std::string name;
};

class FunctionArgument {
    public:
        FunctionArgument(std::unique_ptr<Type>&& type, const std::string& name):
            type(std::move(type)),
            name(name)
        {}

        FunctionArgument(FunctionArgument&&) = default;
        FunctionArgument& operator=(FunctionArgument&&) = default;

    public:
        std::unique_ptr<Type> type;
        std::string name;
};

class FunctionEntity : public Entity {
    public:
        FunctionEntity(std::unique_ptr<Type>&& return_type, const std::string& name):
            return_type(std::move(return_type)),
            name(name)
        {}

        virtual void accept(EntityVisitor&) const;

    public:
        std::unique_ptr<Type> return_type;
        std::string name;
        std::vector<FunctionArgument> arguments;
        std::vector<std::unique_ptr<Instruction>> instructions;
};

/*
 * Instruction definitions
 */
class Instruction {
    public:
        virtual ~Instruction() = 0;
        virtual void accept(InstructionVisitor&) const = 0;
};

class BlockInstruction : public Instruction {
    public:
        explicit BlockInstruction() = default;
        explicit BlockInstruction(std::vector<std::unique_ptr<Instruction>>&& instructions):
            instructions(std::move(instructions))
        {}

        virtual void accept(InstructionVisitor&) const;

    public:
        std::vector<std::unique_ptr<Instruction>> instructions;
};

class DeclarationInstruction : public Instruction {
    public:
        DeclarationInstruction(std::unique_ptr<Type>&& type, const std::string& name, std::unique_ptr<Expression>&& expression):
            type(std::move(type)),
            name(name),
            expression(std::move(expression))
        {}

        virtual void accept(InstructionVisitor&) const;

    public:
        std::unique_ptr<Type> type;
        std::string name;
        std::unique_ptr<Expression> expression;
};

class ExpressionInstruction : public Instruction {
    public:
        explicit ExpressionInstruction(std::unique_ptr<Expression>&& expression):
            expression(std::move(expression))
        {}

        virtual void accept(InstructionVisitor&) const;

    public:
        std::unique_ptr<Expression> expression;
};

class IfInstruction : public Instruction {
    public:
        explicit IfInstruction(std::unique_ptr<Expression>&& cond):
            condition(std::move(cond))
        {}

        virtual void accept(InstructionVisitor&) const;

    public:
        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Instruction>> true_instrs;
        std::vector<std::unique_ptr<Instruction>> false_instrs;
};

class WhileInstruction : public Instruction {
    public:
        explicit WhileInstruction(std::unique_ptr<Expression>&& cond):
            condition(std::move(cond))
        {}

        virtual void accept(InstructionVisitor&) const;

    public:
        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Instruction>> instructions;
};

class ReturnInstruction : public Instruction {
    public:
        explicit ReturnInstruction(std::unique_ptr<Expression>&& expression):
            expression(std::move(expression))
        {}

        virtual void accept(InstructionVisitor&) const;

    public:
        std::unique_ptr<Expression> expression;
};

class AssemblyInstruction : public Instruction {
    public:
        explicit AssemblyInstruction(const std::string& a): assembly(a) {}
        virtual void accept(InstructionVisitor&) const;

    public:
        std::string assembly;
};

/*
 * Expression definitions
 */
class Expression {
    public:
        virtual ~Expression() = 0;
        virtual void accept(ExpressionVisitor&) const = 0;
};

class IdentExpression : public Expression {
    public:
        explicit IdentExpression(const std::string& name): name(name) {}
        virtual void accept(ExpressionVisitor&) const;

    public:
        std::string name;
};

template<typename T>
class ValueExpression : public Expression {
    public:
        explicit ValueExpression(const T& value): value(value) {}
        virtual void accept(ExpressionVisitor&) const;

    public:
        T value;
};

typedef ValueExpression<int> IntegerExpression;
typedef ValueExpression<char> CharExpression;
typedef ValueExpression<std::string> StringExpression;

class TrueExpression : public Expression {
    public:
        virtual void accept(ExpressionVisitor&) const;
};

class FalseExpression : public Expression {
    public:
        virtual void accept(ExpressionVisitor&) const;
};

class NullExpression : public Expression {
    public:
        virtual void accept(ExpressionVisitor&) const;
};

class UnaryExpression : public Expression {
    public:
        enum class Operator {
            Plus,
            Minus,
            Not,
            BitNot,
        };

        static const char* operator_str(Operator op);

    public:
        UnaryExpression(Operator op, std::unique_ptr<Expression>&& expression):
            op(op),
            expression(std::move(expression))
        {}

        virtual void accept(ExpressionVisitor&) const;

    public:
        Operator op;
        std::unique_ptr<Expression> expression;
};

class BinaryExpression : public Expression {
    public:
        enum class Operator {
            Add,
            Sub,
            Mul,
            Div,
            Mod,
            Or,
            And,
            BitOr,
            BitAnd,
            BitXor,
            Eq,
            Neq,
            Inf,
            InfEq,
            Sup,
            SupEq,
            Lshift,
            Rshift,
        };

        static const char* operator_str(Operator op);

    public:
        BinaryExpression(Operator op, std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right):
            op(op),
            left(std::move(left)),
            right(std::move(right))
        {}

        virtual void accept(ExpressionVisitor&) const;

    public:
        Operator op;
        std::unique_ptr<Expression> left;
        std::unique_ptr<Expression> right;
};

class AffectationExpression : public Expression {
    public:
        AffectationExpression(std::unique_ptr<Expression>&& affected, std::unique_ptr<Expression>&& value):
            affected(std::move(affected)),
            value(std::move(value))
        {}

        virtual void accept(ExpressionVisitor&) const;

    public:
        std::unique_ptr<Expression> affected;
        std::unique_ptr<Expression> value;
};

class CastExpression : public Expression {
    public:
        CastExpression(std::unique_ptr<Type>&& type, std::unique_ptr<Expression>&& expression):
            type(std::move(type)),
            expression(std::move(expression))
        {}

        virtual void accept(ExpressionVisitor&) const;

    public:
        std::unique_ptr<Type> type;
        std::unique_ptr<Expression> expression;
};

class AccessExpression : public Expression {
    public:
        explicit AccessExpression(std::unique_ptr<Expression>&& expression):
            expression(std::move(expression))
        {}

        virtual void accept(ExpressionVisitor&) const;

    public:
        std::unique_ptr<Expression> expression;
};

class CallExpression : public Expression {
    public:
        explicit CallExpression(const std::string& name):
            function_name(name)
        {}

        virtual void accept(ExpressionVisitor&) const;

    public:
        std::string function_name;
        std::vector<std::unique_ptr<Expression>> arguments;
};

/*
 * Types definitions
 */

class Type {
    public:
        virtual ~Type() = 0;
        virtual void accept(TypeVisitor& v) const = 0;
        virtual std::size_t size() const = 0;
};

class VoidType : public Type {
    public:
        virtual std::size_t size() const;
        virtual void accept(TypeVisitor&) const;
};

class ScalarType : public Type {
    public:
        explicit ScalarType(std::size_t size): size_(size) {}
        virtual std::size_t size() const;

    private:
        std::size_t size_;
};

class IntegerType : public ScalarType {
    public:
        using ScalarType::ScalarType;
        virtual void accept(TypeVisitor&) const;
};

class BooleanType : public ScalarType {
    public:
        using ScalarType::ScalarType;
        virtual void accept(TypeVisitor&) const;
};

class CharType : public ScalarType {
    public:
        using ScalarType::ScalarType;
        virtual void accept(TypeVisitor&) const;
};

class NullType : public ScalarType {
    public:
        using ScalarType::ScalarType;
        virtual void accept(TypeVisitor&) const;
};

class PointerType : public ScalarType {
    public:
        PointerType(std::unique_ptr<Type>&& pointed_type, std::size_t size):
            ScalarType(size),
            pointed_type_(std::move(pointed_type))
        {}

        virtual void accept(TypeVisitor&) const;
        Type* pointed_type() const { return pointed_type_.get(); }

    private:
        std::unique_ptr<Type> pointed_type_;
};

/*
 * Visitors
 */
class EntityVisitor {
    public:
        virtual void visit(const AssemblyEntity&) = 0;
        virtual void visit(const GlobalEntity&) = 0;
        virtual void visit(const FunctionEntity&) = 0;
};

class InstructionVisitor {
    public:
        virtual void visit(const BlockInstruction&) = 0;
        virtual void visit(const DeclarationInstruction&) = 0;
        virtual void visit(const ExpressionInstruction&) = 0;
        virtual void visit(const IfInstruction&) = 0;
        virtual void visit(const WhileInstruction&) = 0;
        virtual void visit(const ReturnInstruction&) = 0;
        virtual void visit(const AssemblyInstruction&) = 0;
};

class ExpressionVisitor {
    public:
        virtual void visit(const IdentExpression&) = 0;
        virtual void visit(const IntegerExpression&) = 0;
        virtual void visit(const CharExpression&) = 0;
        virtual void visit(const StringExpression&) = 0;
        virtual void visit(const TrueExpression&) = 0;
        virtual void visit(const FalseExpression&) = 0;
        virtual void visit(const NullExpression&) = 0;
        virtual void visit(const UnaryExpression&) = 0;
        virtual void visit(const BinaryExpression&) = 0;
        virtual void visit(const AffectationExpression&) = 0;
        virtual void visit(const CastExpression&) = 0;
        virtual void visit(const AccessExpression&) = 0;
        virtual void visit(const CallExpression&) = 0;
};

class TypeVisitor {
    public:
        virtual void visit(const VoidType&) = 0;
        virtual void visit(const IntegerType&) = 0;
        virtual void visit(const BooleanType&) = 0;
        virtual void visit(const CharType&) = 0;
        virtual void visit(const NullType&) = 0;
        virtual void visit(const PointerType&) = 0;
};

/*
 * operator<<
 */
class PrintEntityVisitor : public EntityVisitor {
    public:
        explicit PrintEntityVisitor(std::ostream&);
        void visit(const AssemblyEntity&);
        void visit(const GlobalEntity&);
        void visit(const FunctionEntity&);

    private:
        std::ostream& o;
};

class PrintInstructionVisitor : public InstructionVisitor {
    public:
        explicit PrintInstructionVisitor(std::ostream&);
        virtual void visit(const BlockInstruction&);
        virtual void visit(const DeclarationInstruction&);
        virtual void visit(const ExpressionInstruction&);
        virtual void visit(const IfInstruction&);
        virtual void visit(const WhileInstruction&);
        virtual void visit(const ReturnInstruction&);
        virtual void visit(const AssemblyInstruction&);

    private:
        std::ostream& o;
};

class PrintExpressionVisitor : public ExpressionVisitor {
    public:
        explicit PrintExpressionVisitor(std::ostream&);
        virtual void visit(const IdentExpression&);
        virtual void visit(const IntegerExpression&);
        virtual void visit(const CharExpression&);
        virtual void visit(const StringExpression&);
        virtual void visit(const TrueExpression&);
        virtual void visit(const FalseExpression&);
        virtual void visit(const NullExpression&);
        virtual void visit(const UnaryExpression&);
        virtual void visit(const BinaryExpression&);
        virtual void visit(const AffectationExpression&);
        virtual void visit(const CastExpression&);
        virtual void visit(const AccessExpression&);
        virtual void visit(const CallExpression&);

    private:
        std::ostream& o;
};

class PrintTypeVisitor : public TypeVisitor {
    public:
        explicit PrintTypeVisitor(std::ostream&);
        virtual void visit(const VoidType&);
        virtual void visit(const IntegerType&);
        virtual void visit(const BooleanType&);
        virtual void visit(const CharType&);
        virtual void visit(const NullType&);
        virtual void visit(const PointerType&);

    private:
        std::ostream& o;
};

std::ostream& operator<<(std::ostream& o, const Program& prog);
std::ostream& operator<<(std::ostream& o, const Entity& entity);
std::ostream& operator<<(std::ostream& o, const Instruction& instr);
std::ostream& operator<<(std::ostream& o, const Expression& expr);
std::ostream& operator<<(std::ostream& o, const Type& type);

} // namespace ast
} // namespace microc

#endif // MICROC_AST_HPP
