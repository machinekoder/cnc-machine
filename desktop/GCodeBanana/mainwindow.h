#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
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

private:
    QGCodeParser *gcodeParser;

    QGraphicsScene *scene;

    void previewGCode(QString code);
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
