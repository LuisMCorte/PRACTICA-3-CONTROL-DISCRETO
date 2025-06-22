#include <Controllino.h>
#include "Stone_HMI_Define.h"
#include "Procesar_HMI.h"

// -------------------- Pines --------------------
const int led       = CONTROLLINO_D0;   // Salida PWM 0
const int led2      = CONTROLLINO_D1;   // Salida PWM 1
const int botonI16  = CONTROLLINO_I16;  // Botón para D0
const int botonI17  = CONTROLLINO_I17;  // Botón para D1

// -------------------- Máquina de estados --------------------
enum LedState { OFF, ON };
LedState stateD0 = OFF;  
LedState stateD1 = OFF;

// -------------------- Variables de botones (antirrebote) --------------------
int lastBtnI16 = HIGH;
int lastBtnI17 = LOW;
unsigned int ultimoTiempoRI16 = 0;
unsigned int ultimoTiempoRI17 = 0;
const unsigned int Delay = 30;  // ms

// -------------------- Variables PWM --------------------
float dutyCyclePercentD0 = 0;
float dutyCyclePercentD1 = 0;

void setup() {
  Serial.begin(115200);     
  Serial2.begin(115200);  

  pinMode(led,  OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(botonI16, INPUT);
  pinMode(botonI17, INPUT);

  HMI_init();
  Stone_HMI_Set_Value("spin_box", "spin_box1", NULL, 0);  // Arranque en 0 
  Stone_HMI_Set_Value("spin_box", "spin_box2", NULL, 0);
}

void loop() {
  // -------- Leer HMI --------
  dutyCyclePercentD0 = HMI_get_value("spin_box", "spin_box1");
  dutyCyclePercentD1 = HMI_get_value("spin_box", "spin_box2");

  // -------- Transiciones de estado por botones --------
  int readingI16 = digitalRead(botonI16); //Leer estado de boton I16
  if (readingI16 != lastBtnI16) { //verificar si es distinto a la ultmia pulsacion
    ultimoTiempoRI16 = millis();
  }
  if ((millis() - ultimoTiempoRI16) > Delay) {
    if (readingI16 == LOW && lastBtnI16 == HIGH) {
      stateD0 = (stateD0 == OFF) ? ON : OFF; //condicion --/v_verdaero/v_falso
    }
  }
  lastBtnI16 = readingI16; //actualiza estado

  int readingI17 = digitalRead(botonI17); //Leer estado de boton I17
  if (readingI17 != lastBtnI17) {//verificar si es distinto a la ultmia pulsacion
    ultimoTiempoRI17 = millis();
  }
  if ((millis() - ultimoTiempoRI17) > Delay) {
    if (readingI17 == LOW && lastBtnI17 == HIGH) {
      stateD1 = (stateD1 == OFF) ? ON : OFF;
    }
  }
  lastBtnI17 = readingI17;//actualiza estado

  // -------- Acciones según el estado --------
  if (stateD0 == ON && dutyCyclePercentD0 >= 0 && dutyCyclePercentD0 <= 100) {
    int pwmValue = map(dutyCyclePercentD0, 0, 100, 0, 255);
    analogWrite(led, pwmValue);
    Serial.print("Duty cycle DE D0(%): ");
    Serial.print(dutyCyclePercentD0);
    Serial.print(" -> PWM value: ");
    Serial.println(pwmValue);
  } else {
    analogWrite(led, 0);  // OFF
  }

  if (stateD1 == ON && dutyCyclePercentD1 >= 0 && dutyCyclePercentD1 <= 100) {
    int pwmValueD1 = map(dutyCyclePercentD1, 0, 100, 0, 255);
    analogWrite(led2, pwmValueD1);
  } else {
    analogWrite(led2, 0);
  }

  Serial.print("D0 "); Serial.print(stateD0 == ON ? "ON " : "OFF "); Serial.print(dutyCyclePercentD0); Serial.print("%  |  ");
  Serial.print("D1 "); Serial.print(stateD1 == ON ? "ON " : "OFF "); Serial.print(dutyCyclePercentD1); Serial.println("%");
}
