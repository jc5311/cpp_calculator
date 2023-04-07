
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //calc digit keys
    connect(ui->pushButton_num0, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num1, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num2, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num3, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num4, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num5, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num6, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num7, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num8, SIGNAL(released()), this,SLOT(digit_pressed()));
    connect(ui->pushButton_num9, SIGNAL(released()), this,SLOT(digit_pressed()));

    //modifier keys
    connect(ui->pushButton_plusminus, SIGNAL(released()), this, SLOT(negate_pressed()));
    connect(ui->pushButton_percent,   SIGNAL(released()), this, SLOT(percent_pressed()));
    connect(ui->pushButton_decimal,   SIGNAL(released()), this, SLOT(decimal_pressed()));

    //clear screen key
    connect(ui->pushButton_clear, SIGNAL(released()), this, SLOT(clear_pressed()));

    //operation keys
    connect(ui->pushButton_divide,   SIGNAL(released()), this, SLOT(oper_pressed()));
    connect(ui->pushButton_multiply, SIGNAL(released()), this, SLOT(oper_pressed()));
    connect(ui->pushButton_add,      SIGNAL(released()), this, SLOT(oper_pressed()));
    connect(ui->pushButton_subtract, SIGNAL(released()), this, SLOT(oper_pressed()));

    //equals key
    connect(ui->pushButton_equals,   SIGNAL(released()), this, SLOT(equals_pressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * MainWindow::digit_pressed()
 *
 * Update the display when the user enters a new digit.
 * * If there is answer on the display, pressing a digit starts a new calculation
 * * Otherwise, append the entered digit to the value on the display
 *
 * Args:    none
 * Returns: void
 */
void MainWindow::digit_pressed()
{
    QPushButton *button = (QPushButton*) sender(); //get a ptr to the digit that was pressed
    double dispValue;

    if (this->ansOnDisp)
    {
        this->ansOnDisp = false; //replacing answer on display with new number
        dispValue = button->text().toDouble();
    }
    else
        dispValue = (ui->display->text() + button->text()).toDouble();

    //Convert the dispValue into a new QString to update the display with
    QString dispText = QString::number(dispValue,'g', 15);

    //update the display's label text
    ui->display->setText(dispText);
}

/*
 * MainWindow::negate_pressed()
 *
 * Flip the sign of the value on the display.
 *
 * Args:    none
 * Returns: void
 */
void MainWindow::negate_pressed()
{
    //if there is an answer being shown and the user modifies it,
    //use it as fresh input
    if (this->ansOnDisp)
        this->ansOnDisp =false;

    //get the value from the display and flip its sign
    double dispValue = ui->display->text().toDouble() * -1.0;

    QString dispText = QString::number(dispValue, 'g', 15);
    ui->display->setText(dispText);
}

/*
 * MainWindow::percent_pressed()
 *
 * Convert the value on the display to a percentage
 *
 * Args:    none
 * Returns: void
 */
void MainWindow::percent_pressed()
{
    //if there is an answer being shown and the user modifies it,
    //use it as fresh input
    if (this->ansOnDisp)
        this->ansOnDisp =false;

    double dispValue = ui->display->text().toDouble()/100.0;
    QString dispText = QString::number(dispValue, 'g', 15);
    ui->display->setText(dispText);
}

/*
 * MainWindow::decimal_pressed()
 *
 * Add a decimal point to the number on the display
 *
 * Note: If there is an answer on the display, pressing the
 * decimal key should be equivalent to entering a digit after
 * usage of the '=' operator in that it begins building a
 * new number to work on.
 */
void MainWindow::decimal_pressed()
{
    QString dispText;

    if (this->ansOnDisp)
    {
        this->ansOnDisp = false;
        dispText = "0";
    }
    else
        dispText = ui->display->text();

    if (!(dispText.count('.') > 0))
    {
        dispText.push_back('.');
        ui->display->setText(dispText);
    }
}

/*
 * MainWindow::clear_pressed()
 *
 * Clear the screen and reset the calculator
 * to defaults
 *
 * Args:    none
 * Returns: void
 */
void MainWindow::clear_pressed()
{
    this->reset();
    ui->display->setText("0");
}

/*
 * MainWindow::oper_pressed()
 *
 * Math operation selected, record the operation
 * and begin accepting input for a new number
 *
 * Args:    none
 * Returns: void
 */
void MainWindow::oper_pressed()
{
    //store the operation
    QPushButton *button = (QPushButton*) sender();
    QString btntxt = button->text();
    this->oper = btntxt[0].toLatin1();

    //if there is no previous value (entering first number of a new calculation)
    //or there is an answer being displayed (user wants to continue calculating from
    //last result), store the value from the screen and accept new input
    if (this->prevVal == 0 || this->ansOnDisp)
    {
        this->ansOnDisp = false;
        this->prevVal = ui->display->text().toDouble();
        ui->display->setText("0");
    }
    //if there is a previous value, perform the operation recorded so far
    //if no errors, store the result and begin accepting more input
    else
    {
        MainWindow::RESULT res = this->calculate(this->oper, this->prevVal, ui->display->text().toDouble());
        if (res.errDetected == true)
        {
            ui->display->setText("ERROR");
            this->reset();
        }
        else
        {
            //store the newly calculated value
            this->prevVal = res.ans;

            //begin accepting new input
            ui->display->setText("0");
        }
    }
}

/*
 * MainWindow::equals_pressed()
 *
 * Equals key pressed. Run the current
 * calculation (if there is one to do) and
 * present the result.
 *
 * Args:    none
 * Returns: void
 */
void MainWindow::equals_pressed()
{
    //if no oper recorded, there is nothing to do
    if (this->oper == 0)
        return;

    MainWindow::RESULT res = this->calculate(this->oper, this->prevVal, ui->display->text().toDouble());
    if (res.errDetected == true)
    {
        ui->display->setText("ERROR");
        this->reset();
    }
    else
    {
        this->reset();

        //display the result
        QString dispText = QString::number(res.ans, 'g', 15);
        ui->display->setText(dispText);
        this->ansOnDisp = true;
    }
}

/*
 * MainWindow::calculate()
 *
 * Perform the calculation requested by the entered
 * operation and numbers
 *
 * Args:
 * oper - char specifier for math operation to perform </, x, -, +>
 * num1 - First number (numerator in case of division)
 * num2 - Second number (denominator in case of division)
 *
 * Returns: void
 */
MainWindow::RESULT MainWindow::calculate(char oper, double num1, double num2)
{
    MainWindow::RESULT res = {false, 0.0};

    switch (oper)
    {
        case '/':
            if (num2 == 0)
                res.errDetected = true;
            else
                res.ans = num1/num2;
            break;

        case 'x':
            res.ans = num1*num2;
            break;

        case '-':
            res.ans = num1-num2;
            break;

        case '+':
            res.ans = num1+num2;
            break;

        default:
            res.errDetected = true;
    }

    return res;
}

/*
 * MainWindow::reset()
 *
 * Clear calculator paramaters stored in the object
 *
 * Args:    none
 * Returns: void
 */
void MainWindow::reset()
{
    this->oper      = 0;
    this->prevVal   = 0.0;
    this->ansOnDisp = false;
}
