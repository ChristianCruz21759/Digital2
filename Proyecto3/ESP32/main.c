#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>

/* WIFI y contraseña */
const char* ssid = "ChristiansMall :D";  // Enter SSID here
const char* password = "quebonitosojostienes";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,5,1);
IPAddress gateway(192,168,5,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

int parqueoS1 = 4;
int parqueoS2 = 4;
int parqueoS1B = 0b1010;
int parqueoS2B = 0b0111;
int parqueoS1Anterior = 0;
int parqueoS2Anterior = 0;
int ParqueoTotal = 8;

// Definición de los pines del decodificador 7447 conectados al Arduino
int pinA = 19; 
int pinB = 21; 
int pinC = 22; 
int pinD = 23; 

#define RX1 27
#define TX1 26
#define RX2 16
#define TX2 17

void setup() {
  //Wire.begin();
  Serial.begin(115200);                             //SERIAL USB
  Serial1.begin(38400, SERIAL_8N1, RX1, TX1);       //SERIAL SOTANO 1
  Serial2.begin(38400, SERIAL_8N1, RX2, TX2);       //SERIAL SOTANO 2 
  //Configuración del servidor wifi como AP
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  //Configuración del servidor
  server.on("/", handle_OnConnect);
  server.on("/MenuPrincipal", handle_Menu);
  server.on("/Sotano1", handle_S1);
  server.on("/Sotano2", handle_S2);
  server.onNotFound(handle_NotFound);

  // Configuración de los pines del decodificador 7447 como salidas
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  //Iniciamos el servidor
  server.begin();
  //Serial.println("HTTP server started");

}


void loop() {
  server.handleClient();//Comienza el servidor
  parqueoS1=binarioADisponibles(parqueoS1B);
  parqueoS2=binarioADisponibles(parqueoS2B);
  /*// Leer desde el primer dispositivo I2C (Tiva 1)
  Wire.beginTransmission(Tiva1Address);
  Wire.endTransmission();

  Wire.requestFrom(Tiva1Address, 1); // Solicita un byte de datos*/
  Serial1.write(0); //Enviar valor a Tiva1 para activar interrupción y que mande un valor de regreso
  //while (!Serial.available());
  if (Serial1.available()) {
    parqueoS1B = Serial1.read(); // parqueoS1B es un valor en binario donde 1111 significa que hay 4 parqueos disponibles
    Serial.print("S1: ");
    Serial.println(parqueoS1B, BIN);
    /*Serial.print("Dato leído desde el primer dispositivo (Tiva 1): ");
    Serial.println(parqueoS1B);
    Serial.print("Parqueos Disponibles:");
    Serial.println(parqueoS1);*/
  }

  /*// Leer desde el segundo dispositivo I2C (Tiva 2)
  Wire.beginTransmission(Tiva2Address);
  Wire.endTransmission();

  Wire.requestFrom(Tiva2Address, 1); // Solicita un byte de datos*/

  Serial2.write(0); //Enviar valor a Tiva para activar interrupción y que mande un valor de regreso
  //while (!Serial2.available());
  if (Serial2.available()) {
    parqueoS2B = Serial2.read(); // parqueoS2B es un valor en binario donde 1111 significa que hay 4 parqueos disponibles
    Serial.print("S2: ");
    Serial.println(parqueoS2B, BIN);
    /*Serial.print("Dato leído desde el segundo dispositivo (Tiva 2): ");
    Serial.println(parqueoS2B);
    Serial.print("Parqueos Disponibles:");
    Serial.println(parqueoS2);*/
  }
  parqueoS1=binarioADisponibles(parqueoS1B);
  parqueoS2=binarioADisponibles(parqueoS2B);

  if (parqueoS1 != parqueoS1Anterior || parqueoS2 != parqueoS2Anterior) {
    if (parqueoS1 != parqueoS1Anterior) {
      parqueoS1Anterior = parqueoS1;
    }
    if (parqueoS2 != parqueoS2Anterior) {
      parqueoS2Anterior = parqueoS2;
    }
    ParqueoTotal = parqueoS2 + parqueoS1;
    actualizarDisplay7447(ParqueoTotal);
  }

  //delay(1000); //Maybe si maybe no
}

void actualizarDisplay7447(int valor) {
  // Convertir el valor a BCD (Binary-Coded Decimal) para el decodificador 7447
  int unidades = valor % 10;
  int decenas = (valor / 10) % 10;

  // Actualizar los pines del decodificador 7447 para mostrar el valor
  digitalWrite(pinA, unidades & 1);
  digitalWrite(pinB, (unidades >> 1) & 1);
  digitalWrite(pinC, (unidades >> 2) & 1);
  digitalWrite(pinD, decenas & 1);
}

int binarioADisponibles(int numero){
  int contador=0;
  int numeroBinario=numero;
  while (numeroBinario > 0) {
    if (numeroBinario & 1) {
      contador++;
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha
  }
  return contador;
}

String SendMenuHTML(int ParqueoS1, int ParqueoS2) {
  String ptr = "<!DOCTYPE html>\n<html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ChristiansMall</title>\n";
  ptr += "<style>\n";
  ptr += "html {font-family: Helvetica; text-align: center; background-color: #f2f2f2;}\n";
  ptr += "body {margin: 0; padding: 0; font-family: Helvetica;}\n";
  ptr += "h1 {color: #444444; margin: 50px auto 30px;}\n";
  ptr += "h2 {color: #444444; margin-bottom: 30px;}\n";
  ptr += ".button-container {display: flex; flex-direction: column; align-items: center;}\n";
  ptr += ".button {width: 200px; background-color: #3498db; border: none; color: white; padding: 20px; text-decoration: none; font-size: 25px; margin: 20px; cursor: pointer; border-radius: 4px; display: flex; justify-content: center; align-items: center;}\n";
  ptr += "p {font-size: 14px; color: #888; margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ChristiansMall</h1>\n";
  ptr += "<h2>Selecciona un Sotano</h2>\n";
  ptr += "<div class=\"button-container\">\n";
  if (ParqueoS1==0){
    ptr += "<a class=\"button\" href=\"/Sotano1\">Sotano 1 &#128274;</a>\n"; //Lleno (los 4 llenos)
  }
  else if (ParqueoS1==4){
    ptr += "<a class=\"button\" href=\"/Sotano1\">Sotano 1 &#x1F578;</a>\n"; //Vacio (los 4 disponibles)
  }
  else {
    ptr += "<a class=\"button\" href=\"/Sotano1\">Sotano 1</a>\n"; //Normal
  }
  
  String ParqueoS1String = String(ParqueoS1);
  ptr+="<h3>No. de Parqueos S1:";
  ptr+=ParqueoS1String;
  ptr+="</h3>";
  if (ParqueoS2==0){
    ptr += "<a class=\"button\" href=\"/Sotano2\">Sotano 2 &#128274;</a>\n"; //Lleno (los 4 llenos)
  }
  else if (ParqueoS2==4){
    ptr += "<a class=\"button\" href=\"/Sotano2\">Sotano 2 &#x1F578;</a>\n"; //Vacio (los 4 disponibles)
  }
  else {
    ptr += "<a class=\"button\" href=\"/Sotano2\">Sotano 2</a>\n"; //Normal
  }
  String ParqueoS2String = String(ParqueoS2);
  ptr+="<h3>No. de Parqueos S2:";
  ptr+=ParqueoS2String;
  ptr+="</h3>";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

String SendSotanoHTML(int Sotano, int parqueoSB) {
  int numeroBinario=parqueoSB;
  String ptr = "<!DOCTYPE html>\n<html>\n<head>\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  if (Sotano==1){
    ptr += "<title>Sotano1</title>\n";
  }
  else if (Sotano==2){
    ptr += "<title>Sotano2</title>\n";
  }
  ptr += "<style>\n";
  ptr += "html {font-family: Helvetica; text-align: center; background-color: #f2f2f2;}\n";
  ptr += "body {margin: 0; padding: 0; font-family: Helvetica;}\n";
  ptr += "h1 {color: #444444; margin: 50px auto 30px;}\n";
  ptr += "h2 {color: #444444; margin-bottom: 30px;}\n";
  ptr += "h4 {color: #444444; margin-bottom: 30px;}\n";
  ptr += "table {border-collapse: collapse; width: 100%; margin: 0;}\n";
  ptr += "th, td {padding: 15px; text-align: center;}\n";
  ptr += "th {background-color: #3498db; color: white;}\n";
  ptr += ".available {background-color: #28A745;}\n";
  ptr += ".unavailable {background-color: #DC3545;}\n";
  ptr += ".button {display: block; width: 80px; background-color: #3498db; border: none; color: white; padding: 13px 30px;text-decoration: none; font-size: 25px; margin: 20px auto 35px; cursor: pointer; border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px; color: #888; margin-bottom: 10px;}\n";
  ptr += ".back-button {position: absolute; top: 20px; left: 20px; background-color: #3498db; border: none; color: white; padding: 10px 20px; text-decoration: none; font-size: 18px; border-radius: 4px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  //BOTON
  ptr += "<a class=\"back-button\" href=\"/MenuPrincipal\">Volver</a>\n";
  ptr += "<h1>ChristiansMall Parking Lots</h1>\n";
  if (Sotano==1){
    ptr += "<h2>Sotano 1</h2>\n";
  }
  else if (Sotano==2){
    ptr += "<h2>Sotano 2</h2>\n";
  }
  
  ptr += "<h4>Parqueos Disponibles</h4>\n";
  ptr += "<table>\n";
  ptr += "<tr>\n";
  ptr += "<th>No. del Parqueo</th>\n";
  ptr += "<th>Disponibilidad</th>\n";
  ptr += "</tr>\n";
  if (Sotano==1){
    //Primer parqueo
    if (!(numeroBinario & 1)) {
      ptr += "<tr class=\"unavailable\">\n";
      ptr += "<td>1</td>\n";
      ptr += "<td>No Disponible &#x1F6AB;</td>\n";
      ptr += "</tr>\n";
    }
    else {
      ptr += "<tr class=\"available\">\n";
      ptr += "<td>1</td>\n";
      ptr += "<td>Disponible &#x2705;</td>\n";
      ptr += "</tr>\n";
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha
    //Segundo Parqueo
    if (!(numeroBinario & 1)) {
      ptr += "<tr class=\"unavailable\">\n";
      ptr += "<td>2</td>\n";
      ptr += "<td>No Disponible &#x1F6AB;</td>\n";
      ptr += "</tr>\n";
    }
    else {
      ptr += "<tr class=\"available\">\n";
      ptr += "<td>2</td>\n";
      ptr += "<td>Disponible &#x2705;</td>\n";
      ptr += "</tr>\n";
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha
    //Tercer parqueo
    if (!(numeroBinario & 1)) {
      ptr += "<tr class=\"unavailable\">\n";
      ptr += "<td>3</td>\n";
      ptr += "<td>No Disponible &#x1F6AB;</td>\n";
      ptr += "</tr>\n";
    }
    else {
      ptr += "<tr class=\"available\">\n";
      ptr += "<td>3</td>\n";
      ptr += "<td>Disponible &#x2705;</td>\n";
      ptr += "</tr>\n";
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha
    //Cuarto Parqueo
    if (!(numeroBinario & 1)) {
      ptr += "<tr class=\"unavailable\">\n";
      ptr += "<td>4</td>\n";
      ptr += "<td>No Disponible &#x1F6AB;</td>\n";
      ptr += "</tr>\n";
    }
    else {
      ptr += "<tr class=\"available\">\n";
      ptr += "<td>4</td>\n";
      ptr += "<td>Disponible &#x2705;</td>\n";
      ptr += "</tr>\n";
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha    
  }
  else if (Sotano==2){
    //Quinto parqueo
    if (!(numeroBinario & 1)) {
      ptr += "<tr class=\"unavailable\">\n";
      ptr += "<td>5</td>\n";
      ptr += "<td>No Disponible &#x1F6AB;</td>\n";
      ptr += "</tr>\n";
    }
    else {
      ptr += "<tr class=\"available\">\n";
      ptr += "<td>5</td>\n";
      ptr += "<td>Disponible &#x2705;</td>\n";
      ptr += "</tr>\n";
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha
    //Sexto Parqueo
    if (!(numeroBinario & 1)) {
      ptr += "<tr class=\"unavailable\">\n";
      ptr += "<td>6</td>\n";
      ptr += "<td>No Disponible &#x1F6AB;</td>\n";
      ptr += "</tr>\n";
    }
    else {
      ptr += "<tr class=\"available\">\n";
      ptr += "<td>6</td>\n";
      ptr += "<td>Disponible &#x2705;</td>\n";
      ptr += "</tr>\n";
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha
    //Septimo parqueo
    if (!(numeroBinario & 1)) {
      ptr += "<tr class=\"unavailable\">\n";
      ptr += "<td>7</td>\n";
      ptr += "<td>No Disponible &#x1F6AB;</td>\n";
      ptr += "</tr>\n";
    }
    else {
      ptr += "<tr class=\"available\">\n";
      ptr += "<td>7</td>\n";
      ptr += "<td>Disponible &#x2705;</td>\n";
      ptr += "</tr>\n";
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha
    //Octavo Parqueo
    if (!(numeroBinario & 1)) {
      ptr += "<tr class=\"unavailable\">\n";
      ptr += "<td>8</td>\n";
      ptr += "<td>No Disponible &#x1F6AB;</td>\n";
      ptr += "</tr>\n";
    }
    else {
      ptr += "<tr class=\"available\">\n";
      ptr += "<td>8</td>\n";
      ptr += "<td>Disponible &#x2705;</td>\n";
      ptr += "</tr>\n";
    }
    numeroBinario = numeroBinario >> 1; // Desplaza un bit a la derecha
  }
  
  ptr += "</table>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void handle_OnConnect() {
  parqueoS1B=0;
  parqueoS2B=0;
  server.send(200, "text/html", SendMenuHTML(parqueoS1,parqueoS2)); 
}

void handle_Menu() {
  server.send(200, "text/html", SendMenuHTML(parqueoS1,parqueoS2)); 
}

void handle_S1() {
  server.send(200, "text/html", SendSotanoHTML(1, parqueoS1B));
}
void handle_S2() {
  server.send(200, "text/html", SendSotanoHTML(2, parqueoS2B));
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

