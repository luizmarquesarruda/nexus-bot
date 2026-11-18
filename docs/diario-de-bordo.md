# Diário de bordo — NEXUS BOT

## 2026-11-17 — consolidação inicial

- Projeto definido como robô assistente móvel de pequeno porte instalado em barril de 5 litros.
- Orçamento-alvo da V1: R$ 800; hardware usado é aceitável.
- Orange Pi Zero 3 H618 com 2 GB escolhida como candidata a cérebro, sujeita a validação de sistema operacional, áudio, temperatura e desempenho.
- ESP32 clássico definido como controlador local de motores, sensores, LEDs e buzzer.
- Decidido que o robô não terá alto-falante nem falará palavras. A resposta sonora será feita por buzzer piezo passivo com padrões próprios.
- Microfone para comandos de voz continua em aberto; preferência inicial por solução USB compatível com Linux.
- SQLite será a opção simples para memória local.
- UART será o canal inicial Orange Pi–ESP32, com timeout e parada segura.
- Repositório renomeado para `nexus-bot` e confirmado público.
- Estrutura inicial de documentação criada. Nenhuma peça foi comprada e nenhum teste físico foi executado.

## Próximas ações

1. Medir altura interna útil, diâmetro interno, gargalo/aberturas, pontos de fixação e interferências do barril.
2. Fotografar e registrar as medidas com régua/paquímetro.
3. Confirmar o anúncio exato e a variante de Orange Pi (RAM e revisão).
4. Definir microfone USB e testar captura de áudio na distribuição Linux escolhida.
5. Fazer benchmark de RAM, temperatura, STT em português e latência sob carga.
6. Levantar massa estimada, velocidade, inclinação e autonomia desejadas para dimensionar motores e bateria.
7. Fechar lista de materiais e custo total antes da compra.

## Registro de validação

- CI: não configurada / não executada.
- Teste de hardware: não validado.
- Reconhecimento de fala em português na Orange Pi: não validado.
- Inferência de LLM local: não validado.
- Mobilidade e autonomia: não validadas.
