# NEXUS BOT

Robô assistente móvel de pequeno porte, com arquitetura híbrida: Orange Pi como unidade de processamento de alto nível e ESP32 como controlador de tempo real para movimento, sensores e sinais sonoros.

O protótipo V1 será instalado em um barril Heineken de 5 litros. As dimensões internas ainda precisam ser medidas; por isso, motores, rodas, bateria e estrutura não estão dimensionados.

## Estado do projeto

- [x] Repositório público `nexus-bot`.
- [x] Plataforma de computação definida: Orange Pi 4 Pro, Allwinner A733, 4 GB RAM.
- [x] Runtime local escolhido: `llama.cpp`; Ollama está descartado.
- [x] Estratégia de autonomia evolutiva supervisionada definida: nível 2,5.
- [x] Um ESP32 principal e dois ESP32 Mini previstos para controle distribuído.
- [x] Saída sonora definida por buzzer piezo passivo; sem alto-falante ou fala sintetizada.
- [ ] Validar placa, revisão, Linux, drivers, temperatura e consumo.
- [ ] Medir o interior do barril e dimensionar a mecânica.
- [ ] Fechar lista de materiais e custo total dentro do orçamento-alvo de R$ 800.
- [ ] Implementar e testar o AI CORE, memória e NEXUS DEVELOPER.
- [ ] Implementar firmware, comunicação UART e testes de segurança.
- [ ] Integrar visão híbrida e hardware físico.

## Arquitetura resumida

- **Orange Pi 4 Pro A733/4 GB:** cérebro Linux e serviços de alto nível. Plataforma escolhida, ainda não validada em bancada.
- **IA local:** `llama.cpp`; Qwen2.5-0.5B-Instruct GGUF Q8_0 é o primeiro candidato experimental, sujeito a benchmark.
- **Memória:** SQLite com FTS5 inicialmente; busca semântica fica para uma etapa posterior se os recursos permitirem.
- **ESP32 principal:** controle de motores, sensores críticos, watchdog e parada segura.
- **Dois ESP32 Mini:** periféricos e sensores auxiliares, com divisão final ainda pendente de validação.
- **Ligação:** UART com protocolo versionado, mensagens delimitadas, timeout, validade curta dos comandos e integridade.
- **Som:** buzzer piezo passivo com padrões originais. Sem alto-falante, voz falada ou síntese de fala.
- **Visão:** estratégia híbrida; OpenCV primeiro e aceleração/modelos adicionais somente após testes compatíveis com a placa.
- **Autonomia:** nível 2,5 — desenvolve e testa melhorias de baixo risco em ambiente isolado; alterações importantes exigem aprovação humana.

## Princípios de engenharia

1. Segurança de movimento não depende da IA nem da conexão com a Orange Pi.
2. ESP32 pode rejeitar qualquer comando; perda de comunicação ou falha crítica leva os motores ao estado parado.
3. NEXUS DEVELOPER não pode alterar o próprio mecanismo de autorização ou as proteções físicas.
4. Código gerado deve passar por sandbox, testes, classificação de risco e possibilidade de rollback.
5. Motores não são alimentados pelos pinos da Orange Pi ou do ESP32.
6. Não assumir dimensões, autonomia, corrente, desempenho ou compatibilidade sem medição.
7. Recursos não testados em hardware real devem ser identificados como **não validados**.

Consulte [o diário de bordo](docs/diario-de-bordo.md) para decisões, correções, validações e próximos passos.
