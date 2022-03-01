#include <VirtualWire.h>
#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);

char *data;

long timer = 0;
float x1;
float y1;
float z1;

float x2;
float y2;
float z2;


void setup() 
{
  vw_set_tx_pin(12);
  vw_setup(2000);
  
  Serial.begin(9600);
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");
     x1 = (mpu.getAccX());
     y1 = (mpu.getAccY());
     z1 = (mpu.getAccZ());
   
}

void loop()
{
    mpu.update();
    x2 = (mpu.getAccX());
    y2 = (mpu.getAccY());
    z2 = (mpu.getAccZ());
    float x_axis = ( x1 - x2);
    float y_axis = (y1 - y2);
    float z_axis = (z1 - z2);
  
  if(y_axis <= -0.40)
  {
    data="f";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Forward");
  }
  else if(y_axis >= 0.35)
  {
    data="b";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Backward");
  }
  else if(z_axis >= -0.70)
  {
    data="r";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Right");
  }
  else if(z_axis <= -1.0)
  {
    data="l";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Left");
  }
  else
  {
    data="s";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Stop");
  }
}
