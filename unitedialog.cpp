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
UniteDialog::UniteDialog(QListIterator<QListWidgetItem *> it, QWidget *parent) : QDialog(parent)
{
    this->setModal(1);
    tabWidget = new QTabWidget(this);
    //Creating Files Tab Widget
    QWidget *filesTab = new QWidget;

    filesList = new QListWidget;
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
    addButton = new QPushButton(tr("Add"));

    QGridLayout *filesTabLayout = new QGridLayout;
    filesTabLayout->addWidget(filesList,1,1,2,2);
    filesTabLayout->addWidget(mvUpButton,3,1);
    filesTabLayout->addWidget(mvDownButton,3,2);
    filesTabLayout->addWidget(deleteButton,4,1);
    filesTabLayout->addWidget(addButton,4,2);
    filesTab->setLayout(filesTabLayout);

    tabWidget->addTab(filesTab,tr("Files"));
    //Creating Settings Tab Widget
    QWidget *settingsTab = new QWidget;

    QLabel *layoutLabel = new QLabel(tr("Layout:"));
    layoutComboBox = new QComboBox;
    layoutComboBox->addItem(tr("Vertical"));
    layoutComboBox->addItem(tr("Horizontal"));
    layoutComboBox->addItem(tr("Grid"));
    QVBoxLayout *layoutLayout = new QVBoxLayout;
    layoutLayout->addWidget(layoutLabel);
    layoutLayout->addWidget(layoutComboBox);

    QLabel *modeLabel = new QLabel(tr("Mode:"));
    modeComboBox = new QComboBox;
    modeComboBox->addItem(tr("Normal"));
    modeComboBox->addItem(tr("Scaled"));
    QVBoxLayout *modeLayout = new QVBoxLayout;
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(modeComboBox);

    gridSizeLabel = new QLabel(tr("Grid Size:"));
    gridSizeLabel->hide();
    QLabel *rowsLabel = new QLabel(tr("Rows:")),
            *colummnsLabel = new QLabel(tr("Columns:"));
    xSpinBox = new QSpinBox;
    ySpinBox = new QSpinBox;
    QGridLayout *gridSizeFrameLayout = new QGridLayout;
    gridSizeFrameLayout->addWidget(rowsLabel,1,1);
    gridSizeFrameLayout->addWidget(xSpinBox,1,2);
    gridSizeFrameLayout->addWidget(colummnsLabel,2,1);
    gridSizeFrameLayout->addWidget(ySpinBox,2,2);
    gridSizeFrame = new QFrame;
    gridSizeFrame->setLayout(gridSizeFrameLayout);
    gridSizeFrame->setFrameShape(QFrame::Panel);
    gridSizeFrame->hide();
    QVBoxLayout *gridSizeLayout = new QVBoxLayout;
    gridSizeLayout->addWidget(gridSizeLabel);
    gridSizeLayout->addWidget(gridSizeFrame);

    QVBoxLayout *settingsTabLayout = new QVBoxLayout;
    settingsTabLayout->addLayout(layoutLayout);
    settingsTabLayout->addLayout(gridSizeLayout);
    settingsTabLayout->addLayout(modeLayout);
    settingsTabLayout->addStretch();
    settingsTab->setLayout(settingsTabLayout);

    tabWidget->addTab(settingsTab,tr("Settings"));
    //Creating Dialog Layout
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(tabWidget);
    previewLabel = new QLabel;
    previewLabel->setText("preview label");
    mainLayout->addWidget(previewLabel);

    this->setLayout(mainLayout);
    updatePreview();
    //Connecting
    connect(filesList,SIGNAL(itemSelectionChanged()),this,SLOT(updateDialog()));
    connect(mvDownButton,SIGNAL(clicked()),this,SLOT(listMoveDown()));
    connect(mvUpButton,SIGNAL(clicked()),this,SLOT(listMoveUp()));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(listDelete()));
    connect(layoutComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(showGidSize(int)));
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
    int newH=0,newW=0;
    for(int i=0;i<filesList->count();i++)
    {
        QListWidgetItem* item = filesList->item(i);
        QString str = item->text();
        QImage *image = new QImage(str);
        images.push_back(image);
        newW+=image->width();
        newH = qMax(newH,image->height());
    }
    QImage *previewImage = new QImage(newW,newH,QImage::Format_RGB32);
    QPainter previewPainter(previewImage);

    int x=0;
    for(int i=0;i<filesList->count();i++)
    {
        int y = (newH - images[i]->height()) / 2;
        previewPainter.drawImage(x,y,*images[i]);
        x+=images[i]->width();
    }
    previewPainter.end();
    previewLabel->setFixedSize(600,600);
    previewLabel->setPixmap(QPixmap::fromImage(previewImage->scaled(600,600)));

}

UniteDialog::~UniteDialog()
{

}
