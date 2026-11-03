// #include <Servo.h>

// Servo goleiro;
// const int pinoJoystick = A2;
// const int pinoServo = 9;

// void setup() {
//   goleiro.attach(pinoServo);
  
//   // Inicia o jogo com o goleiro perfeitamente em pÃ© (centro)
//   goleiro.write(90); 
//   delay(500); // DÃ¡ meio segundo para estabilizar
// }

// void loop() {
//   // LÃª a posiÃ§Ã£o do polegar no joystick
//   int valorJoystick = analogRead(pinoJoystick);
  
//   // Mapeia o joystick para a amplitude segura do servo (15 a 165 graus)
//   int anguloServo = map(valorJoystick, 0, 1023, 15, 165);
  
//   // Executa o movimento
//   goleiro.write(anguloServo);
  
//   // Atraso mÃ­nimo para o movimento ficar fluido
//   delay(15);
// }

// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <Servo.h>

// const int COLUNAS = 20;
// const int LINHAS = 4;
// int antes = HIGH;
// int pinoBotao = 10;
// int rele = 3;

// LiquidCrystal_I2C lcd(0x27, COLUNAS, LINHAS);

// const int BTN_TIME1 = 8;
// const int BTN_TIME2 = 9;
// const int PONTOS_PARA_VENCER = 3;
// int placar1 = 0;
// int placar2 = 0;

// //GOLEIRO (SERVO)
// Servo goleiro;
// const int pinoServo = 11;
// const int botaoEsquerda = 12;
// const int botaoDireita = 13;

// int anguloAtual = 90;  // posição em que o servo está agora
// int anguloAlvo = 90;   // posição para onde ele deve ir
// unsigned long ultimoPassoServo = 0;

// //FIM DE JOGO
// bool fimDeJogo = false;
// int telaFim = 0;
// unsigned long ultimaTroca = 0;

// //Debounce
// bool ultimoEstado1 = HIGH;
// bool ultimoEstado2 = HIGH;

// // Desenha a moldura e limpa o interior
// void moldura() {
//   lcd.clear();
//   for (int c = 0; c < COLUNAS; c++) {
//     lcd.setCursor(c, 0);
//     lcd.print('-');
//     lcd.setCursor(c, LINHAS - 1);
//     lcd.print('-');
//   }
//   for (int l = 1; l < LINHAS - 1; l++) {
//     lcd.setCursor(0, l);
//     lcd.print('|');
//     lcd.setCursor(COLUNAS - 1, l);
//     lcd.print('|');
//   }
// }

// // Centraliza o texto dentro da moldura
// void centralizar(String texto, int linha) {
//   int largura = COLUNAS - 2; // area util, sem as bordas
//   if (texto.length() > largura) {
//     texto = texto.substring(0, largura);
//   }
//   int inicio = 1 + (largura - texto.length()) / 2;
//   lcd.setCursor(inicio, linha);
//   lcd.print(texto);
// }

// String textoPlacar() {
//   return String(placar1) + " x " + String(placar2);
// }

// void mostrarAbertura() {
//   moldura();
//   centralizar("Penalidade", 1);
//   centralizar("MaCInMa", 2);
//   delay(3000);

//   // Contagem regressiva: meio segundo cada numero
//   for (int i = 3; i >= 1; i--) {
//     moldura();
//     centralizar("COMECA EM", 1);
//     centralizar(String(i), 2);
//     delay(500);
//   }
// }

// void mostrarPlacar() {
//   moldura();
//   centralizar("Placar:", 1);
//   centralizar(textoPlacar(), 2);
// }

// void mostrarGol() {
//   moldura();
//   centralizar("GOOOL!", 1);
//   centralizar(textoPlacar(), 2);
//   delay(1500);
//   mostrarPlacar();
// }

// void mostrarDefesa() {
//   moldura();
//   centralizar("DEFESAAA!", 1);
//   centralizar(textoPlacar(), 2);
//   delay(1500);
//   mostrarPlacar();
// }

// void mostrarTelaFim(int tela) {
//   moldura();
//   if (tela == 0) {
//     centralizar("PARTIDA", 1);
//     centralizar("ENCERRADA", 2);
//   } else if (tela == 1) {
//     centralizar("PLACAR FINAL", 1);
//     centralizar(textoPlacar(), 2);
//   } else {
//     centralizar("VENCEDOR:", 1);
//     if (placar1 > placar2) {
//       centralizar("ATACANTE", 2);
//     } else {
//       centralizar("GOLEIRO", 2);
//     }
//   }
// }

// void verificarVitoria() {
//   if (placar1 >= PONTOS_PARA_VENCER || placar2 >= PONTOS_PARA_VENCER) {
//     fimDeJogo = true;
//     telaFim = 0;
//     ultimaTroca = millis() - 2000; // forca a primeira tela aparecer logo
//   }
// }

// void setup() {
//   lcd.init();
//   lcd.backlight();

//   pinMode(BTN_TIME1, INPUT_PULLUP);
//   pinMode(BTN_TIME2, INPUT_PULLUP);

//   // Servo do goleiro
//   goleiro.attach(pinoServo);
//   pinMode(botaoEsquerda, INPUT_PULLUP);
//   pinMode(botaoDireita, INPUT_PULLUP);
//   goleiro.write(90);  // começa em pé (centro)

//   mostrarAbertura();
//   mostrarPlacar();

//   pinMode(pinoBotao, INPUT_PULLUP);
//   pinMode(rele, OUTPUT);
//   digitalWrite(rele, HIGH);

// }

// void loop() {
//   bool estado1 = digitalRead(BTN_TIME1);
//   bool estado2 = digitalRead(BTN_TIME2);
//   int agora = digitalRead(pinoBotao);

//   // Goleiro: com INPUT_PULLUP, o botão apertado lê LOW
//   if (digitalRead(botaoEsquerda) == LOW) {
//     anguloAlvo = 10;    // cai 80 graus para a esquerda
//   }
//   else if (digitalRead(botaoDireita) == LOW) {
//     anguloAlvo = 170;   // cai 80 graus para a direita
//   }
//   else {
//     anguloAlvo = 90;    // nenhum botão apertado: volta em pé
//   }

//   // A cada 5 ms anda 1 grau até chegar no alvo
//   if (millis() - ultimoPassoServo >= 5) {
//     ultimoPassoServo = millis();

//     if (anguloAtual < anguloAlvo) {
//       anguloAtual = anguloAtual + 1;
//     }
//     else if (anguloAtual > anguloAlvo) {
//       anguloAtual = anguloAtual - 1;
//     }

//     goleiro.write(anguloAtual);
//   }

//   // Botão 1: ponto do time 1 (ignorado se a partida acabou)
//   if (estado1 == LOW && ultimoEstado1 == HIGH && !fimDeJogo) {
//     placar1++;
//     mostrarGol();
//     verificarVitoria();
//     delay(50);
//   }

//   // Botão 2: ponto do time 2 (ignorado se a partida acabou)
//   if (estado2 == LOW && ultimoEstado2 == HIGH && !fimDeJogo) {
//     placar2++;
//     mostrarDefesa();
//     verificarVitoria();
//     delay(50);
//   }

//   // Partida encerrada: alterna as telas a cada 2 segundos
//   if (fimDeJogo && millis() - ultimaTroca >= 2000) {
//     mostrarTelaFim(telaFim);
//     telaFim = (telaFim + 1) % 3;
//     ultimaTroca = millis();
//   }

//   ultimoEstado1 = estado1;
//   ultimoEstado2 = estado2;

//   if (agora == LOW && antes == HIGH){
//     digitalWrite(rele, LOW);
//     delay(70);
//     digitalWrite(rele, HIGH);
//   }

//   antes = agora;
//   delay(20);

// }

