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
#include "communicator.h"
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

    Communicator *communicator;

    //setting loading and saving to ini
    void loadSettings();
    void saveSettings();

    void previewGCode(QString code);
    QString translateGCode(QString code, double x, double y, double z);
    void drawGrid();
    void clearPreview();
    
private:
    Ui::MainWindow *ui;

    void sendCommand(QString command);
    void logText(QString text);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void codeChanged(bool valid);
    void refreshPreview();

    void on_loadFileButton_clicked();
    void on_refreshButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_11_clicked();
    void on_sendButton_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_24_clicked();
    void on_pushButton_25_clicked();
    void on_pushButton_31_clicked();
    void on_pushButton_32_clicked();
    void on_pushButton_33_clicked();
    void on_pushButton_29_clicked();
    void on_pushButton_30_clicked();
    void on_pushButton_26_clicked();
    void on_pushButton_18_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_28_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_spinBox_2_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
