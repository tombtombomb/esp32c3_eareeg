#ifndef __AD7689_DRIVER_H
#define __AD7689_DRIVER_H

#include "ADCCommon.hpp"


class AD7689_Driver:public ADCCommon{

    public:   
        void startCoversion(void);
        void init(u16_t cfg);
        void conversionConfig(u16_t cfg);
        void readLine(u16_t* data);
        u16_t readRawdata(void);
        u16_t readAndconfig(u16_t cfg);

    protected:
        u16_t currentCFG;
};
#endif