# Serviço assistant

Ponto de entrada futuro para a máquina de estados, orquestração de tarefas, interface local e cliente UART.

O serviço não deve acionar motores diretamente. Toda solicitação de movimento passa por validação de intenção e pelo protocolo com o ESP32.

Status: não implementado.
