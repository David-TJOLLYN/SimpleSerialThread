#ifndef STM32_COM_H
#define STM32_COM_H

#include <QObject>
#include "serialthread.h"

class STM32_COM : public SerialThread
{
    Q_OBJECT
public:
    STM32_COM();

public slots:
    void analyseData();

private:
    QList<char> _rawDatas;
    QList<int> _datas;
    int idx;
    bool valid;
};

#endif // STM32_COM_H
