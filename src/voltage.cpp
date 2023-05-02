#include <voltage.h>


void voltage::begin(byte pin){
    pinMode(pin, INPUT);
}

void voltage::begin(byte pin, byte pin2) {
    pinMode(pin, INPUT);
    pinMode(pin2, INPUT);
}

voltage::voltage(byte pin, voltagetype voltagetype){
    _pin = pin;
    _voltagetype = voltagetype;
    if(_voltagetype == DC){
        _scannum = 1;
    } else {
        _scannum = 10;
    }
    begin(_pin);
}
voltage::voltage(byte pin, byte pin2, voltagetype voltagetype, phase phase){
    _pin = pin;
    _pin2 = pin2;
    _voltagetype = voltagetype;
    _phase = phase;
    if(_voltagetype == DC){
        _scannum = 1;
    } else {
        _scannum = 10;
    }
    begin(_pin, _pin2);
}
void voltage::setScan(int scannum){
    _scannum = scannum;
}
int voltage::getScan(){
    return _scannum;
}
void voltage::setCalibration(float calib, bool pinNum){
    if(pinNum){
        _calib1 = calib;
    } else {
        _calib = calib;
    }
}

float voltage::getCalibration(bool pinNum){
    if(pinNum){
        return _calib1;
    } else {
        return _calib;
    }
}
float voltage::getV(byte pin, bool pinNum){
    float tempv, v;
    v = 0;
    for(int i = 0; i < _scannum; i++){
        tempv = pinNum ? analogRead(pin) * _calib1 : analogRead(pin) * _calib;
        if(tempv > v){
            v = tempv;
        }
    }
    return v;
}


float voltage::getVoltage() {
    if(_phase == SINGLE){
        return getV(_pin);
    }
    else {
        return (getV(_pin) + getV(_pin2, true)) * 0.863; //0.863 is the constant to calculate double phase voltage
    }
}

float voltage::autoCalibrate(float voltage, bool pinNum){
    if(pinNum){
        _calib1 = (voltage / getV(_pin, pinNum)) * _calib1;
        return _calib1;
    } else {
        _calib = (voltage / getV(_pin, pinNum)) * _calib;
        return _calib;
    }
}
//fun :)