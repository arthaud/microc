%filenames              parser
%implementation-header  parser_impl.hpp
%scanner-token-function d_scanner.lex()
%namespace              microc

%token OPAR CPAR OCBRA CCBRA COMMA SEMICOLON EXPORT IF ELSE WHILE FOR
      REGISTER STRUCT VOID ASM INT CHAR BOOL TRUE FALSE RETURN BREAK CONTINUE
      NULL_t SIZEOF DOT ARROW COLON OSBRA CSBRA INTEGER CHARACTER STRING IDENT

%polymorphic
      PROGRAM: ast::Program;
      ENTITY: std::unique_ptr<ast::Entity>;
      PARAMETERS: std::vector<ast::FunctionArgument>;
      INSTRUCTIONS: std::vector<std::unique_ptr<ast::Instruction>>;
      INSTRUCTION: std::unique_ptr<ast::Instruction>;
      EXPRESSION: std::unique_ptr<ast::Expression>;
      ARGUMENTS: std::vector<std::unique_ptr<ast::Expression>>;
      TYPE: std::unique_ptr<ast::Type>;
      INTEGER: int;
      CHAR: char;
      STRING: std::string;

%type <PROGRAM> prog
%type <ENTITY> entity
%type <PARAMETERS> parameters, parameters_end
%type <INSTRUCTIONS> instructions, else
%type <INSTRUCTION> instruction
%type <EXPRESSION> expression
%type <ARGUMENTS> arguments, arguments_end
%type <TYPE> type
%type <INTEGER> integer
%type <CHAR> character
%type <STRING> string, ident

%right AFFECT
%left OR
%left AND
%left BIT_OR
%left BIT_XOR
%left BIT_AND
%left EQ NEQ
%left INF INFEQ SUP SUPEQ
%left LSHIFT RSHIFT
%left PLUS MINUS
%left MULT DIV MOD
%right NOT BIT_NOT

%%

prog
  :             {}
  | prog entity { d_prog.entities.push_back(std::move($2)); }
;

entity
  : ASM OPAR string CPAR SEMICOLON
      { $$ = std::make_unique<ast::AssemblyEntity>($3); }
  | type ident SEMICOLON
      { $$ = std::make_unique<ast::GlobalEntity>(std::move($1), $2); }
  | type ident OPAR parameters CPAR OCBRA instructions CCBRA
      {
        auto f = std::make_unique<ast::FunctionEntity>(std::move($1), $2);
        f->arguments = std::move($4);
        f->instructions = std::move($7);
        $$ = std::move(f);
      }
;

parameters
  :   { $$ = std::vector<ast::FunctionArgument>(); }
  | type ident parameters_end
      {
        $<PARAMETERS>$ = std::move($3);
        $<PARAMETERS>$.emplace($<PARAMETERS>$.begin(), std::move($1), $2);
      }
;

parameters_end
  :   { $$ = std::vector<ast::FunctionArgument>(); }
  | COMMA type ident parameters_end
      {
        $$ = std::move($4);
        $<PARAMETERS>$.emplace($<PARAMETERS>$.begin(), std::move($2), $3);
      }
;

instructions
  :   { $$ = std::vector<std::unique_ptr<ast::Instruction>>(); }
  | instruction instructions
      {
        $<INSTRUCTIONS>$ = std::move($2);
        $<INSTRUCTIONS>$.emplace($<INSTRUCTIONS>$.begin(), std::move($1));
      }
;

instruction
  : OCBRA instructions CCBRA
      { $$ = std::make_unique<ast::BlockInstruction>(std::move($2)); }
  | type ident SEMICOLON
      { $$ = std::make_unique<ast::DeclarationInstruction>(std::move($1), $2, nullptr); }
  | type ident AFFECT expression SEMICOLON
      { $$ = std::make_unique<ast::DeclarationInstruction>(std::move($1), $2, std::move($4)); }
  | expression SEMICOLON
      { $$ = std::make_unique<ast::ExpressionInstruction>(std::move($1)); }
  | IF OPAR expression CPAR OCBRA instructions CCBRA else
      {
        auto e = std::make_unique<ast::IfInstruction>(std::move($3));
        e->true_instrs = std::move($6);
        e->false_instrs = std::move($8);
        $$ = std::move(e);
      }
  | WHILE OPAR expression CPAR OCBRA instructions CCBRA
      {
        auto e = std::make_unique<ast::WhileInstruction>(std::move($3));
        e->instructions = std::move($6);
        $$ = std::move(e);
      }
  | RETURN expression SEMICOLON
      { $$ = std::make_unique<ast::ReturnInstruction>(std::move($2)); }
  | ASM OPAR string CPAR SEMICOLON
      { $$ = std::make_unique<ast::AssemblyInstruction>($3); }
;

else
  :   { $$ = std::vector<std::unique_ptr<ast::Instruction>>(); }
  | ELSE IF OPAR expression CPAR OCBRA instructions CCBRA else
      {
        auto e = std::make_unique<ast::IfInstruction>(std::move($4));
        e->true_instrs = std::move($7);
        e->false_instrs = std::move($9);
        $$ = std::vector<std::unique_ptr<ast::Instruction>>();
        $<INSTRUCTIONS>$.push_back(std::move(e));
      }
  | ELSE OCBRA instructions CCBRA
      { $$ = std::move($3); }
;

expression
  : expression AFFECT expression
      { $$ = std::make_unique<ast::AffectationExpression>(std::move($1), std::move($3)); }
  | expression OR expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Or, std::move($1), std::move($3)); }
  | expression AND expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::And, std::move($1), std::move($3)); }
  | expression BIT_OR expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::BitOr, std::move($1), std::move($3)); }
  | expression BIT_XOR expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::BitXor, std::move($1), std::move($3)); }
  | expression BIT_AND expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::BitAnd, std::move($1), std::move($3)); }
  | expression EQ expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Eq, std::move($1), std::move($3)); }
  | expression NEQ expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Neq, std::move($1), std::move($3)); }
  | expression INF expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Inf, std::move($1), std::move($3)); }
  | expression INFEQ expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Inf, std::move($1), std::move($3)); }
  | expression INF expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::InfEq, std::move($1), std::move($3)); }
  | expression SUP expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Sup, std::move($1), std::move($3)); }
  | expression SUPEQ expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::SupEq, std::move($1), std::move($3)); }
  | expression LSHIFT expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Lshift, std::move($1), std::move($3)); }
  | expression RSHIFT expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Rshift, std::move($1), std::move($3)); }
  | expression PLUS expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Add, std::move($1), std::move($3)); }
  | expression MINUS expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Sub, std::move($1), std::move($3)); }
  | expression MULT expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Mul, std::move($1), std::move($3)); }
  | expression DIV expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Div, std::move($1), std::move($3)); }
  | expression MOD expression
      { $$ = std::make_unique<ast::BinaryExpression>(ast::BinaryOperator::Mod, std::move($1), std::move($3)); }
  | PLUS expression %prec NOT
      { $$ = std::make_unique<ast::UnaryExpression>(ast::UnaryOperator::Plus, std::move($2)); }
  | MINUS expression %prec NOT
      { $$ = std::make_unique<ast::UnaryExpression>(ast::UnaryOperator::Minus, std::move($2)); }
  | NOT expression
      { $$ = std::make_unique<ast::UnaryExpression>(ast::UnaryOperator::Not, std::move($2)); }
  | BIT_NOT expression
      { $$ = std::make_unique<ast::UnaryExpression>(ast::UnaryOperator::BitNot, std::move($2)); }
  | MULT expression %prec NOT
      { $$ = std::make_unique<ast::AccessExpression>(std::move($2)); }
  | OPAR type CPAR expression %prec NOT
      { $$ = std::make_unique<ast::CastExpression>(std::move($2), std::move($4)); }
  | OPAR expression CPAR
      { $$ = std::move($2); }
  | ident OPAR arguments CPAR %prec NOT
      {
        auto e = std::make_unique<ast::CallExpression>($1);
        e->arguments = std::move($3);
        $$ = std::move(e);
      }
  | ident     { $$ = std::make_unique<ast::IdentExpression>($1); }
  | integer   { $$ = std::make_unique<ast::IntegerExpression>($1); }
  | character { $$ = std::make_unique<ast::CharExpression>($1); }
  | string    { $$ = std::make_unique<ast::StringExpression>($1); }
  | NULL_t    { $$ = std::make_unique<ast::NullExpression>(); }
  | TRUE      { $$ = std::make_unique<ast::TrueExpression>(); }
  | FALSE     { $$ = std::make_unique<ast::FalseExpression>(); }
;

arguments
  :   { $$ = std::vector<std::unique_ptr<ast::Expression>>(); }
  | expression arguments_end
      {
        $<ARGUMENTS>$ = std::move($2);
        $<ARGUMENTS>$.emplace($<ARGUMENTS>$.begin(), std::move($1));
      }
;

arguments_end
  :   { $$ = std::vector<std::unique_ptr<ast::Expression>>(); }
  | COMMA expression arguments_end
      {
        $<ARGUMENTS>$ = std::move($3);
        $<ARGUMENTS>$.emplace($<ARGUMENTS>$.begin(), std::move($2));
      }
;

type
  : VOID      { $$ = std::make_unique<ast::VoidType>(); }
  | INT       { $$ = std::make_unique<ast::IntegerType>(1); }
  | BOOL      { $$ = std::make_unique<ast::BooleanType>(1); }
  | CHAR      { $$ = std::make_unique<ast::CharType>(1); }
  | type MULT { $$ = std::make_unique<ast::PointerType>(std::move($1), 1); }
;

integer
  : INTEGER { $$ = sanitizeIntegerToken(d_scanner.matched()); }
;

character
  : CHARACTER { $$ = sanitizeCharacterToken(d_scanner.matched()); }
;

string
  : STRING { $$ = sanitizeStringToken(d_scanner.matched()); }
;

ident
  : IDENT { $$ = d_scanner.matched(); }
;
