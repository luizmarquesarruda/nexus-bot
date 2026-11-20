# Arquitetura de software — NEXUS BOT V1

## 1. Escopo

A V1 possui duas imagens de firmware independentes:

- ESP32 principal: autoridade local de segurança e, após seleção/validação do hardware, controle de movimento e periféricos.
- ESP32-CAM: captura e transmissão de vídeo. Não controla tração e não participa da parada de emergência.

Não existe computador de placa única, serviço Python, LLM, banco de memória ou protocolo com Orange Pi na arquitetura vigente. A interface do operador e o protocolo de comunicação ainda serão definidos.

## 2. Camadas do ESP32 principal

| Camada | Responsabilidade | Estado |
|---|---|---|
| Interface do operador | Receber solicitações autorizadas de operação | Não implementada |
| Aplicação / orquestração | Coordenar modos sem controlar GPIO diretamente | Não implementada |
| Supervisão de segurança | Validar intertravamentos, falhas, armamento e reset | Núcleo lógico inicial implementado |
| Domínio de movimento | Converter solicitações em referências limitadas | Não implementado |
| Domínio de sensores | Normalizar leituras, unidades, validade e timestamp | Não implementado |
| HAL / drivers | GPIO, PWM, encoder, I²C, SPI, UART e ADC conforme placa real | Não implementada |
| Diagnóstico | Estado, falhas, causa da última parada e telemetria | Saída serial mínima |

A camada de segurança pode negar qualquer solicitação. Nenhum módulo de alto nível pode contornar essa decisão.

## 3. Supervisão de segurança

A máquina de estados inicial contém:

- BOOT: inicialização; movimento proibido.
- SAFE: intertravamentos saudáveis; tração ainda não armada.
- ARMED: solicitação explícita de armamento aceita; é apenas uma permissão lógica, não aciona hardware.
- EMERGENCY_STOP_LATCHED: circuito de emergência não confirmado; exige circuito saudável e reset explícito.
- FAULT_LATCHED: driver ou sensor crítico inválido; exige correção da causa e reset explícito.

Flags de saúde começam em false. Entrada ausente, não configurada ou inválida deve falhar de forma segura. Após reset de uma falha, o sistema retorna a SAFE; o armamento exige uma ação separada.

**Limite de segurança:** a máquina de estados não substitui o circuito elétrico de emergência. O circuito físico deve interromper a habilitação/energia da tração sem depender do ESP32, firmware, Wi-Fi ou câmera. A implementação atual não possui leitura GPIO desse circuito e não aciona driver algum.

## 4. Fluxos e isolamento

1. Solicitação do operador (futura) entra pela camada de interface.
2. A aplicação valida formato, origem, prazo e estado operacional.
3. A supervisão de segurança verifica intertravamentos e permissões.
4. O domínio de movimento (futuro) calcula referências limitadas.
5. A HAL (futura) aplica sinais apenas após seleção e validação do driver e da pinagem.
6. Diagnóstico registra estado e motivo de rejeição/parada.

GPS, NFC, câmera e sensores ambientais são fontes de dados; não podem liberar a tração nem sobrepor o intertravamento. O fluxo de vídeo permanece na ESP32-CAM e não pode bloquear o ciclo crítico do controlador principal.

## 5. Regras de execução

- Evitar delay() e operações de rede/armazenamento no caminho crítico.
- Aquisição de sensores deve ter timeout, validade explícita e timestamp monotônico.
- Dados antigos, ausentes, fora de faixa ou incoerentes são inválidos.
- A perda de comunicação com o operador deve produzir estado seguro e nunca manter uma ordem de movimento indefinidamente.
- Reinicialização, reconexão ou limpeza de falha não podem retomar movimento automaticamente.
- O firmware não deve atribuir GPIOs até que modelo/revisão da placa e periféricos sejam confirmados.
- Motores, servos e lógica têm domínios de alimentação dimensionados separadamente.

## 6. Organização do código

    firmware/
      esp32/
        platformio.ini
        include/nexus/safety/SafetyController.hpp
        src/main.cpp
        src/safety/SafetyController.cpp
      esp32-cam/
        README.md

    tests/
      host/
        CMakeLists.txt
        safety_controller_test.cpp

O núcleo de segurança fica independente de Arduino para ser testado em computador antes de ser ligado ao hardware. O arquivo main.cpp só inicializa diagnóstico e chama o núcleo; ainda não configura GPIO nem controla tração.

## 7. Sequência de implementação

1. Compilar o esqueleto e executar os testes unitários da máquina de estados.
2. Selecionar a placa ESP32 e revisar GPIOs de boot, alimentação e periféricos.
3. Definir e testar o circuito físico de emergência e a interface de feedback.
4. Selecionar driver, motores e encoders; implementar HAL e controle com saídas inicialmente inibidas.
5. Implementar aquisição de sensores críticos e validação de dados.
6. Integrar GPS, NFC, sensores ambientais, Pan-Tilt e buzzer individualmente.
7. Definir a interface do operador, autenticação e expiração de comandos.
8. Desenvolver firmware da ESP32-CAM após confirmar variante, sensor, PSRAM e alimentação.
9. Integrar, testar falhas, ruído, consumo, temperatura, estabilidade e autonomia.

## 8. Estado de validação

A máquina de estados e os testes de host constituem o primeiro código de referência. Build PlatformIO, CI, GPIO, circuitos, motores, sensores, comunicação, vídeo e testes físicos permanecem **não validados** até execução e registro de evidências.
