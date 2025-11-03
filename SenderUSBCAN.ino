#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);  // CS en el pin 10

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Inicializar MCP2515
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  // Inicializar generador aleatorio
  randomSeed(analogRead(0));

  Serial.println("CAN bus inicializado, enviando valores aleatorios...");
}

void loop() {
  // Generar un valor aleatorio entre 0 y 1023
  int valorRandom = random(0, 1024);

  // Empaquetar la lectura en dos bytes (alta y baja)
  canMsg.can_id  = 0x200;     // ID arbitrario
  canMsg.can_dlc = 2;         // Solo enviamos 2 bytes
  canMsg.data[0] = highByte(valorRandom);
  canMsg.data[1] = lowByte(valorRandom);

  // Enviar el mensaje
  if (mcp2515.sendMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print("Valor aleatorio enviado: ");
    Serial.println(valorRandom);
  } else {
    Serial.println("Error enviando mensaje CAN");
  }

  delay(1000);
}
