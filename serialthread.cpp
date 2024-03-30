#include "serialthread.h"
#include "iostream"

SerialThread::SerialThread() {}

SerialThread::~SerialThread(){
    closePortCOM();
}

int SerialThread::openPortCOM(const QString &portCOM){

    // Convert QString to std::wstring
    std::wstring portCOMW = portCOM.toStdWString();

    // Concatenate wide-character string literal with portCOM
    std::wstring comPortW = L"\\\\.\\" + portCOMW;

    // Open the COM port
    _serial = CreateFile(comPortW.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (_serial == INVALID_HANDLE_VALUE) return 4;

    _isOpened = true;

    return 0;
}

int SerialThread::closePortCOM(){
    _isRunning = false;
    _isOpened = false;
    CloseHandle(_serial);
    return 0;
}

int SerialThread::ConfigurePortCOM(const QSerialPort::BaudRate &baudrate){

    if(!_isOpened) return 1;

    DCB dcbSerialParams;
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    // Get port COM configuration
    if (!GetCommState(_serial, &dcbSerialParams)) {
        CloseHandle(_serial);
        return 5;
    }

    // Set port COM new configuration
    dcbSerialParams.BaudRate = baudrate; // Set baud rate to 115200
    dcbSerialParams.ByteSize = 8;          // 8 data bits
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 stop bit
    dcbSerialParams.Parity = NOPARITY;     // No parity

    // Implement new configuration
    if (!SetCommState(_serial, &dcbSerialParams)) {
        CloseHandle(_serial);
        return 6;
    }

    _isConfigured = true;
    return 0;
}

void SerialThread::run(){
    DWORD bytesRead;
    char _buffer[4];

    _isRunning = _isOpened && _isConfigured;
    if(!_isRunning) return;

    while(_isRunning && _isOpened) {
        if (!ReadFile(_serial, _buffer, sizeof(_buffer), &bytesRead, NULL)) {
            break;
        }

        if (bytesRead > 0) {
            // Process received data
            QMutexLocker locker(&_mutex);
            for(int i=0;i<bytesRead;i++) {
                _array.enqueue(_buffer[i]);
            }

            emit dataAvailable();
        }
        msleep(10);
    }
}

QQueue<char> SerialThread::getData(){
    QMutexLocker locker(&_mutex);
    QQueue<char> copy = _array;
    _array.clear();
    return copy;
}

QString SerialThread::getErrorMsg(int error){
    QString msg = "";
    switch (error) {
    case 0:
        break;
    case 1:
        msg = "Port COM not opened.\n";
        break;
    case 2:
        msg = "Port COM not configured.\n";
        break;
    case 3:
        msg = "Port COM not running.\n";
        break;
    case 4:
        msg = "Fail to Open port COM.\n";
        break;
    case 5:
        msg = "Fail to get current COM port settings.\n";
        break;
    case 6:
        msg = "Fail to set COM port settings.\n";
        break;
    case 7:
        msg = "Fail reading from COM port.\n";
        break;
    default:
        msg = "Unknown error.\n";
        break;
    }
    return msg;
}


