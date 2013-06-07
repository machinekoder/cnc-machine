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

#ifdef USB
bool Communicator::connectUsb()
{
    int usbCheckTimerInterval = 50;

    if (activeConnections & UsbConnection)  // We are already connected
    {
        return true;
    }

    usb_init();
#ifdef DEBUG
    usb_set_debug(2);
#endif

    if ((estickv2Handle = locateEstickv2()) == NULL)
    {
      qWarning() << "Could not open the eStickv2 device\n";
      return false;
    }

    if (usb_set_configuration(estickv2Handle, 1) < 0)
    {
        return false;
    }
    if (usb_claim_interface(estickv2Handle, 0) < 0)
    {
        return false;
    }

    activeConnections |= UsbConnection;
    emit usbConnected();

    usbCheckTimer = new QTimer(this);
    usbCheckTimer->setInterval(usbCheckTimerInterval);
    usbCheckTimer->setTimerType(Qt::PreciseTimer);
    usbCheckTimer->setSingleShot(false);
    connect(usbCheckTimer, SIGNAL(timeout()),
            this, SLOT(usbTask()));
    usbCheckTimer->start();

    return true;
}

void Communicator::closeUsb()
{
    usbCheckTimer->stop();
    usb_release_interface(estickv2Handle, 0);
    usb_close(estickv2Handle);
    activeConnections &= ~UsbConnection;
    emit usbDisconnected();
}

bool Communicator::isUsbConnected()
{
    return (activeConnections & UsbConnection);
}
#endif

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

#ifdef USB
void Communicator::incomingUsbData()
{

}

usb_dev_handle *Communicator::locateEstickv2()
{
    unsigned char located = 0;
    struct usb_bus *bus;
    struct usb_device *dev;
    usb_dev_handle *device_handle = 0;

    usb_find_busses();
    usb_find_devices();

    for (bus = usb_busses; bus; bus = bus->next)
    {
      for (dev = bus->devices; dev; dev = dev->next)
      {
        if (dev->descriptor.idVendor == 0xfefe)
        {
          located++;
          device_handle = usb_open(dev);
  #ifdef DEBUG
          qDebug() << "eStickv2 Device found";
          qDebug() << QString("eStickv2 Device Found @ Address %1").arg(dev->filename);
          qDebug() << QString("eStickv2 Vendor ID 0x0%1").arg(dev->descriptor.idVendor);
          qDebug() << QString("eStickv2 Product ID 0x0%1").arg(dev->descriptor.idProduct);
  #endif
        }
  #ifdef DEBUG
        else qDebug() << QString("** usb device %1 found **").arg(dev->filename);
  #endif
      }
    }

    if (device_handle==0) return (0);
    else return (device_handle);
}

void Communicator::usbTask()
{
    char receiveData[100];
    QByteArray data;
    int receiveStatus;
    receiveStatus = usb_bulk_read(estickv2Handle,BULK_IN_EP,(char *)&receiveData[0],100+DL,1);

    if (receiveStatus > 0)
    {
        data = QByteArray::fromRawData(&receiveData[DL], receiveStatus-DL);
    }
}

#endif

bool Communicator::sendData(const QByteArray &data)
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
#ifdef USB
    else if (activeConnections & UsbConnection)
    {
        int transmitTimeout = 10;
        int transmit = 0;
        int sendStatus;
        QByteArray sendData;

        transmit = data.size();

        sendData.append(0x00ff & ((transmit+1) >> 8));  // Highbyte
        sendData.append(0x00ff & (transmit+1));         // Lowbyte
        sendData.append(data);

        sendStatus = usb_bulk_write(estickv2Handle, BULK_OUT_EP, sendData.data(), transmit+DL, transmitTimeout);


        if (sendStatus == (transmit+DL))
        {
#ifdef DEBUG
            qDebug() << "sending succeded";
#endif
            return true;
        }
        else
        {
#ifdef DEBUG
            qDebug() << "error sending data";
#endif
            closeUsb();
            return false;
        }

    }
#endif
    else
    {
        return false;
    }

    return true;
}

void Communicator::incomingByte(char byte)
{
}
