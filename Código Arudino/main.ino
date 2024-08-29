#include <Adafruit_MLX90614.h> // Biblioteca do Sensor de Temperatura
#include <LiquidCrystal.h> // Biblioteca do Painel de LED
#include "Servo.h" // Biblioteca do Servo

Servo servoirriga; // Declaração do ServoMotor
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); // Define os pinos que serão utilizados para ligação ao display
Adafruit_MLX90614 temp = Adafruit_MLX90614(); // Declaração do Sensor de temperatura

double tempambiente = 0; // Declaração da variável da temperatura ambiente
const int pinoSensor = 1; // Pino do Sensor de Umidade
const int pinoPOT = A2; // Pino do Potenciômetro
int valorLido; // Valor retornado do Sensor de Umidade
int analogSoloSeco = 400; // Valor Medido com o solo Seco
int analogSoloMolhado = 150; // Valor Medido com o solo Molhado
int percSoloSeco = 0; // MENOR PERCENTUAL DO SOLO SECO
int percSoloMolhado = 100; // MAIOR PERCENTUAL DO SOLO MOLHADO
int temperaturaPotenciometro = 0; // Variável que armazena o valor do potenciômetro de controle de temperatura

void setup() {
  lcd.begin(16, 2); // Define o número de colunas e linhas do LCD
  pinMode(4, OUTPUT); // Pino do Led
  pinMode(pinoPOT, INPUT); // Pino do Potenciômetro
  servoirriga.attach(2); // Declarando a porta do Servo Motor
  servoirriga.write(0); // Define ângulo 0 para o servo motor
  Serial.begin(9600); // Porta monitor serial
  temp.begin(); // Inicializa sensor de temperatura
  pinMode(3, OUTPUT); // Porta em que está ligada a Ponte H e o Motor DC (Cooler)
}

void loop() {
  digitalWrite(4, HIGH); // Ascende o Led para mostrar o funcionamento
  lcd.clear(); // Limpa a tela do Painel de LED
  lcd.setCursor(3, 0); // Posiciona o cursor na coluna 3, linha 0;
  
  temperaturaPotenciometro = map(analogRead(pinoPOT), 0, 1023, 0, 40); // Lê a temperatura máxima definida pelo usuário no potenciômetro
  lcd.print("U: ");
  lcd.print(temperaturaPotenciometro);
  lcd.print(" C"); // Envia o valor definido pelo usuário
  
  lcd.setCursor(3, 1); // Posiciona o cursor na coluna 3, linha 1;
  tempambiente = temp.readAmbientTempC();
  lcd.print("A: ");
  lcd.print(tempambiente);
  lcd.print(" C"); // Envia o valor da temperatura atual
  
  Serial.print("Temperatura Ambiente = "); // Imprime no Serial
  Serial.print(tempambiente); // Imprime no Serial o valor da Temperatura
  Serial.println("*C"); // Imprime no Serial
  
  valorLido = constrain(analogRead(pinoSensor), analogSoloMolhado, analogSoloSeco); // Mantém valorLido dentro do intervalo (entre analogSoloMolhado e analogSoloSeco)
  valorLido = map(valorLido, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco); // Executa a função "map" de acordo com os parâmetros passados
  
  Serial.print("Umidade do solo: "); // Imprime o texto no Monitor Serial
  Serial.print(valorLido); // Imprime no Monitor Serial o percentual de umidade do solo
  Serial.println("%"); // Imprime o caractere no Monitor Serial
  
  LigaIrrigador();
  LigaCooler();
  delay(2000); // Delay de 2 segundos
}

void LigaIrrigador() {
  if (valorLido < 60) {
    servoirriga.write(180);
  } else {
    servoirriga.write(0);
  }
}

void LigaCooler() {
  if (tempambiente > temperaturaPotenciometro) {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }
}