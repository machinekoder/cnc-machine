#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gcodeParser = new QGCodeParser(ui->textEdit, this);

    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(Qt::white));
    ui->graphicsView->setScene(scene);

    scene->addRect(0,0,200,200);
    ui->graphicsView->fitInView(0,0,200,200);

    connect(gcodeParser, SIGNAL(codeChanged(bool)),
            this, SLOT(codeChanged(bool)));
}

MainWindow::~MainWindow()
{
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
        scene->clear();
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

    scene->clear();
    //QPainter painter(scene->add);
    //painter.setPen(Qt::black);
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
                    qDebug() << command;
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

                if ((x == 0) || (y == 0))
                {
                    continue;
                }
                scene->addLine(globalX, globalY, x, y, pen);
                qDebug() << x << y;
                globalX = x;
                globalY = y;
            }
        }
    }
    ui->graphicsView->fitInView(scene->itemsBoundingRect());
}
