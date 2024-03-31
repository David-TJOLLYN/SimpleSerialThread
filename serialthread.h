#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QQueue>
#include <QMutex>
#include <windows.h>

class SerialThread : public QThread
{
    Q_OBJECT
public:
    SerialThread();
    ~SerialThread();
    int openPortCOM(const QString &portCOM);
    int closePortCOM();
    int ConfigurePortCOM(const QSerialPort::BaudRate &baudrate);

    QQueue<char> getData();

    bool isOpened(){return _isOpened;}
    bool isConfigured(){return _isConfigured;}
    bool isRunning(){return _isRunning;}

    QString getErrorMsg(int error);

public slots:
    int sendByte(uint8_t byte);

signals:
    void dataAvailable();

protected:
    void run() override;

private:
    HANDLE _serial;
    QQueue<char> _array;
    QMutex _mutex;

    bool _isOpened;
    bool _isConfigured;
    bool _isRunning;

};

/* 0 : success
 * 1 : Port COM not opened
 * 2 : Port COM not configured
 * 3 : Port COM not running
 * 4 : Fail to Open port COM
 * 5 : Fail to get current COM port settings
 * 6 : Fail to set COM port settings
 * 7 : Fail reading from COM port
 * 8 : Fail writing in COM port
 * 9 : Error while writin in COM port
 *
*/

#endif // SERIALTHREAD_H
