# Firmware ESP32

Projeto inicial PlatformIO para ESP32 DevKit clássico, usando framework Arduino e ArduinoJson.

## O que o esqueleto faz

- Recebe quadros JSON por UART2 a 115200 bps.
- Valida versão, tipo e campos básicos.
- Responde com ACK ou evento de falha.
- Aplica limites de velocidade e TTL aos comandos drive.
- Expira localmente a solicitação de movimento.
- Controla padrões sonoros no buzzer e um LED configurável.

## Segurança e limitações

- `NEXUS_MOTOR_OUTPUT_ENABLED=0` por padrão: nenhuma saída física de motor está implementada.
- Driver, motores e pinagem ainda não foram escolhidos.
- UART RX16/TX17, buzzer GPIO25 e LED GPIO2 são referências provisórias; verificar a placa real antes de ligar qualquer circuito.
- O TTL é limitado a 350 ms no firmware.
- O campo de sequência ainda não bloqueia replay ou mensagens fora de ordem.
- Não há checksum/CRC, sensores, encoders, watchdog externo ou circuito de emergência implementados.

## Compilar

Com PlatformIO instalado:

```bash
cd firmware/esp32
pio run
```

A compilação e o teste em placa física ainda estão **não validados**.
