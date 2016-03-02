#include "fileslist.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

FilesList::FilesList(QWidget *parent) : QListWidget(parent)
{
    filters << "*.jpg" << "*.png";
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setMinimumWidth(300);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
    this->setIconSize(QSize(100,75));
    this->hide();
}

bool FilesList::readFolder()
{

    imagesFolder = new QDir(QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks));

    QFileInfoList imagesList = imagesFolder->entryInfoList(filters,QDir::Files);
    if(imagesList.empty())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Folder %1. is empty").arg(QDir::toNativeSeparators(imagesFolder->absolutePath())));
        imagesFolder->~QDir();
        return 0;
    }
    QListIterator<QFileInfo> i(imagesList);
    while(i.hasNext())
    {
        QFileInfo cur = i.next();
        QImage *img = new QImage(cur.absoluteFilePath());
        QListWidgetItem* it = new QListWidgetItem(cur.fileName(),this);
        it->setIcon(QPixmap::fromImage(img->scaled(800,600).scaled(100,75,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
        it->setData(Qt::UserRole,cur.absoluteFilePath());

    }

    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(upateSelectionSignal()));

    this->show();
    return 1;
}

void FilesList::closeFolder()
{
    this->hide();
    this->clear();
    imagesFolder->~QDir();
}

void FilesList::upateSelectionSignal()
{
    emit hasSelectedItems(!this->selectedItems().empty());
}

FilesList::~FilesList()
{

}
