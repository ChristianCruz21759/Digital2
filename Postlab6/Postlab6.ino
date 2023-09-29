
#include <SPI.h>
#include <SD.h>

//Declaracion de Variables

File myFile;
File root;

int option = 1;
int state = 0;
int change = 0;
int maxOptions = -2;

const int button1 = PUSH1;
const int button2 = PUSH2;

int lastButton1State = LOW;
int lastButton2State = LOW;
int button1State;
int button2State;

long debounceDelay = 200;

//Setup inicial
void setup() {
  //Comunicacion Serial
  Serial.begin(115200);
  //Pines
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  //Modulo SPI
  SPI.setModule(2);
  //Funcion para escribir menu
  menu();

  Serial.println("Initializing SD card...");
  //Iniciar SD
  if (!SD.begin(2)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");
  //Contar archivos en la SD
  countFiles(root);

  change = 1;

}

//Loop Principal
void loop() {
  //Leer botones
  int reading1 = digitalRead(button1);
  int reading2 = digitalRead(button2);

  switch (state) {
    case 0:
      //Debouncers
      if (lastButton1State == HIGH && reading1 == LOW) {
        button1State = HIGH;
      } else if (lastButton1State == LOW && reading1 == HIGH) {
        button1State = LOW;
      }

      if (lastButton2State == HIGH && reading2 == LOW) {
        button2State = HIGH;
      } else if (lastButton2State == LOW && reading2 == HIGH) {
        button2State = LOW;
      }

      //Boton para cambiar opcion en el menu
      if (button1State == HIGH) {
        option++;
        change = 1;
        if (option >= maxOptions + 1) {
          option = 1;
        }
        delay(debounceDelay);
      }

      //Boton para seleccionar opcion
      if (button2State == HIGH) {
        Serial.print("Seleccionaste la opción ");
        Serial.println(option);
        delay(debounceDelay);
        state = 1;
      }
      //Mostrar menu
      if (change == 1) {
        menu();
        change = 0;
      }
      //Leer monitor serial
      if (Serial.available() > 0) {
        String inputString = Serial.readStringUntil('\n');
        maxOptions++; //Incrementar menu

        char new_name[] = {'t', 'e', 's', 't', maxOptions + 48, '.', 't', 'x', 't'};  //Nuevo nombre
        Serial.println(new_name);

        myFile = SD.open(new_name, FILE_WRITE); //Crear nuevo archivo

        //Escribir al nuevo archivo
        if (myFile) {
          Serial.print("Escribiendo a nuevo archivo...");
          myFile.println(inputString);
          //Cerrar
          myFile.close();
          Serial.println("Listo.");
        } else {
          Serial.println("Error creando un nuevo archivo");
        }
        Serial.println(inputString);
      }

      lastButton1State = reading1;
      lastButton2State = reading2;

      break;
    case 1:

      //Leer archivo
      char name[] = {'t', 'e', 's', 't', option + 48, '.', 't', 'x', 't'};
      readFile(name);
      state = 0;
      break;
  }
}

void menu(void) {

  Serial.println("======================================");
  Serial.println("Menú");
  for (int i = 1; i < maxOptions + 1; i++) {
    if (option == i) {
      Serial.print("> ");
    }
    Serial.print("Opción ");
    Serial.print(i);
    Serial.println(":");
  }

  for (int j = 0; j < 13 - maxOptions; j++) {
    Serial.print("\n");
  }

}

//Funcion para leer archivos con el parametro de nombre
void readFile(char temp[]) {
  myFile = SD.open(temp);
  if (myFile) {
    Serial.print(temp);
    Serial.println(":");
    //Leer hasta que no haya nada dentro del archivo
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    //Cerrar el arcvhivo
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error abriendo archivo");
  }
  Serial.print('\n');
  delay(1000);
  change = 1;
}

//Funcion para contar archivos dentro de la SD
void countFiles(File dir) {
  while (true) {
    maxOptions++;
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    entry.close();
  }
}
