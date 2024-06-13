# Vault Management

Sistema de Segurança para Cofre com Detecção de Presença e Verificação de Senha

Descrição e Funcionamento
Este projecto é um sistema de segurança para um cofre que detecta a presença de um indivíduo na sala e solicita a inserção de uma senha de 4 dígitos para permitir o acesso. Se a senha for inserida incorretamente três vezes, um alarme é disparado. Além disso, se a senha correta não for inserida dentro de um minuto, o alarme também é disparado. O sistema utiliza um sensor PIR para detectar movimento, um teclado matricial para inserção da senha, um display LCD para exibir informações e um buzzer e LED para sinalização do alarme.

Materiais Necessários
Arduino Uno
Sensor PIR (Infravermelho Passivo)
Teclado Matricial 4x4
Display LCD 16x2
Buzzer
LED (indicador de alarme)
Resistores (220 ohms)
Placa de ensaio (Breadboard)
Cabos Jumpers

Funcionamento
Inicialização: O sistema exibe uma mensagem inicial no LCD ao ser ligado.
Detecção de Presença: O sensor PIR detecta movimento na sala. Quando detectado, o sistema solicita a inserção da senha no display LCD.
Inserção da Senha: O usuário tem três tentativas para inserir a senha correta usando o teclado matricial.
Se a senha for inserida corretamente, o sistema exibe uma mensagem de "Acesso Permitido" no LCD.
Se a senha for inserida incorretamente três vezes, o alarme é disparado (buzzer e LED são ativados).
Tempo Limite: O usuário tem um minuto para inserir a senha correta. Se o tempo expirar sem a senha correta, o alarme é disparado.
Alarme: Quando o alarme é disparado, o buzzer e o LED ficam ativados por um período de tempo (por exemplo, 5 segundos).

Nota: Devem usar o thinkercad ou o proteus para a simulação do sistema e remeter até o dia 13 às 23:59
Não é necessário relatório, apenas o projecto e o código devidamente documento.
