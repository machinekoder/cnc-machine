#include "communicator.h"

Communicator::Communicator(QObject *parent) :
    QObject(parent)
{
#ifdef SERIALPORT
    serialPort = NULL;
#endif
}

Communicator::~Communicator()
{
#ifdef SERIALPORT
    if (serialPort != NULL)
        serialPort->close();
#endif
}

#ifdef SERIALPORT
bool Communicator::connectSerialPort(const QString &device)
{
    if (serialPort != NULL)
        closeSerialPort();

    serialPort = new SerialPort(this);
    serialPort->setPort(device);
    serialPort->setDataBits(SerialPort::Data8);
    serialPort->setParity(SerialPort::NoParity);
    serialPort->setStopBits(SerialPort::OneStop);
    serialPort->setRate(115200);
    if (serialPort->open(QIODevice::ReadWrite))
    {
        connect(serialPort, SIGNAL(readyRead()),
                this, SLOT(incomingSerialData()));
        activeConnections |= SerialConnection;

        emit serialPortConnected();
        serialPort->write("\r");

        qDebug() << "GCodeBanana: connected to serial device";
        return true;
    }
    else
    {
        qDebug() << "GCodeBanana: not connected to serial device";
        return false;
    }

}


void Communicator::closeSerialPort()
{
    serialPort->close();
    serialPort->deleteLater();
    serialPort = NULL;
    activeConnections &= ~SerialConnection;
    emit serialPortDisconnected();
//    endQueue();
}
#endif

#ifdef SERIALPORT
bool Communicator::isSerialPortConnected()
{
    return (serialPort != NULL);
}
#endif

#ifdef SERIALPORT
void Communicator::incomingSerialData()
{
    QByteArray data;
    while (serialPort->bytesAvailable() != 0)
    {
       data = serialPort->read(1);
       incomingByte(data.at(0));
    }
}
#endif

void Communicator::sendData(const QByteArray &data)
{
    if (activeConnections & NetworkConnection)
    {
        //tcpSocket->write(data);
    }
#ifdef SERIALPORT
    else if (activeConnections & SerialConnection)
    {
        serialPort->flush();
        serialPort->write(data);
    }
#endif
}

void Communicator::incomingByte(char byte)
{
}
