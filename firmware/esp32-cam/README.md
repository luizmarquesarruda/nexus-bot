# Firmware — ESP32-CAM independente

Este diretório reserva o firmware da câmera, separado do controlador principal.

## Responsabilidade

- Inicializar o sensor de imagem da variante selecionada.
- Capturar e transmitir vídeo.
- Informar disponibilidade/diagnóstico por uma interface ainda a definir.

## Limites obrigatórios

- Não controlar motores, servos de tração ou driver.
- Não implementar nem substituir a parada de emergência.
- Não ser requisito para inicialização ou locomoção básica do robô.
- Não bloquear, reiniciar ou alterar o ciclo crítico do ESP32 principal.
- Perda de vídeo, rede ou alimentação da câmera não pode impedir a parada segura.

## Pendências antes do código específico

Confirmar modelo e revisão da placa, sensor de imagem, PSRAM, antena, alimentação, método de transmissão, orçamento de banda e interface com o controlador principal. Não selecionar board, GPIOs ou biblioteca de câmera antes dessas verificações.

Estado: estrutura documental inicial; firmware, compilação, vídeo, consumo e integração **não validados**.
