#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "helper.h"
#include "lpclib.h"
using namespace std;

/*
 * from lex.yy.c
 */
extern char yytext[];
#define BufferSize   1024

void yyerror(char * msg, ...) {
  va_list args;
  char errbuf[BufferSize];
  int errlen;

  va_start(args, msg);
  vsprintf(errbuf, msg, args);
  va_end(args);
  errlen = strlen(errbuf);
  if (errlen > BufferSize) {
    cout << "parse error - message too long" << endl;
  } else {
    string show = "   " + __scanned + "\n   ";
    for (unsigned int k = 0; k < __scanned.length()-__lasttok.length(); k++)
      show += " ";
    for (unsigned int k = 0; k < __lasttok.length(); k++)
      show += "^";
    show += "\n";
    printf("** Expression %s\n%s", errbuf, show.c_str());
  }
  __errorsFound = true;
}

/*
void Failure(char * msg, ...)
{
  va_list args;
  char errbuf[BufferSize];
  int errlen;

  va_start(args, msg);
 vsprintf(errbuf, msg, args);
  va_end(args);
  errlen = strlen(errbuf);
  if (errlen > BufferSize) {
    printf("\n*** Failure: Failure Message too long\n\n");
  } else {
    printf("\n*** Failure: %s\n\n", errbuf);
  }
  exit(1);
}
*/

bool ASTNode::Check()
{
  if (whichVar != '.')
    return true;

  map<string, FunctionRec>::iterator result = functionMap.find(fnName);
  if (result == functionMap.end())
    {
      cout << "** '" << fnName << "' is not defined\n";
      return false;
    }

  FunctionRec rec = (*result).second;

  int actualArgs;
  if (!leftChild)
    actualArgs = 0;
  else if (!rightChild)
    actualArgs = 1;
  else
    actualArgs = 2;
  int numParams = rec.whatType;
  if (actualArgs != numParams)
    {
        cout << "** '" << fnName << "' expects " << numParams << " arguments.\n";
        return false;
    }

  switch (numParams)
    {
     case 0:
       return true;
     case 1:
       return leftChild->Check();
     case 2:
       return leftChild->Check() && rightChild->Check();
    default:
      cout << "*** CRAZY SCHATZ FAIL ERROR!\n";
      return false;
    }
}

double ASTNode::Evaluate()
{
  if (whichVar != '.')
    {
      if (whichVar == 'x')
	return (__xval);
      else
	return (__yval);
    }

  map<string, FunctionRec>::iterator result = functionMap.find(fnName);
  if (result == functionMap.end())
    {
      Error(string("Can't evaluate expression - '") + fnName + "' is not defined.");
    }

  FunctionRec rec = (*result).second;

  int actualArgs;
  if (!leftChild)
    actualArgs = 0;
  else if (!rightChild)
    actualArgs = 1;
  else
    actualArgs = 2;
  int numParams = rec.whatType;
  if (actualArgs != numParams)
    {
      cout << "********** BAD EXPRESSION *********\n";
      cout << "** Function '" << fnName << "'\n";
      cout << "** Expected " << numParams << " arguments,\n";
      cout << "** Called with " << actualArgs << "\n";
      cout << "***********************************\n";
      return 0;
    }

  switch (numParams)
    {
     case 0:
       return ((*(rec.ptr.noArg))());
     case 1:
       return ((*(rec.ptr.oneArg))(leftChild->Evaluate()));
     case 2:
       return ((*(rec.ptr.twoArg))(leftChild->Evaluate(), rightChild->Evaluate()));
    default:
      cout << "*** CRAZY SCHATZ FAIL ERROR!\n";
      return -9000;
    }

}

ASTNode::~ASTNode()
{
  if (leftChild)
    delete leftChild;
  if (rightChild)
    delete rightChild;
}

void ASTNode::Show()
{
  if (fnName == "")
    cout << whichVar;
  else
    {
      cout << fnName;
      cout << "(";
      if (leftChild)
	leftChild->Show();
      if (rightChild)
	{
	  cout << ", ";
	  rightChild->Show();
	}
      cout << ")";
    }
}

map<string, FunctionRec> ASTNode::functionMap;

static void SmartInsert(string functionName, FunctionRec rec)
{
  if (  ASTNode::functionMap.find(functionName) != ASTNode::functionMap.end())
    {
      cout << "*********** FATAL ERROR ***********\n";
      cout << "** Attempt to register duplicate\n";
      cout << "** function '" << functionName << "'\n";
      cout << "***********************************\n";
      exit (-1);
    }
  ASTNode::functionMap[functionName] = rec;
}

void ASTNode::RegisterFunction (string functionName, double (*fnPtr)(double, double))
{
  double val;

  // Range check (approximate)
  for (int x = -1; x <= 1; x++)
    for (int y = -1; y <= 1; y++)
      {
	val = (*fnPtr)(x, y);
	if (val < -1 || val > 1)
	  {
	    cout << "*********** FATAL ERROR ***********\n";
	    cout << "** The function '" << functionName << "' is ill-behaved:\n";
	    cout << "** " << functionName << "(" << x << ", " << y << ") = " << val << endl;
	    cout << "***********************************\n";
	    exit(-1);
	  }
      }

  FunctionRec rec;
  rec.whatType = TwoArg;
  (rec.ptr).twoArg = fnPtr;
  SmartInsert(functionName, rec);
}

void ASTNode::RegisterFunction (string functionName, double (*fnPtr)(double))
{
  double val;

  // Range check (approximate)
  for (int x = -1; x <= 1; x++)
    {
      val = (*fnPtr)(x);
      if (val < -1 || val > 1)
	{
	  cout << "*********** FATAL ERROR ***********\n";
	  cout << "** The function '" << functionName << "' is ill-behaved:\n";
	  cout << "** " << functionName << "(" << x << ") = " << val << endl;
	  cout << "***********************************\n";
	  exit(-1);
	}
    }

  FunctionRec rec;
  rec.whatType = OneArg;
  (rec.ptr).oneArg = fnPtr;
  SmartInsert(functionName, rec);
}

void ASTNode::RegisterFunction (string functionName, double (*fnPtr)())
{
  double val;

  for (int i = 0; i < 10; i++)
    {
      val = (*fnPtr)();
      if (val < -1 || val > 1)
	{
	  cout << "*********** FATAL ERROR ***********\n";
	  cout << "** The function '" << functionName << "' is ill-behaved:\n";
	  cout << "** " << functionName << "() = " << val << endl;
	  cout << "***********************************\n";
	  exit(-1);
	}
    }

  FunctionRec rec;
  rec.whatType = NoArg;
  (rec.ptr).noArg = fnPtr;
  SmartInsert(functionName, rec);
}
