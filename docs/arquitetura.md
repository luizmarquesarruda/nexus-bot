# Arquitetura do sistema

## Objetivo

Construir um robô assistente móvel, pequeno e multifuncional, dentro de um barril de 5 litros. A V1 deve combinar mobilidade e interação, mantendo o controle físico independente da camada de IA.

## Blocos

### 1. Computação de alto nível — Orange Pi Zero 3 H618, 2 GB (candidata)

Responsabilidades previstas:
- Linux e inicialização dos serviços.
- Orquestração de tarefas e interface local.
- Captura e processamento de comandos de voz em português.
- Memória persistente local (SQLite é suficiente para a V1).
- Inferência local experimental, condicionada a benchmarks de RAM, temperatura e latência.
- Envio de comandos de alto nível ao ESP32.

A placa e a variante de memória ainda precisam ser conferidas no anúncio e testadas fisicamente. Não considerar desempenho de LLM, aceleração de GPU, Bluetooth ou áudio como garantidos.

### 2. Controle embarcado — ESP32 clássico

Responsabilidades:
- Acionamento dos motores por driver apropriado.
- Leitura de sensores, encoders e bumpers.
- Controle de LEDs e buzzer piezo passivo.
- Limites de velocidade, watchdog e parada segura.
- Rejeição de comandos inválidos ou fora dos limites.

O ESP32 continua responsável pela segurança do movimento mesmo se a Orange Pi travar ou reiniciar.

### 3. Comunicação

UART entre Orange Pi e ESP32. O protocolo deverá incluir versão, tipo de mensagem, sequência, payload e verificação de integridade. Comandos de movimento terão validade curta; ausência de atualização dentro do timeout provoca parada.

### 4. Voz e sinais

- Entrada: microfone conectado à Orange Pi, após validação de compatibilidade Linux/ALSA e qualidade acústica dentro do barril.
- Saída: buzzer piezo passivo comandado pelo ESP32, com padrões sonoros originais.
- Não haverá alto-falante nem fala sintetizada na V1.
- Palavra de ativação fica para etapa posterior; começar com botão físico ou interface no celular.

### 5. Mobilidade e percepção

Hipótese inicial: duas rodas motrizes laterais e uma roda auxiliar. A escolha final depende do diâmetro interno, altura útil, centro de gravidade, peso e piso. Sensores frontais de distância, bumpers e detecção de borda serão avaliados. Não dimensionar motores antes de estimar massa total e torque requerido.

## Fluxo operacional

1. Orange Pi inicia os serviços e verifica a conexão UART.
2. ESP32 inicia com motores desabilitados.
3. Sensores e estado de segurança são verificados.
4. Comando de voz ou interface gera uma intenção.
5. A camada de comportamento valida a intenção e envia uma ordem limitada.
6. ESP32 verifica limites, sensores e validade temporal antes de acionar os motores.
7. Qualquer falha crítica resulta em parada local e registro do evento.

## Estados mínimos

- BOOT
- SAFE_IDLE
- READY
- LISTENING
- THINKING
- MOVING
- OBSTACLE_STOP
- COMMUNICATION_LOST
- FAULT
- SHUTDOWN

## Decisões em aberto

- Medidas internas e geometria de fixação do barril.
- Microfone e caminho de áudio comprovadamente compatíveis.
- Sistema de bateria, BMS, fusível e conversores.
- Motores, redução, rodas e driver após cálculo mecânico/elétrico.
- Modelo de STT em português após benchmark.
- Viabilidade e utilidade de LLM local na Orange Pi de 2 GB.
