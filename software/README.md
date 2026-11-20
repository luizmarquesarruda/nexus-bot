# Organização do firmware — NEXUS BOT V1

A V1 é uma arquitetura embarcada composta por um ESP32 convencional principal e uma ESP32-CAM independente. Não utiliza Orange Pi, SBC, LLM ou IA embarcada.

## ESP32 principal

Responsável por:
- Tração, motores e encoders.
- Sensores anticolisão e bumpers.
- Emergência física, watchdog e estado seguro.
- GPS, NFC e sensores ambientais.
- Controle de um único Pan-Tilt da câmera.
- Buzzer piezo passivo.
- Monitoramento de bateria e diagnóstico.
- Interface de comando do operador.

Organizar o firmware em módulos independentes, com tarefas não bloqueantes, máquinas de estado explícitas, timeouts e validação de dados. Funções críticas de movimento não devem aguardar operações demoradas de GPS, NFC, câmera ou rede.

## ESP32-CAM

Firmware separado para captura e transmissão de vídeo. Não controla motores, não implementa segurança de tração e não é requisito para a locomoção básica.

A interface entre a câmera e o controlador principal será escolhida após seleção dos módulos e testes de transmissão. Não assumir UART nem compartilhar controle de movimento com a câmera.

## Segurança

- Emergência física independente do software.
- Sensores anticolisão ligados ao ESP32 principal.
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

## Estado

Esta pasta descreve a organização planejada. Não declarar firmware, pinagem, protocolo, vídeo, autonomia ou testes como concluídos sem evidência reproduzível no hardware real. Itens não ensaiados permanecem **não validados**.
