#include <iostream>
#include <string>
#include "calculator.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stod;

//convert the value string for a given number into a double
double Calculator::Number::getNumber(void)
{
    return stod(this->value.c_str()) * (this->isNegative?-1:1);
}

//reset the parameters for a Number object
void Calculator::Number::reset(void)
{
    this->value = "0";
    this->containsDecimal = false;
    this->isNegative = false;
}

//run the calculator program
void Calculator::run(void)
{
    updateDisplay();

    while(1)
    {
        cout << "Enter char: ";
        char input; cin >> input;

        if (state == CS_ANSWER)
            reset();
        parseChar(input);
        updateDisplay();
    }
}

//parse input received from the user
void Calculator::parseChar(char input)
{
    switch(input)
    {
        case 'e':
        exit(0);
        break;

        case 'c':
        reset();
        break;

        case '=':
        calculate();
        break;

        case '+':
        case '-':
        case '/':
        case '*':
        oper = input;
        state = CS_NUM2;
        break;

        default:
        updateCurrNumber(input);
    }
}

//when requested, perform the calculation entered by the user
void Calculator::calculate(void)
{
    double fnum1 = stod(num1.value.c_str()) * (num1.isNegative?-1:1);
    double fnum2 = stod(num2.value.c_str()) * (num2.isNegative?-1:1);
    state = CS_ANSWER;

    switch (oper)
    {
        case '/':
        if (fnum2 == 0)
            errDetected = true;
        else
            ans = fnum1/fnum2;
        break;

        case '*':
        ans = fnum1*fnum2;
        break;

        case '-':
        ans = fnum1-fnum2;
        break;

        case '+':
        ans = fnum1+fnum2;
        break;
    }
}

//update the number currently referenced by the calculator state
//this method effectively pushes/pops digits to the number as well
//as adding a decimal point or negating the value when requested
void Calculator::updateCurrNumber(char input)
{
    Number &currNum = state == CS_NUM1? num1:num2;

    if (isdigit(input)) //add digit
    {
        //add digit to current number
        if (currNum.value == "0")
            currNum.value = input;
        else
            currNum.value.push_back(input);
    }
    else
    {
        switch (input)
        {
            case '.': //add decimal point
            if (!currNum.containsDecimal)
            {
                currNum.value.push_back(input);
                currNum.containsDecimal = true;
            }
            break;

            case '~': //flip sign
            currNum.isNegative = !currNum.isNegative;
            break;

            case 'b': //delete char
            if (currNum.value.size() > 1)
            {
                if (currNum.value[currNum.value.size() - 1]  == '.')
                    currNum.containsDecimal = false;
                currNum.value.pop_back();
            }
            else
                currNum.value = "0";
            break;
        }
    }
}

//update the display printout of the calculator
void Calculator::updateDisplay(void)
{
    cout << "Display: ";
    if (state == CS_ANSWER)
        if (errDetected)
            cout << "ERROR";
        else
            cout << ans;
    else
    {
        cout << num1.getNumber() << oper;
        if (state == CS_NUM2)
            cout << num2.getNumber();
    }
    cout << endl;
}

//reset all calculator parameters
void Calculator::reset(void)
{
    num1.reset();
    num2.reset();
    ans = 0;
    oper = '\0';
    state = CS_NUM1;
    errDetected = false;
}