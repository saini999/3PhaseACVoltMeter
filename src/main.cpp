#include <Arduino.h>
#include <BlockNot.h>
#include <SevSeg.h>
#include <voltage.h>

#define phaseR A0
#define phaseY A1
#define phaseB A2
#define calibratepin A3

BlockNot refresh(1, SECONDS);
BlockNot scan(250, MILLISECONDS);

SevSeg d;

voltage vR(phaseR), vY(phaseY), vB(phaseB), v1(phaseR, phaseY), v2(phaseY, phaseB), v3(phaseB, phaseR);

double Rv, Yv, Bv, RYv, YBv, BRv;
bool calibrated;
int8_t cycle;

void setupDisplay(){
  const byte dig1 = 2;
  const byte dig2 = 3;
  const byte dig3 = 4;
  const byte dig4 = 5;
  const byte segA = 6;
  const byte segB = 7;
  const byte segC = 8;
  const byte segD = 9;
  const byte segE = 10;
  const byte segF = 11;
  const byte segG = 12;
  const byte segDP = 13;
  const uint8_t numDigits = 4;
  d.Begin(COMMON_ANODE, numDigits, dig1, dig2, dig3, dig4, segA, segB, segC, segD, segE, segF, segG, segDP);
  d.SetBrightness(100);
}

void volt() {
  Rv = vR.getVoltage();
  Yv = vY.getVoltage();
  Bv = vB.getVoltage();
  RYv = v1.getVoltage();
  YBv = v2.getVoltage();
  BRv = v3.getVoltage();
}

void calibrate() {
  if(!calibrated){
    vR.autoCalibrate(300);
    vY.autoCalibrate(300);
    vB.autoCalibrate(300);
    v1.autoCalibrate(300);
    v1.autoCalibrate(300, true);
    v2.autoCalibrate(300);
    v2.autoCalibrate(300, true);
    v3.autoCalibrate(300);
    v3.autoCalibrate(300, true);
    calibrated = true;
  }
}

void printVoltage(float voltage) {
  char buffer[5];
  sprintf(buffer, "%04d", (int)voltage);
  d.DisplayString(buffer, 0);
}

void setup() {
  setupDisplay();
  pinMode(calibratepin, INPUT);
  vR.setCalibration(0.560);
  vY.setCalibration(0.560);
  vB.setCalibration(0.560);
  v1.setCalibration(0.560);
  v1.setCalibration(0.560, true);
  v2.setCalibration(0.560);
  v2.setCalibration(0.560, true);
  v3.setCalibration(0.560);
  v3.setCalibration(0.560, true);
  refresh.reset();
}

void loop() {
  if(digitalRead(calibratepin)){
    calibrate();
  }
  if(scan.triggered()){
    volt();
  }
  if(refresh.triggered()){
    cycle++;
  }
  switch ( cycle )
  {
    case 0:
      d.DisplayString("nAS ", 0);
      break;
    case 1:
      d.DisplayString("r   ", 0);
      break;
    case 2:
      printVoltage(Rv);
      break;
    case 3:
      d.DisplayString("Y   ", 0);
      break;
    case 4:
      printVoltage(Yv);
      break;
    case 5:
      d.DisplayString("B   ", 0);
      break;
    case 6:
      printVoltage(Bv);
      break;
    case 7:
      d.DisplayString("rY  ", 0);
      break;
    case 8:
      printVoltage(RYv);
      break;
    case 9:
      d.DisplayString("Yb  ", 0);
      break;
    case 10:
      printVoltage(YBv);
      break;
    case 11:
      d.DisplayString("br  ", 0);
      break;
    case 12:
      printVoltage(BRv);
      break;
    default:
      cycle = 0;
      break;
  }
}