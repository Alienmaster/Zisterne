// https://www.mymakerstuff.de/2016/05/24/arduino-tutorial-der-ultraschallsensor/
// https://github.com/gavinlyonsrepo/NOKIA5110_TEXT
// https://daniel-ziegler.com/arduino/mikrocontroller/2017/06/16/Funksteckdose-arduino/
// https://forum.arduino.cc/index.php?topic=134022.0


#include <NOKIA5110_TEXT.h>

// Display Parameters
#define inverse false
#define contrast 0xB1 // default is 0xBF set in LCDinit, Try 0xB1 - 0xBF if your display is too dark
#define bias 0x12 // LCD bias mode 1:48: Try 0x12 or  0x13 or 0x14 usually. 
#define CE 2
#define RST 3
#define DC 4
#define DIN 5
#define CLK 6

// Create an LCD object
NOKIA5110_TEXT mylcd(RST, CE, DC, DIN, CLK);
// cistern data (cylinder)
int radius = 125;                           // radius in cm 
int height = 220;                           // height in cm
long surface = (radius * radius) * PI;      // volume in cm

int trigger = 9;                            // Trigger Pin
int echo = 8;                               // Echo Pin
long duration = 0;                          // Time in milliseconds to get back to the sensor
long level = 0;                             // The distance from the sensor to the water
long litres = 0;                            // amount of litres in the cistern

void setup() {
    mylcd.LCDInit(inverse, contrast, bias);  // init the lCD
    mylcd.LCDClear();                        // clear whole screen
    Serial.begin(9600);
    pinMode(trigger, OUTPUT);                // Trigger Pin als Ausgang definieren
    pinMode(echo, INPUT);                    // Echo Pin als Eingang defnieren
}
        
void loop() {
    mylcd.LCDClear(0X00);                    // clear screen
    digitalWrite(trigger, LOW);              // Set at first trigger LOW
    delay(5);
    digitalWrite(trigger, HIGH);             // Set trigger HIGH to send ultrasonic wave
    delay(10);
    digitalWrite(trigger, LOW);              // Trigger to LOW to finish send
    duration = pulseIn(echo, HIGH);          // Time till ultrasonic wave returns
    level = height - ((duration / 2) / 29.1);             // Calculation from time to cm
    litres = (level * surface) / 1000;        // litres in the cistern

    Serial.print(litres);                   // Den Weg in Zentimeter ausgeben
    Serial.println(" Liter");               //
    
    char litresC[8];
    dtostrf(litres, 0, 0, litresC);
    
    
    if (litres > 0){
        mylcd.LCDFont(9);                        // Print in the first column the amount of water
        mylcd.LCDgotoXY(2, 0);
        mylcd.LCDString(litresC);
        mylcd.LCDFont(7);                        // Last column in the middle "Liter"
        mylcd.LCDgotoXY(12, 4);
        mylcd.LCDString("LITER");
    }
    else{
        mylcd.LCDFont(7);                        // Print in the first column the amount of water
        mylcd.LCDgotoXY(12, 2);
        mylcd.LCDString("ERROR");
    }
    
    
    
    delay(2000);                             // Nach einer Sekunde wiederholen

}