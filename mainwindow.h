#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class QSplitter;
class QListWidgetItem;
class QScrollBar;
class QScrollArea;
class QLabel;
class QAction;
class QMenu;
class FilesList;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    bool loadFile(const QString &);
    void QtToCvStringTransform(QString qstr);

private:
    void createActions();
    void createMenus();
    void updateMenus();

    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    FilesList *listWidget;
    QSplitter *mainSplitter;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;
    QImage *openedImage;
    bool fileOpened,folderOpened,fileChanged;
    QStringList mimeTypeFilters;
    QStringList picturesLocations;

    QAction *openFileAct;
    QAction *closeFileAct;
    QAction *deleteAct;
    QAction *openFolderAct;
    QAction *closeFolderAct;
    QAction *saveFileAct;
    QAction *saveFileAsAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *resizeImageAct;
    QAction *turnLeftAct;
    QAction *turnRightAct;
    QAction *uniteImagesAct;
    QAction *tiltCorrectionAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *turnMenu;


signals:

private slots:
    void openFile();
    void closeFile();
    void saveImage();
    void saveAs();

    void deleteFile();
    void openFolder();
    void closeFolder();

    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void showImage(QListWidgetItem* img);

    void resizeImage();
    void turnImage();
    void uniteImages();
    void tiltCorrection();
};

#endif // MAINWINDOW_H
