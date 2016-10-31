#include "unitedialog.h"
#include <QTableWidget>
#include <QListWidget>
#include <QLayout>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QListWidgetItem>
#include <QPainter>
#include <QFileDialog>
UniteDialog::UniteDialog(QListIterator<QListWidgetItem *> it, QWidget *parent) : QDialog(parent)
{
    this->setModal(1);
    tabWidget = new QTabWidget();

    QWidget *filesTab = new QWidget();

    filesList = new QListWidget();
    while(it.hasNext())
    {
        new QListWidgetItem(it.next()->data(Qt::UserRole).toString(),filesList);
    }
    mvUpButton = new QPushButton(tr("Move Up"));
    mvUpButton->setDisabled(1);
    mvDownButton = new QPushButton(tr("Move Down"));
    mvDownButton->setDisabled(1);
    deleteButton = new QPushButton(tr("Delete"));
    deleteButton->setDisabled(1);
    saveButton = new QPushButton(tr("Save Image"));
    updateButton = new QPushButton(tr("Update"));
    updateButton->setEnabled(1);

    QGridLayout *filesTabLayout = new QGridLayout();
    filesTabLayout->addWidget(filesList,1,1,2,2);
    filesTabLayout->addWidget(mvUpButton,3,1);
    filesTabLayout->addWidget(mvDownButton,3,2);
    filesTabLayout->addWidget(deleteButton,4,1,1,2);
    filesTab->setLayout(filesTabLayout);

    tabWidget->addTab(filesTab,tr("Files"));

    QWidget *settingsTab = new QWidget();

    QLabel *layoutLabel = new QLabel(tr("Layout:"));
    layoutComboBox = new QComboBox();
    layoutComboBox->addItem(tr("Vertical"));
    layoutComboBox->addItem(tr("Horizontal"));
    if(filesList->count()>2)
        layoutComboBox->addItem(tr("Grid"));

    QVBoxLayout *layoutLayout = new QVBoxLayout();
    layoutLayout->addWidget(layoutLabel);
    layoutLayout->addWidget(layoutComboBox);

    gridSizeLabel = new QLabel(tr("Grid Size:"));
    gridSizeLabel->hide();
    QLabel *rowsLabel = new QLabel(tr("Rows:"));
    xSpinBox = new QSpinBox();
    xSpinBox->setValue(2);
    xSpinBox->setMinimum(2);
    xSpinBox->setMaximum(filesList->count()-1);
    QGridLayout *gridSizeFrameLayout = new QGridLayout();
    gridSizeFrameLayout->addWidget(rowsLabel,1,1);
    gridSizeFrameLayout->addWidget(xSpinBox,1,2);
    gridSizeFrame = new QFrame();
    gridSizeFrame->setLayout(gridSizeFrameLayout);
    gridSizeFrame->setFrameShape(QFrame::Panel);
    gridSizeFrame->hide();
    QVBoxLayout *gridSizeLayout = new QVBoxLayout();
    gridSizeLayout->addWidget(gridSizeLabel);
    gridSizeLayout->addWidget(gridSizeFrame);

    QVBoxLayout *settingsTabLayout = new QVBoxLayout();
    settingsTabLayout->addLayout(layoutLayout);
    settingsTabLayout->addLayout(gridSizeLayout);
    settingsTabLayout->addStretch();
    settingsTab->setLayout(settingsTabLayout);

    tabWidget->addTab(settingsTab,tr("Settings"));

    previewLabel = new QLabel();
    previewLabel->setText("preview label");

    QGridLayout *previewLayout = new QGridLayout();
    previewLayout->addWidget(previewLabel,1,1,1,2);
    previewLayout->addWidget(updateButton,2,1);
    previewLayout->addWidget(saveButton,2,2);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(previewLayout);

    this->setLayout(mainLayout);
    previewImage = new QImage();
    updatePreview();

    connect(filesList,SIGNAL(itemSelectionChanged()),this,SLOT(updateDialog()));
    connect(mvDownButton,SIGNAL(clicked()),this,SLOT(listMoveDown()));
    connect(mvUpButton,SIGNAL(clicked()),this,SLOT(listMoveUp()));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(listDelete()));
    connect(layoutComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(showGidSize(int)));
    connect(updateButton,SIGNAL(clicked()),this,SLOT(updatePreview()));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveImage()));
}

void UniteDialog::updateDialog()
{
    mvUpButton->setEnabled(filesList->currentRow()+1);
    mvDownButton->setEnabled(filesList->currentRow()+1);
    deleteButton->setEnabled(filesList->currentRow()+1);

}

void UniteDialog::listMoveDown()
{
    int currentIndex = filesList->currentRow();
    QListWidgetItem *currentItem = filesList->takeItem(currentIndex);
    filesList->insertItem(currentIndex+1, currentItem);
    filesList->setCurrentRow(currentIndex+1);
}

void UniteDialog::listMoveUp()
{
    int currentIndex = filesList->currentRow();
    QListWidgetItem *currentItem = filesList->takeItem(currentIndex);
    filesList->insertItem(currentIndex-1, currentItem);
    filesList->setCurrentRow(currentIndex-1);
}

void UniteDialog::listDelete()
{
    int currentIndex = filesList->currentRow();
     QListWidgetItem *currentItem = filesList->takeItem(currentIndex);
     currentItem->~QListWidgetItem();
     if((filesList->count()<=2)&&(layoutComboBox->count()==3))
     {
         layoutComboBox->removeItem(2);
     }
}

void UniteDialog::showGidSize(int i)
{
    if(i==2)
    {
        gridSizeLabel->show();
        gridSizeFrame->show();
    }
    else
    {
        if(!gridSizeLabel->isHidden())
        {
            gridSizeFrame->hide();
            gridSizeLabel->hide();
        }
    }
}

void UniteDialog::updatePreview()
{
   if(!previewImage->isNull())
        previewImage->~QImage();
    int layoutType = layoutComboBox->currentIndex();

    QList<QImage*> images;
    int newH=0,newW=0;

    if(layoutType==0)
    {
        for(int i=0;i<filesList->count();i++)
        {
            QListWidgetItem* item = filesList->item(i);
            QString str = item->text();
            QImage *image = new QImage(str);
            images.push_back(image);
            newH+=image->height();
            newW = qMax(newW,image->width());
        }
        previewImage = new QImage(newW,newH,QImage::Format_RGB32);
        previewImage->fill(Qt::white);
        QPainter previewPainter(previewImage);

        int y=0;
        for(int i=0;i<filesList->count();i++)
        {
            int x = (newW - images[i]->width()) / 2;
            previewPainter.drawImage(x,y,*images[i]);
            y+=images[i]->height();
        }
        previewPainter.end();
        foreach (QImage* it, images) {
            it->~QImage();
        }
        previewLabel->setFixedSize(600,600);
        previewLabel->setPixmap(QPixmap::fromImage(previewImage->scaled(600,600)));
    }
    if(layoutType==1)
    {
        for(int i=0;i<filesList->count();i++)
        {
            QListWidgetItem* item = filesList->item(i);
            QString str = item->text();
            QImage *image = new QImage(str);
            images.push_back(image);
            newW+=image->width();
            newH = qMax(newH,image->height());
        }

        previewImage = new QImage(newW,newH,QImage::Format_RGB32);
        previewImage->fill(Qt::white);
        QPainter previewPainter(previewImage);

        int x=0;
        for(int i=0;i<filesList->count();i++)
        {
            int y = (newH - images[i]->height()) / 2;
            previewPainter.drawImage(x,y,*images[i]);
            x+=images[i]->width();
        }
        previewPainter.end();
        foreach (QImage* it, images) {
            it->~QImage();
        }
        previewLabel->setFixedSize(600,600);
        previewLabel->setPixmap(QPixmap::fromImage(previewImage->scaled(600,600)));
    }
    if(layoutType==2)
    {
        int gridX = xSpinBox->value(),
            gridY = filesList->count()/gridX;
        int rowMaxHeight[gridY];
        int additionalPart = filesList->count()%gridX,
                addPartHeight=0,addPartWidth=0;

        for(int i=0; i<gridY; i++)
        {
            rowMaxHeight[i]=0;
            int rowWidth=0;
            for(int j=0; j<gridX; j++)
            {
                QListWidgetItem* item = filesList->item(i*gridX+j);
                QString str = item->text();
                QImage *image = new QImage(str);
                images.push_back(image);
                rowWidth+=image->width();
                rowMaxHeight[i]=qMax(rowMaxHeight[i],image->height());
            }

            newW=qMax(newW,rowWidth);
            newH+=rowMaxHeight[i];
        }

        if(additionalPart)
        {
            for(int k=0; k<additionalPart;k++)
              {
                  QListWidgetItem* item = filesList->item(gridY*gridX+k);
                  QString str = item->text();
                  QImage *image = new QImage(str);
                  images.push_back(image);
                  addPartWidth+=image->width();
                  addPartHeight=qMax(addPartHeight,image->height());
              }
            newW=qMax(newW,addPartWidth);
            newH+=addPartHeight;
        }


        previewImage = new QImage(newW,newH,QImage::Format_RGB32);
        previewImage->fill(Qt::white);
        QPainter previewPainter(previewImage);

        int y=0;
        for(int i=0; i<gridY; i++)
        {
            int x=0;
            for(int j=0; j<gridX; j++)
            {
                x+=images[i*gridX+j]->width();
            }
            x=(newW-x)/2;
            for(int j=0; j<gridX; j++)
            {
                QImage* img = images[i*gridX+j];
                previewPainter.drawImage
                        (x,y+(rowMaxHeight[i]-img->height())/2,*img);
                x+=img->width();
            }
            y+=rowMaxHeight[i];
        }
        if(additionalPart)
        {
            int x=0;
            for(int k=0; k<additionalPart; k++)
            {
                x+=images[gridY*gridX+k]->width();
            }
            x=(newW-x)/2;
            for(int k=0; k<additionalPart; k++)
            {
                QImage* img = images[gridY*gridX+k];
                previewPainter.drawImage
                        (x,y+(addPartHeight-img->height())/2,*img);
                x+=img->width();
            }
        }

        previewPainter.end();
        foreach (QImage* it, images) {
            it->~QImage();
        }
        previewLabel->setFixedSize(600,600);
        previewLabel->setPixmap(QPixmap::fromImage(previewImage->scaled(600,600)));
    }



}

void UniteDialog::saveImage()
{
    QString imagePath = QFileDialog::getSaveFileName( this, tr("Save File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)") );
    previewImage->save(imagePath);
}

UniteDialog::~UniteDialog()
{
    if(previewImage!=NULL)
    {
        previewImage->~QImage();
    }
}
