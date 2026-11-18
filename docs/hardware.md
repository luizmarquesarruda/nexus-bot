# Hardware — requisitos e seleção preliminar

Este documento separa candidatos de componentes aprovados. Nenhum item está liberado para compra apenas por constar aqui.

## Computação

| Bloco | Candidato | Situação |
|---|---|---|
| SBC | Orange Pi Zero 3 H618, 2 GB | Candidata; validar variante, Linux, RAM, temperatura, áudio e desempenho |
| MCU | ESP32 DevKit com ESP-WROOM-32 | Candidato preferido para controle |
| Armazenamento | microSD 128 GB de marca confiável | Capacidade desejada; verificar compatibilidade e autenticidade |
| Microfone | USB compatível com Linux/ALSA | Em aberto; testar antes da compra definitiva |
| Saída sonora | Buzzer piezo passivo, preferencialmente 3,3 V | Decisão funcional; validar corrente e circuito de acionamento |

## Mobilidade

Hipótese de estudo: dois motores DC com redução e encoder, duas rodas motrizes e uma roda auxiliar. Driver de motor dimensionado pela corrente de bloqueio dos motores, não apenas pela corrente nominal.

Não escolher motor, driver, roda ou bateria até:
1. medir o espaço interno;
2. estimar massa total e distribuição de peso;
3. definir diâmetro das rodas e velocidade desejada;
4. levantar corrente nominal e de bloqueio dos motores;
5. calcular autonomia pretendida e margem térmica.

## Sensores a avaliar

- ToF frontal para distância/obstáculos.
- Bumpers físicos como camada redundante.
- Sensores inferiores para bordas/queda, se houver risco de escadas.
- Encoders para odometria básica.
- IMU apenas se trouxer benefício demonstrável na V1.

## Energia

Arquitetura prevista com alimentação de motores separada da alimentação lógica, reguladores adequados e referência de terra comum conforme o projeto elétrico. Prever fusível, proteção contra inversão, BMS compatível com a química e número de células, chave geral e corte físico da alimentação dos motores.

Não alimentar motores diretamente pela Orange Pi ou pelos GPIOs do ESP32. Não definir tensão/capacidade de bateria sem fechar os motores e o orçamento de energia.

## Critérios de compra

- Compatibilidade documentada com a placa e o sistema operacional escolhidos.
- Corrente contínua e de pico com margem.
- Dimensões e fixação compatíveis com o barril medido.
- Custo total com frete, tributos, conectores, cabos e proteção.
- Disponibilidade de documentação e possibilidade de reposição.

## Limite financeiro

Orçamento-alvo da V1: até R$ 800, aceitando componentes usados quando houver teste e procedência. O orçamento só será considerado fechado quando incluir computação, controle, microSD, microfone, buzzer, motores, driver, rodas, sensores, bateria, BMS, conversores, proteção, cabos e estrutura.
