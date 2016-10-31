#ifndef UNITEDIALOG_H
#define UNITEDIALOG_H

#include <QDialog>

class QTabWidget;
class QLabel;
class QComboBox;
class QSpinBox;
class QListWidget;
class QFrame;
class QListWidgetItem;

class UniteDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UniteDialog(QListIterator<QListWidgetItem*> it, QWidget *parent = 0);
    ~UniteDialog();

signals:

private slots:
    void updateDialog();
    void listMoveUp();
    void listMoveDown();
    void listDelete();
    void showGidSize(int);
    void updatePreview();
    void saveImage();

private:
    QTabWidget *tabWidget;
    QLabel *previewLabel,*gridSizeLabel;
    QComboBox *layoutComboBox, *modeComboBox;
    QPushButton *mvUpButton, *mvDownButton, *deleteButton, *saveButton, *updateButton;
    QSpinBox *xSpinBox, *ySpinBox;
    QListWidget *filesList;
    QFrame *gridSizeFrame;
    QImage *previewImage;


};

#endif // UNITEDIALOG_H
