#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "disptypedialog.h"

dispTypeDialog::dispTypeDialog(QWidget *parent, TypeOfWidget t) : QDialog(parent), t(t)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    switch(t)
    {
    case TYPE:
        createType();

        layout->addWidget(type);
        break;

    case MODEL_LIM:
        createModelLim();

        layout->addWidget(model);
        layout->addWidget(lim);
        break;

    case TYPE_MODEL_LIM:
        createType();
        createModelLim();

        layout->addWidget(type);
        layout->addWidget(model);
        layout->addWidget(lim);
        break;
    }

    QPushButton* btn = new QPushButton("Ok");
    btn->setFixedSize(80, 25);

    connect(btn, SIGNAL(clicked()), this, SLOT(okClicked()));

    QHBoxLayout* oklayout = new QHBoxLayout;
    oklayout->setSpacing(0);
    oklayout->addWidget(btn);
    oklayout->setSpacing(0);

    layout->addLayout(oklayout);
    setLayout(layout);
}

void dispTypeDialog::createType()
{
    type = new QWidget(this);
    QLabel* typelbl = new QLabel("Input type:");
    typeline = new QLineEdit;

    QHBoxLayout* typelayout = new QHBoxLayout;
    typelayout->addWidget(typelbl);
    typelayout->addWidget(typeline);
    type->setLayout(typelayout);
}

void dispTypeDialog::createModelLim()
{
    model = new QWidget(this);
    QLabel* modellbl = new QLabel("Input model:");
    modelline = new QLineEdit;

    QHBoxLayout* modellayout = new QHBoxLayout;
    modellayout->addWidget(modellbl);
    modellayout->addWidget(modelline);
    model->setLayout(modellayout);

    lim = new QWidget(this);
    QLabel* limlbl = new QLabel("Input limit:");
    limline = new QLineEdit;

    QHBoxLayout* limlayout = new QHBoxLayout;
    limlayout->addWidget(limlbl);
    limlayout->addWidget(limline);
    lim->setLayout(limlayout);
}

void dispTypeDialog::okClicked()
{
    switch(t)
    {
    case TYPE:
        emit typeSignal(typeline->text());
        break;

    case MODEL_LIM:
        emit ModelLimSignal(modelline->text(), limline->text().toFloat());
        break;

    case TYPE_MODEL_LIM:
        emit typeModelLimSignal(typeline->text(), modelline->text(), limline->text().toFloat());
        break;
    }
}

void dispTypeDialog::closeEvent(QCloseEvent *)
{
    emit closing();
}
