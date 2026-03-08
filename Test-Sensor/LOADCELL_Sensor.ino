#include "HX711.h"

float calibration_factor=448.49; 
#define zero_factor 1
#define DOUT  33
#define CLK   32
#define DEC_POINT  2

float offset=0;
float get_units_kg();

HX711 scale;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Load Cell");
  delay(2000);
  scale.begin(DOUT, CLK);
  Serial.println(calibration_factor);
  scale.set_scale(calibration_factor); 
  scale.tare(1000);   
}
void loop() 
{ 
  Serial.print("Reading: ");
  float data = scale.get_units(10);
  Serial.print(data);
  Serial.println(" g");
}