# Firmware e software — NEXUS BOT

A V1 é um sistema embarcado convencional, sem Orange Pi, sem LLM e sem IA embarcada. O controle será feito pelo ESP32 principal; a ESP32-CAM será independente para vídeo.

## Firmware do ESP32 principal

Responsabilidades:
- Controle de motores e encoders.
- Sensores anticolisão e bumpers.
- Parada de emergência e estado seguro.
- GPS, NFC e sensores ambientais.
- Controle do Pan-Tilt da câmera.
- Buzzer piezo passivo.
- Monitoramento de bateria e diagnóstico.
- Comunicação e comandos do operador.

Organizar o firmware em módulos independentes, com tarefas não bloqueantes, máquinas de estado, timeouts, watchdog e validação dos dados. Funções críticas de movimento não devem aguardar operações demoradas de GPS, NFC ou rede.

## ESP32-CAM

Firmware separado para captura e transmissão de vídeo. Não controla motores, não implementa segurança de tração e não deve ser necessário para a locomoção básica. O protocolo de integração com o ESP32 principal será definido após selecionar a placa e testar a transmissão.

## Interfaces candidatas

- GPS: UART.
- PN532: SPI ou I²C.
- Sensores ambientais: I²C ou GPIO.
- Encoders: GPIO com interrupções/contagem apropriada.
- Servos Pan-Tilt: PWM ou controlador dedicado, conforme corrente e temporização.
- Comunicação com operador: Wi-Fi ou BLE, a definir conforme alcance e requisitos.

A pinagem e os barramentos não estão fechados. Verificar conflitos de GPIO, endereços, níveis lógicos, memória e capacidade de processamento antes da implementação.

## Segurança

- Emergência física independente do software.
- Sensores anticolisão conectados ao ESP32 principal.
- Watchdog e timeout para comandos.
- Dados inválidos ou antigos não podem autorizar movimento.
- Falha de GPS, NFC ou câmera não pode impedir a parada segura.
- Limites de velocidade e estados de falha devem ser testados em bancada antes da montagem.

## Plano de validação

1. Compilar firmware mínimo e validar placa/alimentação.
2. Testar driver, motores e encoders com rodas suspensas.
3. Testar emergência física e anticolisão antes de permitir deslocamento.
4. Integrar GPS, NFC e sensores ambientais um por vez.
5. Integrar Pan-Tilt e buzzer.
6. Validar ESP32-CAM e vídeo separadamente.
7. Testar todos os módulos juntos, inclusive falhas de comunicação, ruído, consumo e temperatura.

## Status

Esta pasta descreve o plano de firmware. Não declarar firmware, pinagem, protocolo, vídeo, autonomia ou testes como concluídos sem evidência reproduzível no hardware real. Os itens ainda não ensaiados permanecem **não validados**.
