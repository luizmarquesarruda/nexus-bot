# Memória local

SQLite é a opção inicial para armazenar configurações, eventos, sessões e memória operacional.

Requisitos:
- esquema versionado;
- transações e integridade;
- cópias de segurança exportáveis;
- retenção configurável sem apagar dados importantes silenciosamente;
- timestamps consistentes;
- testes de recuperação após encerramento inesperado.

Não armazenar credenciais em texto puro.

Status: não implementado.
