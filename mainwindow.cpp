#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPushButton>
#include<QMessageBox>
#include<map>
#include<stack>
#include<cmath>
#include<QClipboard>
#include<QApplication>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText("0");
    waitForOperand=true;
    connectSlots();
    setShortcutKeys();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::abortOperation()
{

    ui->lineEdit->setText("0");
    waitForOperand=true;
    QMessageBox::warning(this,"运算错误","错误！");
}



void MainWindow::connectSlots()
{
    QPushButton *digitaBtns[10]={
        ui->digtalBtn0,ui->digtalBtn1,ui->digtalBtn2,ui->digtalBtn3,ui->digtalBtn4,ui->digtalBtn5,ui->digtalBtn6,ui->digtalBtn7,ui->digtalBtn8,ui->digtalBtn9
    };

    for(auto btn:digitaBtns)
        connect(btn,&QPushButton::clicked,this,&MainWindow::digitClicked);

    QPushButton *operatorBtns[8]={
        ui->addBtn,ui->subtractionBtn,ui->mulBtn,ui->divBtn,ui->leftBracketBtn,ui->rightBracketBtn,ui->powBtn,ui->percrntBtn

        };
    for(auto btn:operatorBtns)
        connect(btn,&QPushButton::clicked,this,&MainWindow::operatorClicked);
    QPushButton *specialOperatorBtns[5]={
        ui->sinBtn,ui->cosBtn,ui->tanBtn,ui->lnBtn,ui->lgBtn
    };
    for(auto btn:specialOperatorBtns)
        connect(btn,&QPushButton::clicked,this,&MainWindow::specialOperatorClicked);
}
 void MainWindow::setShortcutKeys()
 {
     Qt::Key key[27]={Qt::Key_0,Qt::Key_1,Qt::Key_2,Qt::Key_3,Qt::Key_4,Qt::Key_5,Qt::Key_6,Qt::Key_7,Qt::Key_8,Qt::Key_9,Qt::Key_Plus,Qt::Key_Minus,Qt::Key_Asterisk,Qt::Key_Slash,
                      Qt::Key_Enter,Qt::Key_Period,Qt::Key_Backspace,Qt::Key_M,Qt::Key_ParenLeft,Qt::Key_ParenRight,Qt::Key_AsciiCircum,
                      Qt::Key_Percent,Qt::Key_S,Qt::Key_C,Qt::Key_T,Qt::Key_N,Qt::Key_G};
     QPushButton *btn[27]={
         ui->digtalBtn0,ui->digtalBtn1,ui->digtalBtn2,ui->digtalBtn3,ui->digtalBtn4,ui->digtalBtn5,ui->digtalBtn6,ui->digtalBtn7,ui->digtalBtn8,ui->digtalBtn9,
         ui->addBtn,ui->subtractionBtn,ui->mulBtn,ui->divBtn,ui->equalBtn,ui->pointBtn,ui->clearAllBtn,ui->signBtn,ui->leftBracketBtn,ui->rightBracketBtn,ui->powBtn,
         ui->percrntBtn,ui->sinBtn,ui->cosBtn,ui->tanBtn,ui->lnBtn,ui->lgBtn
     };
     for(int i=0;i<27;i++)
         btn[i]->setShortcut(QKeySequence(key[i]));
     ui->clearAllBtn->setShortcut(QKeySequence("Ctrl+Backspace"));

 }

void MainWindow::digitClicked()
{
    QPushButton *digitBtn=static_cast<QPushButton*>(sender());

    QString value=digitBtn->text();

    if(ui->lineEdit->text()=="0"&&value=="0")
        return;

    if(waitForOperand)
    {
        ui->lineEdit->setText(value);
        waitForOperand=false;
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text()+value);
    }
}


void MainWindow::operatorClicked()
{
    QPushButton *clickedBtn=qobject_cast<QPushButton*>(sender());
    QString clickedOperator=clickedBtn->text();
    if(waitForOperand==true&&clickedOperator=='(')//首符号为‘（’的到时候
        {
            ui->lineEdit->setText(clickedOperator);
            waitForOperand=false;
        }
    else if(waitForOperand==false)
        ui->lineEdit->setText(ui->lineEdit->text()+clickedOperator);


 //以上更具有鲁棒性


//    if(waitForOperand==false&&ui->lineEdit->text().data()[ui->lineEdit->text().size()-1].isDigit()==true)//在 waitforoperand允许的情况下，并且字符串最后一个字符为数字的情况下
//        ui->lineEdit->setText(ui->lineEdit->text()+clickedOperator);
//    else if(waitForOperand==false&&clickedOperator=='(')
//        ui->lineEdit->setText(ui->lineEdit->text()+clickedOperator);
//    else if(waitForOperand==false&&ui->lineEdit->text().data()[ui->lineEdit->text().size()-1]=='%')
//        ui->lineEdit->setText(ui->lineEdit->text()+clickedOperator);
//    else if(waitForOperand==true&&clickedOperator=='(')//首符号为‘（’的到时候
//    {
//        ui->lineEdit->setText(clickedOperator);
//        waitForOperand=false;
//    }


}
void MainWindow::specialOperatorClicked()
{
    QPushButton *specialOperatorBtn=static_cast<QPushButton*>(sender());
    QString value=specialOperatorBtn->text();
    if(waitForOperand)
    {
        ui->lineEdit->setText(value);
        waitForOperand=false;
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text()+value);
    }
}


void MainWindow::on_equalBtn_clicked()
{

    double result=0.0;
    try
    {
        result=compute(inToPost(ui->lineEdit->text()));
//        QMessageBox::warning(this,"提示",inToPost(ui->lineEdit->text()));//查看后缀表达式
    }
    catch(const char *er)
    {
        error=er;
        abortOperation();
        return;
    }

    ui->lineEdit->setText(ui->lineEdit->text()+'='+QString::number(result));
    log=ui->lineEdit->text();
    waitForOperand=true;
}

void MainWindow::on_clearBtn_clicked()
{
    //将显示字符串退格
    QString str=ui->lineEdit->text();
    if(str!='0'&&waitForOperand==false)
    {
        ui->lineEdit->setText(str.left(str.count()-1));
        waitForOperand=false;
    }
    else
    {
        ui->lineEdit->setText("0");
        waitForOperand=true;
    }



}

void MainWindow::on_clearAllBtn_clicked()
{
    //将当前显示全部归零
    ui->lineEdit->setText("0");
    waitForOperand=true;

}



void MainWindow::on_signBtn_clicked()
{

   QString text=ui->lineEdit->text();
   QChar sign=text[text.size()-1];
   if(sign=='-')
   {
       text.remove(text.size()-1,1);
   }
   else {
       text.append('-');
   }

   ui->lineEdit->setText(text);
}



void MainWindow::on_pointBtn_clicked()
{
    if(waitForOperand==false&&ui->lineEdit->text().data()[ui->lineEdit->text().size()-1].isDigit()==true)
         ui->lineEdit->setText(ui->lineEdit->text()+".");

}

QString MainWindow::inToPost(QString infix) throw(const char*)//将中缀表达式转换为后缀表达式
{
    std::stack<char> stack;
    char current=0;
    QString postfix;
    std::map<char,int> priority;
    priority['+']=0;
    priority['-']=0;
    priority['*']=1;
    priority['/']=1;
    priority['^']=2;
    priority['s']=3;
    priority['c']=3;
    priority['t']=3;
    priority['n']=3;
    priority['g']=3;


    for(int i=0;i<infix.length();i++)
    {
        current=infix[i].toLatin1();
        if(isdigit(current))
        {
            postfix.push_back(current);
            continue;
        }
        switch (current)
        {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':

            if(infix[i-1]!=')')
            {
                if(infix[i-1].isDigit())
                    postfix.push_back('#');
                else if(infix[i-1]=='%')
                {}
//                    infix.begin();
                else
                    throw "expression is illegality";
            }
            if(stack.empty()==false)
            {
                char tempTop=stack.top();
                while(tempTop!='('&&priority[current]<=priority[tempTop])
                {
                    stack.pop();
                    postfix.push_back(tempTop);
                    if(stack.empty())
                        break;
                    tempTop=stack.top();
                }
            }
            stack.push(current);

            break;

        case's':
        case'c':
        case't':
            if(i>0&&infix[i-1].isDigit())
                throw"表达式非法";
            if(!stack.empty())
            {
                char temTop=stack.top();
                while(temTop!='('&&priority[current]<priority[temTop])
                {
                    stack.pop();
                    postfix.push_back(temTop);
                    if(stack.empty())
                        break;
                    temTop=stack.top();
                }
            }
            stack.push(current);
            i=i+2;
            break;

        case'l':
            if(i>0&&infix[i-1].isDigit())
                throw"表达式非法";
            if(!stack.empty())
            {
                char temTop=stack.top();
                while(temTop!='('&&priority[infix[i+1].toLatin1()]<priority[temTop])
                {
                    stack.pop();
                    postfix.push_back(temTop);
                    if(stack.empty())
                        break;
                    temTop=stack.top();
                }
            }
            stack.push(infix[i+1].toLatin1());
            i=i+1;
            break;

        case'.':
            postfix.push_back(current);
            break;
        case'%':
            postfix.push_back('#');
            postfix.push_back(current);
            break;
        case'(':
            stack.push(current);
            break;
        case')':
             if(infix[i-1]!='%')
                 postfix.push_back('#');//右括号说明前方数字输入完成 标识一下

            char temTop;
            temTop=stack.top();
            while (temTop!='(') {
                stack.pop();
                postfix.push_back(temTop);
                temTop=stack.top();

            }
            stack.pop();
            break;

        default:
            throw "expression has illegality charactor";
            break;
        }
    }

    if(infix[infix.size()-1]!=')')
    {
        if(infix[infix.size()-1].isDigit())
            postfix.push_back('#');
        else if(infix[infix.size()-1]=='%')
        {}
        else
            throw "expression is illegality";

    }
    while(!stack.empty())
    {
        char tempOut=stack.top();
        stack.pop();
        postfix.push_back(tempOut);
    }
    return postfix;

}

double MainWindow::compute(QString s) throw(const char*)//计算后缀表达式
{
    std::stack<double> stack;
    QString str;
    double curr;

    double temNum1;
    double temNum2;
    for(auto i=s.begin();i!=s.end();i++)
    {
        if((*i).isDigit())
        {
            str.push_back(*i);
            continue;
        }
        switch ((*(i)).toLatin1())
        {
        case '+':
            temNum1=stack.top();
            stack.pop();
            temNum2=stack.top();
            stack.pop();
            stack.push(temNum2+temNum1);
            break;
        case '-':
            temNum1=stack.top();
            stack.pop();
            temNum2=stack.top();
            stack.pop();
            stack.push(temNum2-temNum1);
            break;
        case '*':
            temNum1=stack.top();
            stack.pop();
            temNum2=stack.top();
            stack.pop();
            stack.push(temNum2*temNum1);
            break;
        case '/':
            temNum1=stack.top();
            stack.pop();
            temNum2=stack.top();
            stack.pop();
            stack.push(temNum2/temNum1);
            break;
        case '^':
            temNum1=stack.top();
            stack.pop();
            temNum2=stack.top();
            stack.pop();
            stack.push(pow(temNum2,temNum1));
            break;
        case '.':
            str.push_back(*i);
            break;
        case'#':
            curr=str.toDouble();
            str.clear();
            stack.push(curr);
            break;
        case'%':
            curr=stack.top();
            stack.pop();
            curr*=0.01;
            stack.push(curr);
            break;
        case's':
            temNum1=stack.top();
            stack.pop();
            stack.push(std::sin(temNum1/180.0*M_PI));
            break;
        case'c':
            temNum1=stack.top();
            stack.pop();
            stack.push(std::cos(temNum1/180.0*M_PI));
            break;
        case't':
            temNum1=stack.top();
            stack.pop();
            stack.push(std::tan(temNum1/180.0*M_PI));
            break;
        case'n':
            temNum1=stack.top();
            stack.pop();
            stack.push(std::log10(temNum1));
            break;
        case'g':
            temNum1=stack.top();
            stack.pop();
            stack.push(std::log(temNum1));
            break;

        default:
            throw "illeaglity";
            break;
        }
    }
    curr=stack.top();
    return curr;
}


void MainWindow::on_action_triggered()//实现粘贴功能
{
    QClipboard *board= QApplication::clipboard();
    QString text=board->text();
    ui->lineEdit->setText(text);

}

void MainWindow::on_action_2_triggered()//显示历史记录
{
    if(log.isEmpty()==true)
        log="No History";
    ui->lineEdit->setText(log);
    waitForOperand=true;
}

void MainWindow::on_action_3_triggered()//清除历史记录
{
    log.clear();
}

void MainWindow::on_action_4_triggered()
{


        ui->percrntBtn->show();
        ui->sinBtn->show();
        ui->cosBtn->show();
        ui->tanBtn->show();
        ui->lnBtn->show();
        ui->lgBtn->show();

}

void MainWindow::on_action_5_triggered()
{
    ui->percrntBtn->hide();
    ui->sinBtn->hide();
    ui->cosBtn->hide();
    ui->tanBtn->hide();
    ui->lnBtn->hide();
    ui->lgBtn->hide();
}
