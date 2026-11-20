# Firmware — ESP32 principal

Este diretório é reservado ao firmware embarcado do controlador principal do NEXUS BOT V1.

## Responsabilidades

- Controle da tração e leitura dos encoders.
- Sensores anticolisão e bumpers.
- Tratamento do botão/circuito de emergência e estados seguros.
- GPS, NFC e sensores ambientais.
- Controle de um único Pan-Tilt dedicado à câmera.
- Monitoramento de bateria e diagnóstico.
- Buzzer piezo passivo.
- Interface de comando do operador e integração não crítica com a ESP32-CAM.

## Arquitetura vigente

- Um ESP32 convencional é a autoridade de movimento e segurança.
- Uma ESP32-CAM independente captura e transmite vídeo.
- A ESP32-CAM não controla motores nem implementa parada de emergência.
- Não há Orange Pi, SBC, LLM embarcado ou cliente UART de computador de alto nível na V1.
- Não adicionar ESP32 auxiliares sem evidência de limitação de GPIO, processamento, barramento ou distribuição física.

## Regras obrigatórias

- Motores/driver iniciam desabilitados.
- Emergência física interrompe a tração sem depender de Wi-Fi, câmera ou lógica de alto nível.
- Dados inválidos ou antigos de sensores não autorizam movimento.
- Watchdog e timeouts levam a estado seguro.
- Falha de GPS, NFC ou ESP32-CAM não pode impedir a parada segura.
- Após boot, falha ou reconexão, não retomar movimento automaticamente.
- Não habilitar saídas físicas antes de selecionar e validar driver, motores, pinagem e circuito de emergência.

## Estado de implementação

O repositório principal ainda não contém firmware funcional do controlador. Plataforma/framework, placa exata, pinagem, barramentos e módulos permanecem pendentes. A implementação e compilação estão **não validadas**.

## Sequência inicial

1. Selecionar a placa ESP32 e confirmar revisão, GPIOs e alimentação.
2. Criar projeto mínimo e validar compilação/boot.
3. Implementar estado seguro, watchdog e interface de emergência.
4. Integrar driver e motores em bancada, com rodas suspensas.
5. Integrar encoders e sensores anticolisão.
6. Integrar GPS, NFC, sensores ambientais, Pan-Tilt e buzzer individualmente.
7. Desenvolver firmware separado para ESP32-CAM e validar vídeo.
8. Integrar módulos e executar testes de falha, consumo, temperatura e interferência.

Nenhum item acima deve ser marcado como concluído sem evidência reproduzível.
