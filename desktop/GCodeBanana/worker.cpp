#include "worker.h"

Worker::Worker(Communicator *communicator, QObject *parent) :
    QObject(parent)
{
    m_waitingCount = 0;

    m_communicator = communicator;
    m_ready = false;
    m_currentLine = -1;
    m_commandList = QStringList();
    m_currentState = StoppedState;
    m_bufferSize = 3;

    aliveTimer = new QTimer(this);
    aliveTimer->setInterval(1000); // 1s
    aliveTimer->start();

    connect(m_communicator, SIGNAL(usbConnected()),
            this, SLOT(usbConnected()));
    connect(m_communicator, SIGNAL(usbDisconnected()),
            this, SLOT(usbDisconnected()));
    connect(m_communicator, SIGNAL(commandReceived(QByteArray)),
            this, SLOT(commandReceived(QByteArray)));

    connect(aliveTimer, SIGNAL(timeout()),
            this, SLOT(aliveTimerTick()));
}

void Worker::start()
{
    if (m_currentState == StoppedState)
    {
        startQueue();
    }
    else if (m_currentState == PausedState)
    {
        resumeQueue();
    }
    m_currentState = RunningState;
    emit currentStateChanged(m_currentState);
}

void Worker::pause()
{
    m_currentState = PausedState;
    emit currentStateChanged(m_currentState);
}

void Worker::stop()
{
    stopQueue();

    m_currentState = StoppedState;
    emit currentStateChanged(m_currentState);
}

void Worker::usbConnected()
{
    m_ready = true;
    emit readyChanged(m_ready);
}

void Worker::usbDisconnected()
{
    m_currentLine = -1;
    m_currentState = StoppedState;
    m_ready = false;
    emit readyChanged(m_ready);
    emit currentLineChanged(m_currentLine);
    emit currentStateChanged(m_currentState);
}

void Worker::commandReceived(const QByteArray command)
{
    if (m_currentState == StoppedState)
    {
        return;
    }

    if (command == "ok")
    {
        m_waitingCount--;
        emit currentLineChanged(m_currentLine - m_waitingCount + 1);

        if (m_waitingCount < m_bufferSize)
        {
            if ((m_currentLine < m_commandList.size())
                    && (m_currentState == RunningState)) // a new command needs to be sent
            {
                sendLine();
            }
        }

        if ((m_waitingCount == 0)
                && (m_currentLine == m_commandList.size())) // we have finished
        {
            stopQueue();
            m_currentState = StoppedState;
            emit currentStateChanged(m_currentState);
            emit finished();
        }
    }
}

void Worker::aliveTimerTick()
{
    sendCommand("alive\n");
}

void Worker::startQueue()
{
    if (m_commandList.size() == 0)
    {
        return;
    }

    // send the first 3 commands
    m_currentLine = 0;
    for (int i = 0; i < m_bufferSize; ++i)
    {
        sendLine();
    }
}

void Worker::resumeQueue()
{
    sendLine();
}

void Worker::stopQueue()
{
    sendCommand("stop\n");
    m_waitingCount = 0;
}

void Worker::sendCommand(const QByteArray data)
{
    m_communicator->sendData(data);
    m_waitingCount++;
}

void Worker::sendLine()
{
    QString line;
    bool running = true;

    while (running)
    {
        if (m_currentLine == m_commandList.size())
        {
            return;
        }

        line = m_commandList.at(m_currentLine);

        if ((line.indexOf("G00") == 0) || (line.indexOf("G01") == 0)) // only G00 and G01 allowed
        {
            running = false;
        }

        if (running == false)
        {
            sendCommand(line.toLocal8Bit() + "\n");
        }

        m_currentLine++;
        emit currentLineChanged(m_currentLine - m_waitingCount + 1);
    }
}
