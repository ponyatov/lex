#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { env_init(); return yyparse(); }

Sym::Sym(string T,string V) { tag=T; val=V; }
Sym::Sym(string V):Sym("sym",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }
void Sym::par(Sym*o) { pars.push_back(o->val); }

string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::tagval() { return "<"+tag+":"+val+">"; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+tagval();
	for (auto pr=pars.begin(),e=pars.end();pr!=e;pr++)
		S += ","+*pr;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym *E = env[val]; if (E) return E; // lookup
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval();
	return this;
}

Sym* Sym::eq(Sym*o) { env[val]=o ; return this; }
Sym* Sym::at(Sym*o) { return new Error(tagval()+"@"+o->tagval()); }

Sym* Sym::add(Sym*o) { return new Error(tagval()+"+"+o->tagval()); }

Str::Str(string V):Sym("str",V) {}
string Str::tagval() { return tagstr(); }
string Sym::tagstr() { return "<"+tag+":'"+val+"'>"; }
Sym* Str::add(Sym*o) {
	if (o->tag=="str") return new Str(val+o->val);
	else return Sym::add(o); }

List::List():Sym("[","]") {}

Op::Op(string V):Sym("op",V) {}
Sym* Op::eval() {
	if (val=="~") return nest[0]; else Sym::eval();
	if (val=="=") return nest[0]->eq(nest[1]);
	if (val=="@") return nest[0]->at(nest[1]);
	if (val=="+") return nest[0]->add(nest[1]);
	return this;
}

Fn::Fn(string V, FN F):Sym("fn",V) { fn=F; }
Sym* Fn::at(Sym*o) { return fn(o); }

Lambda::Lambda():Sym("^","^") {}

Dep::Dep(Sym*A,Sym*B,Sym*C):Sym("dep","") { push(A); push(B); push(C); }

File::File(string V):Sym("file",V) { fh=NULL; }
File::~File() { if (fh) fclose(fh); }
Sym* File::file(Sym*o) { return new File(o->val); }

Error::Error(string V):Sym("error",V) { yyerror(val); }

map<string,Sym*> env;
void env_init() {
	env["file"] = new Fn("file",File::file);
}
