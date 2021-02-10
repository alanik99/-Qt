#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "inputReadingsDialog.h"

InputReadingsDialog::InputReadingsDialog(QWidget *parent)
    : QDialog(parent)
{
    QLabel* namelbl = new QLabel("Input name:");
    nameline = new QLineEdit;

    QHBoxLayout* toplayout = new QHBoxLayout;
    toplayout->addWidget(namelbl);
    toplayout->addWidget(nameline);

    inputDefAndForce = new QWidget(this);

    QLabel* deflbl = new QLabel("Input Deformation:");
    defline = new QLineEdit;
    QHBoxLayout* deflayout = new QHBoxLayout;
    deflayout->addWidget(deflbl);
    deflayout->addWidget(defline);

    QLabel* Flbl = new QLabel("Input Force:");
    Fline = new QLineEdit;
    QHBoxLayout* Flayout = new QHBoxLayout;
    Flayout->addWidget(Flbl);
    Flayout->addWidget(Fline);

    QVBoxLayout* v = new QVBoxLayout;
    v->addLayout(deflayout);
    v->addLayout(Flayout);
    inputDefAndForce->setLayout(v);
    inputDefAndForce->setVisible(false);

    inputMeas = new QWidget(this);
    QLabel* inputMeaslbl = new QLabel("Input Measurement:");
    inputMeasline = new QLineEdit;

    QHBoxLayout* inputMeaslayout = new QHBoxLayout;
    inputMeaslayout->addWidget(inputMeaslbl);
    inputMeaslayout->addWidget(inputMeasline);
    inputMeas->setLayout(inputMeaslayout);
    inputMeas->setVisible(false);

    QPushButton* btn = new QPushButton("Ok");
    btn->setFixedSize(80, 25);
    connect(btn, SIGNAL(clicked()), this, SLOT(okClicked()));

    QHBoxLayout* oklayout = new QHBoxLayout;
    oklayout->setSpacing(0);
    oklayout->addWidget(btn);
    oklayout->setSpacing(0);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(toplayout);
    layout->addWidget(inputDefAndForce);
    layout->addWidget(inputMeas);
    layout->addLayout(oklayout);

    connect(nameline, SIGNAL(textChanged(const QString&)), this, SLOT(DisplaySlot(const QString&)));
}

void InputReadingsDialog::okClicked()
{
    if(inputMeas->isVisible())
        emit nameAndMeas(nameline->text(), inputMeasline->text().toFloat());

    else if(inputDefAndForce->isVisible())
        emit nameAndMeas(nameline->text(), defline->text().toFloat(), Fline->text().toFloat());
    else
        QMessageBox::warning(this, "Warning", "Error! Incorrect name!");
}

void InputReadingsDialog::DisplaySlot(const QString& s)
{
    if(s.contains("TD"))
    {
        inputDefAndForce->setVisible(true);
        inputMeas->setVisible(false);
        update();
    }
    else if(s.contains("DD") || s.contains("DT"))
    {
        inputDefAndForce->setVisible(false);
        inputMeas->setVisible(true);
    }
}

void InputReadingsDialog::closeEvent(QCloseEvent *)
{
    emit closing();
}
