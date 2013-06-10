#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QStringList>
#include <QTime>
#include "communicator.h"

class Worker : public QObject
{

    Q_OBJECT
    Q_PROPERTY(QStringList commandList READ commandList WRITE setCommandList NOTIFY commandListChanged)
    Q_PROPERTY(int currentLine READ currentLine NOTIFY currentLineChanged)
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(WorkingStates currentState READ currentState NOTIFY currentStateChanged)
    Q_PROPERTY(int bufferSize READ bufferSize WRITE setBufferSize NOTIFY bufferSizeChanged)

    Q_ENUMS(WorkingStates)

public:

    typedef enum {
        StoppedState = 0u,
        RunningState = 1u,
        PausedState  = 2u
    } WorkingStates;

    explicit Worker(Communicator *communicator,QObject *parent = 0);

    void start();
    void pause();
    void stop();
    
    QStringList commandList() const
    {
        return m_commandList;
    }

    int currentLine() const
    {
        return m_currentLine;
    }

    bool isReady() const
    {
        return m_ready;
    }

    WorkingStates currentState() const
    {
        return m_currentState;
    }

    int bufferSize() const
    {
        return m_bufferSize;
    }

signals:
    void finished();
    void commandListChanged(QStringList arg);
    void currentLineChanged(int arg);
    void readyChanged(bool arg);
    void currentStateChanged(Worker::WorkingStates arg);
    void bufferSizeChanged(int arg);

public slots:

void setCommandList(QStringList arg)
{
    if (m_commandList != arg) {
        m_commandList = arg;
        emit commandListChanged(arg);
    }
}

void setBufferSize(int arg)
{
    if (m_bufferSize != arg) {
        m_bufferSize = arg;
        emit bufferSizeChanged(arg);
    }
}

private:
    int m_waitingCount;
    Communicator *m_communicator;
    QStringList m_commandList;
    int m_currentLine;
    bool m_ready;
    WorkingStates m_currentState;
    int m_bufferSize;

    void startQueue();
    void resumeQueue();
    void stopQueue();
    void sendCommand(const QByteArray data);
    void sendLine();

private slots:
    void usbConnected();
    void usbDisconnected();
    void commandReceived(const QByteArray command);
};

#endif // WORKER_H
