/* API WeMos D1 mini Demo */
/* Proyecto realizado para demostrar las capacidades de la WeMos D1 para obtener información de una API hosteada localmente */
/* Y mostrarlas en un LCD de 20x4 */
/* SADI : https://es.wikipedia.org/wiki/Sistema_Argentino_de_Interconexi%C3%B3n */
/* Sato Grande: https://es.wikipedia.org/wiki/Represa_de_Salto_Grande */
/* Craneado por Juan Blanc en Agosto 2024, adaptacion del código mostrado en https://github.com/juanstdio/worldclock_LCD */ 

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <LiquidCrystal_I2C.h>
#define lcdDir 0x27
LiquidCrystal_I2C lcd(lcdDir,20,4);

byte MAQ[] = {
  B11011,
  B10001,
  B01010,
  B01110,
  B01010,
  B10001,
  B11011,
  B11111,
};

byte LAGO[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11111,
  B11111,
};

byte RIO[] = {
  B11100,
  B10010,
  B10010,
  B11110,
  B11100,
  B10110,
  B10011,
  B10001,
};

byte RAYO[] = {
  B00100,
  B01100,
  B11000,
  B11111,
  B00110,
  B01100,
  B01000,
  B10000,
};

byte SGA[] = {
  B10010,
  B10010,
  B10110,
  B10100,
  B11000,
  B11000,
  B11110,
  B11110,
};
byte SGB[] = {
  B00000,
  B00000,
  B00000,
  B10000,
  B10000,
  B11000,
  B11110,
  B11111,
};

byte RAD0[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10000
};


byte RAD1[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11000,
  B00100,
  B10100
};

byte RAD2[] = {
  B00000,
  B00000,
  B00000,
  B11100,
  B00010,
  B11001,
  B00101,
  B10101
};

byte RAD3[] = {
  B00000,
  B00000,
  B00000,
  B11100,
  B00010,
  B00001,
  B00001,
  B10001
};

byte DE4D[] = {
  B11011,
  B11011,
  B00100,
  B00000,
  B00000,
  B11111,
  B10001,
  B10001
};

byte DA[] = {
  B11000,
  B10100,
  B10100,
  B11010,
  B00001,
  B00011,
  B00101,
  B00011
};

byte DE[] = {
  B11000,
  B10100,
  B10100,
  B11010,
  B00101,
  B00111,
  B00100,
  B00011
};


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  WiFi.mode(WIFI_STA);
  WiFi.begin("SSID", "PWD");

  lcd.setCursor(0,0);
  lcd.print(F("Conectando "));

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wifi OK!");
  delay(200);
  lcd.clear();
}

void loop() {
  /* lcd.createChar(0,RIO); REPLACED by char*/ 
  /* lcd.createChar(1,LAGO); REPLACED by char */ 
  lcd.createChar(0,DA); /*Demanda actual */
  lcd.createChar(1,DE); /* Predespacho (Demanda estimada) */ 
  lcd.createChar(2,MAQ);  /*Maquinas salto grande */
  lcd.createChar(3,RAYO);  /*Rayito! */
  lcd.createChar(4,SGA);  /*Salto Grande logo */
  lcd.createChar(5,RAD0);  /*Señal frame 0 */
  lcd.createChar(6,RAD1);  /*Señal frame 1 */
  lcd.createChar(7,RAD2);  /*Señal frame 2 */
  
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient UPS;
    HTTPClient SADI;
    HTTPClient SG;
    HTTPClient CHECK_OK; 
    CHECK_OK.begin(client, "http://1.1.1.1");
    int httpCode0 = CHECK_OK.GET();  
    if (httpCode0 == 301) {      
            Serial.println("OK_INTERNET");  
        } 
    else {
            lcd.createChar(7,DE4D);
            Serial.print("HTTP Code: ");
            Serial.println(httpCode0); 
            lcd.setCursor(1, 0);
            lcd.write(byte(7));
            delay(10000);
            ESP.restart();
         }
   CHECK_OK.end();  


    /* Start Drawing crap */
    UPS.begin(client, "http://192.168.0.8:5005/api/last_ups_data");  
    int httpCode = UPS.GET();
    Serial.println(httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = UPS.getString();
      Serial.println(payload);
      int loadIndex = payload.indexOf("\"loadpct\":") + 10;
      int endIndex = payload.indexOf(",", loadIndex);
      String loadpct = payload.substring(loadIndex, endIndex);
      
      int loadIndex2 = payload.indexOf("\"linev\":") + 8;
      int endIndex2 = payload.indexOf(",", loadIndex2);
      String linev = payload.substring(loadIndex2, endIndex2);

      lcd.setCursor(0, 0);
      lcd.print("UPS ");
      lcd.print(loadpct);
      lcd.print("%");
      lcd.print(" VIn ");
      lcd.print(linev);
      lcd.write(byte(3));    
    }
    UPS.end();
    /* second part */
    SADI.begin(client, "http://192.168.0.8:5005/api/last_sadi");  // URL actualizada
    int httpCode2 = SADI.GET();
    Serial.println(httpCode2);
    if (httpCode2 == HTTP_CODE_OK) {
      String payload = SADI.getString();
      Serial.println(payload);
      int demHoyIndex = payload.indexOf("\"DemHoy\":") + 9;
      int endIndex = payload.indexOf(".", demHoyIndex);  // Hasta el primer punto
      String demHoy = payload.substring(demHoyIndex, endIndex);
      int PreIndex = payload.indexOf("\"Predespacho\":") + 14;
      int PreendIndex = payload.indexOf(".", PreIndex);  // Hasta el primer punto
      String PreDespacho = payload.substring(PreIndex, PreendIndex);
      Serial.println(PreDespacho);
      lcd.setCursor(0, 1);
      lcd.print("SI");
      lcd.write(byte(0));
      lcd.print(demHoy);
      lcd.print("MW");
      lcd.write(byte(3)); /* caracterr ignorado por falta de espacio */
      lcd.print(" ");
      lcd.write(byte(1));
      lcd.print(PreDespacho);
      lcd.print("MW");
      // Limpia caracteres residuales
    }
    SADI.end();
    /* Salto Grande */
    SG.begin(client, "http://192.168.0.8:5005/api/last_sg");  // URL para last_sg
    int httpCode3 = SG.GET();
    Serial.println(httpCode3);
    if (httpCode3 == HTTP_CODE_OK) {
      String payload = SG.getString();
      Serial.println(payload);
      // Obtener PotI hasta el primer espacio
      int potIIndex = payload.indexOf("\"PotI\":\"'") + 9;
      int potIEnd = payload.indexOf(" ", potIIndex);
      String potI = payload.substring(potIIndex, potIEnd);
      // Obtener MaquinasActivas
      int maquinasIndex = payload.indexOf("\"MaquinasActivas\":\"'") + 20;
      int maquinasEnd = payload.indexOf("'", maquinasIndex);
      String maquinasActivas = payload.substring(maquinasIndex, maquinasEnd);
      // Obtener avgCotaEmbalse
      int cotaEmbalseIndex = payload.indexOf("\"avgCotaEmbalse\":\" '") + 20;
      int cotaEmbalseEnd = payload.indexOf(" ", cotaEmbalseIndex);
      String avgCotaEmbalse = payload.substring(cotaEmbalseIndex, cotaEmbalseEnd);
      avgCotaEmbalse.replace(",", ".");
      // Obtener avgCotaRestitu
      // Obtener avgCotaRestitu hasta la coma y limpiar comillas
      int cotaRestituIndex = payload.indexOf("\"avgCotaRestitu\":\" '") + 19;
      int cotaRestituEnd = payload.indexOf(" ", cotaRestituIndex);
      String avgCotaRestitu = payload.substring(cotaRestituIndex, cotaRestituEnd);
      avgCotaRestitu.trim();  // Limpiar espacios en blanco adicionales
      avgCotaRestitu.replace(",", ".");
      avgCotaRestitu.replace("'", "");
      // Mostrar PotI y MaquinasActivas en la posición (0,2)
      lcd.setCursor(0, 2);
      lcd.print("SG");
      lcd.write(byte(4));
      lcd.print("  ");
      lcd.print(potI);
      lcd.print(" MW");
      lcd.write(byte(3));
      lcd.print(" ");
      lcd.write(byte(2));
      lcd.print(maquinasActivas);
   // Mostrar avgCotaEmbalse y avgCotaRestitu en la posición (3,0)
      lcd.setCursor(0, 3);
      lcd.print("L");
      lcd.print(avgCotaEmbalse); //LAGO
      lcd.print(" ");
      lcd.print("R");
      lcd.print(avgCotaRestitu); // RIO
    }
    SG.end();
  }
  
  //delay(15000);
  // DELAY casero
    const int totalDuration = 15000;  // Duración total en milisegundos
    const int updateInterval = totalDuration / 4;  // Intervalo de actualización
    const byte fillChar = 8;  // Asegúrate de haber creado un carácter personalizado para la barra, como un bloque lleno

    for (int i = 0; i <= 15; i++) {
        lcd.setCursor(17, 3);  
        lcd.print(" "); 
        delay(250);
        lcd.setCursor(17, 3);
        lcd.write(byte(5)); 
        delay(250);
        lcd.setCursor(17, 3);
        lcd.write(byte(6)); 
        delay(250);
        lcd.setCursor(17, 3);
        lcd.write(byte(7)); 
        delay(250);
        }
        // Espera el intervalo de actualización
        delay(updateInterval);
}
