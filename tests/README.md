# Estratégia de testes — NEXUS BOT V1

A estratégia de testes acompanha a arquitetura embarcada: ESP32 convencional principal e ESP32-CAM independente. Não há testes de Orange Pi, SBC, LLM, SQLite, reconhecimento de fala ou enlace UART com computador de alto nível na V1.

## Testes de firmware do ESP32 principal

- Boot com driver de tração desabilitado.
- Estado seguro após reset, watchdog e brownout.
- Parada de emergência física independente de Wi-Fi e câmera.
- Timeout de comando e parada após perda de comunicação com o operador.
- Rejeição de comandos inválidos, fora de faixa ou expirados.
- Sensores anticolisão: leituras válidas, inválidas, antigas e desconectadas.
- Bumpers e prioridade de parada.
- Encoders e controle de velocidade.
- GPS, NFC e sensores ambientais individualmente.
- Pan-Tilt e buzzer.
- Monitoramento de bateria e comportamento em baixa tensão.

## Testes da ESP32-CAM

- Inicialização e estabilidade de captura.
- Transmissão de vídeo e reconexão.
- Consumo e temperatura.
- Coexistência com o ESP32 principal sem bloquear tarefas críticas.
- Falha, reinício ou desconexão da câmera sem afetar a parada segura.

## Testes elétricos e mecânicos

- Polaridade, tensão, continuidade e ausência de curto.
- Corrente de partida e bloqueio dos motores.
- Capacidade térmica do driver e conversores.
- Ruído dos motores/servos sobre sensores, GPS, NFC e rádio.
- Fixação, centro de gravidade, estabilidade e tração.
- Autonomia medida em condições registradas.

## Critério de aceite

Nenhum teste será marcado como aprovado sem execução e registro do resultado. Testes ainda não executados devem constar como **não validados**. Testes de emergência devem ser realizados primeiro em bancada, com tração mecanicamente segura e procedimento de corte acessível.


## Testes automatizados iniciais

O núcleo de segurança C++ pode ser testado sem ESP32, driver ou motores:

```sh
cmake -S tests/host -B build/host
cmake --build build/host --parallel
ctest --test-dir build/host --output-on-failure
```

Os testes cobrem inicialização fail-closed, armamento explícito, prioridade e retenção de emergência, falha crítica e reset sem retomada automática. Eles validam a lógica de software, não o circuito elétrico nem o comportamento físico.

O workflow `.github/workflows/firmware.yml` executa os testes de host e compila o alvo genérico PlatformIO. Resultado de CI e compilação só devem ser declarados aprovados após execução no GitHub. Testes elétricos, de sensores, tração, câmera, consumo e autonomia permanecem **não validados**.
