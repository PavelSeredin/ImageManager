#include <QtWidgets>
#include "mainwindow.h"
#include "fileslist.h"
#include "resizedialog.h"
#include "unitedialog.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    fileChanged=0;
    fileOpened=0;
    folderOpened=0;

    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);

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

void MainWindow::createActions()
{
    openFileAct = new QAction(tr("Open File"), this);
    openFileAct->setShortcut(tr("Ctrl+O"));
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));

    closeFileAct = new QAction(tr("Close File"),this);
    closeFileAct->setDisabled(1);
    connect(closeFileAct,SIGNAL(triggered()), this, SLOT(closeFile()));

    saveFileAct = new QAction(tr("Save File"),this);
    saveFileAct->setDisabled(1);
    connect(saveFileAct, SIGNAL(triggered()), this, SLOT(saveImage()));

    saveFileAsAct = new QAction(tr("Save File As"),this);
    saveFileAsAct->setDisabled(1);
    connect(saveFileAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));


    openFolderAct = new QAction(tr("Open Folder"),this);
    openFolderAct->setShortcut(tr("Ctrl+F"));
    connect(openFolderAct, SIGNAL(triggered()), this, SLOT(openFolder()));

    closeFolderAct = new QAction(tr("Close Folder"),this);
    closeFolderAct->setDisabled(1);
    connect(closeFolderAct, SIGNAL(triggered()), this, SLOT(closeFolder()));

    deleteAct = new QAction(tr("Delete File"),this);
    deleteAct->setDisabled(1);
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteFile()));

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

    tiltCorrectionAct = new QAction(tr("Tilt Correction"),this);
    tiltCorrectionAct->setDisabled(1);
    connect(tiltCorrectionAct,SIGNAL(triggered()),this,SLOT(tiltCorrection()));

    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showImage(QListWidgetItem*)));
    connect(listWidget,SIGNAL(hasSelectedItems(bool)),uniteImagesAct,SLOT(setEnabled(bool)));
    connect(listWidget,SIGNAL(hasSelectedItems(bool)),deleteAct,SLOT(setEnabled(bool)));
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(closeFileAct);
    fileMenu->addAction(saveFileAct);
    fileMenu->addAction(saveFileAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(openFolderAct);
    fileMenu->addAction(closeFolderAct);
    fileMenu->addAction(deleteAct);
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
    editMenu->addAction(tiltCorrectionAct);

    turnMenu = new QMenu(tr("Turn"),this);
    turnMenu->setDisabled(1);
    turnMenu->addAction(turnLeftAct);
    turnMenu->addAction(turnRightAct);

    editMenu->addMenu(turnMenu);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(editMenu);
}

void MainWindow::openFile()
{
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File"), QDir::currentPath());
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.selectFile(windowFilePath());
    while (dialog.exec() == QDialog::Accepted && !openedImage->save(dialog.selectedFiles().first())) {}
}

void MainWindow::saveImage()
{
    openedImage->save(windowFilePath());
    fileChanged=0;
    updateMenus();
}

void MainWindow::closeFile()
{
    if(fileChanged && (QMessageBox::question(this, "Close file","Save Changes?",QMessageBox::Yes|QMessageBox::No)== QMessageBox::Yes))
    {
        this->saveImage();
    }
    delete openedImage;
    openedImage=NULL;
    fileOpened=false;
    fileChanged=false;
    setWindowFilePath(QString());
    imageLabel->setPixmap(QPixmap());
    imageLabel->adjustSize();
    updateMenus();
}

void MainWindow::deleteFile()
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
            if(curFile.fileName()==windowFilePath())
            {
                setWindowFilePath(QString());
                imageLabel->setPixmap(QPixmap());
                imageLabel->adjustSize();
            }
            curFile.remove();
        }

    }
    updateMenus();
}

void MainWindow::openFolder()
{
    folderOpened = listWidget->readFolder();
    setWindowFilePath(listWidget->getFolder());
    updateMenus();
}

void MainWindow::closeFolder()
{
    listWidget->closeFolder();
    folderOpened = false;
    updateMenus();
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
    updateMenus();
}

void MainWindow::resizeImage()
{
    QSize* imageNewSize = new QSize(imageLabel->size());
    ResizeDialog rD(this,imageNewSize);
    if(rD.exec() == QDialog::Accepted)
    {
        QImage tmpImage = openedImage->scaled(*imageNewSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        delete openedImage;
        openedImage = new QImage(tmpImage);
        imageLabel->setPixmap(QPixmap::fromImage(*openedImage));
        imageLabel->adjustSize();
    }
    fileChanged=1;
    updateMenus();
}

void MainWindow::turnImage()
{
    QMatrix matrix;
    if(this->sender()==turnLeftAct)
    {
        matrix.rotate(-90);
    }
    else
    {
        matrix.rotate(90);
    }
    QImage tmpImage = openedImage->transformed(matrix);
    delete openedImage;
    openedImage = new QImage(tmpImage);
    imageLabel->setPixmap(QPixmap::fromImage(*openedImage));
    imageLabel->adjustSize();
    fileChanged=1;
    updateMenus();
}

void MainWindow::uniteImages()
{
    QList<QListWidgetItem*> imagesToUnite = listWidget->selectedItems();
    QListIterator<QListWidgetItem*> it(imagesToUnite);
    UniteDialog uD(it,this);
    uD.exec();
    foreach (QListWidgetItem* iter, imagesToUnite)
    {
        delete iter;
    }


}

void MainWindow::updateMenus()
{
    openFileAct->setEnabled(!fileOpened);
    closeFileAct->setEnabled(fileOpened);
    saveFileAct->setEnabled(fileChanged && fileOpened);
    saveFileAsAct->setEnabled(fileOpened);

    openFolderAct->setEnabled(!folderOpened);
    closeFolderAct->setEnabled(folderOpened);

    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
    closeFolderAct->setEnabled(!listWidget->isHidden());


    uniteImagesAct->setEnabled(folderOpened);
    resizeImageAct->setEnabled(fileOpened);
    turnMenu->setEnabled(fileOpened);
    turnLeftAct->setEnabled(fileOpened);
    turnRightAct->setEnabled(fileOpened);
    tiltCorrectionAct->setEnabled(fileOpened);
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

bool MainWindow::loadFile(const QString &fileName)
{
    delete openedImage;
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

    if (!fitToWindowAct->isChecked())
         imageLabel->adjustSize();

    fileOpened=true;
    updateMenus();
    setWindowFilePath(fileName);
    return true;
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

void MainWindow::tiltCorrection()
{
    QImage srcImg = openedImage->convertToFormat(QImage::Format_RGB32);
    cv::Mat src( srcImg.height(), srcImg.width(), CV_8UC4, const_cast<uchar*>(srcImg.bits()), srcImg.bytesPerLine() );
    //imshow("opened",src);

    Mat src_scaled = Mat::zeros((500.0*src.rows)/src.cols, 500, src.type());

    Point center = Point(0,0);
    double scale = 500.0/src.cols;

    Mat mat_scale = getRotationMatrix2D(center,0.0,scale);

    warpAffine(src, src_scaled, mat_scale, src_scaled.size());
    //imshow("scaled",src_scaled);

    Mat canny=Mat::zeros(src_scaled.rows,src_scaled.cols,src_scaled.type());
    Canny(src_scaled,canny,50,200);
    //imshow("canny",canny);

    std::vector<Vec4i> lines;
    HoughLinesP(canny, lines, 1, CV_PI/180, 50, 50, 10 );
    float tiltangle = 0.0;
    for( size_t i = 0; i < lines.size(); i++ )
    {
      Vec4i l = lines[i];
      if(l[0]==l[2])
          continue;
      if(l[1]==l[3])
          continue;
      float tg = 1.0*(l[1]-l[3])/(l[0]-l[2]);
      float angle = (180.0*qAtan(tg))/CV_PI;
      int K=0;
      if(tg<-1)
         K=1;
      if(tg>1)
          K=-1;
      tiltangle+=angle + K*90;
    }
    tiltangle/=lines.size();

    center= Point(src.cols/2,src.rows/2);
    Mat mat_tilt = getRotationMatrix2D(center,tiltangle,1.0);
    Mat result = Mat::zeros(src.rows,src.cols,src.type());
    warpAffine(src,result,mat_tilt,result.size());

    delete openedImage;
    openedImage = new QImage((uchar*) result.data, result.cols, result.rows, result.step, QImage::Format_ARGB32);

    imageLabel->setPixmap(QPixmap::fromImage(*openedImage));
    imageLabel->adjustSize();
    fileChanged=1;
    updateMenus();
    //imshow("result",result);

}

MainWindow::~MainWindow()
{

}
