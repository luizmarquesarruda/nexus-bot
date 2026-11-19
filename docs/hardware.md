# Hardware — requisitos e seleção preliminar

Este documento separa decisões aprovadas, candidatos e itens pendentes. A inclusão de um componente não autoriza compra automática.

## Controladores

| Bloco | Definição | Situação |
|---|---|---|
| MCU principal | Um ESP32 convencional | Arquitetura aprovada; modelo/placa exatos pendentes |
| Câmera | Uma ESP32-CAM independente | Arquitetura aprovada; variante e antena pendentes |
| ESP32 Mini auxiliares | Não usar na V1 | Descartados para a primeira versão |
| Segundo ESP32 convencional | Somente se testes justificarem | Não previsto inicialmente |

## Periféricos previstos no ESP32 principal

| Periférico | Candidato/interface | Observações |
|---|---|---|
| GPS | Módulo GNSS com antena, UART | Uso externo; validar antena, fixação e recepção |
| NFC | PN532, SPI ou I²C | Etiquetas passivas; alcance depende da antena, montagem e superfície |
| Sensores ambientais | I²C/GPIO | Selecionar somente os sensores úteis à V1 |
| Encoders | GPIO/interrupts | Necessários para estimativa de deslocamento e velocidade |
| Anticolisão | ToF/ultrassônico e bumpers | Sensores críticos ligados diretamente ao principal |
| Pan-Tilt | Um conjunto com dois eixos | Exclusivo para orientar a câmera; servos ainda não selecionados |
| Buzzer | Piezo passivo | Sem alto-falante ou voz falada |
| Bateria | Química/tensão/capacidade em aberto | Dimensionar após motores e consumo medidos |

## Câmera

ESP32-CAM independente para captura e transmissão de vídeo. Confirmar variante, sensor, qualidade necessária, disponibilidade de PSRAM, antena/conector, método de transmissão e consumo. A câmera não participa do circuito de emergência nem controla os motores.

## Mobilidade

A locomoção será exclusivamente por rodas instaladas na parte inferior do corpo cilíndrico. Não haverá pernas laterais.

Não fechar motores, rodas, driver ou bateria até:
1. medir altura útil, diâmetro interno, abertura e pontos de fixação;
2. estimar massa total e centro de gravidade;
3. definir piso, velocidade e inclinação esperados;
4. obter corrente nominal e corrente de bloqueio dos motores;
5. calcular torque, autonomia e margem térmica.

O driver deve ser dimensionado pela corrente de bloqueio e dissipação, não apenas pela corrente nominal.

## Energia e segurança

- Separar alimentação de motores, servos e lógica.
- Não alimentar motores pelos pinos do ESP32.
- Prever fusível próximo à fonte, chave geral, proteção de polaridade e conversores DC-DC dimensionados com margem.
- O circuito de emergência deve desabilitar fisicamente a tração.
- Verificar ruído dos motores/servos e seus efeitos sobre GPS, NFC, câmera e leituras analógicas.
- Não definir bateria, bitola ou reguladores antes de fechar os componentes e medir consumo.

## Critérios de seleção e compra

- Conferir modelo e revisão exatos, documentação e pinagem.
- Confirmar tensão lógica, corrente de pico e compatibilidade de interfaces.
- Verificar dimensões e conectores reais do anúncio.
- Comparar custo total com frete e tributos.
- Preferir peças com documentação e reposição.
- Não comprar com base apenas em descrições genéricas de marketplace.

## Estado

A arquitetura de controladores foi aprovada, mas placas, sensores, motores, driver, bateria, circuito de emergência e estrutura ainda precisam ser selecionados e testados. Não há validação física declarada nesta atualização.
