#ifndef WIDGETS_H
#define WIDGETS_H

#include <QPushButton>
#include <QLineEdit>
#include <QString>

class Button : public QPushButton{
public:
    Button(const QString text, QWidget *parent = 0);
    ~Button();
    void decorateButton();
    const QString getText();

};



class LineEdit : public QLineEdit{
public:
    LineEdit(QString text, QWidget *parent = 0);
    ~LineEdit();

    void display(QString input, bool cls);
    virtual QSize sizeHint();
    void decorateLineEdit();

private:
    QString temp;

};

#endif // WIDGETS_H
