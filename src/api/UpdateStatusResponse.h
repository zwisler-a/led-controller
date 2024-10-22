//
// Created by Alex on 20.10.2024.
//

#ifndef LEDSTRIP_UPDATESTATUSRESPONSE_H
#define LEDSTRIP_UPDATESTATUSRESPONSE_H


#include <WString.h>
#include "led.h"

class UpdateStatusResponse {
public:
    bool on;
    int brightness;
    int spectrumRGB;


    String toJson() const;

    static UpdateStatusResponse fromColorTransition(ColorTransition transition, bool onState);

};


#endif //LEDSTRIP_UPDATESTATUSRESPONSE_H
