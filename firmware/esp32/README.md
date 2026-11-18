# Firmware ESP32

Firmware do controlador embarcado do NEXUS BOT.

Responsabilidades:
- Controle dos motores via driver externo.
- Leitura de sensores e encoders.
- Watchdog e parada segura por timeout UART.
- Controle de LEDs e buzzer piezo passivo.
- Publicação de telemetria e eventos.

Regras:
- Motores iniciam desabilitados.
- Nunca confiar na Orange Pi para limites críticos.
- Rejeitar comandos inválidos ou expirados.
- Perda de comunicação deve parar o robô.
- Não retomar movimento automaticamente após reconexão.

A implementação ainda não foi iniciada. Plataforma, framework (Arduino-ESP32 ou ESP-IDF) e pinagem serão escolhidos após definição da placa e periféricos.
