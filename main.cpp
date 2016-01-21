#include "lpclib.h"
#include "expression.h"

using namespace std;

// The library doesn't have a DrawPoint function, so I defined one here...
static void DrawPoint(int x, int y, smallint r, smallint g, smallint b);
string createExpressionText(int depthLevel);

int main()
{


    double WindowWidth=600;
    double WindowHeight=600;
    GraphicsWindow(WindowWidth,WindowHeight,"black");

    double x, y;
    string text;
    string userChoice;
    int maxDepth=2, minDepth =2;
    string lastChoice = "1";
    Expression * expr;
    Expression * colorRed;
    Expression * colorGreen;
    Expression * colorBlue;


    while (1)
    {

        cout << "Options:\n1)Enter an expression\n2)Set recursion depth (currently set to " << minDepth << " and " << maxDepth << ")\n3)Random Greyscale Image\n4)Random Color Image\n5)Quit\n\n";
        cout << "Choice: ";
        userChoice = GetLine();
        if(userChoice == ""){
                userChoice = lastChoice;
        }

        if(userChoice=="1"){
                lastChoice = "1";
            cout << "Enter an expression: ";
            text = GetLine();
            expr = new Expression(text);
            if (! expr->SyntaxIsValid())
            {
                cout << "Sorry, that expression isn't valid.\n\n";
            }
            else
            {
//                cout << "Enter coordinates in the range [-1,1]:\n";
//                cout << "x = ";
//                x = GetDouble();
//                cout << "y = ";
//                y = GetDouble();

                for(int xCord =0; xCord < WindowWidth; xCord++){
                    double xValue = (xCord/(WindowWidth/2)-1);
                    for (int yCord =0; yCord < WindowHeight; yCord++)
                    {
                        double yValue = (yCord/(WindowHeight/2)-1);
                        double color = (((expr->Evaluate(xValue, yValue)) + 1)/2)*255;
                        DrawPoint(xCord,yCord, color,color,color);
                    }
                }
                UpdateDisplay();

        }
        continue;
    }
            else if(userChoice=="2")
                {
                    lastChoice = "2";
                    minDepth =500;
                    maxDepth =-500;

                    cout << "Please set a minimum and maximum depth:\nMaximum Depth: ";
                    maxDepth = GetInt();
                    while (maxDepth <0){
                        cout << "   -ERROR: Please enter a value larger than 0: ";
                        maxDepth = GetInt();
                    }
                    cout << "Minimum Depth: ";
                    minDepth = GetInt();
                    while(minDepth>maxDepth || minDepth<0){
                        cout << "   -ERROR: Please enter a value smaller than or equal to Max Depth: ";
                        minDepth = GetInt();
                    }
                    cout << "\n\n";
                    continue;


                }
            else if(userChoice=="3")
                {
                    lastChoice = "3";
            cout << "Now generating a Random Greyscale Image\n\n";
            int recursiveDepth = RandomInt(minDepth,maxDepth);
            string textExpression = createExpressionText(recursiveDepth);
            cout << "Greyscale equation: " << textExpression;
            expr = new Expression(textExpression);
                    if (! expr->SyntaxIsValid())
                    {
                        cout << "Sorry, that expression isn't valid.\n\n";
                    }
                    else
                    {

                        for(int xCord =0; xCord < WindowWidth; xCord++){
                            double xValue = (xCord/(WindowWidth/2)-1);
                            for (int yCord =0; yCord < WindowHeight; yCord++)
                            {
                                double yValue = (yCord/(WindowHeight/2)-1);
                                double color = (((expr->Evaluate(xValue, yValue)) + 1)/2)*255;
                                DrawPoint(xCord,yCord, color,color,color);
                            }
                        }
                        UpdateDisplay();

        }

                     cout << "\n\n";
                    continue;
                }
            else if(userChoice=="4")
                {
                    lastChoice ="4";
                    cout << "Now generating a Random Color Image";
                     int recursiveDepth = RandomInt(minDepth,maxDepth);

                     string textExpression = createExpressionText(recursiveDepth);
                     colorRed = new Expression(textExpression);
                     cout << "\n\nRed: " << textExpression;
                     cout << endl;
                    textExpression = createExpressionText(recursiveDepth);
                    cout << "\n\nBlue: " << textExpression;
                    cout << endl;
                     colorGreen = new Expression(textExpression);
                    textExpression = createExpressionText(recursiveDepth);
                    cout << "\n\nGreen: " << textExpression;
                    cout << endl;
                     colorBlue = new Expression(textExpression);


                        for(int xCord =0; xCord < WindowWidth; xCord++){
                            double xValue = (xCord/(WindowWidth/2)-1);
                            for (int yCord =0; yCord < WindowHeight; yCord++)
                            {
                                double yValue = (yCord/(WindowHeight/2)-1);
                                double Rcolor = (((colorRed->Evaluate(xValue, yValue)) + 1)/2)*255;
                                double Gcolor = (((colorGreen->Evaluate(xValue, yValue)) + 1)/2)*255;
                                double Bcolor = (((colorBlue->Evaluate(xValue, yValue)) + 1)/2)*255;

                                DrawPoint(xCord,yCord, Rcolor,Gcolor,Bcolor);
                            }
                        }
                        UpdateDisplay();




                    cout << "\n\n";
                    continue;
                }
                else if(userChoice =="5") {
                        cout << "   -Closing";
                    break;
                }
                else{
                    continue;
                }
        delete expr;
        delete colorBlue;
        delete colorGreen;
        delete colorRed;
    }

    return 0;
}



static void DrawPoint(int x, int y, smallint r, smallint g, smallint b)
{
    DefineColor(".", r, g, b);
    DrawLine(x, y, x, y, ".");
}

string createExpressionText(int depthLevel)
{
    int random = RandomInt(0,4);
    if(depthLevel ==0)
    {
        if (random <2)
            return "x";
        else
            return "y";
    }
    else if (depthLevel >0)
    {
        if (random ==0)
            return ("sin(pi*" +createExpressionText(depthLevel-1) + ")");
        else if(random ==1)
            return ("cos(pi*" + createExpressionText(depthLevel-1) +")");
        else if(random ==2)
            return ("avg(" + createExpressionText(depthLevel-1) + "," + createExpressionText(depthLevel-1) + ")");
        else
            return (createExpressionText(depthLevel-1) + "*" + createExpressionText(depthLevel-1));
    }

}


