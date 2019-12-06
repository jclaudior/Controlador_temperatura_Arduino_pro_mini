/* Controlador por temperatura ultilizando modulo display TM1637 e sensor de temperatura DTH22
 Por:João CLaudio RIbeiro
 Contato:
 email:contato@joaoclaudioribeiro.com.br
 Tel:(11)94710-8920
*/

// Include the library:
#include <TM1637Display.h>//BIBLIOTECA MODULO DISPLAY SEGMENTOS
#include <DHT.h>; //BIBLIOTECA DHT22
#include <EEPROM.h>

#define DHTPIN 9 //PINO DIGITAL UTILIZADO PELO DHT22
#define DHTTYPE DHT22 //DEFINE O MODELO DO SENSOR (DHT22 / AM2302)

DHT dht(DHTPIN, DHTTYPE); //PASSA OS PARÂMETROS PARA A FUNÇÃO

// PINO DE COMUNICACÃO DISPLAY 1
#define CLK1 2
#define DIO1 3

// PINO DE COMUNICACÃO DISPLAY 2
#define CLK2 4
#define DIO2 5

// PINO DE COMUNICACÃO DISPLAY 3
#define CLK3 6
#define DIO3 7

// DEFINE OS PINOS DOS RELES
#define rele1 10
#define rele2 11

//Define Pino do botão edição
#define edit A3


//Define Pino do botão decremento
#define decre 12

//Define Pino do botão incremento
#define incre A0

//TEMPERATURA DE GATIHO DOS RELES
int tempAtiva = 0;


//Variavel do Menu
int menu = 0;


// CRIA O OBJETO DYSPLAY
TM1637Display display1 = TM1637Display(CLK1, DIO1);
TM1637Display display2 = TM1637Display(CLK2, DIO2);
TM1637Display display3 = TM1637Display(CLK3, DIO3);

//      -         A
//    |   |     F   B
//      -         G
//    |   |     E   C
//      -         D

// CRIA O SIMBOLO DE CELCIUS:
const uint8_t EDI[] = {
  SEG_A | SEG_F | SEG_E | SEG_G | SEG_D,  // E
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_C | SEG_B,  // D
   SEG_E | SEG_F //I
};

const uint8_t R[] = {
  SEG_A | SEG_F | SEG_E | SEG_G | SEG_B | SEG_C // E
};

const uint8_t CELCIUS[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};

//CRIA O SIMBOLO DE FAHRENHEIT
const uint8_t FAHRENHEIT[] = {                               
  SEG_A | SEG_F | SEG_E | SEG_G,  // F
};

// CRIA O PINO DE UMIDADE
const uint8_t UMIDADE[] = {
  SEG_E | SEG_F | SEG_G | SEG_B | SEG_C  // H
};

void setup() {
  pinMode(rele1,OUTPUT);//DEFINE O PINO DO RELE1 COMO SAIDA
  pinMode(rele2,OUTPUT);//DEFINE O PINO DO RELE2 COMO SAIDA
  pinMode(edit,INPUT_PULLUP);
  pinMode(incre,INPUT_PULLUP);
  pinMode(decre,INPUT_PULLUP);
  tempAtiva = EEPROM.read(1);
  Serial.begin(9600); //INICIALIZA A SERIAL
  dht.begin(); //INICIALIZA A FUNÇÃO
  // AJUSTA LUMINOSIDADE DOS DYSPLAYs NO MAXIMO:
  display1.setBrightness(7);
  display2.setBrightness(7);
  display3.setBrightness(7);
  //LIMPA OS DISPLAYs
  display1.clear();
  display2.clear();
  display3.clear();
  //DELAYDE 1s PARA INICIAR
  delay(1000);
}

void loop() { 
 
  switch(menu){
   case 1:
     //menu de edição
      display1.showNumberDec(tempAtiva, false, 3, 0);//IPRIME A TEMPERATURA EM FAHRENHEIT NO DISPLAY2
      display1.setSegments(FAHRENHEIT, 1, 3);//IMPRIME O SIMBOLO DE FAHRENHEIT NO DISPLAY2
      display2.showNumberDec(tempAtiva, false, 3, 0);//IPRIME A TEMPERATURA EM FAHRENHEIT NO DISPLAY2
      display2.setSegments(FAHRENHEIT, 1, 3);//IMPRIME O SIMBOLO DE FAHRENHEIT NO DISPLAY2
      display3.showNumberDec(tempAtiva, false, 3, 0);//IPRIME A TEMPERATURA EM FAHRENHEIT NO DISPLAY2
      display3.setSegments(FAHRENHEIT, 1, 3);//IMPRIME O SIMBOLO DE FAHRENHEIT NO DISPLAY2
      //se precionado o botão de decremento subtrai 1 da temperatura de ativação
      if(digitalRead(decre)==0){
        if(tempAtiva > 0){
           tempAtiva --;
        }
      }
      //se precionado o botão de incremento adiciona 1 a temperatura de ativação
      if(digitalRead(incre)==0){
        if(tempAtiva < 251){
          tempAtiva ++;
        }
      }
      //se precionado o botão de edição volta para o menu de operação e grava na memoria do arduino a temperatura de ativação
      if(digitalRead(edit)==0){
        if(menu>0){
           //LIMPA OS DISPLAYs
          //display1.clear();
          //display2.clear();
          //display3.clear();
          if(tempAtiva != EEPROM.read(1)){
            EEPROM.write(1,tempAtiva);
          }
          menu --;
        }
      }
   break;

   default:
   //menu de operação
      if(digitalRead(edit)==0){
        //se botão editar for precionado vai para o menu de edição
        if(menu<2){
           //LIMPA OS DISPLAYs
          //display1.clear();
          //display2.clear();
          //display3.clear();
          menu ++;
        }
      }
      float temperature = dht.readTemperature(); //LE A TEMPERATURA EM GRAUS CELCIUS
      int umidade = (int)dht.readHumidity();//LE A UMIDADE
      int fahrenheit = (int)((temperature*9)/5)+32; //CALCULA A TEMPERATURA EM FAHRENHEIT
      int celcius = (int)temperature;//GRAVA A TEMPERATURA EM CELCIUS SEM OS DECIMAIS
  
      display1.showNumberDec(celcius, false, 2, 0);//IMPRIME A TEMPERATURA EM CELCIUS NO DISPLAY1
      display1.setSegments(CELCIUS, 2, 2);//IMPRIME O SIMBOLO DE CELCIUS NO DISPLAY1
      display2.showNumberDec(fahrenheit, false, 3, 0);//IPRIME A TEMPERATURA EM FAHRENHEIT NO DISPLAY2
      display2.setSegments(FAHRENHEIT, 1, 3);//IMPRIME O SIMBOLO DE FAHRENHEIT NO DISPLAY2
      display3.showNumberDec(umidade, false, 3, 0);//IMPRIME A UMIDADE NO DISPLAY3
      display3.setSegments(UMIDADE, 1, 3);//IMPRIME O SIMBOLO DE HUMIDADE NO DISPLAY3

      //SE A TEMPERATURA ATUAL EM FAHRENHEIT FOR IGUAL OU MENOR A TempAtiva - 2 ATIVA O RELE1 SE A TEMPERATURA FOR IGUAL TempAtiva ou maior DESATIVA RELE1
      if(fahrenheit <= (tempAtiva - 2)){
        digitalWrite(rele1,1);
      }else if(fahrenheit >= tempAtiva){
        digitalWrite(rele1,0);
      }
      //SE A TEMPERATURA ATUAL EM FAHRENHEIT FOR IGUAL OU MENOR A TempAtiva - 6 ATIVA O RELE2 SE NÃO MANTEM ELE DESLIGADO
   
      if(fahrenheit <= (tempAtiva-6)){
        digitalWrite(rele2,1);
      }else{
        digitalWrite(rele2,0);
      }

      //DELAY DE 1s DE EXECUÇÃO
      
   break;   
  }
  delay(1000);
  




}
