# Protocolo UART Orange Pi ↔ ESP32

## Objetivo

Definir uma interface explícita entre o processo de alto nível e o controlador de movimento. A implementação inicial usa quadros JSON delimitados por LF e validação local no ESP32.

## Transporte

- UART full-duplex.
- Baud rate inicial: 115200 bps (a validar na bancada).
- 8 data bits, no parity, 1 stop bit (8N1).
- Delimitador: LF real (byte 0x0A); CR é ignorado pelo firmware.
- Codificação: ASCII no conteúdo JSON inicial.
- Tamanho máximo do quadro: 256 bytes.

## Envelope

```json
{"v":1,"seq":42,"type":"cmd","name":"drive","left":0.20,"right":0.20,"ttl_ms":200}
```

Campos:
- `v`: versão do protocolo.
- `seq`: contador para correlacionar comando e ACK.
- `type`: categoria (`cmd`, `ack` ou `fault` na primeira implementação).
- `name`: comando.
- Campos restantes variam conforme o comando.

**Limitação conhecida da primeira versão:** `seq` ainda não implementa rejeição de mensagens repetidas ou fora de ordem; não deve ser tratado como proteção anti-replay. CRC/checksum também ainda não está implementado. A comunicação é um enlace local de protótipo, não um protocolo seguro contra um dispositivo malicioso conectado à UART.

## Comandos implementados no esqueleto

- `drive`: velocidades normalizadas de cada lado entre -1.0 e 1.0 e `ttl_ms` entre 1 e 350.
- `stop`: cancela a solicitação de movimento.
- `beep_pattern`: padrões permitidos (`ack`, `thinking`, `error`, `obstacle`, `shutdown`).
- `set_led`: liga/desliga o LED configurado.

O ESP32 rejeita velocidades fora do intervalo e TTL inválido. O TTL é aplicado localmente, sem depender da Orange Pi. O driver dos motores permanece desabilitado no código (`NEXUS_MOTOR_OUTPUT_ENABLED=0)) até que hardware e pinagem sejam definidos.

## Regras obrigatórias

1. Motores desabilitados durante boot.
2. STOP cancela a solicitação de movimento.
3. Comando de movimento expira localmente após seu TTL.
4. Comando inválido ou fora dos limites não deve acionar os motores.
5. Perda de comunicação não pode manter movimento indefinidamente; TTL curto é a proteção inicial.
6. Após reinício, não retomar movimento automaticamente.
7. O ESP32 mantém limites e intertravamentos independentemente da Orange Pi.

## Testes necessários

- Mensagem fragmentada em múltiplas leituras.
- Duas mensagens concatenadas.
- JSON malformado e quadro acima do limite.
- TTL expirado durante movimento.
- Velocidade e TTL fora dos limites.
- Perda de UART durante movimento.
- Reinício de cada lado da conexão.
- STOP prioritário.
- Futuramente: sequência repetida, checksum/CRC e negociação de sessão.

Estado: contrato e código iniciais; testes físicos e firmware compilado **não validados**.
