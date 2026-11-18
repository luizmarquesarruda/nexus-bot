# Estratégia de testes

## Testes de software

- Parser e validação do protocolo UART.
- Máquina de estados do assistente.
- Expiração de comandos e tratamento de falhas.
- Persistência e recuperação do SQLite.
- Tratamento de falhas de reconhecimento de fala.

## Testes de firmware em bancada

- Boot com motores desabilitados.
- STOP prioritário.
- Timeout UART durante movimento.
- Bumper e sensor de distância.
- Encoders e controle de velocidade.
- Buzzer e LEDs.
- Reinício do ESP32 e da Orange Pi sem retomada involuntária.

## Testes integrados

- Orange Pi desconectada durante movimento.
- Ruído elétrico de motores na UART e alimentação lógica.
- Reconexão e recuperação de estado.
- Comandos de voz em português em ambiente silencioso e ruidoso.
- Temperatura e consumo sob carga contínua.

## Critério de aceite

Nenhum teste será marcado como aprovado sem execução e registro do resultado. Testes ainda não executados devem constar como **não validados**.
