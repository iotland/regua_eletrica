#include <Ultrasonic.h>
#include <SPI.h>
#include <Ethernet.h>
 
//Define os parametros para o sensor ultrasonico HC-SR04
#define PINO_TRIGGER  6 //Porta ligada ao pino Trigger do sensor
#define PINO_ECHO     7 //Porta ligada ao pino Echo do sensor
//Inicializa o sensor ultrasonico
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);
 
//Definicoes de IP, mascara de rede e gateway
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,88);          //Aqui voce devera colocar um IP que esteja disponivel na sua rede 
 
//Inicializa o servidor web na porta 80
EthernetServer server(80);
 
void setup()
{
  //Inicializa a interface de rede
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}
 
void loop() {
  float cmMsec;
  long microsec = ultrasonic.timing();
  //Le e armazena as informacoes do sensor ultrasonico
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
 
  //Aguarda conexao do browser
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == 'n' && currentLineIsBlank) {
          //aqui voce define como sera sua pagina, qual vai ser o estilo da sua pagiana 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 2"); //Recarrega a pagina a cada 2seg
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //Configura o texto e imprime o titulo no browser
          client.print("<font color=#FF0000><b><u>");
          client.print("Envio de informacoes pela rede utilizando Arduino");
          client.print("</u></b></font>");
          client.println("<br />");
          client.println("<br />");
          //Mostra o estado da porta digital 3
          int porta_digital = digitalRead(3);
          client.print("Porta Digital 3 : ");
          client.print("<b>");
          client.print(porta_digital);
          client.println("</b>");
          client.print("  (0 = Desligada, 1 = Ligada)");
          client.println("<br />");
          //Mostra as informacoes lidas pelo sensor ultrasonico
          client.print("Sensor Ultrasonico : ");
          client.print("<b>");
          client.print(cmMsec);
          client.print(" cm");
          client.println("</b></html>");
          break;
        }
        if (c == 'n') {
          
          currentLineIsBlank = true;
        } 
        else if (c != 'r') {

          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();
    }
}
