#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>

class Communicator : public QObject
{
    Q_OBJECT
public:
    explicit Communicator(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // COMMUNICATOR_H
