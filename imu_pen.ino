
#include <SparkFunMPU9250-DMP.h>

MPU9250_DMP imu;
#define tact 13

int16_t writting_time = 3;
int16_t data_row = 0;

bool flag = false;
bool pre_flag = false;
bool flag_state = false;
double n_time = 0;

void setup() 
{
  Serial.begin(230400);

  while((imu.begin() != INV_SUCCESS)){
  } // error occured
  
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

  imu.setGyroFSR(2000); // Set gyro to 2000 dps
  imu.setAccelFSR(2); // Set accel to +/-2g

  imu.setLPF(5); // Set LPF corner frequency to 5Hz

  imu.setSampleRate(100); // Set sample rate to 10Hz

  imu.setCompassSampleRate(100); // Set mag rate to 10Hz

  pinMode(tact, INPUT_PULLUP);

  Serial.println("CLEARDATA");
  //Serial.println("LABEL,Time,AccX,AccY,AccZ,millis");
  Serial.println("LABEL,Time,GyroX,GyroY,GyroZ,millis");
}

void loop() 
{
  /*
  if(flag_state){
    n_time = millis();
//    while((millis()-n_time) < (writting_time * 1000)){
//      send2plx();
//    }


*/

  if(flag_state){
    while(data_row < 300){
      send2plx();
      data_row++;
    }
    init_();
  }
  
  if(!digitalRead(tact)) flag = true;
  else  flag = false;
  
  if((pre_flag != flag) * (!flag))  flag_state = true;

  pre_flag = flag;

}

void send2plx(void){
  while ( !imu.dataReady() ){
    
  }
  imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
  printIMUData();
}

void init_(void){
  flag_state = false;
  data_row = 0;
}

void printIMUData(void)
{  
  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);
  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);
  float magX = imu.calcMag(imu.mx);
  float magY = imu.calcMag(imu.my);
  float magZ = imu.calcMag(imu.mz);
  
  Serial.print("DATA,TIME,");
  //Serial.print(accelX); Serial.print(",");
  //Serial.print(accelY); Serial.print(",");
  //Serial.print(accelZ); Serial.print(",");
  Serial.print(gyroX); Serial.print(",");
  Serial.print(gyroY); Serial.print(",");
  Serial.print(gyroZ); Serial.print(",");
  Serial.print(millis());
  Serial.println();
}
