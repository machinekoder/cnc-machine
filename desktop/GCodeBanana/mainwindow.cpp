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

    // initialize parser
    gcodeParser = new QGCodeParser(ui->textEdit, this);

    // initialize scene
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(Qt::black));
    ui->graphicsView->setScene(scene);

    // create markers
    hLine = scene->addLine(0,-10,1000,-10, QPen(QBrush(Qt::white), 0, Qt::SolidLine));
    vLine = scene->addLine(0,-10,1000,-10, QPen(QBrush(Qt::white), 0, Qt::SolidLine));
    hLine->setZValue(1);
    vLine->setZValue(1);
    hideCrosshair();

    applicationSettings.boardWidth = 200.0;
    applicationSettings.boardHeight = 200.0;
    applicationSettings.boardStepX = 10.0;
    applicationSettings.boardStepY = 10.0;

    drawGrid();

    connect(gcodeParser, SIGNAL(codeChanged(bool)),
            this, SLOT(codeChanged(bool)));

    ui->graphicsView->setMouseTracking(true);

    ui->previewDock->installEventFilter(this);
    scene->installEventFilter(this);

    QTimer::singleShot(200, this, SLOT(refreshPreview()));
}

MainWindow::~MainWindow()
{
    saveSettings();

    delete ui;
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

void MainWindow::showCrosshair()
{
     //if (ui->crosshairCheck->isChecked()) {
                hLine->setVisible(true);
                vLine->setVisible(true);
     //       }
}

void MainWindow::hideCrosshair()
{
        hLine->setVisible(false);
        vLine->setVisible(false);
}

void MainWindow::setCrosshairPos(QPointF point)
{
    vLine->setLine(point.x(), vLine->line().y1(), point.x(), vLine->line().y2());
    hLine->setLine(hLine->line().x1(), point.y(),hLine->line().x2(), point.y());
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

void MainWindow::previewGCode(QString code)
{
    double globalX = 0.0;
    double globalY = 0.0;
    double globalZ = 0.0;

    double globalF = 500.0;
    double maxF    = 500.0;
    double time    = 0.0;
    double length  = 0.0;

    QStringList lines;
    QString gCommand;

    clearPreview();
    QPen pen(Qt::red);

    lines = code.split("\n");
    foreach (QString line, lines)
    {
        line = line.trimmed();
        gCommand = "";

        if (!line.isEmpty())
        {
            if (line.indexOf("G00") == 0)
            {
                gCommand = "G00";
            }
            else if (line.indexOf("G01") == 0)
            {
                gCommand = "G01";
            }
        }

        if (gCommand != "")
        {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
            double localLength = 0.0;

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
                else if (command.indexOf("F") == 0)
                {
                    command.remove(0,1);
                    globalF = command.toDouble();
                }
            }

            if (x == 0.0)
            {
                x = globalX;
            }
            if (y == 0.0)
            {
                y = globalY;
            }
            if (z == 0.0)
            {
                z = globalZ;
            }

            localLength = sqrt(pow(globalX-x,2.0) + pow(globalY-y,2.0));
            length += localLength;

            if (gCommand == "G00")
            {
                time += localLength/maxF;
            }
            else if (gCommand == "G01")
            {
                QGraphicsLineItem *item;
                item = scene->addLine(globalX, applicationSettings.boardHeight - globalY, x, applicationSettings.boardHeight - y, pen);
                previewItems.append(item);

                time += localLength/globalF;
            }

            globalX = x;
            globalY = y;
            globalZ = z;
        }
    }

    qDebug() << time << "min";
    qDebug() << length << "mm";
    refreshPreview();
}

QString MainWindow::translateGCode(QString code, double x, double y, double z)
{
    double oldX = 0.0;
    double oldY = 0.0;
    double oldZ = 0.0;
    double oldF = 0.0;
    QStringList lines;
    QStringList newLines;
    QString     gCommand;

    lines = code.split("\n");

    foreach (QString line, lines)
    {
        QString newLine;

        line = line.trimmed();
        gCommand = "";

        if (!line.isEmpty())
        {
            if (line.indexOf("G00") == 0)
            {
                gCommand = "G00";
            }
            else if (line.indexOf("G01") == 0)
            {
                gCommand = "G01";
            }
        }

        if (gCommand != "")
        {
            QStringList commands = line.split(" ");
            oldX = 0.0;
            oldY = 0.0;
            oldZ = 0.0;
            oldF = 0.0;

            foreach (QString command, commands)
            {
                if (command.indexOf("X") == 0)
                {
                    command.remove(0,1);
                    oldX = command.toDouble();
                    oldX += x;
                }
                else if (command.indexOf("Y") == 0)
                {
                    command.remove(0,1);
                    oldY = command.toDouble();
                    oldY += y;
                }
                else if (command.indexOf("Z") == 0)
                {
                    command.remove(0,1);
                    oldZ = command.toDouble();
                    oldZ += z;
                }
                else if (command.indexOf("F") == 0)
                {
                    command.remove(0,1);
                    oldF = command.toDouble();
                }
            }

            newLine = "";
            newLine.append(gCommand);

            if (oldX != 0.0)
            {
                newLine.append(QString(" X%1").arg(oldX));
            }
            if (oldY != 0.0)
            {
                newLine.append(QString(" Y%1").arg(oldY));
            }
            if (oldZ != 0.0)
            {
                newLine.append(QString(" Z%1").arg(oldZ));
            }
            if (oldF != 0.0)
            {
                newLine.append(QString(" F%1").arg(oldF));
            }
        }
        else
        {
            newLine = line;
        }

        newLines.append(newLine);
    }

    return newLines.join("\n");
}

void MainWindow::refreshPreview()
{
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::drawGrid()
{
    QPen gridPen(QColor(50, 50, 50));

    // draw Grid
    scene->addRect(0,0,
                   applicationSettings.boardWidth,
                   applicationSettings.boardHeight, gridPen);

    for (int x = 0; x < applicationSettings.boardWidth; x += applicationSettings.boardStepX)
    {
        scene->addLine(x, 0, x, applicationSettings.boardHeight, gridPen);
    }

    for (int y = 0; y < applicationSettings.boardHeight; y += applicationSettings.boardStepY)
    {
        scene->addLine(0, y,
                       applicationSettings.boardWidth,
                       y, gridPen);
    }

    // draw Labels


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
    else if (obj == scene)
    {
        static QPointF pressPoint;

        if (event->type() == QEvent::GraphicsSceneMousePress)
        {
            QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            pressPoint = mouseEvent->scenePos();
        }
        else if (event->type() == QEvent::GraphicsSceneMouseRelease)
        {
            QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            QPointF movement = mouseEvent->scenePos() - pressPoint;
            ui->textEdit->setPlainText(translateGCode(ui->textEdit->toPlainText(), movement.x(), -movement.y(), 0.0));   // translate the GCode
        }
        else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QGraphicsSceneMouseEvent *mouseEvent = (QGraphicsSceneMouseEvent*)event;

            //vIndexMarker->setPos(14,mouseEvent->scenePos().y() * zoomFactor);
            //hIndexMarker->setPos(mouseEvent->scenePos().x() * zoomFactor,-14);
            //ui->outputLabel->setText(QString("%1<br>%2").arg(mouseEvent->scenePos().x(),6,'f',2,' ')
            //                                                .arg(-mouseEvent->scenePos().y(),6,'f',2,' '));

            showCrosshair();
            //if (ui->crosshairCheck->isChecked())
                setCrosshairPos(mouseEvent->scenePos());
        }
        else if (event->type() == QEvent::Enter)
        {
           showCrosshair();
        }
        else if (event->type() == QEvent::Leave)
        {
           hideCrosshair();
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
void MainWindow::on_refreshButton_clicked()
{
    refreshPreview();
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
