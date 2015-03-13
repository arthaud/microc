#ifndef MICROC_AST_HPP
#define MICROC_AST_HPP

#include <memory>
#include <string>
#include <vector>
#include <ostream>

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
            std::vector< std::unique_ptr<Entity> > entities;
    };

    class Entity {
        public:
            virtual ~Entity() = 0;
            virtual void accept(EntityVisitor& v) const = 0;
    };

    class AssemblyEntity : public Entity {
        public:
            AssemblyEntity(const std::string&);
            virtual void accept(EntityVisitor& v) const;

        public:
            std::string assembly;
    };

    class GlobalEntity : public Entity {
        public:
            GlobalEntity(std::unique_ptr<Type>, const std::string&);
            virtual void accept(EntityVisitor& v) const;

        public:
            std::unique_ptr<Type> type;
            std::string name;
    };

    class FunctionArgument {
        public:
            std::string name;
            std::unique_ptr<Type> type;
    };

    class FunctionEntity : public Entity {
        public:
            FunctionEntity(std::unique_ptr<Type>, const std::string&);
            virtual void accept(EntityVisitor& v) const;

        public:
            std::unique_ptr<Type> return_type;
            std::string name;
            std::vector<FunctionArgument> arguments;
            std::vector< std::unique_ptr<Instruction> > instructions;
    };

    /*
     * Instruction definitions
     */
    class Instruction {
        public:
            virtual ~Instruction() = 0;
            virtual void accept(InstructionVisitor& v) const = 0;
    };

    class BlockInstruction : public Instruction {
        public:
            virtual void accept(InstructionVisitor& v) const;

        public:
            std::vector< std::unique_ptr<Instruction> > instructions;
    };

    class DeclarationInstruction : public Instruction {
        public:
            DeclarationInstruction(std::unique_ptr<Type>, const std::string&, std::unique_ptr<Expression>);
            virtual void accept(InstructionVisitor& v) const;

        public:
            std::unique_ptr<Type> type;
            std::string name;
            std::unique_ptr<Expression> expression;
    };

    class ExpressionInstruction : public Instruction {
        public:
            ExpressionInstruction(std::unique_ptr<Expression>);
            virtual void accept(InstructionVisitor& v) const;

        public:
            std::unique_ptr<Expression> expression;
    };

    class IfInstruction : public Instruction {
        public:
            IfInstruction(std::unique_ptr<Expression>);
            virtual void accept(InstructionVisitor& v) const;

        public:
            std::unique_ptr<Expression> condition;
            BlockInstruction true_block;
            BlockInstruction false_block;
    };

    class WhileInstruction : public Instruction {
        public:
            WhileInstruction(std::unique_ptr<Expression>);
            virtual void accept(InstructionVisitor& v) const;

        public:
            std::unique_ptr<Expression> condition;
            BlockInstruction block;
    };

    class ReturnInstruction : public Instruction {
        public:
            ReturnInstruction(std::unique_ptr<Expression>);
            virtual void accept(InstructionVisitor& v) const;

        public:
            std::unique_ptr<Expression> expression;
    };

    class AssemblyInstruction : public Instruction {
        public:
            AssemblyInstruction(const std::string&);
            virtual void accept(InstructionVisitor& v) const;

        public:
            std::string assembly;
    };

    /*
     * Expression definitions
     */
    class Expression {
        public:
            virtual ~Expression() = 0;
            virtual void accept(ExpressionVisitor& v) const = 0;
    };

    class IdentExpression : public Expression {
        public:
            IdentExpression(const std::string&);
            virtual void accept(ExpressionVisitor& v) const;

        public:
            std::string name;
    };

    template<typename T>
    class ValueExpression : public Expression {
        public:
            ValueExpression(const T& v): value(v) {}
            virtual void accept(ExpressionVisitor& v) const;

        public:
            T value;
    };

    typedef ValueExpression<int> IntegerExpression;
    typedef ValueExpression<char> CharExpression;
    typedef ValueExpression<std::string> StringExpression;

    class TrueExpression : public Expression {
        public:
            virtual void accept(ExpressionVisitor& v) const;
    };

    class FalseExpression : public Expression {
        public:
            virtual void accept(ExpressionVisitor& v) const;
    };

    class NullExpression : public Expression {
        public:
            virtual void accept(ExpressionVisitor& v) const;
    };

    class UnaryExpression : public Expression {
        public:
            enum class Operator {
                Plus,
                Minus,
                Not
            };

            static const char* operator_cstr(Operator op);

        public:
            UnaryExpression(Operator, std::unique_ptr<Expression>);
            virtual void accept(ExpressionVisitor& v) const;

        public:
            Operator op;
            std::unique_ptr<Expression> expression;
    };

    class BinaryExpression : public Expression {
        public:
            enum class Operator {
                Add,
                Sub,
                Mul
            };

            static const char* operator_cstr(Operator op);

        public:
            BinaryExpression(Operator, std::unique_ptr<Expression>, std::unique_ptr<Expression>);
            virtual void accept(ExpressionVisitor& v) const;

        public:
            Operator op;
            std::unique_ptr<Expression> left;
            std::unique_ptr<Expression> right;
    };

    class AffectationExpression : public Expression {
        public:
            AffectationExpression(std::unique_ptr<Expression>, std::unique_ptr<Expression>);
            virtual void accept(ExpressionVisitor& v) const;

        public:
            std::unique_ptr<Expression> affected;
            std::unique_ptr<Expression> value;
    };

    class CastExpression : public Expression {
        public:
            CastExpression(std::unique_ptr<Type>, std::unique_ptr<Expression>);
            virtual void accept(ExpressionVisitor& v) const;

        public:
            std::unique_ptr<Type> type;
            std::unique_ptr<Expression> expression;
    };

    class AccessExpression : public Expression {
        public:
            AccessExpression(std::unique_ptr<Expression>);
            virtual void accept(ExpressionVisitor& v) const;

        public:
            std::unique_ptr<Expression> expression;
    };

    class CallExpression : public Expression {
        public:
            CallExpression(const std::string&);
            virtual void accept(ExpressionVisitor& v) const;

        public:
            std::string function_name;
            std::vector< std::unique_ptr<Expression> > arguments;
    };

    /*
     * Types definitions
     */

    class Type {
        public:
            virtual ~Type() = 0;
            virtual void accept(TypeVisitor& v) const = 0;
            virtual std::size_t getSize() const = 0;
    };

    class VoidType : public Type {
        public:
            virtual std::size_t getSize() const;
            virtual void accept(TypeVisitor& v) const;
    };

    class ScalarType : public Type {
        public:
            ScalarType(std::size_t size);
            virtual std::size_t getSize() const;

        private:
            std::size_t size;
    };

    class IntegerType : public ScalarType {
        public:
            using ScalarType::ScalarType;
            virtual void accept(TypeVisitor& v) const;
    };

    class BooleanType : public ScalarType {
        public:
            using ScalarType::ScalarType;
            virtual void accept(TypeVisitor& v) const;
    };

    class CharType : public ScalarType {
        public:
            using ScalarType::ScalarType;
            virtual void accept(TypeVisitor& v) const;
    };

    class NullType : public ScalarType {
        public:
            using ScalarType::ScalarType;
            virtual void accept(TypeVisitor& v) const;
    };

    class PointerType : public ScalarType {
        public:
            PointerType(std::unique_ptr<Type> pointedType, std::size_t size);
            virtual void accept(TypeVisitor& v) const;
            Type* getPointedType() const;

        private:
            std::unique_ptr<Type> pointedType;
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
            PrintEntityVisitor(std::ostream& o);
            void visit(const AssemblyEntity&);
            void visit(const GlobalEntity&);
            void visit(const FunctionEntity&);

        private:
            std::ostream& o;
    };

    class PrintInstructionVisitor : public InstructionVisitor {
        public:
            PrintInstructionVisitor(std::ostream& o);
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
            PrintExpressionVisitor(std::ostream& o);
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
            PrintTypeVisitor(std::ostream& o);
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
}
}

#endif // MICROC_AST_HPP
