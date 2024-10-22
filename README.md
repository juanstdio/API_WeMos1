# WeMos D1 Mini FastAPI Demo

Este proyecto utiliza una **pantalla LCD de 20x4 conectada a una WeMos D1 Mini** a través de la interfaz **I2C**, 
para mostrar datos obtenidos de CAMMESA y la Represa de Salto Grande desde una API casera a través de una red WiFi. También incluye verificación de la conexión a internet mediante una solicitud HTTP.

#### **CAMMESA**: Demanda Actual y Demanda Estimada (Predespacho) 
#### **Salto Grande**: Potencia Instantánea, Máquinas activas, Altura Lago Salto Grande y Altura Río Uruguay

![image](https://raw.githubusercontent.com/juanstdio/API_WeMos1/refs/heads/main/photo_4951855146783059849_y.jpg)
###Instalación de Librerías
Para este proyecto, necesitarás instalar las siguientes librerías en tu entorno de desarrollo Arduino:

**ESP8266WiFi**: Esta librería ya está incluida en el paquete de placas ESP8266 en el gestor de tarjetas de Arduino.

**ESP8266HTTPClient**: También viene incluida en el mismo paquete de ESP8266.

**WiFiClient**: Forma parte de las librerías por defecto cuando se trabaja con ESP8266.

**LiquidCrystal_I2C**: Ve al Gestor de Librerías en el IDE de Arduino.
Busca LiquidCrystal_I2C y selecciona una de las versiones disponibles (recomendamos usar una versión estable).
Instálala y asegúrate de que esté correctamente integrada en tu proyecto.


Se detalla cómo conectar la pantalla LCD 20x4 a la WeMos D1 Mini:

***Conexión:***

 WeMos D1 Mini Case
```
SCL Screen -> Pin D1 (Wemos D1 Mini)
SDA Screen -> Pin D2 (Wemos D1 Mini)
5v Screen -> 5v (Wemos D1 Mini)
GND Screen -> GND (Wemos D1 Mini)
```

## Licencia
Este proyecto está bajo la Licencia Apache 2.0:
**Permite**: Uso, distribución, modificación, sublicenciamiento.

**Requiere**: Mantener los avisos de derechos de autor y una copia de la licencia.

**Garantiza**: Libertad de uso, incluso en proyectos propietarios.

**No garantiza**: Responsabilidad del software, es decir, no ofrece garantías sobre su funcionamiento.
