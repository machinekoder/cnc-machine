#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //nasty workaround for finding the settings directory
#ifndef PORTABLE
    QSettings tmpConfig(QSettings::IniFormat, QSettings::UserScope, "qremotecontrol", "settings");
    settingsDir = QFileInfo(tmpConfig.fileName()).absolutePath() + "/";
#else
    settingsDir = QDir::currentPath() + "/settings/";
#endif
    loadSettings();

    gcodeParser = new QGCodeParser(ui->textEdit, this);

    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(Qt::white));
    ui->graphicsView->setScene(scene);

    int width = 200;
    int height = 200;
    int stepX = 20;
    int stepY = 20;

    QPen gridPen(Qt::gray);

    scene->addRect(0,0,width,height, gridPen);

    for (int x = 0; x < width; x += stepX)
    {
        scene->addLine(x, 0, x, height, gridPen);
    }

    for (int y = 0; y < height; y += stepY)
    {
        scene->addLine(0, y, width, y, gridPen);
    }

    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    connect(gcodeParser, SIGNAL(codeChanged(bool)),
            this, SLOT(codeChanged(bool)));

    ui->previewDock->installEventFilter(this);
    ui->graphicsView->installEventFilter(this);
}

void MainWindow::loadSettings()
{
    QSettings settings(settingsDir + "settings.ini", QSettings::IniFormat);

    settings.beginGroup("window");
        this->restoreState(settings.value("state", QByteArray()).toByteArray());
        this->restoreGeometry(settings.value("geometry", QByteArray()).toByteArray());
    settings.endGroup();
}

void MainWindow::saveSettings()
{
    QSettings settings(settingsDir + "settings.ini", QSettings::IniFormat);
    settings.clear();

    settings.beginGroup("window");
        settings.setValue("state",this->saveState());
        settings.setValue("geometry", this->saveGeometry());
    settings.endGroup();
}


MainWindow::~MainWindow()
{
    saveSettings();

    delete ui;
}

void MainWindow::codeChanged(bool valid)
{
    if (valid)
    {
        previewGCode(ui->textEdit->toPlainText());
    }
    else
    {
        clearPreview();
    }
}

void MainWindow::on_loadFileButton_clicked()
{
    QFile file(":/test/test2");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString text(file.readAll());
        ui->textEdit->setPlainText(text);
    }
}

void MainWindow::previewGCode(QString code)
{
    QStringList lines = code.split("\n");

    clearPreview();

    QPen pen(Qt::black);
    double globalX = 0.0;
    double globalY = 0.0;

    foreach (QString line, lines)
    {
        line = line.trimmed();
        if (!line.isEmpty())
        {
            if (line.indexOf("G00") == 0)
            {
                double x = 0.0;
                double y = 0.0;

                QStringList commands = line.split(" ");

                foreach (QString command, commands)
                {
                    if (command.indexOf("X") == 0)
                    {
                        command.remove(0,1);
                        x = command.toDouble();
                    }
                    else if (command.indexOf("Y") == 0)
                    {
                        command.remove(0,1);
                        y = command.toDouble();
                    }
                }

                globalX = x;
                globalY = y;
            }
            else if (line.indexOf("G01") == 0)
            {
                double x = 0.0;
                double y = 0.0;

                QStringList commands = line.split(" ");

                foreach (QString command, commands)
                {
                    if (command.indexOf("X") == 0)
                    {
                        command.remove(0,1);
                        x = command.toDouble();
                    }
                    else if (command.indexOf("Y") == 0)
                    {
                        command.remove(0,1);
                        y = command.toDouble();
                    }
                }

                if ((x == 0.0) || (y == 0.0))
                {
                    continue;
                }

                QGraphicsLineItem *item;
                item = scene->addLine(globalX, globalY, x, y, pen);
                previewItems.append(item);
                globalX = x;
                globalY = y;
            }
        }
    }

    refreshPreview();
}

void MainWindow::translateGCode(QString code, double x, double y, double z)
{
    QStringList lines = code.split("\n");

    foreach (QString line, lines)
    {
        line = line.trimmed();
        if (!line.isEmpty())
        {
            if (line.indexOf("G00") == 0)
            {
                double x = 0.0;
                double y = 0.0;
                double z = 0.0;

                QStringList commands = line.split(" ");

                foreach (QString command, commands)
                {
                    if (command.indexOf("X") == 0)
                    {
                        command.remove(0,1);
                        x = command.toDouble();
                    }
                    else if (command.indexOf("Y") == 0)
                    {
                        command.remove(0,1);
                        y = command.toDouble();
                    }
                    else if (command.indexOf("Z") == 0)
                    {
                        command.remove(0,1);
                        z = command.toDouble();
                    }
                }
            }
            else if (line.indexOf("G01") == 0)
            {
                double x = 0.0;
                double y = 0.0;

                QStringList commands = line.split(" ");

                foreach (QString command, commands)
                {
                    if (command.indexOf("X") == 0)
                    {
                        command.remove(0,1);
                        x = command.toDouble();
                    }
                    else if (command.indexOf("Y") == 0)
                    {
                        command.remove(0,1);
                        y = command.toDouble();
                    }
                    else if (command.indexOf("Z") == 0)
                    {
                        command.remove(0,1);
                        z = command.toDouble();
                    }
                }
            }
        }
    }
}

void MainWindow::refreshPreview()
{
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::clearPreview()
{
    for (int i = 0; i < previewItems.count(); ++i)
    {
        scene->removeItem(previewItems[i]);
        delete previewItems[i];
    }
    previewItems.clear();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->previewDock)
    {
        if (event->type() == QEvent::Resize)
        {
            refreshPreview();
            return true;
        }

    }
    else if (obj == ui->graphicsView)
    {
        if (event->type() == QEvent::MouseMove)
        {

        }
    }
    return QMainWindow::eventFilter(obj, event);
}
void MainWindow::on_refreshButton_clicked()
{
    refreshPreview();
}
