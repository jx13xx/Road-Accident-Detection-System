#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet2.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp2.h>
#include <Twitter.h>
#include <util.h>
#include <SPI.h>
#include <LiquidCrystal.h>  
#include <Servo.h>

int pos =0;
Servo myservo;

int Contrast=0; 
int rs = 7, enable = 6 , d4 = 5, d5 = 4 , d6 = 3 , d7 = 2;
LiquidCrystal lcd(rs, enable, d4,d5,d6,d7);

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
 


String outputState1 = "off";
String header;

int serv_motr=10;
int counter_rest = 11;

void setup() {
  Serial.begin(9600);
  analogWrite(8,Contrast);
  lcd.begin(16,2);
  myservo.attach(9);

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  pinMode(counter_rest,OUTPUT);

  pinMode(serv_motr,OUTPUT);

}


void receive(){
  digitalWrite(counter_rest,HIGH);
  EthernetClient client = server.available();
  
  
  int q0 = digitalRead(A0);
  Serial.println(q0);
  int q1 = digitalRead(A1);
  Serial.println(q1);
  int road = digitalRead(A3);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println("Refresh: 3");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE HTML");
  client.println("<html>");
  
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<title> State Machine </title>");
  client.println("<body>");

  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #77878A;}</style></head>");

  
  

  


   
  if(q0 == LOW && q1 == LOW){
    if(road == HIGH){
      digitalWrite(serv_motr,HIGH);
      
      lcd.setCursor(0,0);
      lcd.print("   Road Closed  ");
      lcd.setCursor(0,1);
      lcd.print("  Acident Ahead   ");

      int pos = 180;
      myservo.write(pos);
      delay(15);

      outputState1 = "Closed";
      

      
      client.println("<body><center><h1> Road is Closed </center> </h1>");
      client.println("<p>Road:" + outputState1 +"</p>");


      digitalWrite(serv_motr,LOW);
     
      
      

    
     }

   else{
    digitalWrite(serv_motr,HIGH);
    
    lcd.setCursor(0,0);
    lcd.write("  Road is Clear ");
    lcd.setCursor(0,1);
    lcd.write("  Drive Safely  ");

    int pos =0;
    myservo.write(pos);
    delay(15);

    outputState1 = "Opened";

    client.println("<body><center><h1> Road is Clear </center> </h1>");
    client.println("<p>Road:" + outputState1 +"</p>");

    digitalWrite(serv_motr,LOW);

    
    
    
    
  }
 }
  
  if( q0 == LOW && q1 == HIGH){
    if( road == HIGH){
      digitalWrite(serv_motr,HIGH);
      lcd.setCursor(0,0);
      lcd.print("   Road Closed  ");
      lcd.setCursor(0,1);
      lcd.print("  Acident Ahead   ");

      int pos = 180;
      myservo.write(pos);
      delay(15);
      outputState1 = "Closed";
      
      client.println("<body><center><h1> Road is Closed </center> </h1>");
      client.println("<p>Road:" + outputState1 +"</p>");

      digitalWrite(serv_motr,LOW);
      
      
     }

    else{
      digitalWrite(serv_motr,HIGH);
      
      lcd.setCursor(0,0);
      lcd.print(" Accidents Ahead");
      lcd.setCursor(0,1);
      lcd.print(" Advise Caution");
    
      int pos =0;
      myservo.write(pos);
      delay(15);

      outputState1 = "Opened";

      client.println("<body><center><h1> Accident Ahead </center> </h1>");
      client.println("<p>Road:" + outputState1 +"</p>");

      digitalWrite(serv_motr,LOW);
      
      
    }
  }
 
 if( q0 == HIGH && q1 == LOW){
    if( road == HIGH){

      digitalWrite(serv_motr,HIGH);
      
      lcd.setCursor(0,0);
      lcd.print("   Road Closed  ");
      lcd.setCursor(0,1);
      lcd.print("  Acident Ahead   ");

      int pos = 180;
      myservo.write(pos);
      delay(15);

      outputState1 = "Closed";

      
      client.println("<body><center><h1> Road is Closed </center> </h1>");
      client.println("<p>Road:" + outputState1 +"</p>");

      digitalWrite(serv_motr,LOW);
      
     }
    else{
      digitalWrite(serv_motr,HIGH);
      lcd.setCursor(0,0);
      lcd.print("  Reduce Speed  ");
      lcd.setCursor(0,1);
      lcd.print("      Ahead    ");

      int pos = 90;
      myservo.write(pos);
      delay(15);

      outputState1 = "Opened";

      
      client.println("<body><center><h1> Reduce Speed </center> </h1>");
      client.println("<p>Road:" + outputState1 +"</p>");

      digitalWrite(serv_motr,LOW);
      
      
    }
 }
 
 if ( q0 == HIGH && q1 == HIGH ){

    digitalWrite(serv_motr,HIGH);

   client.println("<h1> Warning! Dectected Major Accidents on Road </h1>");
   client.println("<p><a href=\"/5/on\"><button class=\"button\">Open</button></a></p>");
   client.println("<p><a href=\"/5/on\"><button class=\"button\">Close</button></a></p>");

   client.println("<p>Road:" + outputState1 +"</p>");
   
  client.println("<button class=\"button\"ON </button>");
  if(header.indexOf("Opened") >= 0) {
    Serial.println("Open");
    utputState1 = "Closed";
  else if (header.indexOf("Closed") >= 0){
    Serial.println("Close");
    outputState1 = "Open";

    

  
  }

  
   

   
  if(outputState1=="Close"){
    client.println("<p><a href=\"/Open\"><button class=\"button\">Open/Close</button></a></p>");
    
   }
   if(outputState1=="Open"){
    client.println("<p><a href=\"/Close\"><button class=\"button button2\">OpenRoad</button></a></p>");
   }
   
  
 
      digitalWrite(serv_motr,LOW);
  
   
 }
client.println("</html>");
client.stop();

digitalWrite(counter_rest, LOW);

}





void loop(){
  receive();
  

}
