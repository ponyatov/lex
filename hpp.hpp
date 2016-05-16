#ifndef _H_LEX
#define _H_LEX

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym {
	string tag,val;
	Sym(string,string); Sym(string);
	vector<Sym*> nest; void push(Sym*);
	vector<string> pars; void par(Sym*);
	virtual string tagval(); string tagstr(); string pad(int);
	virtual string dump(int=0);
	virtual Sym* eval();
	virtual Sym* eq(Sym*);
	virtual Sym* at(Sym*);
	virtual Sym* add(Sym*);
};
extern map<string,Sym*> env;
extern void env_init();

struct Error: Sym { Error(string); };

struct Str: Sym { Str(string); string tagval(); Sym*add(Sym*); };

struct List: Sym { List(); };

struct Op: Sym { Op(string); Sym*eval(); };
typedef Sym* (*FN)(Sym*);
struct Fn: Sym { Fn(string,FN); FN fn; Sym*at(Sym*); };
struct Lambda: Sym { Lambda(); };

struct Dep: Sym { Dep(Sym*,Sym*,Sym*); };

struct File: Sym { File(string); ~File(); FILE *fh; static Sym*file(Sym*); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_LEX
