
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    struct RESULT
    {
        bool errDetected;
        double ans;
    };

    Ui::MainWindow *ui;
    double prevVal = 0.0;
    char oper = 0;
    bool ansOnDisp = false;

    RESULT calculate(char oper, double num1, double num2);
    void reset();

private slots:
    void digit_pressed();
    void negate_pressed();
    void clear_pressed();
    void percent_pressed();
    void decimal_pressed();
    void oper_pressed();
    void equals_pressed();
};

#endif // MAINWINDOW_H
