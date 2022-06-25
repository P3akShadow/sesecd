%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"
%}

definition = 
add \+
sub -
eq ==
leq <=
le <
geq >=
ge >
mul \*
div div
and &&
or \|\|
paropen \(
parclose \)
listopen \[
listclose \]

comment --.*
whitespace [\t ]+
number  [0-9][0-9]*
negnumber \{-[0-9][0-9]*\}
newline \n

main main
if if
then then
else else
varid [a-z][a-z|A-Z|0-9|\_|\']*

guard \n[\t ]*\|

invalid .

%%
{definition} {return DEFINITION_TOK;}
{add} {return ADD_TOK;}
{or} {}
{sub} {printf("found sub\n");return SUB_TOK;}
{eq} {}
{leq} {}
{le} {return LE_TOK;}
{geq} {}
{ge} {}
{mul} {printf("found mul\n"); return MUL_TOK;}
{div} {}
{and} {}

{if} {return IF_TOK;}
{else} {return ELSE_TOK;}
{then} {return THEN_TOK;}

{paropen} {return PAROPEN_TOK;}
{parclose} {return PARCLOSE_TOK;}

{listopen} {}
{listclose} {}

{comment} {printf("found comment\n");}
{whitespace} {}

{number} {yylval.number = atoi(yytext); return NUM_TOK;}
{negnumber} {return NUM_TOK;}

{newline} {return NEWLINE_TOK;}

{main} {return MAIN_TOK;}
{varid} {yylval.id = strdup(yytext); return ID_TOK;}

{invalid} {fprintf(stderr, "Error - symbol not found\n");}


%%

int yywrap(void){return 1;}