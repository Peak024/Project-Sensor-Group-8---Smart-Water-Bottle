#include <OneWire.h>
#include <DallasTemperature.h>

//กำหนดใช้งานบัส OneWire ที่ GPIO4
#define ONE_WIRE_BUS_PIN 22

//ตั้งค่าการใช้งาน OneWire เพื่อเชื่อมต่อกับ Sensor ใดๆ ที่เป็น OneWire
OneWire oneWire(ONE_WIRE_BUS_PIN);

//สร้างาตัวแปรจาก Libarry ของ Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void setup(void) {
  //เริ่มต้นใช้งานการสื่อสาร serial
  Serial.begin(115200);

  //เริ่มต้นใช้งาน Library
  sensors.begin();
}

void loop(void) {
  sensors.requestTemperatures(); //ส่งคำสั่งขออ่านค่าอุณหภูมิ

  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0)); //อ่านค่า Sensor ตัวแรกที่พบ
  
  delay(1000);//หน่วงเวลาเพื่ออ่านค่าอุณหภูิมทุกๆ 1 วินาที
}