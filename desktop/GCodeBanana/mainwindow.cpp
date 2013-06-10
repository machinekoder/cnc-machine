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

    // initialize communicator
    communicator = new Communicator(this);
    connect(communicator, SIGNAL(usbConnected()),
            this, SLOT(usbConnected()));
    connect(communicator, SIGNAL(usbDisconnected()),
            this, SLOT(usbDisconnected()));

    // initialize worker
    worker = new Worker(communicator, this);
    connect(worker, SIGNAL(finished()),
            this, SLOT(workerFinished()));
    connect(worker, SIGNAL(currentLineChanged(int)),
            this, SLOT(workerCurrentLineChanged(int)));
    connect(worker, SIGNAL(readyChanged(bool)),
            ui->startButton, SLOT(setEnabled(bool)));
    connect(worker, SIGNAL(currentStateChanged(Worker::WorkingStates)),
            this, SLOT(workerCurrentStateChanged(Worker::WorkingStates)));

    // initialize parser
    gcodeParser = new QGCodeParser(ui->textEdit, this);
    connect(gcodeParser, SIGNAL(codeChanged(bool)),
            this, SLOT(codeChanged(bool)));

    // initialize scene
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(Qt::black));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(true);

    // create markers
    hLine = scene->addLine(0,-10,1000,-10, QPen(QBrush(Qt::white), 0, Qt::SolidLine));
    vLine = scene->addLine(0,-10,1000,-10, QPen(QBrush(Qt::white), 0, Qt::SolidLine));
    hLine->setZValue(2);
    vLine->setZValue(2);
    hideCrosshair();

    applicationSettings.boardWidth = 200.0;
    applicationSettings.boardHeight = 200.0;
    applicationSettings.boardStepX = 10.0;
    applicationSettings.boardStepY = 10.0;

    drawGrid();

    ui->previewDock->installEventFilter(this);
    scene->installEventFilter(this);

    QTimer::singleShot(200, Qt::VeryCoarseTimer,this, SLOT(refreshPreview()));

    // disable unnecessary visuals
    ui->serialDeviceCombo->setVisible(false);

    // autoconnect
    communicator->connectUsb();
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
    ui->graphicsView->fitInView(0,0,applicationSettings.boardWidth,applicationSettings.boardHeight, Qt::KeepAspectRatio);
    //ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::usbConnected()
{
    ui->connectButton->setText(tr("Disconnect"));
    ui->connectButton->setStyleSheet("background-color:green");
}

void MainWindow::usbDisconnected()
{
    ui->connectButton->setText(tr("Connect"));
    ui->connectButton->setStyleSheet("background-color:red");
}

void MainWindow::workerCurrentLineChanged(int arg)
{
    ui->textEdit->setCurrentLine(arg);
}

void MainWindow::workerCurrentStateChanged(Worker::WorkingStates state)
{
    switch (state)
    {
    case Worker::StoppedState: ui->startButton->setText(tr("Start"));
        break;
    case Worker::RunningState: ui->startButton->setText(tr("Pause"));
        break;
    case Worker::PausedState: ui->startButton->setText(tr("Resume"));
        break;
    }
}

void MainWindow::workerFinished()
{
    logText(tr("Machine: finished"));
}

void MainWindow::on_startButton_clicked()
{
    switch (worker->currentState())
    {
    case Worker::StoppedState: worker->setCommandList(ui->textEdit->toPlainText().split('\n'));
        worker->start();
        break;
    case Worker::RunningState: worker->pause();
        break;
    case Worker::PausedState: worker->start();
        break;
    }
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

    refreshPreview();
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

void MainWindow::sendCommand(QString command)
{
    if (communicator->sendData(command.toUtf8() + "\n"))
    {
        logText(tr("Sent: ") + command);
    }
    else
    {
        logText(tr("Not connected"));
    }
}

void MainWindow::logText(QString text)
{
    ui->logEdit->appendPlainText(text);
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

void MainWindow::on_loadFileButton_clicked()
{
    QFile file(":/test/test2");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString text(file.readAll());
        ui->textEdit->setPlainText(text);
    }
}

void MainWindow::on_connectButton_clicked()
{
    if (communicator->isUsbConnected())
    {
        communicator->closeUsb();
    }
    else
    {
        communicator->connectUsb();
    }
}

void MainWindow::on_sendButton_clicked()
{
    if (ui->sendCommandEdit->text().isEmpty())
    {
        return;
    }

    sendCommand(ui->sendCommandEdit->text());
    ui->sendCommandEdit->clear();
}

void MainWindow::on_pushButton_11_clicked()
{
    sendCommand("move x 0.1");
}

void MainWindow::on_pushButton_12_clicked()
{
    sendCommand("move x 1");
}

void MainWindow::on_pushButton_21_clicked()
{
    sendCommand("move x 10");
}

void MainWindow::on_pushButton_8_clicked()
{
    sendCommand("move x 100");
}

void MainWindow::on_pushButton_13_clicked()
{
    sendCommand("move x -0.1");
}

void MainWindow::on_pushButton_14_clicked()
{
    sendCommand("move x -1");
}

void MainWindow::on_pushButton_15_clicked()
{
    sendCommand("move x -10");
}

void MainWindow::on_pushButton_16_clicked()
{
    sendCommand("move x -100");
}

void MainWindow::on_pushButton_clicked()
{
    sendCommand("move y 0.1");
}

void MainWindow::on_pushButton_10_clicked()
{
    sendCommand("move y 1");
}

void MainWindow::on_pushButton_9_clicked()
{
    sendCommand("move y 10");
}

void MainWindow::on_pushButton_7_clicked()
{
    sendCommand("move y 100");
}

void MainWindow::on_pushButton_22_clicked()
{
    sendCommand("move y -0.1");
}

void MainWindow::on_pushButton_23_clicked()
{
    sendCommand("move y -1");
}

void MainWindow::on_pushButton_24_clicked()
{
    sendCommand("move y -10");
}

void MainWindow::on_pushButton_25_clicked()
{
    sendCommand("move y -100");
}

void MainWindow::on_pushButton_31_clicked()
{
    sendCommand("move z 0.1");
}

void MainWindow::on_pushButton_32_clicked()
{
    sendCommand("move z 1");
}

void MainWindow::on_pushButton_33_clicked()
{
    sendCommand("move z 10");
}

void MainWindow::on_pushButton_29_clicked()
{
    sendCommand("move z -0.1");
}

void MainWindow::on_pushButton_30_clicked()
{
    sendCommand("move z -1");
}

void MainWindow::on_pushButton_26_clicked()
{
    sendCommand("move z -10");
}

void MainWindow::on_pushButton_18_clicked()
{
    sendCommand("home x");
}

void MainWindow::on_pushButton_20_clicked()
{
    sendCommand("home y");
}

void MainWindow::on_pushButton_19_clicked()
{
    sendCommand("home z");
}

void MainWindow::on_pushButton_28_clicked()
{
    sendCommand("home all");
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    sendCommand(QString("move feed xy %1").arg(arg1));
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    sendCommand(QString("move feed z %1").arg(arg1));
}

void MainWindow::on_pushButton_17_clicked()
{
    sendCommand("stop");
}

void MainWindow::on_pushButton_2_clicked()
{
    sendCommand("test");
}
