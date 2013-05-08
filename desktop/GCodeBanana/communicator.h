#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#define SERIALPORT

#include <QObject>
#include <QDebug>
#ifdef SERIALPORT
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>
QT_USE_NAMESPACE_SERIALPORT
#endif

class Communicator : public QObject
{
    Q_OBJECT
    Q_FLAGS(ActiveConnection ActiveConnections)

public:
    enum ActiveConnection {
        NoConnection = 0x00,
        SerialConnection = 0x01,
        NetworkConnection = 0x02
    };
    Q_DECLARE_FLAGS(ActiveConnections, ActiveConnection)

    explicit Communicator(QObject *parent = 0);
    ~Communicator();


#ifdef SERIALPORT
    bool connectSerialPort(const QString &device);
    void closeSerialPort();
#endif
    
    bool isSerialPortConnected();
    void sendData(const QByteArray &data);
    void incomingByte(char byte);
signals:

#ifdef SERIALPORT
    void serialPortConnected();
    void serialPortDisconnected();
#endif

private slots:
#ifdef SERIALPORT
    void incomingSerialData();
#endif

private:
#ifdef SERIALPORT
    SerialPort *serialPort;
#endif

ActiveConnections activeConnections;
    
};

#endif // COMMUNICATOR_H
