// Definição de constantes para identificar em quais "portas" (pinos) os sensores estão conectados
int pinoUmidade = A4; 
int pinoLuz = A0;      
int pinoTemp = A1;    

// Definição dos pinos para os componentes que executam ações (saídas)
int motorPin = 7;   // Saída digital que enviará sinal para a base do transistor
int ledPin = 13;    // Saída digital para ligar/desligar o LED

// Espaços na memória para armazenar os números lidos pelos sensores
int umidade = 0;
int luz = 0;
float temperatura = 0; // "float" permite números com vírgula (decimal)

void setup() {
  // Inicializa a comunicação entre o Arduino e o computador (para ver os dados na tela)
  Serial.begin(9600);
  
  // Configura os pinos dos atuadores para agirem como saídas de energia
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  // --- LEITURA E ARMAZENAMENTO ---
  
  // Lê o valor elétrico (0 a 1023) do sensor de umidade e guarda na variável
  umidade = analogRead(pinoUmidade);
  
  // Lê o valor do sensor de temperatura e armazena temporariamente
  int leituraTemp = analogRead(pinoTemp);
  
  // Lê a intensidade de luz e guarda na variável
  int luz = analogRead(pinoLuz);
  
  // --- LÓGICA DA LUZ ---

  // Estrutura de decisão: se o valor da luz for menor que o limite (escuro)...
  if (luz < 500) {  
    digitalWrite(ledPin, HIGH); // ...envia 5V para o pino 13 (liga o LED)
  } else {
    digitalWrite(ledPin, LOW);  // ...caso contrário, corta a energia (desliga o LED)
  }

  // Pequena pausa para estabilizar as leituras do processador
  delay(500);

  // --- CÁLCULO MATEMÁTICO ---

  // Converte o valor binário (0-1023) em voltagem real (0V-5V)
  float tensao = leituraTemp * (5.0 / 1023.0);
  // Converte a voltagem em graus Celsius baseando-se na escala do sensor TMP36
  temperatura = (tensao - 0.5) * 100;

  // --- SAÍDA DE DADOS (MONITOR SERIAL) ---

  // Exibe os nomes e valores das variáveis formatados na tela do computador
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print(" | Luz: ");
  Serial.print(luz);
  Serial.print(" | Temp: ");
  Serial.println(temperatura);

  // --- LÓGICA DO MOTOR (IRRIGAÇÃO) ---

  // Cria um "estado" lógico começando como Falso (motor desligado por padrão)
  bool ligarMotor = false;

  // Verifica condição 1: Se a umidade estiver abaixo do limite (solo seco)
  if (umidade < 400) {
    ligarMotor = true; // Muda o estado para Verdadeiro
  }

  // Verifica condição 2: Se a temperatura estiver acima do limite (muito quente)
  if (temperatura > 30) {
    ligarMotor = true; // Muda o estado para Verdadeiro (independente da umidade)
  }

  // --- EXECUÇÃO FINAL ---

  // Se qualquer uma das condições acima resultou em 'true' (Verdadeiro)...
  if (ligarMotor) {
    digitalWrite(motorPin, HIGH); // ...ativa o pino 7 (o motor gira)
  } else {
    digitalWrite(motorPin, LOW);  // ...desativa o pino 7 (o motor para)
  }

  // Espera 1 segundo antes de recomeçar todo o ciclo de leitura e decisão
  delay(1000); 
}