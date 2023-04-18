#include "mbed.h"
#include "accelerometer.h"
#include "gyro.h"
#include <cstdint>

static BufferedSerial pc(USBTX, USBRX); // tx, rx

FileHandle *mbed::mbed_override_console(int fd)
{
   return &pc;
}

AnalogOut Aout(PA_4);
InterruptIn btnRecord(BUTTON1);
EventQueue queue(150 * EVENTS_EVENT_SIZE);
Thread t;
Accelerometer acc;
Gyro gyro;
double Accel[3]={0};
double Gyro[3]={0};
double  accAngleX=0;
double  accAngleY=0;
double elapsedTime=0;
double roll, pitch, yaw;
double gyroAngleX=0;
double gyroAngleY=0;
int counter=0;
int idR[128] = {0};
int indexR = 0;

void record(void) {

  acc.GetAcceleromterSensor(Accel);
  acc.GetAcceleromterCalibratedData(Accel);

  //printf("Calibrated ACC= %f, %f, %f\n", Accel[0], Accel[1], Accel[2]);
  pc.write(&Accel[0], 8);
  pc.write(&Accel[1], 8);
  pc.write(&Accel[2], 8);
}

void stopRecord(void) {
  //printf("---stop---\n");
  for (auto &i : idR)
    queue.cancel(i);
}

void startRecord(void) {
  //printf("---start---\n");
  int i = 128;
  while (i--) {
    idR[indexR++] = queue.call_every(10ms, record);
    indexR = indexR % 32;
  }
}

int main() {
  t.start(callback(&queue, &EventQueue::dispatch_forever));
  while (1) {
        if (pc.readable()) {
            uint8_t input;
            pc.read(&input, 1);
            if (uint8_t(input) == 0X10) {
                queue.call(startRecord);
            }

        }
  } 
  //int Start = 0;
/*  while(Start == 0) {
      if (pc.readable()) {
         uint8_t input;
         pc.read(&input, 1);
         if (input == 0X10) {
            Start = 1;
            startRecord();
         }
      }
  }*/
}