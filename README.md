# NEXUS BOT

Robô móvel compacto, com controle embarcado por microcontrolador e câmera independente. A V1 prioriza movimento confiável, segurança física, identificação de pontos por NFC e localização externa por GPS quando houver céu aberto.

O corpo será cilíndrico e a locomoção será exclusivamente por rodas na parte inferior. As dimensões internas ainda precisam ser medidas; motores, rodas, bateria e estrutura não estão dimensionados.

## Arquitetura V1 aprovada

- **1 ESP32 convencional principal:** motores, encoders, sensores anticolisão, emergência, GPS, NFC, sensores ambientais, Pan-Tilt da câmera, bateria e coordenação.
- **1 ESP32-CAM independente:** captura e transmissão de vídeo. Não controla motores nem funções de segurança.
- **Sem IA embarcada e sem Orange Pi** na V1.
- **Sem três ESP32 Mini:** GPS, NFC e sensores ambientais serão conectados ao ESP32 principal.
- Um segundo ESP32 convencional só será considerado se testes demonstrarem uma limitação concreta de GPIO, processamento, barramentos ou distribuição física.
- Um único Pan-Tilt para a câmera.
- Buzzer piezo passivo com padrões sonoros próprios; sem alto-falante ou voz falada.

## Estado do projeto

- [x] Arquitetura V1 simplificada aprovada e registrada.
- [x] Controlador principal único ESP32 definido.
- [x] ESP32-CAM independente definida para vídeo.
- [ ] Selecionar modelos exatos das placas e módulos.
- [ ] Medir o interior do corpo e definir massa/centro de gravidade.
- [ ] Dimensionar motores, rodas, driver e bateria.
- [ ] Definir pinagem, barramentos e alimentação.
- [ ] Implementar e testar firmware do ESP32 principal.
- [ ] Integrar e testar GPS, NFC, sensores e Pan-Tilt.
- [ ] Validar vídeo e coexistência com o controlador principal.
- [ ] Executar testes de segurança, consumo, interferência e autonomia.

## Princípios de engenharia

1. A parada de emergência deve desabilitar fisicamente a tração e não depender de rede ou software de alto nível.
2. Sensores anticolisão ficam ligados ao ESP32 principal.
3. Falha de GPS, NFC ou câmera não pode impedir a parada segura.
4. Não adicionar microcontroladores sem necessidade demonstrada por testes.
5. Separar e dimensionar a alimentação de motores, servos e lógica.
6. Não assumir dimensões, correntes, autonomia ou compatibilidade sem medição.
7. Recursos sem teste físico devem ser identificados como **não validados**.

Consulte [o diário de bordo](docs/diario-de-bordo.md) para decisões cronológicas e [a arquitetura](docs/arquitetura.md) para responsabilidades dos módulos.


## Código e estrutura de firmware

A estrutura inicial compilável do ESP32 principal está em [firmware/esp32](firmware/esp32/README.md). A documentação de camadas, fluxo, estados de segurança e sequência de integração está em [docs/arquitetura-software.md](docs/arquitetura-software.md).

- ESP32 principal: núcleo lógico de segurança independente de Arduino, com saídas de tração ainda não implementadas.
- ESP32-CAM: diretório separado; o firmware específico aguarda a confirmação da variante da placa e do sensor.
- Testes locais do núcleo: `cmake -S tests/host -B build/host && cmake --build build/host && ctest --test-dir build/host --output-on-failure`.
- Compilação ESP32: `pio run --project-dir firmware/esp32 --environment esp32dev`.

O alvo PlatformIO esp32dev é apenas uma configuração genérica e provisória de compilação. Placa física, GPIOs, driver, motores, sensores, circuito de emergência, vídeo e validação em bancada permanecem **não validados**.
