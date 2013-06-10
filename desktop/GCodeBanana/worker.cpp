#include "worker.h"

Worker::Worker(Communicator *communicator, QObject *parent) :
    QObject(parent)
{
    m_communicator = communicator;


}
