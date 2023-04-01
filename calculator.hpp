#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <string>

//CALC_STATE is used to maintain a state for the calculator, affecting
//how the display is updated and how certain operations are handled
typedef enum
{
    CS_NUM1,
    CS_NUM2,
    CS_ANSWER,
} CALC_STATE;

class Calculator
{
public:
    Calculator(){};
    void run(void);
    
private:
    struct Number
    {
        //Since we don't need the numbers to be a double until calculation
        //use strings to faciliate adding and removing digits.
        std::string value = "0";
        bool containsDecimal = false;
        bool isNegative = false;

        double getNumber(void);
        void reset(void);
    };

    Number num1;
    Number num2;
    double ans;
    char oper = '\0';
    int state = CS_NUM1;
    bool errDetected = false;

    void parseChar(char input);
    void calculate(void);
    void updateCurrNumber(char input);
    void updateDisplay(void);
    void reset(void);
};

#endif //__CALCULATOR_H__