#include <QtWidgets>
#include "mainwindow.h"
#include "fileslist.h"
#include "resizedialog.h"
#include "unitedialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    listWidget = new FilesList();

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Dark);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    //scrollArea->setWidgetResizable(true);
    //scrollArea->setMinimumSize(QSize(480,320));
    //scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    scrollArea->setWidget(imageLabel);

    mainSplitter = new QSplitter(this);
    mainSplitter->addWidget(listWidget);
    mainSplitter->addWidget(scrollArea);
    mainSplitter->setStretchFactor(1,2);
    setCentralWidget(mainSplitter);

    createActions();
    createMenus();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

bool MainWindow::loadFile(const QString &fileName)
{
    openedImage = new QImage(fileName);
    if (openedImage->isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        imageLabel->setPixmap(QPixmap());
        imageLabel->adjustSize();
        return false;
    }
    imageLabel->setPixmap(QPixmap::fromImage(*openedImage));
    scaleFactor = 1.0;

    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
         imageLabel->adjustSize();

    fileOpened=true;
    updateMenus();
    setWindowFilePath(fileName);
    return true;
 }

void MainWindow::open()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("Open File"), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    closeFileAct = new QAction(tr("Close File"),this);
    closeFileAct->setDisabled(1);

    deleteAct = new QAction(tr("Delete"),this);
    deleteAct->setDisabled(1);
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteImage()));

    openFolderAct = new QAction(tr("Open Folder"),this);
    openFolderAct->setShortcut(tr("Ctrl+F"));
    connect(openFolderAct, SIGNAL(triggered()), listWidget, SLOT(readFolder()));

    closeFolderAct = new QAction(tr("Close Folder"),this);
    closeFolderAct->setDisabled(1);
    connect(closeFolderAct, SIGNAL(triggered()), listWidget, SLOT(closeFolder()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    resizeImageAct = new QAction(tr("Resize"),this);
    resizeImageAct->setDisabled(1);
    connect(resizeImageAct, SIGNAL(triggered()), this, SLOT(resizeImage()));

    turnLeftAct = new QAction(tr("Left"),this);
    turnLeftAct->setDisabled(1);
    connect(turnLeftAct, SIGNAL(triggered()), this, SLOT(turnImage()));

    turnRightAct = new QAction(tr("Right"),this);
    turnRightAct->setDisabled(1);
    connect(turnRightAct, SIGNAL(triggered()), this, SLOT(turnImage()));

    uniteImagesAct = new QAction(tr("Unite"),this);
    uniteImagesAct->setDisabled(1);
    connect(uniteImagesAct,SIGNAL(triggered()),this,SLOT(uniteImages()));

    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showImage(QListWidgetItem*)));
    connect(listWidget,SIGNAL(hasSelectedItems(bool)),uniteImagesAct,SLOT(setEnabled(bool)));
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(closeFileAct);
    fileMenu->addAction(deleteAct);
    fileMenu->addSeparator();
    fileMenu->addAction(openFolderAct);
    fileMenu->addAction(closeFolderAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    editMenu = new QMenu(tr("&Edit"),this);
    editMenu->addAction(resizeImageAct);
    editMenu->addAction(uniteImagesAct);

    turnMenu = new QMenu(tr("Turn"),this);
    turnMenu->setDisabled(1);
    turnMenu->addAction(turnLeftAct);
    turnMenu->addAction(turnRightAct);

    editMenu->addMenu(turnMenu);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(editMenu);
}

void MainWindow::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
    closeFolderAct->setEnabled(!listWidget->isHidden());
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::showImage(QListWidgetItem* img)
{
    QString tst = img->data(Qt::UserRole).toString();
    loadFile(tst);
}

void MainWindow::deleteImage()
{
    if(QMessageBox::question(this, "Delete","Remove?",QMessageBox::Yes|QMessageBox::No)== QMessageBox::Yes)
    {
        QList<QListWidgetItem*> files = listWidget->selectedItems();
        QListIterator<QListWidgetItem*> it(files);
        while(it.hasNext())
        {
            QListWidgetItem* curItem = it.next();
            QFile curFile(curItem->data(Qt::UserRole).toString());
            delete curItem;
            curFile.remove();
        }
    }
}

void MainWindow::resizeImage()
{
    QSize* imageNewSize = new QSize(imageLabel->size());
    ResizeDialog rD(this,imageNewSize);
    if(rD.exec() == QDialog::Accepted)
    {
        QImage tmpImage = openedImage->scaled(*imageNewSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        openedImage->~QImage();
        openedImage = new QImage(tmpImage);
        imageLabel->setPixmap(QPixmap::fromImage(*openedImage));
        imageLabel->adjustSize();
    }
}

void MainWindow::turnImage()
{
    QMatrix matrix;
    if(this->sender()==turnLeftAct)
    {
        matrix.rotate(90);
    }
    else
    {
        matrix.rotate(-90);
    }
    QImage tmpImage = openedImage->transformed(matrix);
    openedImage->~QImage();
    openedImage = new QImage(tmpImage);
    imageLabel->setPixmap(QPixmap::fromImage(*openedImage));
    imageLabel->adjustSize();
}

void MainWindow::uniteImages()
{
    QList<QListWidgetItem*> imagesToUnite = listWidget->selectedItems();
    QListIterator<QListWidgetItem*> it(imagesToUnite);
    UniteDialog uD(it,this);
    uD.exec();


}

void MainWindow::updateMenus()
{
    closeFileAct->setEnabled(fileOpened);
    deleteAct->setEnabled(fileOpened);
    resizeImageAct->setEnabled(fileOpened);
    turnMenu->setEnabled(fileOpened);
    turnLeftAct->setEnabled(fileOpened);
    turnRightAct->setEnabled(fileOpened);
}
