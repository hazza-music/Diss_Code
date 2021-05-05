#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_Sensor.h>

float Accel_values[] = {0,0,0};
float Prev_accel_values[] = {0,0,0};
float Avg_accel_values[] = {0,0,0};
float Smoothed_accel_values[] = {0,0,0};
float Accel_diff[]= {0,0,0};
float Prev_accel_diff[]= {0,0,0};
float Smoothed_diff_values[]={0,0,0};

// i2c
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();

//High + Low Pass Filters
float diff(float x, float y) //x = Accel Value, Y = Prev Accel Value
{
  float differentiator = x - y;
  return differentiator;
}

float avg(float x, float y) //x = accel_value, y = prev_accel_value
{
  float moving_average = x + y /2.0; //Low pass filter Moving Average
  return moving_average;
}

void setupSensor()
{
  // Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
}

void setup() 
{
 #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println("LSM raw read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
    while (1);
  }
  setupSensor();
  delay(1);
}

void loop() 
{
  lsm.read();
  Accel_values[0] = ((float)lsm.accelData.x);
  Accel_values[1] = ((float)lsm.accelData.y);
  Accel_values[2] = ((float)lsm.accelData.z);

// For troubleshooting the Accel Values before High pass 
  for(int i=0;i<3;i++){
 // Serial.print(Accel_values[i]); //Display array values
  //Serial.print(" ");
  }
  // Serial.println(" ");

// For Dissertation, Moving average without the differentiation
  for(int i=0;i<3;i++){
  Avg_accel_values[i] = avg(Accel_values[i], Prev_accel_values[i]);
  Serial.print(Avg_accel_values[i]); Serial.print(" ");
  }
  
  //For loop for the high pass
  for(int i=0;i<3;i++){
  Accel_diff[i] = diff(Accel_values[i], Prev_accel_values[i]);
  Prev_accel_values[i] = Accel_values[i];
 // Serial.print(Accel_diff[i]); Serial.print(" "); //Troubleshooting
  }

//  Serial.println(" ");
  
//  for loop for the low pass of the high pass figures
  for(int i=0;i<3;i++){
  Smoothed_diff_values[i] = avg(Accel_diff[i], Prev_accel_diff[i]);
  Prev_accel_diff[i] = Accel_diff[i];
  // Differentiated movement values
  //Serial.print(Smoothed_diff_values[i]); Serial.print(" "); //printing the difference in movement
  }
  // Gaining x movement data
  //Serial.print(Accel_values[0]);
  Serial.println(" ");

    delay(500);
}
