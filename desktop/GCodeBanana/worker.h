#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QStringList>
#include "communicator.h"

class Worker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList commandList READ commandList WRITE setCommandList NOTIFY commandListChanged)
    Q_PROPERTY(int currentLine READ currentLine NOTIFY currentLineChanged)

public:
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

signals:
    void commandListChanged(QStringList arg);
    void currentLineChanged(int arg);

public slots:

void setCommandList(QStringList arg)
{
    if (m_commandList != arg) {
        m_commandList = arg;
        emit commandListChanged(arg);
    }
}

private:
    Communicator *m_communicator;
    QStringList m_commandList;
    int m_currentLine;
};

#endif // WORKER_H
