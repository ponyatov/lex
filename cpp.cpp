#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { env_init(); return yyparse(); }

Sym::Sym(string T,string V) { tag=T; val=V; }
Sym::Sym(string V):Sym("",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }
void Sym::par(Sym*o) { pars[o->val]=o; }

string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::tagval() { return "<"+tag+":"+val+">"; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+tagval();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym *E = env[val]; if (E) return E; // lookup
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval();
	return this;
}

Str::Str(string V):Sym("str",V) {}
string Str::tagval() { return tagstr(); }
string Sym::tagstr() { return "'"+val+"'"; }

List::List():Sym("[","]") {}

Op::Op(string V):Sym("op",V) {}

Lambda::Lambda():Sym("^","^") {}

Dep::Dep(Sym*A,Sym*B,Sym*C):Sym("dep","") { push(A); push(B); push(C); }

map<string,Sym*> env;
void env_init() {}
