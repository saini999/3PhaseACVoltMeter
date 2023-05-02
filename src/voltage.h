#ifndef voltage_h
#define voltage_h
#include <Arduino.h>

//Create Variables for Current type and phase type

enum voltagetype{
    DC, AC
};

enum phase{
    SINGLE, DOUBLE
};

class voltage {
    private:
        //Private Varibales
        byte _pin, _pin2;
        voltagetype _voltagetype;
        phase _phase;
        int _scannum;
        float _calib, _calib1;
        //Function to setup pins as input
        void begin(byte pin);
        void begin(byte pin, byte pin2);
        float getV(byte pin, bool pinNum = false);
    public:
        voltage(byte pin, voltagetype voltagetype = DC); // Initialize DC or AC Single Phase Voltage reading
        voltage(byte pin, byte pin2, voltagetype voltagetype = AC, phase phase = DOUBLE); //Initialize AC Double Phase Voltage Reading
        void setScan(int scannum); //Set Scan numbers to scan voltage multiple times before getting result, helps with keeping AC voltage Reading Stable
        int getScan(); //get the scan number
        void setCalibration(float calib, bool pinNum = false); //set Calibration Value
        float getCalibration(bool pinNum = false); // get Calibration Value
        float getVoltage(); //Read the Voltage
        float autoCalibrate(float voltage, bool pinNum = false); //to be called once to get the calibration Value
};

#endif
// fun :)