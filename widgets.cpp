#include "widgets.h"
#include <QSize>
#include <QFont>

// Button Declaration
Button::Button(const QString text,QWidget *parent) :QPushButton(text, parent)
{
    decorateButton();
}

Button::~Button()
{

}

void Button::decorateButton()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(40);

    QFont font;
    font.setFamily("Courier New");
    font.setPixelSize(14);
    font.setBold(true);

    setFont(font);
}

const QString Button::getText()
{
    return text();
}




//LineEdit Declaration
LineEdit::LineEdit(QString text, QWidget *parent):QLineEdit(text, parent)
{
    temp = "";
    decorateLineEdit();
}

LineEdit::~LineEdit()
{

}

void LineEdit::display(QString input, bool cls)
{
    if((input == "0" && text() == "0") || cls){clear(); setText(input); temp = "";}
    else{ temp += input; setText(temp);}
}


QSize LineEdit::sizeHint()
{
    return QSize(300, 22);
}

void LineEdit::decorateLineEdit()
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setFrame(true);
    setMaxLength(20);
    setAlignment(Qt::AlignRight);
    setReadOnly(true);

    QFont font;
    font.setPixelSize(24);
    font.setFamily("Courier New");
    font.setBold(true);

    setFont(font);
}
