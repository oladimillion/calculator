#include "mainwindow.h"
#include <QDebug>
#include <QList>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    initialize();
    windowResize();
    addWidgets();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initialize()
{
    clearAll();
    icon = QIcon(":/icon/icon.png");
    setWindowIcon(icon);
    signalMapper = new QSignalMapper(this);
    gl = new QGridLayout(this);
}

void MainWindow::addWidgets()
{
    addLineEdit();
    createButtons();
    setLayout(gl);
}

void MainWindow::windowResize()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(310, 270);
}

void MainWindow::addLineEdit()
{
    screen = new LineEdit("0");
    gl->addWidget(screen, 0, 0, 1, 5);
}

void MainWindow::createButtons()
{

    mapDigitButtons();
    mapControlButtons();

    connect(signalMapper, SIGNAL(mapped(QString)), this, SIGNAL(clicked(QString)));
    connect(this, SIGNAL(clicked(QString)), this, SLOT(keypressedText(QString)));
}

void MainWindow::mapDigitButtons()
{
    int row = 2, col = 2;
    for(int i = NumOfDigitButton-1; i >= 0 ; i--)
    {
        button[i] = new Button(QString::number(i), this);

        if(col < 0) { col = 2; row++; }
        if(i == 0) gl->addWidget(button[i], row, 0);
        else gl->addWidget(button[i], row, col);

        col--;

        connect(button[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button[i], QString::number(i));
    }

}

void MainWindow::mapControlButtons()
{
	QList<Button *> buttonList;
	
    buttonList.append(new Button("Backspace", this));//backspace
    buttonList.append(new Button("CE", this));//clear screen
    buttonList.append(new Button("y^x", this));//power
    buttonList.append(new Button("x\302\262", this));//sq
    buttonList.append(new Button("\303\267", this));//division
    buttonList.append(new Button("x\342\210\232 y", this));//nth root
    buttonList.append(new Button("\303\227", this));//multiplication
    buttonList.append(new Button("\342\210\232", this));//sqrt
    buttonList.append(new Button("-", this));//subtraction
    buttonList.append(new Button("1/x", this));//reciprocal
    buttonList.append(new Button(".", this));//dot
    buttonList.append(new Button("\302\261", this));//changesign
    buttonList.append(new Button("+", this));//addition
    buttonList.append(new Button("=", this));//equals

    int row = 1, col = 0;
    for(int i = 0; i < buttonList.size(); i++)
    {
        if(col == 5  || col == 1) row++;
        if(i < 9 && col == 5) col = 3;
        else if(i >= 9 && col == 5) col = 1;
        if(i == 0)
        {
            gl->addWidget(buttonList[i], row, col, 1, 2);
            col += 2;
        }
        else
        {
            gl->addWidget(buttonList[i], row, col);
            col++;
        }

        connect(buttonList[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(buttonList[i], buttonList[i]->text());
    }
	
	//buttonList.clear();
}

void MainWindow::filterInput(QString input)
{
    if(input == "Backspace")
    {
        backspaceClicked();
    }
    else if(input == "CE")
    {
        clearScreen();
    }
    else if(input == "\302\261")
    {
        changeSignClicked();
    }
    else if(input == "\303\227" || input == "\303\267" || input == "-" || input == "+"
            || input == "x\342\210\232 y" || input == "y^x")
    {
        _cycleMore(input);
    }
    else if(input == "\342\210\232" || input == "x\302\262" || input == "1/x")
    {
        _cycleOnce(input);
    }
    else if(input == "=")
    {
        equalClicked(input);
    }
    else if(input == ".")
    {
        dotClicked(input);
    }
    else if(input == "0")
    {
        zeroClicked(input);
    }
    else
    {
        otherDigitClicked(input);
    }
}

void MainWindow::clearScreen()
{
    clearAll();
    screen->display("0", true);
}

void MainWindow::backspaceClicked()
{
    QString temp;

    if(DO_CALC || ERROR)return;

    WAITING = false;
    PAUSE = false;

    QString sliceText = rightV;
    sliceText.resize(sliceText.size()-1);

    if(sliceText.size() == 0 || sliceText == "-") temp ="0";
    else temp = sliceText;

    screen->display(temp, true);
    _register();

    DONE = false;
}

void MainWindow::changeSignClicked()
{
    if(ERROR || rightV == "0")return;

    QString temp = rightV;

    if(temp.startsWith("-")){ temp.remove(0, 1); }
    else{ temp.prepend("-");}

    screen->display(temp, true);
    rightV = screen->text();

    DONE = false;
}

void MainWindow::_cycleMore(QString input)
{
    if(ERROR)return;

    DO_CALC = true;
    WAITING = true;

    if(EQUAL_C){re_register();}

    QString result = cycleMore();
    if(result != "")screen->display(result, true);

    _register(input);

    if(PAUSE){ re_register();}
    DONE = true;
}

void MainWindow::_cycleOnce(QString input)
{
    if(ERROR)return;

    DO_CALC = true;
    RESET = true;
	WAITING = false;

    re_register();

    QString result = cycleOnce(input);
    if(result != "")screen->display(result, true);

    _register();
    DONE = false;
}

void MainWindow::equalClicked(QString /*input*/)
{
    if(RESET || ERROR) return;

    DO_CALC = true;
    EQUAL_C = true;
    WAITING = true;

    QString result = cycleMore();

    if(result != ""){ screen->display(result, true);}

    _register();

    if(PAUSE){ re_register();}
    DONE = false;
}

void MainWindow::dotClicked(QString input)
{
    WAITING = false;

    if(rightV.indexOf('.') != -1 || PAUSE || ERROR) return;
    if(PAUSE || EQUAL_C){screen->display("0", true); re_register();}
    if(DO_CALC || RESET){screen->display("0", true); DO_CALC = false; RESET = false;}
    QString temp = rightV;

    if(temp == "0")temp += input;
    else temp = input;

    screen->display(temp, false);
    _register();

    DONE = false;
}

void MainWindow::zeroClicked(QString input)
{
    WAITING = false;

    if(rightV == "0" || screen->text() == "0" || ERROR) return;
    if(DO_CALC || RESET){screen->display("0", true); DO_CALC = false; RESET = false;}
    if(PAUSE || EQUAL_C){screen->display("0", true); re_register();}

    screen->display(input, false);
    _register();

    DONE = false;
}

void MainWindow::otherDigitClicked(QString input)
{
    WAITING = false;

    if(ERROR)return;
    if(PAUSE || EQUAL_C){ re_register();}
    if(rightV == "0" || DO_CALC || RESET)
    {screen->display("0", true); DO_CALC = false; RESET = false;}

    screen->display(input, false);
    _register();

    DONE = false;
}

void MainWindow::_register(QString command)
{
    if(WAITING){ if(command != "=")_operator = command; leftV = screen->text();}
    else { rightV = screen->text(); }
}

void MainWindow::re_register()
{
	PAUSE = false;
    EQUAL_C = false;
	
    leftV = "0";
    _operator = "";
    rightV = screen->text();
    
}

void MainWindow::clearAll()
{
    DO_CALC = false;
    WAITING = false;
    RESET = false;
    ERROR = false;
    PAUSE = false;
    EQUAL_C = false;
    DONE = false;
    
	leftV = "0";
    rightV = "0";
    _operator = "";
}

const QString MainWindow::cycleMore()
{
    QString result;

    if(!WAITING || EQUAL_C ||!DONE)
    {
        if(_operator == "+"){ result = QString::number(leftV.toDouble() + rightV.toDouble()); }
        else if(_operator == "-"){ result = QString::number(leftV.toDouble() - rightV.toDouble()); }
        else if(_operator == "\303\227") { result = QString::number(leftV.toDouble() * rightV.toDouble()); }
        else if(_operator == "\303\267"){
            if(rightV == "0"){  result = abort();}
            else result = QString::number(leftV.toDouble() / rightV.toDouble());
        }else if(_operator == "y^x"){
            PAUSE = true;
            result = QString::number(pow(leftV.toDouble(), rightV.toDouble()));
        }else if(_operator == "x\342\210\232 y"){
            PAUSE = true;
            if(rightV < "0") { result = abort(); }
            else{
                leftV = QString::number(1/leftV.toDouble());
                result = QString::number(pow(rightV.toDouble(), leftV.toDouble()));
            }
        }
    }
    else
        result = "";

    if(result == "inf") {result = "INFINITY"; ERROR = true;}

    return result;
}

const QString MainWindow::cycleOnce(QString command)
{
    QString result;

    if(command == "1/x"){
        if(rightV == "0") { result = abort(); }
        else result = QString::number(1/(rightV.toDouble()));
    }else if(command == "x\302\262"){
        result = QString::number(pow(rightV.toDouble(), 2));
    }
    else if(command == "\342\210\232"){
        if(rightV < "0") { result = abort();}
        else result = QString::number(sqrt(rightV.toDouble()));
    }

    if(result == "inf") {result = "INFINITY"; ERROR = true;}

    return result;
}

const QString MainWindow::abort()
{
   DO_CALC = false; ERROR = true; return "ERROR";
}

void MainWindow::keypressedText(QString text)
{
    filterInput(text);
}
