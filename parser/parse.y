%filenames              parser
%implementation-header  parser_impl.hpp
%scanner-token-function d_scanner.lex()
%namespace              microc

%token OPAR CPAR OCBRA CCBRA COMMA SEMICOLON AFFECT EXPORT IF ELSE WHILE FOR
      REGISTER STRUCT VOID ASM INT CHAR BOOL TRUE FALSE RETURN BREAK CONTINUE
      NULL_t SIZEOF DOT ARROW INF INFEQ SUP SUPEQ EQ NEQ PLUS MINUS OR AND
      BIT_OR BIT_XOR BIT_XAND LSHIFT RSHIFT MULT DIV MOD NOT BIT_NOT COLON
      OSBRA CSBRA INTEGER CHARACTER STRING IDENT

%polymorphic
      PROGRAM: ast::Program;
      ENTITY: std::unique_ptr<ast::Entity>;
      PARAMETERS: std::vector<ast::FunctionArgument>;
      INSTRUCTIONS: std::vector<std::unique_ptr<ast::Instruction>>;
      INSTRUCTION: std::unique_ptr<ast::Instruction>;
      TYPE: std::unique_ptr<ast::Type>;
      STRING: std::string;

%type <PROGRAM> prog
%type <ENTITY> entity
%type <PARAMETERS> parameters, parameters_end
%type <INSTRUCTIONS> instructions
%type <INSTRUCTION> instruction
%type <TYPE> type
%type <STRING> string, ident

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
  | ASM OPAR string CPAR SEMICOLON
      { $$ = std::make_unique<ast::AssemblyInstruction>($3); }
  | type ident SEMICOLON
      { $$ = std::make_unique<ast::DeclarationInstruction>(std::move($1), $2, nullptr); }
;

type
  : VOID      { $$ = std::make_unique<ast::VoidType>(); }
  | INT       { $$ = std::make_unique<ast::IntegerType>(1); }
  | BOOL      { $$ = std::make_unique<ast::BooleanType>(1); }
  | CHAR      { $$ = std::make_unique<ast::CharType>(1); }
  | type MULT { $$ = std::make_unique<ast::PointerType>(std::move($1), 1); }
;

string
  : STRING { $$ = sanitizeStringToken(d_scanner.matched()); }
;

ident
  : IDENT { $$ = d_scanner.matched(); }
;
