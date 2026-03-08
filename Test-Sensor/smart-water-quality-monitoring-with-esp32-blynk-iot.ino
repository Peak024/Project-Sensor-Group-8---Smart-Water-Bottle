#define BLYNK_AUTH_TOKEN "cIV2w_XOQMO2gm8u0FRdCnhIaPQjGw0I"
#define BLYNK_TEMPLATE_ID "TMPL639m6ubrN"
#define BLYNK_TEMPLATE_NAME "Water Quality Monitoring"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>

#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Peak";
char pass[] = "lolxdlmao";

char auth[] = BLYNK_AUTH_TOKEN;

namespace pin {
    const byte tds_sensor = 34;
}

namespace device {
    float aref = 3.3; // Vref, this is for 3.3v compatible controller boards, for Arduino use 5.0v.
}

namespace sensor {
    float ec = 0;
    unsigned int tds = 0;
    float ecCalibration = 1;
}

void setup() {
    Serial.begin(115200); // Debugging on hardware Serial 0
    Blynk.begin(auth, ssid, pass);
    // Initialize LCD
  lcd.init();
  lcd.backlight();
  // Display "Tech Trends Shameer"
  lcd.setCursor(3, 0);
  lcd.print("Tech Trends ");
  lcd.setCursor(3, 1);
  lcd.print("  Shameer    ");
  delay(2000);  // Wait for 3 seconds
// Clear the LCD and display "Water Quality Monitoring"
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Water Quality");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring   ");
  lcd.setCursor(12, 0);  // Adjust the position for the heart symbol 
  
  delay(2000);
  lcd.clear();

}

void loop() {
    Blynk.run();
    readTdsQuick();
    delay(1000);
}

void readTdsQuick() {
    // Read the raw analog value and convert to voltage
    float rawEc = analogRead(pin::tds_sensor) * device::aref / 1024.0;
    
    // Debugging: Print the raw analog value
    Serial.print(F("Raw Analog Value: "));
    Serial.println(rawEc);

    // Adjust this offset based on the sensor's dry reading (without immersion)
    float offset = 0.14; // Set this to the observed raw analog value in air

    // Apply calibration and offset compensation
    sensor::ec = (rawEc * sensor::ecCalibration) - offset;

    // If the EC is below zero after adjustment, set it to zero
    if (sensor::ec < 0) sensor::ec = 0;

    // Convert voltage value to TDS value using a cubic equation
    sensor::tds = (133.42 * pow(sensor::ec, 3) - 255.86 * sensor::ec * sensor::ec + 857.39 * sensor::ec) * 0.5;

    // Debugging: Print the TDS and EC values
    Serial.print(F("TDS: "));
    Serial.println(sensor::tds);
    Serial.print(F("EC: "));
    Serial.println(sensor::ec, 2);
    lcd.setCursor(0, 0);
    lcd.print("TDS: ");

    lcd.setCursor(4,0);
    lcd.print(sensor::tds);

    lcd.setCursor(0,1);
    lcd.print("EC: ");

    lcd.setCursor(4,1);
    lcd.print(sensor::ec);

    // Send data to Blynk virtual pins
    Blynk.virtualWrite(V0, sensor::tds);
    Blynk.virtualWrite(V1, sensor::ec);
}



// #define BLYNK_AUTH_TOKEN "cIV2w_XOQMO2gm8u0FRdCnhIaPQjGw0I"
// #define BLYNK_TEMPLATE_ID "TMPL639m6ubrN"
// #define BLYNK_TEMPLATE_NAME "Water Quality Monitoring"

// #define BLYNK_PRINT Serial
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <BlynkSimpleEsp32.h>
// #include <LiquidCrystal_I2C.h>

// // LCD configuration: Address 0x27, 16 chars, 2 lines
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// // WiFi credentials
// char ssid[] = "Peak";
// char pass[] = "lolxdlmao";
// char auth[] = BLYNK_AUTH_TOKEN;

// // Pin and Constants
// namespace pin {
//     const byte tds_sensor = 34; // ขา Analog ที่ใช้
// }

// namespace device {
//     float aref = 3.3;          // แรงดันอ้างอิงของ ESP32
//     int adc_resolution = 4095; // 12-bit ADC สำหรับ ESP32
// }

// namespace sensor {
//     float voltage = 0;
//     unsigned int tds = 0;
//     float temperature = 25.0;  // ค่าอุณหภูมิมาตรฐาน (ถ้าไม่มีเซนเซอร์ temp ให้ใช้ค่านี้)
// }

// // ฟังก์ชันคำนวณค่า TDS
// void readTdsSensor() {
//     // 1. อ่านค่า Analog และแปลงเป็น Voltage (สำคัญ: หารด้วย 4095)
//     int rawValue = analogRead(pin::tds_sensor);
//     sensor::voltage = (float)rawValue * device::aref / device::adc_resolution;

//     // 2. Temperature Compensation (ปรับค่าตามอุณหภูมิ เพื่อความแม่นยำ)
//     // สูตร: V_final = V_raw / (1 + 0.02 * (T - 25))
//     float compensationCoefficient = 1.0 + 0.02 * (sensor::temperature - 25.0);
//     float compensationVoltage = sensor::voltage / compensationCoefficient;

//     // 3. แปลง Voltage เป็น TDS (สูตร Cubic Equation)
//     float tdsValue = (133.42 * pow(compensationVoltage, 3) - 255.86 * pow(compensationVoltage, 2) + 857.39 * compensationVoltage) * 0.5;
    
//     // ป้องกันค่าติดลบหรือค่ากระโดดกรณีไม่ได้จุ่มน้ำ
//     if (tdsValue < 0) tdsValue = 0;
//     sensor::tds = (unsigned int)tdsValue;

//     // --- การแสดงผลทาง Serial Monitor ---
//     Serial.print("ADC: "); Serial.print(rawValue);
//     Serial.print(" | Voltage: "); Serial.print(sensor::voltage);
//     Serial.print("V | TDS: "); Serial.print(sensor::tds);
//     Serial.println(" ppm");

//     // --- การแสดงผลทาง LCD ---
//     // บรรทัดแรก: TDS
//     lcd.setCursor(0, 0);
//     lcd.print("TDS: ");
//     lcd.print(sensor::tds);
//     lcd.print(" ppm    "); // เติมช่องว่างเผื่อล้างตัวเลขเก่า

//     // บรรทัดที่สอง: Voltage (ช่วยให้เราเช็คความผิดปกติได้ง่ายขึ้น)
//     lcd.setCursor(0, 1);
//     lcd.print("Volt: ");
//     lcd.print(sensor::voltage, 2);
//     lcd.print(" V      ");

//     // --- ส่งข้อมูลไป Blynk ---
//     Blynk.virtualWrite(V0, sensor::tds);      // ตั้งค่าใน App เป็น TDS
//     Blynk.virtualWrite(V1, sensor::voltage);  // ตั้งค่าใน App เป็น Voltage/EC
// }

// // ใช้ BlynkTimer เพื่อไม่ให้ loop ทำงานหนักเกินไป
// BlynkTimer timer;

// void myTimerEvent() {
//     readTdsSensor();
// }

// void setup() {
//     Serial.begin(115200);
    
//     // เริ่มทำงาน LCD
//     lcd.init();
//     lcd.backlight();
    
//     lcd.setCursor(2, 0);
//     lcd.print("Tech Trends");
//     lcd.setCursor(4, 1);
//     lcd.print("Shameer");
//     delay(3000);
//     lcd.clear();

//     // เริ่มทำงาน Blynk
//     Blynk.begin(auth, ssid, pass);

//     // ตั้งเวลาอ่านเซนเซอร์ทุกๆ 1.5 วินาที (แทนการใช้ delay ใน loop)
//     timer.setInterval(1500L, myTimerEvent);
// }

// void loop() {
//     Blynk.run();
//     timer.run(); // จัดการคิวการทำงานของ Timer
// }