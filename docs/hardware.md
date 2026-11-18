# Hardware — requisitos e seleção preliminar

Este documento separa decisões de arquitetura, candidatos e itens ainda não validados. A presença de um componente nesta lista não autoriza compra automática.

## Computação e controle

| Bloco | Definição/candidato | Situação |
|---|---|---|
| SBC principal | Orange Pi 4 Pro, Allwinner A733, 4 GB RAM | Plataforma definida; variante, revisão, Linux, drivers, consumo e temperatura ainda precisam ser verificados na placa física |
| Runtime de IA | `llama.cpp` | Escolhido; ainda não validado na placa |
| Modelo inicial | Qwen2.5-0.5B-Instruct GGUF Q8_0 | Candidato de benchmark, não decisão final de desempenho |
| MCU principal | ESP32 convencional/DevKit | Quantidade e papel definidos; modelo exato pendente |
| MCUs auxiliares | Dois ESP32 Mini | Previstos; variantes e responsabilidades finais pendentes |
| Armazenamento | microSD de boa procedência | Estratégia inicial; capacidade/classe a selecionar conforme disponibilidade e testes |
| Saída sonora | Buzzer piezo passivo, preferencialmente 3,3 V | Decisão funcional; corrente e circuito de acionamento precisam ser conferidos |
| Câmera | Módulo compatível com Linux/CSI ou USB | Não selecionada |
| Microfone/voz | Não é requisito aprovado na arquitetura atual | Não comprar sem reabrir a decisão |

## Distribuição proposta dos microcontroladores

- ESP32 convencional: motores, sensores críticos, watchdog, validação de comandos e parada segura.
- ESP32 Mini 1: sensores auxiliares.
- ESP32 Mini 2: monitoramento de energia, buzzer e periféricos.

A distribuição dos auxiliares é uma proposta de organização, não um pinout aprovado. Confirmar variantes, GPIO disponíveis, níveis lógicos, alimentação e comunicação antes de desenvolver a placa/cabeamento.

## Armazenamento e software

Começar com microSD. SSD M.2/NVMe não é requisito da primeira versão. Reduzir gravações desnecessárias, usar logs rotativos e agrupar gravações persistentes. SQLite/FTS5 é a memória inicial; embeddings só serão avaliados depois de medir recursos reais.

## Mobilidade

Hipótese em avaliação: duas rodas motrizes laterais e uma roda auxiliar. Não escolher motor, driver, roda ou bateria até:
1. medir altura útil, diâmetro interno, abertura, pontos de fixação e obstáculos internos do barril;
2. estimar massa total e distribuição de peso;
3. definir velocidade, inclinação e piso desejados;
4. levantar corrente nominal e corrente de bloqueio dos motores;
5. calcular torque, autonomia e margem térmica.

Driver de motor deve ser dimensionado pela corrente de bloqueio e dissipação, não apenas pela corrente nominal.

## Sensores a avaliar

- ToF frontal para distância/obstáculos.
- Bumpers físicos como camada redundante.
- Sensores inferiores para bordas/queda, caso o ambiente inclua risco de escadas.
- Encoders para odometria básica.
- IMU somente se trouxer benefício demonstrável na V1.

## Energia e segurança

Separar alimentação dos motores e alimentação lógica. A Orange Pi e os ESP32 não fornecem corrente aos motores. Prever bateria com BMS adequado à química e número de células, fusível próximo à fonte, chave geral, proteção contra polaridade/transientes, conversores DC-DC com margem e corte físico independente da alimentação dos motores.

Não definir tensão, capacidade de bateria, bitola ou driver sem fechar motores e orçamento de potência. Confirmar se o buzzer pode ser acionado pelo GPIO; usar transistor/MOSFET se a corrente exigir.

## Limite financeiro

Orçamento-alvo global da V1: até R$ 800, aceitando usados quando houver procedência e teste. O custo só será considerado fechado quando incluir SBC, placas ESP32, microSD, câmera/sensores, buzzer, motores, driver, rodas, bateria, BMS, conversores, proteção, cabos, conectores, estrutura, frete e tributos.

## Critérios de compra

- Compatibilidade documentada com Orange Pi 4 Pro/A733 e Linux escolhido.
- Corrente contínua e de pico com margem.
- Dimensões e fixação compatíveis com as medidas reais do barril.
- Custo total, incluindo frete e tributos.
- Documentação e possibilidade de reposição.
- Não comprar com base apenas em descrição genérica de marketplace.
