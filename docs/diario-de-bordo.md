# Diário de bordo — NEXUS BOT

Este documento é o registro cronológico das decisões, correções, hipóteses, validações e próximos passos do projeto. Entradas antigas são preservadas; quando uma decisão posterior substitui outra, a substituição fica explícita.

## 2026-11-17 — registro inicial preservado

1. Projeto definido como robô assistente móvel de pequeno porte instalado em um barril de 5 litros.
2. Orçamento-alvo inicial da V1: R$ 800; componentes usados podem ser considerados.
3. Orange Pi Zero 3 H618 com 2 GB foi registrada como candidata inicial de computação.
4. ESP32 clássico foi definido como controlador local de motores, sensores, LEDs e buzzer.
5. Foi decidido que o robô não terá alto-falante nem fala sintetizada. A resposta sonora será feita por padrões próprios em buzzer piezo passivo.
6. Microfone para comandos de voz ficou em aberto na proposta inicial.
7. SQLite foi considerado para memória local.
8. UART foi escolhida como ligação inicial Orange Pi–ESP32, com timeout e parada segura.
9. Repositório público criado com o nome `nexus-bot`.
10. Nenhuma peça foi comprada e nenhum teste físico foi executado.

## 2026-11-18 — consolidação de arquitetura e correções de rumo

### Decisões vigentes

11. **Computador principal corrigido:** a plataforma definida passa a ser Orange Pi 4 Pro, com SoC Allwinner A733 e 4 GB de RAM, conforme a placa selecionada nas conversas do projeto. A Orange Pi Zero 3 H618/2 GB deixa de ser a plataforma-alvo. A variante exata, revisão, alimentação, suporte Linux e disponibilidade de drivers ainda precisam ser conferidos no produto físico.
12. **Sistema operacional:** usar Linux minimalista, sem desktop permanente, com acesso por SSH. Preferir a imagem/kernel suportados pelo fabricante quando forem necessários drivers específicos do A733. Compatibilidade real ainda não validada.
13. **Runtime de IA:** usar `llama.cpp` para inferência local. **Ollama está descartado** para este projeto e não deve voltar a ser proposto como runtime padrão.
14. **Modelo inicial experimental:** Qwen2.5-0.5B-Instruct em formato GGUF, quantização Q8_0, como primeiro candidato para benchmark em CPU ARM64. Não é desempenho validado nem compromisso definitivo de modelo.
15. **NPU:** o acelerador anunciado para a placa não deve ser tratado como acelerador universal de LLM. Qualquer uso depende de runtime, drivers, conversão e benchmark específico; até lá, NPU é experimental.
16. **Armazenamento:** começar com microSD de boa procedência. SSD M.2/NVMe não faz parte dos requisitos da primeira versão. Evitar escrita excessiva e implementar rotação de logs e gravações agrupadas.
17. **Memória persistente:** SQLite como base local, com FTS5 para pesquisa textual. Embeddings e busca semântica ficam para etapa posterior, somente se RAM, CPU e armazenamento permitirem.
18. **Linguagem de integração:** Python nos serviços de alto nível, com interfaces explícitas entre módulos e sem dependências circulares.
19. **Objetivo de IA:** construir uma IA pequena no início, mas com arquitetura preparada para desenvolver novas habilidades. Não confundir evolução de capacidades com treinamento contínuo dos pesos do modelo.
20. **Autonomia escolhida — nível 2,5:** autonomia evolutiva supervisionada. O NEXUS pode identificar limitações, pesquisar soluções, criar ferramentas, escrever código, corrigir código em ambiente isolado e executar testes automaticamente. Melhorias classificadas como baixo risco podem ser instaladas automaticamente após aprovação dos testes e com rollback disponível. Mudanças em módulos centrais, permissões, comportamento persistente ou interfaces críticas exigem aprovação humana.
21. **Limites invioláveis do nível 2,5:** o NEXUS não pode alterar o próprio mecanismo de autorização, desativar ou enfraquecer o NEXUS GUARD, conceder permissões a si mesmo, apagar trilhas de auditoria, modificar os limites de segurança física ou assumir controle direto dos motores. Nenhum código gerado deve ir diretamente para produção sem testes e política de aprovação.
22. **Novo módulo — NEXUS DEVELOPER:** subsistema responsável por pesquisar, planejar, gerar ferramentas/scripts, executar testes, comparar resultados e preparar propostas de evolução. O módulo deve operar em ambiente isolado, com permissões mínimas, comandos permitidos por lista, limites de recursos, logs e possibilidade de reversão.
23. **Novo conceito — NEXUS SKILL FACTORY:** processo para transformar uma capacidade ausente em uma ferramenta independente: detectar a lacuna, especificar a habilidade, desenvolver, testar, avaliar risco e incorporar conforme a política do nível 2,5.
24. **Memória evolutiva escolhida:** registrar episódios, contexto, ações, resultados e confiança; identificar padrões; sugerir mudanças e novas habilidades. A memória pode evoluir, mas não pode alterar automaticamente regras de segurança ou limites de ação.
25. **Visão escolhida — híbrida:** começar com captura e processamento leve usando OpenCV; ativar análise mais pesada sob demanda. MobileCLIP-S0 e VIPLite são experimentos possíveis para aceleração/visão, sem garantia de compatibilidade com a NPU. YOLO leve pode ser avaliado posteriormente para detecção de objetos; não é requisito inicial.
26. **Autonomia operacional escolhida — controlada:** iniciar em modo de observação/resposta. Depois habilitar apenas comandos previamente autorizados. Iniciativas automáticas devem ser limitadas a ações expressamente permitidas e não podem resultar em movimento perigoso.
27. **Controle embarcado:** arquitetura prevista com um ESP32 convencional como controlador principal e dois ESP32 Mini como auxiliares. Distribuição proposta: ESP32 principal para motores, sensores críticos, watchdog e parada segura; Mini 1 para sensores auxiliares; Mini 2 para monitoramento de energia, buzzer e periféricos. Modelos, pinagem, tensões e divisão final ainda não foram validados.
28. **Áudio corrigido:** não haverá voz falada, alto-falante, Piper ou síntese de fala. A comunicação sonora será por buzzer piezo passivo, preferencialmente compatível com 3,3 V, acionado pelo ESP32 e com padrões sonoros próprios. Entrada por microfone não está aprovada como requisito da arquitetura atual; não presumir reconhecimento de fala.
29. **Comunicação entre placas:** UART entre Orange Pi e ESP32 principal. O protocolo deve ter framing/delimitação, versão, sequência, ACK, timeout, validade curta dos comandos, deduplicação, validação rigorosa e mecanismo de integridade (CRC/checksum). 115200 baud é apenas valor inicial de bancada, sujeito a teste.
30. **Segurança física:** Orange Pi/LLM apenas solicitam ações. O ESP32 principal é soberano na validação de movimento e pode rejeitar qualquer comando. Perda de comunicação, travamento da Orange Pi, comando expirado ou falha crítica devem deixar motores parados. A parada de emergência deve existir em hardware e não depender da IA.
31. **Forma física:** robô com rodas, pequeno, alojado em barril Heineken de 5 litros. Altura útil, diâmetro interno, gargalo, fixações, massa e centro de gravidade ainda não foram medidos. Não dimensionar motores, bateria ou estrutura definitiva sem essas medições.
32. **Limite financeiro:** manter R$ 800 como orçamento-alvo global da V1. A placa principal pode consumir parcela relevante do orçamento. Fechar custo total — inclusive frete, tributos, bateria, BMS, conversores, driver, motores, rodas, sensores, cabos e estrutura — antes de autorizar compras.
33. **Arquitetura de software:** separar pelo menos BRAIN, MEMORY, VISION, ATTENTION, PLANNER, GUARD, DEVELOPER, SKILLS e HEALTH/diagnósticos. A IA não deve controlar GPIO ou motores diretamente.
34. **Estratégia de implementação:** primeiro criar e validar o AI CORE em bancada, sem motores e sem câmera: inicialização Linux, `llama.cpp`, modelo, orquestrador, memória, logs e testes offline. Integrar visão e controle físico somente depois.
35. **Sem Docker na primeira etapa:** evitar adicionar Docker inicialmente em uma SBC de recursos limitados. Usar isolamento por usuário, diretórios de trabalho separados, ambientes Python, permissões mínimas e ferramentas de sandbox adequadas. Reavaliar somente se houver necessidade demonstrada.
36. **Não adicionar SQLite/embeddings/serviços pesados sem medição:** o projeto começa simples; qualquer aumento de consumo deve ser medido na placa real.

### Correções documentais necessárias

37. README anterior e documentos iniciais ainda apontavam Orange Pi Zero 3/2 GB como placa candidata principal. Corrigido nesta revisão para Orange Pi 4 Pro/A733/4 GB como plataforma definida.
38. Documentação inicial descrevia reconhecimento de fala/microfone como parte do caminho principal. Corrigido: entrada por microfone e reconhecimento de fala não são requisitos aprovados; saída sonora é exclusivamente buzzer passivo, sem voz falada.
39. Referências a execução de IA local foram qualificadas como experimentais e dependentes de benchmark. `llama.cpp` é o runtime escolhido; Qwen2.5-0.5B-Instruct Q8_0 é somente o modelo inicial candidato.
40. Foram incorporados os módulos NEXUS DEVELOPER e NEXUS SKILL FACTORY e as restrições de autonomia de nível 2,5.
41. As especificações de hardware, desempenho, temperatura, autonomia, visão e compatibilidade com NPU permanecem **não validadas** até ensaio na placa física.

## Plano de execução — sequência de fases

42. Fase 1 — confirmar variante/revisão da Orange Pi 4 Pro, instalar Linux suportado e medir boot, RAM, temperatura, armazenamento e estabilidade.
43. Fase 2 — compilar/instalar `llama.cpp`, executar Qwen2.5-0.5B-Instruct GGUF Q8_0 e medir latência, RSS, pico de RAM, temperatura e estabilidade.
44. Fase 3 — construir NEXUS BRAIN e orquestrador, sem acesso direto ao hardware.
45. Fase 4 — implementar NEXUS MEMORY com SQLite/FTS5, esquema versionado e testes de persistência.
46. Fase 5 — implementar NEXUS ATTENTION com fila de eventos, prioridades, deduplicação e controle de carga.
47. Fase 6 — implementar NEXUS PLANNER e catálogo inicial de ferramentas.
48. Fase 7 — implementar NEXUS GUARD, níveis de permissão, classificação de risco, logs invioláveis para o processo normal e testes de rejeição.
49. Fase 8 — implementar NEXUS DEVELOPER/SKILL FACTORY em sandbox, testes automatizados, aprovação por risco e rollback.
50. Fase 9 — estabelecer protocolo UART e firmware do ESP32 principal, testando mensagens inválidas, fragmentadas, duplicadas, expiradas e perda de comunicação.
51. Fase 10 — integrar os dois ESP32 Mini, um por vez, após fechar suas responsabilidades e interfaces.
52. Fase 11 — implementar visão híbrida: OpenCV primeiro; NPU/MobileCLIP/VIPLite e detector leve somente após benchmark.
53. Fase 12 — integrar chassis, energia e módulos ao barril, com ensaios de parada, temperatura, ventilação, vibração, autonomia e estabilidade.

## Estado de validação em 2026-11-18

- Plataforma escolhida: Orange Pi 4 Pro A733/4 GB — **não validada fisicamente**.
- Linux, drivers A733 e NPU — **não validados**.
- `llama.cpp` no hardware-alvo — **não validado**.
- Qwen2.5-0.5B-Instruct GGUF Q8_0 — candidato; **não validado no hardware-alvo**.
- SQLite/FTS5 — decisão de arquitetura; implementação **não iniciada/não validada**.
- NEXUS DEVELOPER nível 2,5 — requisito de projeto; implementação **não iniciada**.
- ESP32 principal + dois ESP32 Mini — arquitetura escolhida; placas, firmware e integração **não validados**.
- Buzzer passivo — decisão de interface sonora; ligação elétrica e padrões **não validados**.
- Visão híbrida — estratégia escolhida; câmera, modelos e NPU **não validados**.
- Dimensões do barril, motores, bateria, autonomia e orçamento final — **pendentes**.
- Código de firmware/software e testes automatizados — não declarados como concluídos nesta atualização.
- CI: **não validada nesta atualização**.

## Próxima prioridade

Executar a Fase 1: confirmar a placa Orange Pi 4 Pro exata e preparar uma imagem Linux suportada. Não avançar para inferência, NPU ou montagem mecânica antes de obter medições reais da placa e do barril.


## 2026-11-18 — clarificação do conceito mecânico

54. **Locomoção sem pernas:** o NEXUS BOT não terá pernas laterais, articuladas ou semelhantes às do R2-D2. A locomoção será feita exclusivamente por rodas instaladas na parte inferior do corpo cilíndrico.
55. **Arranjo das rodas ainda em aberto:** duas rodas motrizes e uma roda auxiliar continuam sendo apenas uma hipótese de chassis, não uma especificação fechada. Quantidade, posição, diâmetro, tipo de roda, suspensão/apoio e geometria dependem das medidas do barril, massa e centro de gravidade.
56. **Cúpula impressa em 3D:** a intenção é fabricar a cúpula por impressão 3D. Material, divisão em peças, espessura, acabamento, encaixe e dimensões serão definidos depois do levantamento físico e da escolha da impressora/processo.
57. **Giro da cúpula não decidido:** manter a cúpula inicialmente como módulo independente, sem escolher servo, motorredutor, rolamento, anel deslizante ou mecanismo de transmissão. A decisão depende de definir se o giro será limitado ou contínuo, quais componentes ficarão na cúpula, massa/inércia, ruído, folga, rota dos cabos, alimentação, limites mecânicos e manutenção.
58. **Referência do artigo R2-D2:** aproveitar o princípio de construir uma estrutura interna de suporte e instalar os módulos nela antes do acabamento externo. Não reproduzir pernas, pés motorizados ou o mecanismo de cúpula da réplica; são soluções de outra escala e finalidade.
59. **Regra de projeto:** não comprar nem dimensionar o mecanismo de rotação da cúpula até comparar pelo menos uma solução fixa, uma solução de giro limitado e uma solução de giro contínuo, considerando cabos e segurança mecânica.


## 2026-11-19 — simplificação e aprovação da arquitetura embarcada V1

60. **Decisão de arquitetura atualizada e aprovada:** a V1 do NEXUS BOT não terá Orange Pi, computador de placa única ou IA embarcada. As referências anteriores a Orange Pi, LLM local, `llama.cpp`, modelos GGUF, NPU, NEXUS BRAIN, NEXUS DEVELOPER e autonomia de nível 2,5 ficam registradas apenas como histórico de propostas abandonadas; não são requisitos ativos e não devem orientar compras ou implementação sem nova decisão explícita do usuário.
61. **Controlador principal único:** usar um ESP32 convencional como controlador central do robô. Responsabilidades previstas: motores e encoders, sensores anticolisão, botão/circuito de emergência, GPS, leitor NFC, sensores ambientais, Pan-Tilt da câmera, monitoramento de bateria e coordenação dos periféricos.
62. **Câmera independente:** manter um ESP32-CAM separado para captura e transmissão de vídeo. A ESP32-CAM não controla motores nem assume funções de segurança. A interface de comunicação entre câmera e controlador principal será definida após selecionar o módulo e validar o fluxo de vídeo.
63. **Três ESP32 Mini descartados na V1:** não instalar placas auxiliares dedicadas a GPS, NFC ou sensores ambientais. O ESP32 principal deve gerenciar esses periféricos diretamente, desde que a pinagem, os barramentos, a memória e a carga de processamento sejam suficientes.
64. **Expansão condicionada a evidência:** considerar um segundo ESP32 convencional somente se testes demonstrarem falta real de GPIOs, saturação de processamento, conflito de barramentos, interferência ou necessidade de distribuição física. Não adicionar controladores preventivamente.
65. **Locomoção:** exclusivamente por rodas na parte inferior do corpo cilíndrico; não haverá pernas laterais. Motores, rodas e driver permanecem pendentes de dimensionamento pela massa, medidas internas, corrente de bloqueio e piso.
66. **Pan-Tilt:** um único conjunto Pan-Tilt dedicado à câmera. A proposta anterior de cúpula giratória e de segundo Pan-Tilt fica descartada; não reintroduzir sem nova decisão explícita.
67. **Comunicação sonora:** buzzer piezo passivo com padrões sonoros próprios; sem alto-falante e sem voz falada.
68. **Segurança:** sensores anticolisão e circuito de parada devem permanecer ligados ao ESP32 principal. A parada de emergência deve desabilitar fisicamente a tração, sem depender da ESP32-CAM, de rede ou de software de alto nível. Perda de câmera, GPS ou NFC não pode impedir a parada segura.
69. **Plano de validação atualizado:** primeiro validar o ESP32 principal em bancada com alimentação protegida, driver, motores, encoders e emergência; depois integrar periféricos um a um (sensores, NFC, GPS e Pan-Tilt); validar a ESP32-CAM separadamente e, por último, testar coexistência, interferência, consumo e falhas. Nenhum desses testes é declarado concluído por esta decisão documental.
70. **Documentação:** README, arquitetura, hardware e software foram alinhados à arquitetura V1 sem IA embarcada e sem três ESP32 Mini. Modelos exatos das placas, pinagem, bateria, motores, sensores, câmera e custos continuam pendentes de seleção e teste.


## 2026-11-19 — limpeza documental ESP32-only

71. **Escopo da limpeza:** consolidar a V1 exclusivamente na arquitetura embarcada já aprovada: um ESP32 convencional principal e uma ESP32-CAM independente para vídeo.
72. **Removido do conteúdo ativo:** documento de protocolo UART Orange Pi–ESP32 e diretórios de documentação de IA, serviço assistant e memória SQLite associados à arquitetura SBC abandonada. O histórico dessas decisões permanece preservado neste diário como registro, não como requisito vigente.
73. **Documentação elétrica revisada:** retiradas dependências da Orange Pi e da UART; reforçada a separação entre potência, lógica e servos, além da emergência física independente de Wi-Fi e câmera.
74. **Firmware e testes:** READMEs alinhados ao controlador ESP32 principal e à ESP32-CAM independente. A implementação real, pinagem, compilação e ensaios físicos continuam **não validados**.
75. **PR #1:** o esqueleto anterior inclui cliente Python e dependência de UART com computador de alto nível, incompatíveis com a arquitetura atual. Deve permanecer fora da V1 e ser encerrado como proposta supersedida; sua branch e histórico não são apagados.
76. **Sem alteração de hardware presumido:** modelos exatos, driver, motores, sensores, bateria, pinagem e interface de vídeo continuam pendentes de seleção e validação.
77. **Sem CI ou validação física declarada:** esta limpeza é documental/estrutural. Não comprova compilação, funcionamento do firmware, vídeo, autonomia ou segurança física.
