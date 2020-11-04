#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void myslot();
private slots:



    void on_clearBtn_clicked();

    void on_clearAllBtn_clicked();

    void on_equalBtn_clicked();

    void on_signBtn_clicked();

    void on_pointBtn_clicked();

    void digitClicked();

    void operatorClicked();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void specialOperatorClicked();
    void on_action_5_triggered();

private:
    Ui::MainWindow *ui;
    //定义实现计算的函数
//    bool calculate(int operand,QString pendingOperator);
    void abortOperation();
    void connectSlots();//连接信号和槽
//    QString pendingOperator;//储存运算符
//    double result;//储存计算结果
    bool waitForOperand;//标记是否等待一个操作数
    void setShortcutKeys();
    QString inToPost(QString infix) throw(const char*);//转换为后缀表达式
    double compute(QString s) throw(const char*);//计算后缀表达式的结果
    QString error;
    QString log;
};

#endif // MAINWINDOW_H
