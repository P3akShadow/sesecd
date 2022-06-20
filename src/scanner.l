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
negnumber -[\t ]*[0-9][0-9]*
newline \n

main main
varid [a-z][a-z|A-Z|0-9|\_|\']*

guard \n[\t ]*\|

invalid .

%%
{definition} {return DEFINITION_TOK;}
{add} {return ADD_TOK;}
{or} {}
{sub} {}
{eq} {}
{leq} {}
{le} {}
{geq} {}
{ge} {}
{mul} {}
{div} {}
{and} {}

{paropen} {return PAROPEN_TOK;}
{parclose} {return PARCLOSE_TOK;}

{listopen} {}
{listclose} {}

{comment} {}
{whitespace} {}

{number} {yylval.number = atoi(yytext); return NUM_TOK;}
{negnumber} {return NUM_TOK;}

{newline} {return NEWLINE_TOK;}

{main} {return MAIN_TOK;}
{varid} {yylval.id = strdup(yytext); return ID_TOK;}

{invalid} {fprintf(stderr, "Error - symbol not found\n");}


%%

int yywrap(void){return 1;}