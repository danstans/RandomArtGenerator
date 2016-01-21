#include <iostream>
#include "helper.h"
using namespace std;

class Expression
{
 public:
  // Constructor: Creates an Expression object from the 
  // given string, issuing an error if the string
  // isn't a valid expression
  Expression(string expressionString);
  
  // Returns true/false to indicate whether the syntax in the string
  // passed on creation was valid
  bool SyntaxIsValid();

  // Evalute this expression given values of x and y
  double Evaluate(double x, double y);
  

  // Display the expression in a standardized form
  void Show();
  
  ~Expression();
  
 private:
  ASTNode * node;
  bool syntaxOk;
  
  friend class ASTNode;
  
  static void Setup();
  static bool setupDone;
};



