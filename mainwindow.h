#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


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

private:
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    FilesList *listWidget;
    QSplitter *mainSplitter;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;
    QImage *openedImage;
    bool fileOpened,folderOpened;

    QAction *openAct;
    QAction *closeFileAct;
    QAction *deleteAct;
    QAction *openFolderAct;
    QAction *closeFolderAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *resizeImageAct;
    QAction *turnLeftAct;
    QAction *turnRightAct;
    QAction *uniteImagesAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *turnMenu;


signals:

private slots:
    void open();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void showImage(QListWidgetItem* img);
    void deleteImage();
    void resizeImage();
    void turnImage();
    void uniteImages();
    void updateMenus();
};

#endif // MAINWINDOW_H
