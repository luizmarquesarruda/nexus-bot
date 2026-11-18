# Arquitetura do sistema — NEXUS BOT

## 1. Objetivo

Construir um robô assistente móvel, pequeno e multifuncional, instalado em um barril Heineken de 5 litros. A IA começa compacta, mas a arquitetura deve permitir que o NEXUS desenvolva ferramentas e capacidades progressivamente, com autonomia evolutiva supervisionada de nível 2,5.

As dimensões internas do barril ainda não foram medidas. Portanto, não congelar desenho mecânico, motores, rodas ou bateria antes das medições.

## 2. Computação de alto nível

### Orange Pi 4 Pro — Allwinner A733, 4 GB RAM

Plataforma definida para o projeto. A variante/revisão exata, imagem Linux, kernel, drivers, consumo e temperatura precisam ser confirmados na placa física.

Responsabilidades:
- Linux minimalista, serviços de inicialização e acesso por SSH.
- Orquestração, planejamento, memória e interfaces.
- Inferência local experimental via `llama.cpp`.
- Coordenação de ferramentas e comunicação UART com ESP32.
- Processamento de visão leve e análise visual sob demanda.

Não presumir aceleração de LLM pela NPU. NPU, runtimes específicos e modelos convertidos são experimentais até validação.

## 3. IA local

- Runtime escolhido: `llama.cpp`. Ollama está descartado.
- Primeiro candidato: Qwen2.5-0.5B-Instruct GGUF Q8_0, executado inicialmente em CPU ARM64.
- O modelo e a quantização só serão confirmados depois de medir latência, RSS, pico de RAM, temperatura e estabilidade.
- O modelo interpreta solicitações e pede ferramentas; não é autoridade de segurança nem controla motores diretamente.
- Não há requisito de treinamento contínuo dos pesos. A evolução inicial ocorre por memória, ferramentas, scripts e módulos.

## 4. Módulos lógicos

### NEXUS BRAIN
Modelo local e interface de raciocínio. Interpreta intenção, gera respostas e solicita ferramentas permitidas.

### NEXUS MEMORY
Memória de curto prazo em RAM e persistência SQLite. FTS5 para busca textual inicial. Registra episódios, contexto, ações, resultados e confiança. Embeddings ficam para etapa posterior.

### NEXUS VISION
Captura da câmera e visão híbrida. Começar com OpenCV para captura, movimento e alterações de cena. Avaliar MobileCLIP-S0/VIPLite e NPU somente após validar drivers e runtime. Um detector leve, como YOLO de tamanho reduzido, pode ser avaliado depois.

### NEXUS ATTENTION
Fila de eventos, prioridade, deduplicação e escalonamento. Controla carga para evitar inferência visual/modelos pesados continuamente.

### NEXUS PLANNER
Transforma objetivos em tarefas menores, verificáveis, com pré-condições, limites e resultados esperados.

### NEXUS GUARD
Camada de autorização e segurança. Valida ferramentas, argumentos, limites, permissões, timeout e estados físicos. A IA não pode editar, desativar ou contornar o GUARD.

### NEXUS DEVELOPER
Pesquisa soluções, identifica lacunas, gera código/ferramentas, executa testes isolados e prepara propostas de evolução. Opera com permissões mínimas, diretório de trabalho isolado, comandos permitidos por lista, limites de recursos, logs e rollback.

### NEXUS SKILL FACTORY
Processo de criação de habilidades independentes: especificar, gerar, testar, avaliar risco, documentar e incorporar conforme a política de aprovação.

### NEXUS HEALTH
Monitora saúde do sistema: temperatura, memória, armazenamento, processos, comunicação, energia e falhas.

## 5. Autonomia evolutiva — nível 2,5

O NEXUS pode automaticamente:
- identificar limitações e registrar oportunidades;
- pesquisar soluções e documentação;
- criar scripts e ferramentas em ambiente isolado;
- executar testes automatizados e corrigir falhas no ambiente de testes;
- instalar mudanças de baixo risco somente quando os critérios de teste forem aprovados e houver rollback.

Exige aprovação humana:
- mudanças em módulos centrais, interfaces críticas, permissões ou comportamento persistente;
- mudanças cujo risco não possa ser classificado como baixo;
- qualquer promoção que altere comportamento operacional relevante.

Proibido autonomamente:
- alterar o próprio sistema de autorização;
- enfraquecer ou desligar o NEXUS GUARD;
- conceder permissões a si mesmo;
- apagar ou adulterar trilhas de auditoria;
- alterar limites de velocidade, parada de emergência ou proteções dos motores;
- obter controle direto dos motores fora do contrato validado.

Não instalar Docker na primeira etapa sem necessidade demonstrada. Isolamento inicial deve usar usuário sem privilégios, diretórios separados, ambientes Python, allowlist de comandos, limites de recursos e execução de testes sem acesso a segredos.

## 6. Controle embarcado

Arquitetura prevista:
- **ESP32 convencional principal:** motores, sensores críticos, watchdog, intertravamentos e parada segura.
- **ESP32 Mini 1:** sensores auxiliares (atribuição proposta).
- **ESP32 Mini 2:** monitoramento de energia, buzzer e periféricos (atribuição proposta).

Modelos exatos, pinagem, tensões e responsabilidades finais dos auxiliares ainda não foram validados.

O ESP32 principal é soberano na segurança do movimento. A Orange Pi solicita ações; o firmware verifica limites, sensores, estado e validade temporal e pode rejeitar qualquer solicitação. Falha da Orange Pi, do modelo ou da UART deve resultar em motores parados.

## 7. Comunicação Orange Pi ↔ ESP32

UART com protocolo versionado. O contrato deverá definir framing/delimitação, sequência, ACK, timeout, deduplicação, integridade (CRC/checksum), validação de tipos e limites, tamanho máximo e validade curta dos comandos. 115200 baud é apenas parâmetro inicial de bancada.

Nenhuma implementação ou teste físico do protocolo é declarado concluído neste documento.

## 8. Som e interação

- Saída sonora por buzzer piezo passivo, preferencialmente compatível com 3,3 V, com padrões próprios.
- Não haverá alto-falante nem fala sintetizada.
- Entrada por microfone/reconhecimento de fala não é requisito aprovado nesta arquitetura.
- Interface inicial pode ser desenvolvida sem depender de reconhecimento de voz.

## 9. Visão híbrida

- Leve: captura sob demanda ou intervalos controlados, OpenCV, movimento e alteração de cena.
- Profunda: classificação/análise visual sob demanda.
- MobileCLIP-S0/VIPLite e NPU são experimentais.
- YOLO leve pode ser avaliado posteriormente para detecção de objetos.
- A câmera não controla os motores diretamente; suas saídas viram evidências para o Planner.
- Hardware de câmera ainda não selecionado/validado.

## 10. Estrutura de software sugerida

```text
NEXUS_BOT/
├── core/
│   ├── orchestrator.py
│   ├── event_bus.py
│   ├── state_manager.py
│   └── scheduler.py
├── intelligence/
│   ├── brain/
│   ├── planner/
│   ├── attention/
│   ├── developer/
│   └── skills/
├── memory/
│   ├── database.py
│   ├── episodic.py
│   ├── retrieval.py
│   └── migrations/
├── vision/
├── security/
├── hardware/
├── behavior/
├── diagnostics/
├── tests/
├── config/
└── main.py
```

## 11. Ordem de implementação

1. Validar Orange Pi 4 Pro, Linux, boot, RAM, temperatura e microSD.
2. Compilar/testar `llama.cpp` e o modelo candidato.
3. Criar BRAIN e orquestrador sem hardware físico.
4. Criar MEMORY SQLite/FTS5.
5. Criar ATTENTION e PLANNER.
6. Criar GUARD e testes de rejeição.
7. Criar DEVELOPER/SKILL FACTORY em ambiente isolado.
8. Implementar UART e firmware do ESP32 principal.
9. Integrar os dois ESP32 Mini.
10. Implementar visão híbrida, primeiro OpenCV.
11. Integrar energia, chassis e sensores após medições do barril.
12. Executar testes integrados e de segurança.

## 12. Estado de validação

Esta é uma especificação de arquitetura. Não significa que o software, o firmware, o modelo, a NPU, a visão ou o hardware já tenham sido implementados/testados. Os resultados devem ser marcados como **não validados** até haver evidência de teste no hardware real.


## 13. Geometria mecânica e cúpula

### Corpo e locomoção

- O corpo será cilíndrico, alojado no barril de 5 litros.
- **Não haverá pernas laterais ou articuladas.** A locomoção será exclusivamente por rodas instaladas na parte inferior do corpo.
- Duas rodas motrizes e uma roda auxiliar são uma configuração candidata, não uma decisão fechada. A geometria final depende das medidas internas do barril, massa, centro de gravidade, piso e velocidade pretendida.
- O conjunto de tração deverá ficar em uma base/estrutura interna removível, separada da função estética da carcaça.
- Não dimensionar motores, rodas, driver ou bateria antes de levantar medidas e estimar a massa total.

### Cúpula

- A cúpula deverá ser fabricada por impressão 3D; material, dimensões, divisão em peças e método de fixação ainda não foram definidos.
- A cúpula deve ser projetada como módulo independente, com espaço para câmera/sensores somente depois de definir os componentes.
- **A rotação permanece em estudo.** Não há decisão entre cúpula fixa, giro limitado ou giro contínuo, nem entre servo, motorredutor ou outro mecanismo.
- Antes de selecionar o acionamento, avaliar: massa e inércia da cúpula; velocidade e precisão necessárias; ruído; folga; rolamento/guia; limites mecânicos; detecção de posição; alimentação; roteamento e torção dos cabos; eventual necessidade de anel deslizante; manutenção e risco de prender dedos/objetos.
- A solução inicial deve priorizar simplicidade e segurança. Não comprar mecanismo de rotação antes de fechar os requisitos.
