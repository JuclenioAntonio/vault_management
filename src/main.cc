#include <Keypad.h>
#include <LiquidCrystal.h>


// Definição das linhas e colunas do teclado matrical
const byte ROWS = 4;
const byte COLS = 4;

byte linhasTeclado[] = {9, 8, 7, 6};
byte colunasTeclado[] = {5, 4, 3, 2};

char teclas[ROWS][COLS] = 
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'S','0','H','D'},
};


// Definição de pinos dos sensores
const int pinoBuzzer = 12;
const int pinoLED = 13;
const int pinoSensorPIR = 10;


// Definição dos pinos do LCD 16x2
const int pinoLCD_RS = A0;
const int pinoLCD_EN = A1;
const int pinoLCD_D4 = A2;
const int pinoLCD_D5 = A3;
const int pinoLCD_D6 = A4;
const int pinoLCD_D7 = A5;


// Variáveis de gesão da senha
char senhaDigitada[5];
int tamanhoSenhaDigitada = 0;
const char senhaCorreta[] = "1234";
int totalTentativaSenha = 1;


// Definição do estado do alarme(buzzer)
bool estadoAlarme = false;


// Controle do tempo de inatividade e alarme 
const unsigned long tempoInatividadePadrao = 5000; // (60 segundos de inatividade)
const unsigned long tempoAtividadeAlarmePadrao = 3000;      // (10 seguntos de atividade)
unsigned long tempoInicialContagem = 0;
unsigned long tempoInicialContagemAtividadeAlarme = 0;


// Definição do objecto para manipulação do Teclado Matricial 4x4
Keypad teclado = Keypad(makeKeymap(teclas), linhasTeclado, colunasTeclado, ROWS, COLS);


// Definição do objecto para manipulação do Display LCD 16x2
LiquidCrystal lcd(pinoLCD_RS, pinoLCD_EN, pinoLCD_D4, pinoLCD_D5, pinoLCD_D6, pinoLCD_D7);


// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

void setup() {
  // Definição da taxa de transmissão (9.600 bits/s)
  Serial.begin(9600);
  
  // Configuração dos pinos de entrada e saída
  pinMode(pinoSensorPIR, INPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(pinoLED, OUTPUT);

  // Inicialização do display LCD
  lcd.begin(16, 2);
  
  // Escreve na tela LCD a mensagem "BEM-VINDO"
  lcd.setCursor(0, 0); 
  lcd.print("BEM-VINDO");
  
  // Escreve na tela LCD a mensagem "ACESSO RESTRITO"
  lcd.setCursor(0, 1);
  lcd.print("ACESSO RESTRITO");
  
  // Inicializa a contagem do tempo de inatividade
  tempoInicialContagem = millis(); 
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

void loop() {  
  // Verifica o tempo de inatividade
  if(verificarInatividade()){
    tempoInicialContagem = millis();
    tempoInicialContagemAtividadeAlarme = millis()
    ativarAlarme(); 
  }
  

  // Detecta o movimento de pessoa na sala
  if (digitalRead(pinoSensorPIR) == HIGH) {
    detectarPresenca();
  } 
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void detectarPresenca() {
  // Limpa a mensagem anterior do display LCD
  lcd.clear();
   
  // Escreve na tela LCD a mensagem "PESSOA PRESENTE!"
  lcd.setCursor(0, 0);
  lcd.print("PESSOA PRESENTE!");

  // Escreve na tela LCD a mensagem "SENHA:"
  lcd.setCursor(0, 1);
  lcd.print("SENHA:");
  
  // Limpar a variável que guarda a senha digitada
  memset(senhaDigitada, 0, sizeof(senhaDigitada));
  
  // Resetar o contador do tamanho de senha
  tamanhoSenhaDigitada = 0;  

  // Renicia o contagem de inatividade
  tempoInicialContagem = millis(); 

  // Leitura da senha digitada pelo usuário
  lerSenhaTeclado();
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
bool verificarInatividade() {
  // Verifica o tempo de inatividade
  bool tempoInatividade = (millis() - tempoInicialContagem) > tempoInatividadePadrao;
  return tempoInatividade;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
bool verificarAtividadeAlarme() {
  // Verifica o tempo de atividade do alarme
  bool tempoAtividade = (millis() - tempoInicialContagemAtividadeAlarme) > tempoAtividadeAlarmePadrao;
  return tempoAtividade;
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void lerSenhaTeclado() {  
  // Define a variável para ler o teclado
  char tecla; 
  
  // Verifica se a senha de 4 digitos e se o tempo de inatividade é menor de 60 segundos
  while ((tamanhoSenhaDigitada < 4) && !verificarInatividade()) {
    // Capta o valor da tecla digitada
    tecla = teclado.getKey();

    if (tecla != 0) {
      senhaDigitada[tamanhoSenhaDigitada] = tecla;
      tamanhoSenhaDigitada++;

      // Escreve a senha digitada no LCD (com asteriscos)
      lcd.setCursor(6 + tamanhoSenhaDigitada, 1);
      lcd.print("*");
    }
    
  }

  // Valida a senha digitada pelo utilizador
  if (tamanhoSenhaDigitada == 4) {
    verificarSenha();
  }

  // Dispara o alarme se o tempo de inatividade for maior que 60 segundos
  if (verificarInatividade()) {
    ativarAlarme(); 

    // Reinicia valor da contagem inicial do tempo de inatividade
    tempoInicialContagem = millis();
  }

}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void verificarSenha() {
  // Valida a senha digitada
  if (strcmp(senhaDigitada, senhaCorreta) == 0) {
    // Escreve mensagem  
    lcd.clear();
    lcd.print("Acesso Permitido");

  } else {
    // Ativa o alarme se a senha for errada 3x
    if (totalTentativaSenha == 3) {
      ativarAlarme();
    } 
    else {
      // Incrementa o contador de tentativa de senhas
      totalTentativaSenha++;
      
      // Concatena a texto com o nº de tentativas restantes
      char err_msg[16];
      int charsWritten = sprintf(err_msg, "SENHA ERRADA(X%d)", 4 - totalTentativaSenha);

      // Escreva na tela as tentativas restantes
      lcd.setCursor(0, 0);
      lcd.print(err_msg);
      
      // Limpa a 2º linha da tela LCD
      lcd.setCursor(0, 1);
      lcd.print("                ");

      // Escreve a palavra "SENHA" na 2ª linha
      lcd.setCursor(0, 1);
      lcd.print("SENHA:");
      
      // Reinicia a contagem do tamanho da senha
      tamanhoSenhaDigitada = 0;

      // Volta no inicio da função até as tentativas se esgotarem
      lerSenhaTeclado();
    }
  }
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void desativarAlarme() {
  // Desativar o buzzer e o LED
  digitalWrite(pinoBuzzer, LOW);
  digitalWrite(pinoLED, LOW);

  // Define o alarme como desativado
  estadoAlarme = false;

  // Reinicia a contagem do tempo de inatividade
  tempoInicialContagem = millis(); 
  tempoInicialContagemAtividadeAlarme = millis();
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
void ativarAlarme() {
  
  // Desativa o buzzer e o led enquanto o tempo de atividade do alarme for maior que 10 segundos
  if(verificarAtividadeAlarme()){
    desativarAlarme();
      
    lcd.setCursor(0, 0);
    lcd.print("1BEM-VINDO");
  
    lcd.setCursor(0, 1);
    lcd.print("1ACESSO RESTRITO");
  } else {
    // Exibir mensagem de alarme no LCD
    lcd.clear();
    lcd.print("ALARME!");
  }
  
  // Aciona o buzzer e o led enquanto o tempo de atividade do alarme for menor que 10 segundos
  while(!verificarAtividadeAlarme()){
    // Ativar o buzzer e o LED
    digitalWrite(pinoBuzzer, HIGH);
    digitalWrite(pinoLED, HIGH);

    // Definir o alarme como ativado
    estadoAlarme = true;
  }
}
