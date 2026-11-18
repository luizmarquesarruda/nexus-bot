# Serviço assistant

Pacote Python inicial para a Orange Pi.

## Implementado

- Codificação e validação de quadros JSON do protocolo UART.
- Validação de velocidade, TTL, tamanho de quadro e respostas.
- Transporte serial com limite de leitura.
- CLI de smoke test que envia STOP e aguarda uma resposta; não envia comandos de movimento.

## Instalação e execução

```bash
cd software/assistant
python3 -m venv .venv
. .venv/bin/activate
pip install -e '.[test]'
pytest
nexus-assistant --port /dev/ttyS1
```

O nome do dispositivo serial depende da imagem Linux e da configuração real da Orange Pi.

## Limitações

- Ainda não existe reconhecimento de fala, LLM, interface de usuário ou memória SQLite implementados.
- Não existe controle de movimento na CLI.
- A porta UART, níveis elétricos e ligação física ainda precisam ser validados.
- Testes automatizados, execução na Orange Pi e comunicação com ESP32 permanecem **não validados**.
