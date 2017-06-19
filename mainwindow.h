#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QSignalMapper>
#include <QIcon>
#include "widgets.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    enum {NumOfDigitButton = 10};

    void initialize();
    void addWidgets();
    void windowResize();

    void addLineEdit();
    void createButtons();
    void mapDigitButtons();
    void mapControlButtons();

    void filterInput(QString input);
    const QString cycleMore();
    const QString cycleOnce(QString command);
    void clearAll();
    void clearScreen();
    void _register(QString command = "=");
    void re_register();
    const QString abort();

    void backspaceClicked();
    void changeSignClicked();
    void _cycleMore(QString input);
    void _cycleOnce(QString input);
    void equalClicked(QString);
    void dotClicked(QString input);
    void zeroClicked(QString input);
    void otherDigitClicked(QString input);

signals:
    void clicked(QString);

protected slots:
    void keypressedText(QString text);

private:
    LineEdit *screen;
    QGridLayout *gl;
    QSignalMapper *signalMapper;
    Button *button[NumOfDigitButton];
    QList<Button *> buttonList;
    QIcon icon;

    QString leftV, rightV, _operator;

    bool DO_CALC;
    bool WAITING;
    bool RESET;
    bool ERROR;
    bool PAUSE;
    bool EQUAL_C;
    bool DONE;
};

#endif // MAINWINDOW_H
