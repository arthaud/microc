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
      ENTITY: std::unique_ptr<ast::AssemblyEntity>;
      STRING: std::string;

%type <PROGRAM> prog
%type <ENTITY> entity
%type <STRING> string

%%

prog
  :             {}
  | prog entity { d_prog.entities.push_back(std::move($2)); }
;

entity
  : ASM OPAR string CPAR SEMICOLON { $$ = std::make_unique<ast::AssemblyEntity>($3); }
;

string
  : STRING { $$ = sanitizeStringToken(d_scanner.matched()); }
;
