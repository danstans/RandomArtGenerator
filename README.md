# Random-Art-Generator


The mathematical functions of sin, cos will always return a value between -1 and 1, in addition to these two functions avg and multiply will also only return a value between -1 and 1. With this in mind, we can use recursion to create very large equations using only these properties. This expression can be found in the createExpressionText function which returns a string.

Each pixel is given a X,Y value, starting from (0,0) up until the size of the screen. Each of these X,Y coordinates will be plugged into the above expression, producing a number between -1 and 1. This value is then translated proportionally into a number between 0, 255. Which is correlated to it's RGB value.

The user is presented a prompt, in case they want a grayscale image or a colored image, and they are also able to provide the depth for the recursion. A higher depth will take longer time for the program to execute because there will be a larger amount of computations that need to be done. A grayscale image will compute a singular expression and then use that value as it's R G and B value. A color image will have to produce 3 different expressions for each different gradiant. 

The best way to run the code is by running the executable file that is found in the /bin folder. Most of the code that I have written can be found in the main.cpp and the expression.cpp/h
