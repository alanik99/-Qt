#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
class QLabel; class QPushButton; class QLineEdit;
class QHBoxLayout; class QVBoxLayout; class QMessageBox;

class InputReadingsDialog : public QDialog
{
    Q_OBJECT
public:
    InputReadingsDialog(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *) override;

signals:
    void nameAndMeas(const QString&, const float);
    void nameAndMeas(const QString&, const float, const float);
    void closing();

private slots:
    void okClicked();
    void DisplaySlot(const QString&);

private:
    QLineEdit* nameline;
    QLineEdit* defline;
    QLineEdit* Fline;
    QLineEdit* inputMeasline;
    QWidget* inputDefAndForce;
    QWidget* inputMeas;
};

#endif // DIALOG_H
