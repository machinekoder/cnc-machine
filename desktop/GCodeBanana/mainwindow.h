#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QSettings>
#include <QFileInfo>
#include "qgcodeparser.h"
#include "codeeditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void codeChanged(bool valid);

    void on_loadFileButton_clicked();

    void on_refreshButton_clicked();

private:
    QGCodeParser *gcodeParser;

    QGraphicsScene *scene;
    QList<QGraphicsLineItem*> previewItems;

    QString settingsDir;

    //setting loading and saving to ini
    void loadSettings();
    void saveSettings();

    void previewGCode(QString code);
    void translateGCode(QString code, double x, double y, double z);
    void refreshPreview();
    void clearPreview();
    
private:
    Ui::MainWindow *ui;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
