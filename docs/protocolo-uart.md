# Protocolo UART Orange Pi ↔ ESP32

## Objetivo

Definir uma interface explícita entre o processo de alto nível e o controlador de movimento. A implementação deve ser tolerante a mensagens parciais, ruído, reinicialização e perda de conexão.

## Transporte

- UART full-duplex.
- Baud rate inicial a validar em bancada: 115200 bps.
- 8 data bits, no parity, 1 stop bit (8N1).
- Delimitador de quadro: newline (LF).
- Codificação: UTF-8 restrita a conteúdo ASCII no protocolo inicial.
- Uma mensagem por linha; tamanho máximo inicial sugerido: 256 bytes.

## Envelope lógico

Formato inicial proposto em JSON por linha:

```json
{"v":1,"seq":42,"type":"cmd","name":"drive","left":0.20,"right":0.20,"ttl_ms":250}
```

Campos:
- `v`: versão do protocolo.
- `seq`: contador crescente para detectar repetição/perda.
- `type`: categoria (`cmd`, `telemetry`, `event`, `ack` ou `fault`).
- Campos adicionais dependem do tipo.

A implementação deverá validar rigorosamente campos, tipos, limites e tamanho antes de executar. O envelope ainda precisa ganhar checksum/CRC antes de ser considerado resistente a corrupção; JSON por linha sozinho não fornece integridade criptográfica nem garantia de entrega.

## Comandos iniciais previstos

- `drive`: velocidade normalizada de cada lado entre -1.0 e 1.0, com TTL curto.
- `stop`: parada imediata.
- `set_led`: padrão de LEDs.
- `beep_pattern`: identificador de padrão sonoro permitido (não frequência arbitrária ilimitada).

A Orange Pi não envia PWM bruto nem contorna os limites locais do ESP32.

## Telemetria

O ESP32 poderá informar:
- estado operacional;
- estado de segurança;
- leituras de sensores;
- contagem dos encoders;
- tensão da bateria, se houver divisor/ADC devidamente projetado;
- falhas e motivo da última parada.

## Regras obrigatórias

1. Motores desabilitados durante boot e até entrar em estado seguro.
2. Comando de movimento expira após `ttl_ms`; valor máximo limitado no firmware.
3. Timeout de comunicação provoca parada local.
4. Comando inválido, versão desconhecida ou sequência repetida é rejeitado.
5. `stop` deve ter prioridade sobre qualquer comando de movimento.
6. Após reinício ou reconexão, não retomar o último movimento.
7. O ESP32 mantém limites de velocidade e intertravamentos independentemente da Orange Pi.

## Testes necessários

- Mensagem fragmentada em múltiplas leituras.
- Duas mensagens concatenadas.
- JSON malformado e quadro acima do limite.
- Sequência repetida ou fora de ordem.
- Perda de UART durante movimento.
- Reinício de cada lado da conexão.
- TTL expirado e comando STOP prioritário.

Este é um contrato inicial para implementação; ainda não há firmware validado.
