#ifndef _H_helper
#define _H_helper

#include <string>
#include <map>
using namespace std;

enum fnType { NoArg=0, OneArg, TwoArg };

struct FunctionRec
{
  fnType whatType;
  union
  {
    double (*twoArg)(double, double);
    double (*oneArg)(double);
    double (*noArg)();
  } ptr;
};

class ASTNode
{
 private:
  std::string fnName;
  char whichVar;
  ASTNode * leftChild;
  ASTNode * rightChild;
 public:
  ASTNode (std::string name, ASTNode * left = 0, ASTNode * right = 0, char what='.') :
    fnName(name), whichVar(what), leftChild(left), rightChild(right) { }
  ASTNode (char what) :
    fnName(""), whichVar(what), leftChild(0), rightChild(0) { }
  void Show();
  double Evaluate();
  bool Check();
  ~ASTNode();

  static map<std::string, FunctionRec> functionMap;
  static void RegisterFunction (string functionName, double (*fnPtr)(double, double));
  static void RegisterFunction (string functionName, double (*fnPtr)(double));
  static void RegisterFunction (string functionName, double (*fnPtr)());
};

extern std::string __scanned;
extern std::string __lasttok;
extern bool __errorsFound;
extern double __xval;
extern double __yval;
extern ASTNode * __parseTree;

void yyerror(char * msg, ...);
// void Failure(char * msg, ...);

// From lex.yy.c
void Inityylex(void);
int yylex(void);

extern char yytext[];
extern int yy_flex_debug;

// From y.tab.c
void Inityyparse(void);
int yyparse(void);

#endif
