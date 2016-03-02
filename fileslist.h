#ifndef FILESLIST_H
#define FILESLIST_H

#include <QListWidget.h>

class QDir;

class FilesList : public QListWidget
{
    Q_OBJECT
public:
    //const int FilePath = 33;
    //const int Image = 34;
    explicit FilesList(QWidget *parent = 0);
    ~FilesList();

signals:
    void hasSelectedItems(bool);
private slots:
    bool readFolder();
    void closeFolder();
    void upateSelectionSignal();
private:
    QStringList filters;
    QDir *imagesFolder;

};

#endif // FILESLIST_H
