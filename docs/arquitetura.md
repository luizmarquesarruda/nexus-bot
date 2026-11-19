# Arquitetura do sistema — NEXUS BOT

## 1. Objetivo

Construir um robô móvel compacto, de corpo cilíndrico, com locomoção por rodas na parte inferior. A V1 será um sistema embarcado convencional, sem computador de placa única e sem IA embarcada.

A estrutura física ainda precisa ser medida. Não dimensionar motores, rodas, bateria ou suportes definitivos antes de conhecer as dimensões internas, massa e centro de gravidade.

## 2. Arquitetura eletrônica aprovada

### ESP32 convencional — controlador principal

Responsável por:
- Controle dos motores e leitura dos encoders.
- Controle do driver de tração e velocidade.
- Leitura dos sensores anticolisão e bumpers.
- Gestão do botão/circuito de emergência e estado seguro.
- Leitura do GPS por UART.
- Comunicação com leitor NFC, preferencialmente PN532 por SPI ou I²C conforme pinagem final.
- Leitura de sensores ambientais.
- Controle de um único Pan-Tilt dedicado à câmera.
- Monitoramento da bateria, dentro dos limites do circuito de medição escolhido.
- Coordenação dos periféricos e comunicação com o operador.

O ESP32 principal é a autoridade de controle de movimento. Dados de GPS, NFC e câmera não podem sobrepor intertravamentos físicos ou a lógica de segurança.

### ESP32-CAM — módulo independente de vídeo

Responsável por captura e transmissão de vídeo. Não controla motores, não executa parada de emergência e não é requisito para o funcionamento básico da locomoção.

O modelo exato, sensor de imagem, antena, alimentação, firmware e protocolo de vídeo ainda serão selecionados e testados. A integração com o principal deve ser leve e não bloquear o controle de movimento.

## 3. Periféricos previstos

| Periférico | Interface candidata | Função |
|---|---|---|
| GPS com antena | UART | Coordenadas e velocidade em ambiente externo |
| PN532/NFC | SPI ou I²C | Identificação de etiquetas e pontos |
| Sensores ambientais | I²C ou GPIO | Temperatura, umidade e futuras medições |
| Encoders | GPIO/interrupts | Odometria e controle de velocidade |
| Sensores anticolisão | GPIO/I²C | Detecção de obstáculos |
| Servos Pan-Tilt | PWM/driver adequado | Orientação da câmera |
| Monitoramento de bateria | ADC/medidor dedicado | Tensão e estado de energia |

As interfaces são candidatas, não pinagem aprovada. Conferir endereços I²C, níveis lógicos, capacidade de corrente, GPIOs de boot e conflitos de periféricos antes de fechar o esquema.

## 4. Expansão e limite de controladores

A V1 terá somente um ESP32 convencional principal e uma ESP32-CAM independente. Três ESP32 Mini não fazem parte da arquitetura.

Um segundo ESP32 convencional só poderá ser considerado se medições e testes evidenciarem uma limitação concreta, como falta de GPIOs, saturação de processamento, conflito de barramentos, interferência ou necessidade real de distribuir sensores. A expansão deverá ter uma interface documentada e falha isolada.

## 5. Segurança e estados de falha

- A parada de emergência deve interromper fisicamente a habilitação/energia da tração, sem depender do firmware.
- Sensores anticolisão ficam conectados ao ESP32 principal.
- Watchdog e timeouts devem levar o sistema a um estado seguro.
- Dados antigos ou inválidos de sensores devem ser descartados.
- Falha de GPS, NFC ou ESP32-CAM não pode impedir a parada dos motores.
- O controle de motores deve ter limites de velocidade, aceleração e tempo de comando.
- A alimentação de motores, servos e lógica deve ser dimensionada separadamente, com referência de terra planejada e proteção contra transientes.

## 6. Comunicação e software embarcado

O firmware do ESP32 principal deve ser organizado em módulos não bloqueantes: tração, encoders, sensores, navegação, periféricos, energia, segurança e diagnóstico. Usar máquinas de estado explícitas e tempos limite.

A comunicação com a ESP32-CAM será definida conforme o módulo e o método de transmissão de vídeo escolhidos. O fluxo de vídeo não deve ocupar tarefas críticas nem bloquear o controle de movimento.

## 7. Navegação

- NFC identifica checkpoints ou locais específicos; não fornece posição contínua.
- GPS é destinado a referência externa e normalmente não funciona com precisão útil dentro de casa.
- Encoders estimam deslocamento, mas acumulam erro.
- Sensores de distância e bumpers detectam obstáculos.
- A combinação e os limites de cada fonte devem ser testados no ambiente real.

## 8. Mecânica e interação

- Locomoção exclusivamente por rodas na parte inferior; sem pernas laterais.
- Um único Pan-Tilt dedicado à câmera.
- Corpo modular, com impressão 3D prevista para peças externas e suportes após medições.
- Buzzer piezo passivo para padrões sonoros próprios; sem alto-falante e sem voz falada.
- A antiga proposta de cúpula giratória e segundo Pan-Tilt não faz parte do projeto atual.

## 9. Sequência de implementação

1. Medir o corpo e registrar espaço útil, fixações e centro de gravidade.
2. Selecionar ESP32 principal e validar alimentação, GPIOs e interfaces.
3. Implementar tração, encoders, anticolisão e parada física em bancada.
4. Integrar GPS, NFC e sensores ambientais individualmente.
5. Integrar Pan-Tilt e buzzer.
6. Validar ESP32-CAM e transmissão de vídeo separadamente.
7. Integrar vídeo e controle sem bloquear tarefas críticas.
8. Testar consumo, ruído elétrico, interferência, falhas, autonomia e segurança no robô montado.

## 10. Estado de validação

Esta documentação registra a arquitetura aprovada, não uma implementação concluída. Placas exatas, pinagem, firmware, circuito de potência, bateria, alcance NFC/GPS, vídeo, autonomia e comportamento integrado permanecem **não validados** até testes físicos documentados.
