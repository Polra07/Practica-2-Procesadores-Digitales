 #include <Arduino.h>

// Contadores para las interrupciones
volatile int contadorISR = 0;
int totalISR = 0;

// Configuración del temporizador
hw_timer_t *temporizador = NULL;
portMUX_TYPE muxTemporizador = portMUX_INITIALIZER_UNLOCKED;

// Función de interrupción del temporizador
void IRAM_ATTR manejarTemporizador() {
  portENTER_CRITICAL_ISR(&muxTemporizador);
  contadorISR++;
  portEXIT_CRITICAL_ISR(&muxTemporizador);
}

void setup() {
  Serial.begin(115200);

  // Inicializa el temporizador 0 con prescaler 80 (1 µs por tick), en modo ascendente
  temporizador = timerBegin(0, 80, true);

  // Asocia la interrupción al temporizador
  timerAttachInterrupt(temporizador, &manejarTemporizador, true);

  // Establece la alarma para que se dispare cada 1 segundo (1 000 000 µs)
  timerAlarmWrite(temporizador, 1000000, true);

  // Habilita la alarma del temporizador
  timerAlarmEnable(temporizador);
}

void loop() {
  if (contadorISR > 0) {
    portENTER_CRITICAL(&muxTemporizador);
    contadorISR--;
    portEXIT_CRITICAL(&muxTemporizador);

    totalISR++;
    Serial.print("Se ha producido una interrupción. Total: ");
    Serial.println(totalISR);
  }
}
