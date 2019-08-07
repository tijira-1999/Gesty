#include <SPI.h>      //SPI library for communicate with the nRF24L01+
#include "RF24.h"     //The main library of the nRF24L01+
#include<Servo.h>

Servo n;
int a = 3;

//const int IN1 = A0;    //Right Motor (-)
//const int IN2 = A1;    //Right Motor (+)

//Define packet for the direction (X axis and Y axis)
int data[2];

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
RF24 radio(7,8);

//Create a pipe addresses for the communicate
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0,OUTPUT);
  pinMode(a,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  n.attach(9);
  n.write(83);
  delay(1000);

  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);

  radio.begin();                    //Start the nRF24 communicate            
  radio.openReadingPipe(1, pipe);   //Sets the address of the transmitter to which the program will receive data.
  //radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available())
    {
      radio.read(data, sizeof(data));
          if(data[0] > 380)
          { //right

            //leds
            digitalWrite(5,LOW);
            digitalWrite(6,LOW); 
            digitalWrite(2,LOW);
            digitalWrite(4,HIGH);
            // motor pins              
            digitalWrite(A1,LOW);
            digitalWrite(A0,HIGH);
            analogWrite(a,255);
            delay(20);
            //servo
            n.write(97);
            digitalWrite(4,LOW);
            delay(20);
          }
      
          if(data[0] < 312)
          { //left

            //leds
            digitalWrite(5,LOW);
            digitalWrite(4,LOW);
            digitalWrite(6,LOW);
            digitalWrite(2,HIGH);
            //motor pins
            digitalWrite(A1,LOW);
            digitalWrite(A0,HIGH);
            analogWrite(a,255);
            delay(20);
            //servo
            n.write(70);
            digitalWrite(2,LOW);
            delay(20);
          }
       
          if(data[1] < 120)
          { //backward 

            //leds
            digitalWrite(4,LOW);
            digitalWrite(5,LOW);
            digitalWrite(2,LOW);
            digitalWrite(6,HIGH);   
            //motor pins      
            digitalWrite(A0,LOW);
            digitalWrite(A1,HIGH);
            analogWrite(a,255);
            delay(10);
      }
  
      if(data[1] > 190)
      { //forward

        //leds
        digitalWrite(4,LOW);
        digitalWrite(6,LOW);
        digitalWrite(2,LOW);
        digitalWrite(5,HIGH);
        //motor pins             
        digitalWrite(A1,LOW);
        digitalWrite(A0,HIGH);
        analogWrite(a,255);
        delay(10);
      }
  
      if(data[0] > 312 && data[0] < 380 && data[1] > 120 && data[1] < 190)
      { //stop car

        //middle position
        analogWrite(a, 0);
        n.write(83);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(2,LOW);      
      }
    }
    
    else
    { // in case of signal loss stop car
      n.write(83);
      analogWrite(a, 0); 
    }
}
