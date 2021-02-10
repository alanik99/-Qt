#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QTableWidget; class QComboBox; class QPushButton; class InputReadingsDialog;
class dispTypeDialog; class QString; class Sensor; class QTableWidgetItem;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QTableWidget* table;
    QComboBox* box;
    QPushButton* btn;
    InputReadingsDialog* d;
    dispTypeDialog* disptypedialog;

    std::map <QString, Sensor*> sensors;
    std::map <QString, Sensor*> sensorsModels;
    std::map <QString, QString> modelType;
    std::map <QString, float> modelLim;
    std::vector<QString> sensorsTypes;
    std::vector <QTableWidgetItem*> items;

    void readFromFile();
    void fillWholeTable();

private slots:
    void on_btn_clicked();
    void unblockBtn();
    void inputMeas(const QString&, const float);
    void inputMeas(const QString&, const float, const float);
    void type(const QString&);
    void typeModelLimSlot(const QString&, const QString&, const float);
    void ModelLimSlot(const QString&, const float);
};

#endif // WIDGET_H
