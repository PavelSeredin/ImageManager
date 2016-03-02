#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

class QCheckBox;
class QLineEdit;
class QLabel;
class QPushButton;

class ResizeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResizeDialog(QWidget *parent = 0, QSize* imageSize = 0);
    ~ResizeDialog();

signals:

private slots:
    void updateWidgets(const QString& text);
    void okClicked();
private:
    double proportion;
    QSize *resultSize;
    QLabel *wLabel, *hLabel;
    QLineEdit *wLineEdit, *hLineEdit;
    QCheckBox *checkBox;
    QPushButton *okButton, *cancelButton;
};

#endif // RESIZEDIALOG_H
