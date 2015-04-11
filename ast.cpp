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
        case UnaryOperator::Plus:   return "+";
        case UnaryOperator::Minus:  return "-";
        case UnaryOperator::Not:    return "!";
        case UnaryOperator::BitNot: return "~";
        default: assert(false && "unknown unary operator");
    }
}

void BinaryExpression::accept(ExpressionVisitor& v) const {
    v.visit(*this);
}

const char* BinaryExpression::operator_str(Operator op) {
    switch(op) {
        case BinaryOperator::Add:    return "+";
        case BinaryOperator::Sub:    return "-";
        case BinaryOperator::Mul:    return "*";
        case BinaryOperator::Div:    return "/";
        case BinaryOperator::Mod:    return "%";
        case BinaryOperator::Or:     return "||";
        case BinaryOperator::And:    return "&&";
        case BinaryOperator::BitOr:  return "|";
        case BinaryOperator::BitAnd: return "&";
        case BinaryOperator::BitXor: return "^";
        case BinaryOperator::Eq:     return "==";
        case BinaryOperator::Neq:    return "!=";
        case BinaryOperator::Inf:    return "<";
        case BinaryOperator::InfEq:  return "<=";
        case BinaryOperator::Sup:    return ">";
        case BinaryOperator::SupEq:  return ">=";
        case BinaryOperator::Lshift: return "<<";
        case BinaryOperator::Rshift: return ">>";
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

std::unique_ptr<Type> VoidType::clone() const {
    return std::make_unique<VoidType>();
}

std::size_t ScalarType::size() const {
    return size_;
}

void IntegerType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

std::unique_ptr<Type> IntegerType::clone() const {
    return std::make_unique<IntegerType>(size_);
}

void BooleanType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

std::unique_ptr<Type> BooleanType::clone() const {
    return std::make_unique<BooleanType>(size_);
}

void CharType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

std::unique_ptr<Type> CharType::clone() const {
    return std::make_unique<CharType>(size_);
}

void NullType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

std::unique_ptr<Type> NullType::clone() const {
    return std::make_unique<NullType>(size_);
}

void PointerType::accept(TypeVisitor& v) const {
    v.visit(*this);
}

std::unique_ptr<Type> PointerType::clone() const {
    return std::make_unique<PointerType>(pointed_type_->clone(), size_);
}

/*
 * operator<<
 */
class PrintEntityVisitor : public EntityVisitor {
    public:
        explicit PrintEntityVisitor(std::ostream& o): o(o) {}

        void visit(const AssemblyEntity& entity) {
            o << "asm(\"" << entity.assembly << "\");";
        }

        void visit(const GlobalEntity& entity) {
            o << *entity.type << " " << entity.name << ";";
        }

        void visit(const FunctionEntity& entity) {
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

    private:
        std::ostream& o;
};

class PrintInstructionVisitor : public InstructionVisitor {
    public:
        explicit PrintInstructionVisitor(std::ostream& o): o(o) {}

        virtual void visit(const BlockInstruction& instr) {
            o << "{" << std::endl;

            for(const auto& ins : instr.instructions) {
                o << *ins << std::endl;
            }

            o << "}";
        }

        virtual void visit(const DeclarationInstruction& instr) {
            o << *instr.type << " " << instr.name;

            if(instr.expression != nullptr) {
                o << " = " << *instr.expression;
            }

            o << ";";
        }

        virtual void visit(const ExpressionInstruction& instr) {
            o << *instr.expression << ";";
        }

        virtual void visit(const IfInstruction& instr) {
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

        virtual void visit(const WhileInstruction& instr) {
            o << "while (" << *instr.condition << ") {" << std::endl;

            for(const auto& ins : instr.instructions) {
                o << *ins << std::endl;
            }

            o << "}";
        }

        virtual void visit(const ReturnInstruction& instr) {
            o << "return " << *instr.expression << ";";
        }

        virtual void visit(const AssemblyInstruction& instr) {
            o << "asm(\"" << instr.assembly << "\");";
        }

    private:
        std::ostream& o;
};

class PrintExpressionVisitor : public ExpressionVisitor {
    public:
        explicit PrintExpressionVisitor(std::ostream& o): o(o) {}

        virtual void visit(const IdentExpression& expr) {
            o << expr.name;
        }

        virtual void visit(const IntegerExpression& expr) {
            o << expr.value;
        }

        virtual void visit(const CharExpression& expr) {
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

        virtual void visit(const StringExpression& expr) {
            o << '"' << expr.value << '"';
        }

        virtual void visit(const TrueExpression&) {
            o << "true";
        }

        virtual void visit(const FalseExpression&) {
            o << "false";
        }

        virtual void visit(const NullExpression&) {
            o << "NULL";
        }

        virtual void visit(const UnaryExpression& expr) {
            o << UnaryExpression::operator_str(expr.op);
            o << '(' << *expr.expression << ')';
        }

        virtual void visit(const BinaryExpression& expr) {
            o << '(' << *expr.left << ')';
            o << BinaryExpression::operator_str(expr.op);
            o << '(' << *expr.right << ')';
        }

        virtual void visit(const AffectationExpression& expr) {
            o << *expr.affected << " = " << *expr.value;
        }

        virtual void visit(const CastExpression& expr) {
            o << "(" << *expr.type << ") " << *expr.expression;
        }

        virtual void visit(const AccessExpression& expr) {
            o << "*(" << *expr.expression << ")";
        }

        virtual void visit(const CallExpression& expr) {
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

    private:
        std::ostream& o;
};

class PrintTypeVisitor : public TypeVisitor {
    public:
        explicit PrintTypeVisitor(std::ostream& o): o(o) {}

        virtual void visit(const VoidType&) {
            o << "void";
        }

        virtual void visit(const IntegerType&) {
            o << "int";
        }

        virtual void visit(const BooleanType&) {
            o << "bool";
        }

        virtual void visit(const CharType&) {
            o << "char";
        }

        virtual void visit(const NullType&) {
            o << "null";
        }

        virtual void visit(const PointerType& type) {
            o << *type.pointed_type() << '*';
        }

    private:
        std::ostream& o;
};

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
