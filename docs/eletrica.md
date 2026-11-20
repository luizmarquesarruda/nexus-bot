# Arquitetura elétrica preliminar — NEXUS BOT V1

## Arquitetura de controle

A V1 utiliza:
- Um ESP32 convencional como controlador principal de movimento, sensores e segurança.
- Uma ESP32-CAM independente para captura e transmissão de vídeo.

Não há Orange Pi, SBC, computador Linux embarcado ou enlace UART com computador de alto nível na arquitetura vigente. A comunicação entre ESP32 principal e ESP32-CAM será definida somente após a seleção dos módulos e testes de vídeo. A câmera não participa da segurança da tração.

## Separação de cargas

A alimentação dos motores deve ser separada da alimentação lógica. O ESP32 não fornece corrente aos motores. O driver recebe energia do barramento de potência e sinais de controle do ESP32 principal.

Servos Pan-Tilt também devem ter alimentação dimensionada separadamente da lógica, conforme corrente de partida e travamento. A referência de GND deve seguir a topologia exigida pelos drivers, evitando que correntes dos motores percorram retornos sensíveis.

A topologia, tensões, bitolas e conversores dependem dos motores, bateria e módulos selecionados. Não fechar o esquema antes dessas definições.

## Proteções previstas

- Bateria com BMS adequado à química, quantidade de células e corrente.
- Fusível próximo à fonte/bateria, dimensionado após medir correntes.
- Chave geral acessível.
- Corte físico independente para alimentação/habilitação dos motores.
- Proteção contra polaridade invertida e transientes conforme os componentes.
- Conversores DC-DC com margem térmica e de corrente.
- Desacoplamento próximo aos drivers e módulos.
- Cabos e conectores dimensionados para corrente e vibração.
- Proteção e alimentação própria para a ESP32-CAM, conforme pico de transmissão.

## Estados seguros

- Ao ligar: driver desabilitado até autoteste e comando explícito.
- Ao perder o comando do operador ou ocorrer timeout interno: ESP32 principal desabilita a tração.
- Ao detectar falha de sensor crítico: parar e sinalizar falha.
- Ao reiniciar qualquer módulo: não retomar movimento automaticamente.
- Ao acionar emergência: cortar/desabilitar fisicamente a tração sem depender do firmware, Wi-Fi ou ESP32-CAM.
- Falha da câmera, GPS ou NFC não pode impedir a parada segura.

## Buzzer

Preferir piezo passivo compatível com 3,3 V. Confirmar corrente do componente. Se exceder a capacidade segura do GPIO, usar transistor/MOSFET e resistor apropriado. Não ligar buzzer desconhecido diretamente ao GPIO sem conferir datasheet.

## Checklist antes de energizar

- [ ] Polaridade e tensão verificadas com multímetro.
- [ ] Continuidade e ausência de curto entre alimentação e GND.
- [ ] Fusível instalado.
- [ ] Driver com motores desconectados no primeiro teste.
- [ ] Reguladores testados com carga fictícia.
- [ ] Temperatura monitorada.
- [ ] Emergência física testada independentemente do software.
- [ ] Sensores críticos e timeouts testados.
- [ ] Motores suspensos do chão no primeiro acionamento.
- [ ] ESP32-CAM testada separadamente antes da integração.

Este documento é uma arquitetura de segurança preliminar, não um esquema elétrico pronto para montagem. Hardware, pinagem, dimensionamento e validação física permanecem **não validados**.
