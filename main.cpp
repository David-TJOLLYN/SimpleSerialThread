#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include "stm32_com.h"

bool isExecuted(int function);

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    int error = 0;
    STM32_COM *serial = new STM32_COM();


    error = serial->openPortCOM("COM6");
    if(!isExecuted(error)) return 1;
    qDebug()<<"Port COM6 opened";

    error = serial->ConfigurePortCOM(QSerialPort::Baud115200);
    if(!isExecuted(error)) return 2;
    qDebug()<<"Port COM6 configured";

    serial->start();

    serial->sendByte(0x61);
    qDebug()<<"Send byte 0x61";

    return a.exec();
}


bool isExecuted(int function){
    STM32_COM *serial = new STM32_COM();
    QString msg = serial->getErrorMsg(function);
    if(!msg.isEmpty()){
        std::cout<<msg.toStdString();
        return false;
    }
    return true;
}
