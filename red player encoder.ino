//encoder
// red player   shoot with key
// Created By Babak Asheri... Gmail: Babak.asheri@gmail.com

#include <digitalWriteFast.h>
#define InterEncoder1 0
#define EncoderPinA 2
#define EncoderPinB 4

#define encoder1

#define motor1A 5
#define motor1B 6
#define motor1_PWM 3

volatile bool EncoderSet1;
volatile long ticksEncoder1 = 0;

int motorpin3 = 9;  
int EnB = 11;
int motorpin4 = 10;

int incomingByte = 0;
boolean flag1 = 0;
boolean flag3 = 0;
boolean flag2 = 0;
boolean flagEn = 0;
boolean flag_persist = 1;
unsigned long time_flag3;
boolean lr_off = 0;

void setup(void) {
Serial.begin(115200);

pinMode(EncoderPinA, INPUT);
digitalWrite(EncoderPinA,LOW);
pinMode(EncoderPinB, INPUT);
digitalWrite(EncoderPinB,LOW);
attachInterrupt(InterEncoder1, Encoder1A, RISING);
pinMode(motor1A,OUTPUT);
pinMode(motor1B,OUTPUT);
pinMode(motor1_PWM,OUTPUT);
pinMode(motorpin3, OUTPUT);
pinMode (EnB,OUTPUT );
pinMode (motorpin4,OUTPUT);
pinMode(7,INPUT);   
pinMode(8,INPUT);
pinMode(12,INPUT);      // doctor's key
}

void loop(void) {
int left_red = digitalRead(7);
int right_red = digitalRead(8);
if (left_red == HIGH && lr_off == 0 && ticksEncoder1 == 0)
{ 
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    
    out(0,255,255);
    delay(10);
    out(0,0,0);
  }
if (left_red == LOW )
{
    out(0,0,0);
  }
if (right_red == HIGH && lr_off == 0 && ticksEncoder1 == 0)
{
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    
  out(255,255,0);
  delay(10);
  out(0,0,0);
  }
if (right_red == LOW )
{ 
  out(0,0,0);
  }

if (right_red == HIGH && left_red == HIGH)
{ 
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    out(0,0,0);
    delay(1000);
  }

int key = digitalRead(12);     //doctor's shoot
if (flag_persist == 1 && left_red == LOW && right_red == LOW)
{ delay(50);
  if (ticksEncoder1> 0)
  {
    analogWrite(motor1_PWM , 100);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B, LOW);
    delay(1);
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    }
    else if (ticksEncoder1 <0)
    {
      analogWrite(motor1_PWM , 100);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, HIGH);
      delay(1);
      analogWrite(motor1_PWM , 0);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, LOW);
      }
  }
  
if (key == HIGH)
{
  lr_off = 1;
  time_flag3 = 0;
  flag3 = 0;
  flag_persist = 0;
  delay(1000);
  if (flagEn == 0)
  {
    // mire aghab , ok
    for (int i=0;i<70; i = i+5){
      analogWrite(motor1_PWM , i);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, HIGH);
      }       
    
    flagEn = 1;
    }
  }
 
if (ticksEncoder1 >= 25)
{  
 
  if (flagEn == 1 && flag2 == 0 && flag3 == 0)
  {   // shoot
    shoot();   
    flagEn = 0;
    flag2 = 1;
    }
  }
//----------------------------------------------------
if (ticksEncoder1 < -50)
{
  if (flagEn == 0 && flag2 == 1 && flag3 == 0 )
  {    //come back
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    delay(200);
      for (int bi=0;bi<70; bi = bi+5){
        analogWrite(motor1_PWM , bi);
        digitalWrite(motor1A,LOW);
        digitalWrite(motor1B, HIGH);
      }   
    flag2 = 0;
    flagEn = 0;
    flag3 = 1;
    }
  }
//----------------------------------------------------
if (ticksEncoder1 >= -10)
{
  if (flagEn == 0 && flag2 == 0 && flag3 == 1)
  {
    analogWrite(motor1_PWM , 255);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B, HIGH);
    delay(100);
    flag2 = 0;
    flagEn = 0;
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    }
  }
//----------------------------------------------------
if (flag3 == 1)
{
  if (ticksEncoder1> 0)
  {
    analogWrite(motor1_PWM , 100);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B, LOW);
    delay(1);
    analogWrite(motor1_PWM , 255);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);    
    }
    
    else if (ticksEncoder1 <0)
    {   
      analogWrite(motor1_PWM , 100);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, HIGH);
      delay(1);
      analogWrite(motor1_PWM , 255);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, LOW);
//      delay(20);
      }
        
  if (flag3 == 1 && ticksEncoder1 == 0)
  {
    time_flag3 = millis();
    if(time_flag3 > 750)
    {
      lr_off = 0;
      flag3 = 0;
      time_flag3 = 0;
      flag_persist = 1;
      }else
      {
        time_flag3 = 0;
        flag_persist = 1;
        }
      }
    }
Serial.print("Encoder 1 :  ");
Serial.println (ticksEncoder1);
  }

void Encoder1A()
{
EncoderSet1 = digitalReadFast(EncoderPinB);
#ifdef encoder1
ticksEncoder1 -= EncoderSet1 ? -1 : +1 ;
#else
ticksEncoder1 += EncoderSet1 ? -1 : +1 ;
#endif
  }

void out (int motorpin1val , int EnAval , int motorpin2val) 
{
  analogWrite (motorpin3 , motorpin1val);
  analogWrite (EnB , EnAval);
  analogWrite (motorpin4 , motorpin2val);
  }

void shoot(){
  analogWrite(motor1_PWM , 0);
  digitalWrite(motor1A,LOW);
  digitalWrite(motor1B, LOW);
  delay(200);

    for (int f=0;f <= 175; f = f+3){
      analogWrite(motor1_PWM , f);
      digitalWrite(motor1A,HIGH);
      digitalWrite(motor1B, LOW);
      } 
  }
