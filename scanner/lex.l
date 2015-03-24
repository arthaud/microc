%filenames = scanner
%implementation-header = "scanner_impl.hpp"
%namespace = microc

%%

[ \r\n\t]+
\/\/[^\n]*\n
\/\*([^\*]|(\*+[^\*\/]))*\*+\/
\(                                  return Parser::OPAR;
\)                                  return Parser::CPAR;
\{                                  return Parser::OCBRA;
\}                                  return Parser::CCBRA;
,                                   return Parser::COMMA;
\;                                  return Parser::SEMICOLON;
=                                   return Parser::AFFECT;
export                              return Parser::EXPORT;
if                                  return Parser::IF;
else                                return Parser::ELSE;
while                               return Parser::WHILE;
for                                 return Parser::FOR;
register                            return Parser::REGISTER;
struct                              return Parser::STRUCT;
void                                return Parser::VOID;
asm                                 return Parser::ASM;
int                                 return Parser::INT;
char                                return Parser::CHAR;
bool                                return Parser::BOOL;
true                                return Parser::TRUE;
false                               return Parser::FALSE;
return                              return Parser::RETURN;
break                               return Parser::BREAK;
continue                            return Parser::CONTINUE;
NULL                                return Parser::NULL_t;
sizeof                              return Parser::SIZEOF;
\.                                  return Parser::DOT;
\-\>                                return Parser::ARROW;
\<                                  return Parser::INF;
\<=                                 return Parser::INFEQ;
\>                                  return Parser::SUP;
\>=                                 return Parser::SUPEQ;
==                                  return Parser::EQ;
\!=                                 return Parser::NEQ;
\+                                  return Parser::PLUS;
\-                                  return Parser::MINUS;
\|\|                                return Parser::OR;
\&\&                                return Parser::AND;
\|                                  return Parser::BIT_OR;
\^                                  return Parser::BIT_XOR;
\&                                  return Parser::BIT_AND;
\<\<                                return Parser::LSHIFT;
\>\>                                return Parser::RSHIFT;
\*                                  return Parser::MULT;
\/                                  return Parser::DIV;
\%                                  return Parser::MOD;
\!                                  return Parser::NOT;
\~                                  return Parser::BIT_NOT;
\:                                  return Parser::COLON;
\[                                  return Parser::OSBRA;
\]                                  return Parser::CSBRA;
(0x[0-9a-fA-F]+)|(0b[01]+)|([0-9]+) return Parser::INTEGER;
\'([^\']|(\\[0nrt\']))\'            return Parser::CHARACTER;
\"((\\.)|[^\"])*\"                  return Parser::STRING;
[a-z_][_0-9A-Za-z]*                 return Parser::IDENT;
.|\n                                throw scanner_exception(lineNr(), matched());
