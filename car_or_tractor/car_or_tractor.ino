//Developed by Claudi Martins de Freitas // Desenvolvido por Claudi Martins de Freitas
//It's a open source project, enjoy it! // É um projeto Open Source, divirta-se
//The comments are in portuguese, sorry about that
//This is a project to remote control car or remote control tractor with wheels //  é um projeto para carrinho de controle remoto ou trator de controle remoto com rodas
//01/11/2016

#define BAUD           (9600)  // VELOCIDADE DE TRANSMISSÃO 9600bps.
#define BUFFER        (64)  //NÚMERO LIMITE DE CARACTERES RECEBÍVEIS POR TRANSMISSÃO
// VARIÁVEIS

char buffer[BUFFER];  // VARIÁVEL DE ARMAZENAMENTO DOS CARACTERES
int sofar;  // TREM DE PULSO
int leitura_zero=0; //VERIFICAR A LEITURA DA SERIAL 1
int disparo=150;  //TEMPO DE DISPARO DIREÇÃO
int disparo2=50;  //TEMPO DE DISPARO CARREGADEIRA
int disparo3=150; //TEMPO DE DISPARO CONJUNTO SUBIDA
int disparo4=30;  //TEMPO DE DISPARO CONJUNTO DESCIDA

//NUMERAÇÃO DAS PORTAS
const int frente=2; // CONSTANTE FRENTE É O PINO DIGITAL 2
const int tras=3; // CONSTANTE TRÁS É O PINO DIGITAL 3
const int esquerda=4; // CONSTANTE ESQUERDA É O PINO DIGITAL 4
const int direita=5;  // CONSTANTE DIREITA É O PINO DIGITAL 5
const int sobe_carregadeira=6;  // CONSTANTE SOBE CARREGADEIRA É O PINO DIGITAL 6
const int desce_carregadeira=7; // CONSTANTE DESCE CARREGADEIRA É O PINO DIGITAL 7
const int sobe_conjunto=8;  // CONSTANTE SOBE CONJUNTO É O PINO DIGITAL 8
const int desce_conjunto=9; // CONSTANTE DESCE CONJUNTO É O PINO DIGITAL 9

//ROTINA
float parsenumber(char code,float val) {//ROTINA PARA DESMEMBRAR CÓGIDO DA SERIAL
  char *ptr=buffer;
  while(ptr && *ptr && ptr<buffer+sofar) {
    if(*ptr==code) {
      return atof(ptr+1);
    }
    ptr=strchr(ptr,' ')+1;
  }
  return val;
}

//EXIBIR MENSAGEM NA SERIAL
void help() {
  Serial.println(F("Mensagem "));
}

//PREPARA A ENTRADA PARA RECEBER AS MENSAGENS DA SERIAL
void ready() {
  sofar=0;  // LIMPAR A ENTRADA DO BUFFER
}

//ETAPA DE SETUP, FAZ QUANDO INICIA
void setup() {
  Serial.begin(BAUD);  // ABRI A COMUNICAÇÃO
  help();  // CHAMA O NOME DA ROTINA
  Serial.flush();
  ready();

  //PREPARAR ENTRADAS OU SAÍDAS
  pinMode(frente, OUTPUT);  //FRENTE
  pinMode(tras, OUTPUT);  //TRÁS
  pinMode(esquerda, OUTPUT);  //ESQUERDA
  pinMode(direita, OUTPUT); //DIREITA
  pinMode(sobe_carregadeira, OUTPUT); //SOBE CARREGADEIRA
  pinMode(desce_carregadeira, OUTPUT);  //DESCE CARREGADEIRA
  pinMode(sobe_conjunto, OUTPUT); //SOBE CONJUNTO
  pinMode(desce_conjunto, OUTPUT);  //DESCE CONJUNTO
}

//LAÇO DE REPETIÇÃO
void loop() {
  // LISTAGEM DE COMANDOS DA SERIAL

  if(Serial.available() > 0) {

  while(Serial.available() > 0) {  //HAVENDO ALGO A RECEBER
    char c=Serial.read();  //PEGAR VALOR RECEBIDO
    Serial.print(c);  //IMPRIMIR A MENSAGEM RECEBIDA
    if(sofar<BUFFER) buffer[sofar++]=c;  // FECHA PACOTE DE MENSAGEM
    if(buffer[sofar-1]==';') break;  //MENSAGEM COMPLETA
  }
  }
  else
  {
   //NADA RECEBIDO
   leitura_zero = leitura_zero + 1;
  }

  if(sofar>0 && buffer[sofar-1]==';') {
    //DIVIDE A MENSAGEM
    buffer[sofar]=0;  //TERMINA O BUFFER PARA AS LETRAS FUNCIONAREM CORRETAMENTE
    Serial.print(F("\r\n"));  //REPLICA AO USUÁRIO
    painel_de_comando();  // SELECIONA A ROTINA DO COMANDO DESEJADO
    ready();
  }
  if(leitura_zero >= 3) {
  modulo(0);
  }
}

//Leia os comandos de input do Buffer e reconhece qualquer dos comandos separando os por linha.
void painel_de_comando() {

  int cmd = parsenumber('J',-1);

  switch(cmd) {
  case  0:
  case  1: { // Frente
    //Serial.println("Frente");
    modulo(1);
    delay(disparo);
    break;
    }
  case  2: { // TRÁS
    //Serial.println("Trás");
    modulo(2);
    delay(disparo);
    break;
    }
  case  3: { // ESQUERDA
    //Serial.println("Esquerda");
    modulo(3);
    delay(disparo);
    break;
    }
  case  4: { // DIREITA
    //Serial.println("Direita");
    modulo(4);
    delay(disparo);
    break;
    }
  case  5: { // FRENTE + DIREITA
    //Serial.println("Frente + Direita");
    modulo(5);
    delay(disparo);
    break;
    }
  case  6: { // FRENTE + ESQUERDA
    //Serial.println("Frente + Esquerda");
    modulo(6);
    delay(disparo);
    break;
    }
  case  7: { // TRÁS + DIREITA
    Serial.println("Trás + Esquerda");
    modulo(7);
    delay(disparo);
    break;
    }
  case  8: { // TRÁS + ESQUERDA
    //Serial.println("Trás + Esquerda");
    modulo(8);
    delay(disparo);
    break;
    }
  }

  cmd = parsenumber('C',-1);

  switch(cmd) {
  case  1: { // SOBE CARREGADEIRA
    //Serial.println("Sobe carregadeira");
    modulo(9);
    delay(disparo2);
    break;
    };
  case  2: { // DESCE CARREGADEIRA
    //Serial.println("Desce carregadeira");
    modulo(10);
    delay(disparo2);
    break;
    };
  case  3: { // SOBE CONJUNTO
    //Serial.println("Desce conjunto");
    modulo(11);
    delay(disparo3);
    break;
    };
  case  4: { // DESCE CONJUNTO
    //Serial.println("Desce conjunto");
    modulo(12);
    delay(disparo4);
    break;
    };
  }

}

//MÓDULO ACIONADO DENTRO DO LOOP RESPONSÁVEL PELOS ACIONAMENTOS
void modulo(int x)
{
    // 0 = DESLIGAR TUDO
    if(x == 0)
     {
        //DESLIGA TODAS AS PORTAS POR SEGURANÇA QUANDO NÃO HOUVER COMANDO
        digitalWrite(frente, HIGH); //DESLIGA FRENTE
        digitalWrite(tras, HIGH); //DESLIGA TRÁS
        digitalWrite(esquerda, HIGH); //DESLIGA ESQUERDA
        digitalWrite(direita, HIGH);  //DESLIGA DIREITA
        digitalWrite(sobe_carregadeira, HIGH);  //DESLIGA SOBE_CARREGADEIRA
        digitalWrite(desce_carregadeira, HIGH); //DESLIGA DESCE_CARREGADEIRA
        digitalWrite(sobe_conjunto, HIGH);  //DESLIGA SOBE_CONJUNTO
        digitalWrite(desce_conjunto, HIGH); //DESLIGA DESCE_CONJUNTO
     }
  //1 = FRENTE
  else if(x == 1)
  {
        digitalWrite(tras, HIGH); //DESLIGA TRÁS
        digitalWrite(frente, LOW);  //LIGA FRENTE
  }
  //2 = TRÁS
  else if(x == 2)
  {
        digitalWrite(frente, HIGH); //DESLIGA FRENTE
        digitalWrite(tras, LOW);  //LIGA TRÁS
  }
  //3 = ESQUERDA
  else if(x == 3)
  {
        digitalWrite(direita, HIGH);  //DESLIGA DIREITA
        digitalWrite(esquerda, LOW);  //LIGA ESQUERDA
  }
  //4 = DIREITA
  else if(x == 4)
  {
        digitalWrite(esquerda, HIGH); //DESLIGA ESQUERDA
        digitalWrite(direita, LOW); //LIGA DIREITA
  }
  //5 = FRENTE + DIREITA
  else if(x == 5)
  {
        digitalWrite(tras, HIGH); //DESLIGA TRÁS
        digitalWrite(esquerda, HIGH); //DESLIGA ESQUERDA
        digitalWrite(frente, LOW);  //LIGA FRENTE
        digitalWrite(direita, LOW); //LIGA DIREITA
  }
  //6 = FRENTE + ESQUERDA
  else if(x == 6)
  {
        digitalWrite(frente, HIGH); //DESLIGA FRENTE
        digitalWrite(direita, HIGH);  //DESLIGA DIREITA
        digitalWrite(frente, LOW);  //LIGA FRENTE
        digitalWrite(esquerda, LOW);  //LIGA ESQUERDA
  }
  //7 = TRÁS + DIREITA
  else if(x == 7)
  {
        digitalWrite(frente, HIGH); //DESLIGA FRENTE
        digitalWrite(esquerda, HIGH); //LIGA ESQUERDA
        digitalWrite(tras, LOW);  //LIGA TRÁS
        digitalWrite(direita, LOW); //DESLIGA DIREITA
  }
  //8 = TRÁS + ESQUERDA
  else if(x == 8)
  {
        digitalWrite(frente, HIGH); //DESLIGA FRENTE
        digitalWrite(direita, HIGH);  //DESLIGA DIREITA
        digitalWrite(tras, LOW);  //LIGA TRÁS
        digitalWrite(esquerda, LOW);  //LIGA ESQUERDA
  }
  //9 = SOBE CARREGADEIRA
  else if(x == 9)
  {
        digitalWrite(desce_carregadeira, HIGH); //DESLIGA DESCE_CARREGADEIRA
        digitalWrite(sobe_carregadeira, LOW); //LIGA SOBE_CARREGADEIRA
  }
  //10 = DESCE CARREGADEIRA
  else if(x == 10)
  {
        digitalWrite(sobe_carregadeira, HIGH);  //DESLIGA SOBE_CARREGADEIRA
        digitalWrite(desce_carregadeira, LOW);  //LIGA DESCE_CARREGADEIRA
  }
  //11 = SOBE CONJUNTO
  else if(x == 11)
  {
        digitalWrite(desce_conjunto, HIGH); //DESLIGA DESCE_CONJUNTO
        digitalWrite(sobe_conjunto, LOW); //LIGA SOBE_CONJUNTO
  }
  //12 = DESCE CONJUNTO
  else if(x == 12)
  {
        digitalWrite(sobe_conjunto, HIGH);  //LIGA SOBE_CONJUNTO
        digitalWrite(desce_conjunto, LOW);  //DESLIGA DESCE_CONJUNTO
  }
   imprime o caracter enviado na serial (Para teste)
   Serial.println(x);
}
