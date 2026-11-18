# Software do NEXUS BOT

A Orange Pi 4 Pro (A733, 4 GB) executará os serviços de alto nível. A placa e o ambiente Linux ainda precisam ser validados fisicamente.

## Runtime e modelo

- Runtime escolhido: `llama.cpp`.
- Ollama está descartado.
- Qwen2.5-0.5B-Instruct GGUF Q8_0 é o primeiro modelo candidato para benchmark, não uma capacidade confirmada.
- Priorizar execução em CPU ARM64 no primeiro teste. NPU e runtimes específicos ficam em avaliação separada.
- Não considerar latência, RAM ou aceleração conhecidos antes de medir na placa real.

## Módulos previstos

- `core/`: orquestração, barramento de eventos, estado e scheduler.
- `intelligence/brain/`: interface do modelo local e prompts.
- `intelligence/planner/`: decomposição e execução de tarefas.
- `intelligence/attention/`: prioridades e controle de carga.
- `intelligence/developer/`: geração/teste controlado de ferramentas e melhorias.
- `intelligence/skills/`: catálogo de habilidades com contratos e permissões.
- `memory/`: SQLite, FTS5 e memória episódica.
- `vision/`: OpenCV inicial e experimentos de visão híbrida.
- `security/`: NEXUS GUARD, permissões, validação e emergência.
- `hardware/`: UART, protocolo e telemetria dos ESP32.
- `diagnostics/`: saúde do sistema, temperatura, memória e armazenamento.

## Autonomia evolutiva — nível 2,5

O NEXUS DEVELOPER poderá identificar lacunas, pesquisar, criar código e testar em ambiente isolado. A promoção automática será limitada a mudanças de baixo risco aprovadas por testes e com rollback. Alterações em módulos centrais, permissões, comportamento persistente ou segurança exigem aprovação humana.

O Developer não pode modificar o GUARD, conceder permissões a si próprio, contornar limites físicos ou apagar auditoria. Código gerado não deve executar com privilégios administrativos. Na primeira etapa, evitar Docker; usar isolamento por usuário, permissões mínimas, allowlist de comandos, diretórios separados e limites de recursos.

## Diretrizes

- Nenhuma rotina de IA controla GPIO/motores diretamente.
- Toda ordem de movimento passa por validação do NEXUS GUARD e pelo firmware ESP32.
- ESP32 principal mantém segurança física mesmo quando a Orange Pi ou a IA falhar.
- Registrar origem, horário, versão, risco, resultado e aprovação das mudanças.
- Manter versão anterior funcional e permitir rollback.
- Visão profunda deve ser ativada sob demanda; não manter inferência pesada contínua.
- SQLite/FTS5 primeiro; embeddings somente após medição.
- Nenhum serviço, modelo ou teste é declarado validado sem execução documentada na placa física.

## Status

Este diretório descreve a arquitetura-alvo. A implementação ainda não foi iniciada ou validada nesta atualização.
