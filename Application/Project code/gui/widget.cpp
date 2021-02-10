#include <QWidget>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <algorithm>
#include "inputReadingsDialog.h"
#include "disptypedialog.h"
#include "Sensors.h"
#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    readFromFile();

    table = new QTableWidget;
    table->setRowCount(8);
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels(QStringList()
    << "Type:" << "Name:" << "Location:" << "Measurement:"
    << "Deformation:" << "Force:" << "Limit:" << "Model:" << "Precision:");

    table->horizontalHeader()->setStretchLastSection(true);
    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    table->setColumnWidth(7, 80);

    box = new QComboBox;
    box->addItem("1. Input the sensor readings by typing its name");
    box->addItem("2. Display list of sensors placed on the stand");
    box->addItem("3. Display list of sensors of the certain type");
    box->addItem("4. Display list of sensors for which the value exceeded the limit");
    box->addItem("5. Remove the selected sensor from the system");
    box->addItem("6. Enter the limit of the sensor by specifying its model");

    btn = new QPushButton("Ok");
    btn->setMaximumSize(80, 40);
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btn_clicked()));

    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(btn);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(table);
    layout->addWidget(box);
    layout->addLayout(hlayout);

    setLayout(layout);
    setMinimumSize(620, 310);

    fillWholeTable();
}

void Widget::on_btn_clicked()
{
    const QCharRef num = box->currentText()[0];

    if(num == '1')
    {
        d = new InputReadingsDialog(this);
        d->show();
        btn->setEnabled(false);

        connect(d, SIGNAL(closing()), this, SLOT(unblockBtn()));

        connect(d, SIGNAL(nameAndMeas(const QString&, const float)), this, SLOT(inputMeas(const QString&, const float)));

        connect(d, SIGNAL(nameAndMeas(const QString&, const float, const float)),
                this, SLOT(inputMeas(const QString&, const float, const float)));
    }

    else if(num == '2')
    {
        table->setRowCount(sensors.size());
        fillWholeTable();
    }

    else if(num == '3')
    {
       disptypedialog = new dispTypeDialog(this, dispTypeDialog::TYPE);
       disptypedialog->show();
       btn->setEnabled(false);

       connect(disptypedialog, SIGNAL(closing()), this, SLOT(unblockBtn()));

       connect(disptypedialog, SIGNAL(typeSignal(const QString&)), this, SLOT(type(const QString&)));
    }

    else if(num == '4')
    {
        fillWholeTable();
        for(int i = 0; i < table->rowCount(); ++i)
            if(table->item(i,3)->text().toFloat() <= table->item(i,6)->text().toFloat())
                for(int j = 0; j < table->columnCount(); ++j)
                   table->item(i, j)->setText("");
    }

    else if(num == '5')
    {
       disptypedialog = new dispTypeDialog(this, dispTypeDialog::TYPE_MODEL_LIM);
       disptypedialog->show();
       btn->setEnabled(false);

       connect(disptypedialog, SIGNAL(closing()), this, SLOT(unblockBtn()));

       connect(disptypedialog, SIGNAL(typeModelLimSignal(const QString&, const QString&, const float)),
               this, SLOT(typeModelLimSlot(const QString&, const QString&, const float)));
    }

    else if(num == '6')
    {
        disptypedialog = new dispTypeDialog(this, dispTypeDialog::MODEL_LIM);
        disptypedialog->show();
        btn->setEnabled(false);

        connect(disptypedialog, SIGNAL(closing()), this, SLOT(unblockBtn()));

        connect(disptypedialog, SIGNAL(ModelLimSignal(const QString&, const float)),
                this, SLOT(ModelLimSlot(const QString&, const float)));
    }
}

void Widget::inputMeas(const QString& name, const float meas)
{
    auto it = sensors.find(name);
    if(it != sensors.end())
    {
        it->second->SetMeasrment(meas);
        d->close();
        fillWholeTable();
    }
    else
        QMessageBox::warning(this, "Warning", "Error! Incorrect name!");
}
void Widget::inputMeas(const QString& name, const float eps, const float F)
{
    auto it = sensors.find(name);
    if(it != sensors.end())
    {
        it->second->SetMeasrment(eps, F);
        d->close();
        fillWholeTable();
    }
    else
        QMessageBox::warning(this, "Warning", "Error! Incorrect name!");
}

void Widget::type(const QString& type)
{
    fillWholeTable();
    if(std::find(sensorsTypes.begin(), sensorsTypes.end(), type) != sensorsTypes.end())
        for(int i = 0; i < table->rowCount(); ++i)
        {
            if(table->item(i, 0)->text() != type)
                for(int j = 0; j < table->columnCount(); ++j)
                {
                    table->item(i, j)->setText("");
                }
        }
    else
        QMessageBox::warning(this, "Warning", "Error! Incorrect type!");

}

void Widget::typeModelLimSlot(const QString& type, const QString& model, const float lim)
{
    if (type == "TD")
        QMessageBox::warning(this, "Warning", "Error! Type TD has neither model nor lim!");

    else if( std::find(sensorsTypes.begin(), sensorsTypes.end(), type) != sensorsTypes.end()
            && modelType[model] == type
            && modelLim[model] == lim)
    {
        for (auto it = sensors.begin(); it != sensors.end(); ++it)
        {
            auto GetType = it->second->GetType();
            auto GetModel = it->second->GetModel();
            auto GetLim = it->second->GetLim();

            if(GetType == type && GetModel == model && GetLim == lim)
            {
                const QString SensorWasDeleted = "Sensor was deleted.";
                const QString t = "\nType: ";
                const QString n = "\nName: ";
                const QString l = "\nLocation: ";

                const QString msg =  SensorWasDeleted +
                        t + GetType +
                        n + it->second->GetName() +
                        l + it->second->GetLocation();

                QMessageBox::information(this, "Info", msg);

                delete it->second;
                sensors.erase(it);
                disptypedialog->close();
                table->setRowCount(sensors.size());
                fillWholeTable();
            }
        }
        for(auto it = sensorsModels.begin(); it != sensorsModels.end(); it++)
            if(it->first == model)
                sensorsModels.erase(it);

        for(auto it = modelType.begin(); it != modelType.end(); it++)
            if(it->first == model)
                modelType.erase(it);

        for(auto it = modelLim.begin(); it != modelLim.end(); it++)
            if(it->first == model)
                modelLim.erase(it);

        for(auto it = sensorsTypes.begin(); it != sensorsTypes.end(); it++)
            if(*it == type)
            {
                sensorsTypes.erase(it);
                break;
            }
    }
    else
        QMessageBox::warning(this, "Warning", "Error! Check the entered data!");
}

void Widget::ModelLimSlot(const QString& model, const float lim)
{
    auto it = sensorsModels.find(model);
    if(it != sensorsModels.end())
    {
        it->second->SetLim(lim);
        disptypedialog->close();
        fillWholeTable();
    }
    else
        QMessageBox::warning(this, "Warning", "Error! Incorrect model!");
}

void Widget::unblockBtn()
{
    btn->setEnabled(true);
}

void Widget::readFromFile()
{
    QFile file(":/docs/file.txt");

    if(!file.open(QIODevice::ReadOnly))
    {
         qWarning("Error! Can't open file for reading");
    }

    QTextStream f(&file);

    while(!f.atEnd())
    {
        QString type = "";
        f >> type;

        Sensor* s = 0;

        if(type == "DD")
        {
            s = new DD();
        }
        else if(type == "DT")
        {
            s = new DT();
        }
        else if(type == "TD")
        {
            s = new TD();
        }

        if(s)
        {
            s->ReadFromFile(f);
            sensors.emplace(s->GetName(), s);
            sensorsModels.emplace(s->GetModel(), s);
            sensorsTypes.push_back(s->GetType());
            modelType.emplace(s->GetModel(), s->GetType());
            modelLim.emplace(s->GetModel(), s->GetLim());
        }
    }
    file.close();
}

void Widget::fillWholeTable()
{
    items.resize(table->columnCount());

    auto it = sensors.begin();
    for(int i = 0; i < table->rowCount(); ++i, ++it)
    {
        for(auto &elem : items)
            elem = new QTableWidgetItem();

        items[0]->setText(QString("%1").arg(it->second->GetType()));
        items[1]->setText(QString("%1").arg(it->second->GetName()));
        items[2]->setText(QString("%1").arg(it->second->GetLocation()));
        items[3]->setText(QString("%1").arg(it->second->GetMeasrment()));
        items[4]->setText(QString("%1").arg(it->second->GetEps()));
        items[5]->setText(QString("%1").arg(it->second->GetF()));
        items[6]->setText(QString("%1").arg(it->second->GetLim()));
        items[7]->setText(QString("%1").arg(it->second->GetModel()));
        items[8]->setText(QString("%1").arg(it->second->GetPrec()));

        for(int j = 0; j < table->columnCount(); ++j)
            table->setItem(i, j, items[j]);
    }
}

Widget::~Widget()
{
    for(auto &elem : items)
        delete elem;
}
