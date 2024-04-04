#include "stm32_com.h"
#include <iostream>
#include <stdint.h>

STM32_COM::STM32_COM() {
    connect(this, &STM32_COM::dataAvailable, this, &STM32_COM::analyseData);
    idx=0;
    valid=false;
}

void STM32_COM::analyseData(){
    QQueue<char> raw = getData();
    while(!raw.isEmpty()) _rawDatas.push_back(raw.dequeue());

    if(_rawDatas.size()<2) return;


    for(;idx<_rawDatas.size()-1;idx++){
        uint8_t lsb = _rawDatas[idx];
        uint8_t msb = _rawDatas[idx+1];

        if(lsb == 255 && msb == 255){
            valid = true;
            _rawDatas.remove(0,idx+1);
            idx=0;
            continue;
        }

        if(valid){
            int add = msb>>4;
            int val = (msb & 0x0F)<<8 | lsb;
            _datas.append(val);
            emit newData(val);
            idx++;
        }
    }
}
