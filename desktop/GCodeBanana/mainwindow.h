#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QSettings>
#include <QFileInfo>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include "qgcodeparser.h"
#include "codeeditor.h"
#include <math.h>

typedef struct {
    double boardWidth;
    double boardHeight;
    double boardStepX;
    double boardStepY;
} ApplicationSettings;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showCrosshair();
    void hideCrosshair();
    void setCrosshairPos(QPointF point);
private:
    QGCodeParser *gcodeParser;

    QGraphicsScene              *scene;
    QList<QGraphicsLineItem*>   previewItems;
    QGraphicsLineItem           *hLine,
                                *vLine;

    QString settingsDir;

    ApplicationSettings applicationSettings;

    //setting loading and saving to ini
    void loadSettings();
    void saveSettings();

    void previewGCode(QString code);
    QString translateGCode(QString code, double x, double y, double z);
    void drawGrid();
    void clearPreview();
    
private:
    Ui::MainWindow *ui;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void codeChanged(bool valid);
    void refreshPreview();

    void on_loadFileButton_clicked();
    void on_refreshButton_clicked();
};

#endif // MAINWINDOW_H
