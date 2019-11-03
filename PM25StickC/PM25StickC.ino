#include <M5StickC.h>
#include <Wire.h>
#include "DFRobot_SHT20.h"
DFRobot_SHT20    sht20;

#define TFT_GREY 0x7BEF

#define DATA_LEN 32


uint16_t CheckSum;
uint16_t CheckCode;

void setup() {
  
  M5.begin();
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 32, 33); // Grove UART RX:32, TX:33
  Wire.begin(0, 26);                       // I2C pin 0:SDA 26:SCL
  
  M5.Lcd.setRotation(2);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("PM2.5");

  sht20.initSHT20(Wire);                   // Init SHT20 Sensor
  delay(100);
  sht20.checkSHT20(); 
}

uint8_t Air_val[32]={0};
int16_t p_val[16]={0};
uint8_t count=0;
const char filler[] = "     ";

#define FONT 1

void LCD_Display_Val(void){              

     for(int i=0,j=0;i<32;i++){
        if(i%2==0){
          p_val[j] = Air_val[i];
          p_val[j] = p_val[j] <<8;
        }else{
          p_val[j] |= Air_val[i];
          j++;
        }
      
     }

     M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);
     
     M5.Lcd.setCursor(0, 10, FONT);
     M5.Lcd.printf("PM1.0 : %d   ", p_val[2]); 

     M5.Lcd.setCursor(0, 20 , FONT);
     M5.Lcd.printf("PM2.5 : %d   ", p_val[3]); 

     M5.Lcd.setCursor(0, 30 , FONT);
     M5.Lcd.printf("PM10  : %d   ", p_val[4]); 

     M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
     M5.Lcd.setCursor(0, 45 , FONT);
     M5.Lcd.printf("PM1.0 : %d   ", p_val[5]); 

     M5.Lcd.setCursor(0, 55 , FONT);
     M5.Lcd.printf("PM2.5 : %d   ", p_val[6]); 

     M5.Lcd.setCursor(0, 65 , FONT);
     M5.Lcd.printf("PM10  : %d   ", p_val[7]); 

     M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
     M5.Lcd.setCursor(0, 80 , FONT);
     M5.Lcd.printf("0.3um : %d   ", p_val[8]); 

     M5.Lcd.setCursor(0, 90 , FONT);
     M5.Lcd.printf("0.5um : %d   ", p_val[9]); 

     M5.Lcd.setCursor(0, 100 , FONT);
     M5.Lcd.printf("1.0um : %d   ", p_val[10]); 

     M5.Lcd.setCursor(0, 110 , FONT);
     M5.Lcd.printf("2.5um : %d   ", p_val[11]);

     M5.Lcd.setCursor(0, 120, FONT);
     M5.Lcd.printf("5.0um : %d   ", p_val[12]);

     M5.Lcd.setCursor(0, 130, FONT);
     M5.Lcd.printf("10um  : %d   ", p_val[13]); 
}


void TempHumRead(void)
{
    float humd = sht20.readHumidity();                  // Read Humidity
    float temp = sht20.readTemperature();               // Read Temperature

     M5.Lcd.setTextColor(TFT_GREEN,TFT_BLACK);
     M5.Lcd.setCursor(0, 140 , FONT);
     M5.Lcd.printf("TMP   : %2.2f ", temp); 

     M5.Lcd.setCursor(0, 150 , FONT);
     M5.Lcd.printf("HUM   : %2.2f ", humd); 
}

void loop() {
  
   if(Serial2.available()) {
      Air_val[count] = Serial2.read();
      Serial.write(Air_val[count]);
      count++;
   }else{
      count=0;
   }
  Serial.printf("data:%d\n", count);
  if(count == DATA_LEN){
    LCD_Display_Val();
    TempHumRead();
    count = 0;
  }
}
