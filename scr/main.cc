//TODO: Link - https://www.usinainfo.com.br/blog/cofre-arduino-projeto-para-controle-de-acesso-com-senha/
#include <Keypad.h>
#include <LiquidCrystal.h>

// Definição de pinos
const int pinoSensorPIR = A2;

const byte ROWS = 4;
const byte COLS = 4;

char teclas[ROWS][COLS] = 
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'S','0','H','D'},
};
byte linhasTeclado[] = {9, 8, 7, 6};
byte colunasTeclado[] = {5, 4, 3, 2};

const int pinoBuzzer = 12;
const int pinoLED = 13;
const int pinoLCD_RS = A0;
const int pinoLCD_EN = A1;
const int pinoLCD_D4 = A2;
const int pinoLCD_D5 = A3;
const int pinoLCD_D6 = A4;
const int pinoLCD_D7 = A5;

// Criação de objetos
Keypad teclado = Keypad(makeKeymap(teclas), linhasTeclado, colunasTeclado, ROWS, COLS);

const char senhaCorreta[] = "1234"; // Definição da senha

LiquidCrystal lcd(pinoLCD_RS, pinoLCD_EN, pinoLCD_D4, pinoLCD_D5, pinoLCD_D6, pinoLCD_D7);



// Variáveis
char senhaDigitada[5];
int tentativaSenha = 0;
int tempoLimite = 60;
int tempoRestante = tempoLimite;
bool alarmeAtivado = false;

void setup() {
  // Inicialização do serial
  Serial.begin(9600);

  // Configuração dos pinos como entrada ou saída
  pinMode(pinoSensorPIR, INPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(pinoLED, OUTPUT);

  // Inicialização do display LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Sistema de Segurança");
}

void loop() {
  // Verificação de presença
  if (digitalRead(pinoSensorPIR) == HIGH) {
    detectarPresenca();
  }

  // Atualização do tempo restante
  if (tempoRestante > 0) {
    tempoRestante--;
  }

  // Verificação do tempo limite
  if (tempoRestante == 0 && !alarmeAtivado) {
    ativarAlarme();
  }
}

void detectarPresenca() {
  // Exibir mensagem de presença no LCD
  lcd.clear();
  lcd.print("Presença detectada");
  lcd.setCursor(0, 1);
  lcd.print("Digite a senha:");

  // Limpar a variável da senha digitada
  memset(senhaDigitada, 0, sizeof(senhaDigitada));

  // Resetar o contador de tentativas de senha
  tentativaSenha = 0;

  // Resetar o tempo limite
  tempoRestante = tempoLimite;

  // Desativar o alarme
  desativarAlarme();

  // Ler a senha digitada pelo usuário
  lerSenhaTeclado();
}

void lerSenhaTeclado() {
  char tecla;

  while (tentativaSenha < 4) {
    tecla = teclado.getKey();
    if (tecla != 0) {
      // Armazenar a tecla digitada na variável da senha
      senhaDigitada[tentativaSenha] = tecla;
      tentativaSenha++;

      // Exibir a senha digitada no LCD (com asteriscos)
      lcd.setCursor(11 + tentativaSenha, 1);
      lcd.print("*");
    }
  }

  // Verificar se a senha completa foi digitada
  if (tentativaSenha == 4) {
    verificarSenha();
  }
}

void verificarSenha() {
  // Comparar a senha digitada com a senha correta
  if (strcmp(senhaDigitada, senhaCorreta) == 0) {
    // Senha correta: exibir mensagem de acesso permitido e desativar o alarme
    lcd.clear();
    lcd.print("Acesso Permitido");
    desativarAlarme();
  } else {
    // Senha incorreta: incrementar o contador de tentativas
    tentativaSenha++;

    // Verificar se o número máximo de tentativas foi excedido
    if (tentativaSenha == 3) {
      // Senha incorreta após 3 tentativas: ativar o alarme
      ativarAlarme();
    } else {
      // Senha incorreta,
      // Senha incorreta, mas ainda há tentativas restantes: exibir mensagem de erro e solicitar nova tentativa
      lcd.setCursor(0, 1);
      lcd.print("Senha Incorreta");
      lcd.setCursor(11, 1);
      lcd.print("Tente novamente:");
    }
  }
}

void ativarAlarme() {
  // Ativar o buzzer e o LED
  digitalWrite(pinoBuzzer, HIGH);
  digitalWrite(pinoLED, HIGH);

  // Definir o alarme como ativado
  alarmeAtivado = true;

  // Exibir mensagem de alarme no LCD
  lcd.clear();
  lcd.print("ALARME!");
}

void desativarAlarme() {
  // Desativar o buzzer e o LED
  digitalWrite(pinoBuzzer, LOW);
  digitalWrite(pinoLED, LOW);

  // Definir o alarme como desativado
  alarmeAtivado = false;
}
