# Arquitetura elétrica preliminar

## Separação de cargas

A alimentação dos motores deve ser separada da alimentação lógica. A Orange Pi e o ESP32 não fornecem corrente aos motores. O driver recebe energia do barramento de potência e sinais de controle do ESP32.

A topologia exata, tensões e bitolas dependem dos motores, bateria e conversores selecionados. O terra de sinal e potência terá referência comum quando exigido pelo driver, com retorno de corrente dos motores roteado para evitar ruído na lógica.

## Proteções previstas

- Bateria com BMS adequado à química, quantidade de células e corrente.
- Fusível próximo à fonte/bateria, dimensionado após medir correntes.
- Chave geral acessível.
- Corte físico independente para alimentação dos motores.
- Proteção contra polaridade invertida e transientes conforme os componentes.
- Conversores DC-DC com margem térmica e de corrente.
- Desacoplamento próximo aos drivers e módulos.
- Cabos e conectores dimensionados para corrente e vibração.

## Estados seguros

- Ao ligar: driver desabilitado até autoteste e comando explícito.
- Ao perder UART: ESP32 desabilita motores após timeout.
- Ao detectar falha de sensor crítico: parar e sinalizar falha.
- Ao reiniciar Orange Pi: ESP32 não retoma movimento automaticamente.
- Ao acionar emergência: cortar potência dos motores sem depender do software.

## Buzzer

Preferir piezo passivo compatível com 3,3 V. Confirmar corrente do componente. Se a corrente exceder a capacidade segura do GPIO, usar transistor/MOSFET e resistor de gate/base apropriado. Não ligar buzzer desconhecido diretamente ao GPIO sem conferir datasheet.

## Checklist antes de energizar

- [ ] Polaridade e tensão verificadas com multímetro.
- [ ] Continuidade e ausência de curto entre alimentação e GND.
- [ ] Fusível instalado.
- [ ] Driver com motores desconectados no primeiro teste.
- [ ] Reguladores testados com carga fictícia.
- [ ] Temperatura monitorada.
- [ ] Emergência e timeout testados.
- [ ] Motores suspensos do chão no primeiro acionamento.

Este documento é uma arquitetura de segurança preliminar, não um esquema elétrico pronto para montagem.
