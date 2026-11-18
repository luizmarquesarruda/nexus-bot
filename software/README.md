# Software da Orange Pi

A Orange Pi executará os serviços de alto nível do NEXUS BOT.

Estrutura planejada:
- `assistant/`: orquestração, estados, interface e integração com UART.
- `ai/`: reconhecimento de fala e experimentos de inferência local.
- `memory/`: persistência SQLite e histórico de eventos.

Diretrizes:
- Serviços separados por responsabilidade.
- Nenhuma rotina de IA controla GPIO/motores diretamente.
- Comandos de movimento passam por validação de comportamento e pelo firmware ESP32.
- Logs devem registrar origem, horário, estado e resultado dos comandos.
- Modelos locais só serão adotados após benchmark real de memória, temperatura e latência.

Nenhum serviço está implementado ou validado ainda.
