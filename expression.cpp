#include "expression.h"
#include "lex.yy.h"
#include <iostream>
#include <cmath>
using namespace std;

double __xval;
double __yval;
double __value;
bool __errorsFound;

double avg(double x, double y)
{
  return ((x + y)/2);
}

double pcos(double x)
{
  return (cos(acos(-1)*x));
}

double psin(double x)
{
  return (sin(acos(-1)*x));
}

double mult (double x, double y)
{
  return (x * y);
}


Expression::Expression(string expressionString)
{
  Setup();
  node = 0;
  Inityylex();
  yy_scan_string(expressionString.c_str());

  syntaxOk = true;
  __errorsFound = false;
  while (1)
    {
      int result = yylex();
      if (__errorsFound == true)
      {
          syntaxOk = false;
          break;
      }
      if (result == 0)
        break;
    }

  if (syntaxOk)
    {
      Inityylex();
      yy_scan_string(expressionString.c_str());
      if (yyparse() != 0)
        syntaxOk = false;
      else
      {
        node = __parseTree;
        syntaxOk = node->Check();
      }
    }


}

bool Expression::SyntaxIsValid()
{
  return (syntaxOk);
}

double Expression::Evaluate(double x, double y)
{
  if (!syntaxOk)
    {
      cout << "Attempt to call Evaluate on invalid expression" << endl;
      exit(-1);
    }
  __xval = x;
  __yval = y;
  return (node->Evaluate());
}

void Expression::Show()
{
  node->Show();
}

Expression::~Expression()
{
  if (node)
    delete node;
}

void Expression::Setup()
{
  if (setupDone == false)
    {
      setupDone = true;
      ASTNode::RegisterFunction ("avg", avg);
      ASTNode::RegisterFunction ("mult", mult);
      ASTNode::RegisterFunction ("psin", psin);
      ASTNode::RegisterFunction ("pcos", pcos);
    }
}

bool Expression::setupDone = false;
