# NEXUS BOT

Robô assistente móvel de pequeno porte, com arquitetura híbrida: Orange Pi como unidade de processamento de alto nível e ESP32 como controlador de tempo real para movimento, sensores, LEDs e sinais sonoros.

O protótipo V1 será instalado em um barril Heineken de 5 litros. As dimensões internas ainda precisam ser medidas; portanto, este repositório registra decisões e interfaces sem congelar o desenho mecânico nem dimensionar motores ou bateria antes das medições.

## Estado do projeto

- [x] Repositório público criado e nomeado `nexus-bot`.
- [x] Arquitetura inicial documentada.
- [ ] Medir altura útil, diâmetro interno e obstáculos internos do barril.
- [ ] Validar Orange Pi Zero 3 H618 (2 GB) com Linux, microfone e carga real.
- [ ] Fechar orçamento completo da V1 antes de comprar componentes.
- [ ] Montar e testar a base móvel em bancada.
- [ ] Integrar percepção, comandos em português e memória local.

## Arquitetura resumida

- **Orange Pi Zero 3:** cérebro Linux; interface, memória local, reconhecimento de fala e coordenação de tarefas.
- **ESP32 clássico:** controle local de motores, sensores, encoders, LEDs, buzzer passivo e intertravamentos.
- **Ligação:** UART entre Orange Pi e ESP32, com protocolo versionado, timeout e parada segura em perda de comunicação.
- **Áudio:** microfone para receber comandos. O robô não terá alto-falante nem síntese de voz; responderá por padrões sonoros próprios em buzzer piezo passivo.
- **IA:** inferência local é uma meta, não uma capacidade presumida. Modelos e latências serão escolhidos após benchmark no hardware real.

## Estrutura

- `docs/`: arquitetura, hardware, elétrica, protocolo e diário de bordo.
- `firmware/esp32/`: firmware do controlador embarcado.
- `software/assistant/`: serviços de alto nível na Orange Pi.
- `software/ai/`: reconhecimento de fala e experimentos de IA local.
- `software/memory/`: persistência e memória local.
- `tests/`: testes automatizados e de bancada.

## Princípios de engenharia

1. Segurança de movimento não depende da IA nem da conexão com a Orange Pi.
2. Perda de comunicação, watchdog ou falha crítica deve levar os motores a estado parado.
3. Motores não são alimentados pelos pinos da Orange Pi ou do ESP32.
4. Não assumir dimensões, autonomia, corrente ou desempenho sem medição.
5. Não comprar componentes até fechar custo total, compatibilidade e margens elétricas.
6. Recursos não validados em hardware real devem ser identificados como **não validados**.

Consulte [o diário de bordo](docs/diario-de-bordo.md) para decisões e próximos passos.
