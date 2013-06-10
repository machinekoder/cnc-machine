#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

//#define SERIALPORT
#define USB
#define DEBUG

#include <QObject>
#include <QDebug>

#ifdef SERIALPORT
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>
QT_USE_NAMESPACE_SERIALPORT
#endif

#ifdef USB
#define BULK_OUT_EP 0x05
#define BULK_IN_EP 0x82
#define DL 2
#include <QTimer>
#include <QQueue>
#include <usb.h>
#endif

class Communicator : public QObject
{
    Q_OBJECT
    Q_FLAGS(ActiveConnection ActiveConnections)

public:
    enum ActiveConnection {
        NoConnection = 0x00,
        SerialConnection = 0x01,
        NetworkConnection = 0x02,
        UsbConnection = 0x04
    };
    Q_DECLARE_FLAGS(ActiveConnections, ActiveConnection)

    explicit Communicator(QObject *parent = 0);
    ~Communicator();

#ifdef SERIALPORT
    bool connectSerialPort(const QString &device);
    void closeSerialPort();
#endif
#ifdef USB
    bool connectUsb();
    void closeUsb();
    bool isUsbConnected();
#endif
    
    bool isSerialPortConnected();
    bool sendData(const QByteArray &data);
    void incomingByte(char byte);

signals:

#ifdef SERIALPORT
    void serialPortConnected();
    void serialPortDisconnected();
#endif
#ifdef USB
    void usbConnected();
    void usbDisconnected();
#endif

    void commandReceived(const QByteArray command);

private slots:
#ifdef SERIALPORT
    void incomingSerialData();
#endif
#ifdef USB
    void usbTask();
#endif

private:
#ifdef SERIALPORT
    SerialPort *serialPort;
#endif
#ifdef USB
    struct usb_dev_handle *estickv2Handle;
    QTimer *usbCheckTimer;

    QByteArray dataBuffer;
    QQueue<QByteArray> dataQueue;
    QString receivedData;
    bool waitingForRespose;

    usb_dev_handle *locateEstickv2 (void);
#endif

    ActiveConnections activeConnections;

    void receivedCommand(QByteArray command);
    
};

#endif // COMMUNICATOR_H
