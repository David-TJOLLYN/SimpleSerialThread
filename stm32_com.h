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

signals:
    void newData(int val);

private:
    QQueue<char> _rawDatas;
    QList<int> _datas;
    int idx;
    bool valid;
};

#endif // STM32_COM_H
