#ifndef DISPTYPEDIALOG_H
#define DISPTYPEDIALOG_H

#include <QDialog>
class QLabel; class QPushButton; class QLineEdit;
class QHBoxLayout; class QVBoxLayout;

class dispTypeDialog : public QDialog
{
    Q_OBJECT

public:
    enum TypeOfWidget {TYPE, MODEL_LIM, TYPE_MODEL_LIM};

    dispTypeDialog(QWidget *parent = nullptr, TypeOfWidget t = TYPE);

protected:
    void closeEvent(QCloseEvent *) override;

private:
    TypeOfWidget t;
    QLineEdit* typeline;
    QLineEdit* modelline;
    QLineEdit* limline;
    QWidget* type;
    QWidget* model;
    QWidget* lim;
    void createType();
    void createModelLim();

signals:
    void typeSignal(const QString&);
    void typeModelLimSignal(const QString&, const QString&, const float);
    void ModelLimSignal(const QString&, const float);
    void closing();

private slots:
    void okClicked();
};

#endif // DISPTYPEDIALOG_H
