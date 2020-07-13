//encoder
// red player   shoot with key
// Created By Babak Asheri... Gmail: Babak.asheri@gmail.com

#include <digitalWriteFast.h>
#define InterEncoder1 0
#define EncoderPinA 2
#define EncoderPinB 4
#define encoder1
#define motor1A 3
#define motor1B 6
#define motor1_PWM 5

volatile bool EncoderSet1;
volatile long ticksEncoder1 = 0;

int motorpin3 = 9;  
int EnB = 10;
int motorpin4 = 11;
int incomingByte = 0;
boolean flag1 = 0;
boolean flag3 = 0;
boolean flag2 = 0;
boolean flagEn = 0;
boolean flag_persist = 1;
unsigned long time_flag3;

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
if (left_red == HIGH )
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
if (right_red == HIGH)
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
  
int key = digitalRead(12);     //doctor's shoot
if (flag_persist == 1 && left_red == LOW && right_red == LOW)
{ delay(50);
  if (ticksEncoder1> 0)
  {
    analogWrite(motor1_PWM , 135);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B, LOW);
    delay(3);
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    }
    else if (ticksEncoder1 <0)
    {
      analogWrite(motor1_PWM , 135);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, HIGH);
      delay(3);
      analogWrite(motor1_PWM , 0);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, LOW);
      }
  }
  
if (key == HIGH)
{
  time_flag3 = 0;
  flag3 = 0;
  flag_persist = 0;
  delay(2000);
  if (flagEn == 0)
  {
    // mire aghab , ok
    analogWrite(motor1_PWM , 165);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, HIGH);
    flagEn = 1;
    }
  }
  
if (ticksEncoder1 >= 35)
{  
  if (flagEn == 1 && flag2 == 0 && flag3 == 0)
  {   // shoot
    analogWrite(motor1_PWM , 125);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B, LOW);
    flagEn = 0;
    flag2 = 1;
    }
  }
//----------------------------------------------------
if (ticksEncoder1 < -50)
{
  if (flagEn == 0 && flag2 == 1 && flag3 == 0 )
  {    //come back
    analogWrite(motor1_PWM , 255);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B, HIGH);
    delay(500);
    analogWrite(motor1_PWM , 165);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, HIGH);
    flag2 = 0;
    flagEn = 0;
    flag3 = 1;
    }
  }
//----------------------------------------------------
if (ticksEncoder1 >= -15)
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
    delay(5);
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    delay(50);      
    }
    else if (ticksEncoder1 <0)
    {
      analogWrite(motor1_PWM , 100);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, HIGH);
      delay(5);
      analogWrite(motor1_PWM , 0);
      digitalWrite(motor1A,LOW);
      digitalWrite(motor1B, LOW);
      }  
  if (flag3 == 1 && ticksEncoder1 == 0)
  {
    time_flag3 = millis();
    if(time_flag3 > 750)
    {
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
  
void buzzer()
{
  digitalWrite(13,HIGH);         //buzzer
  delay(200);
  digitalWrite(13,LOW);
  delay(50);
  digitalWrite(13,HIGH);         //buzzer
  delay(200);
  digitalWrite(13,LOW);
  delay(50);
  digitalWrite(13,HIGH);         //buzzer
  delay(200);
  digitalWrite(13,LOW);
  delay(50);
  digitalWrite(13,HIGH);         //buzzer
  delay(200);
  digitalWrite(13,LOW);
  delay(50); 
  }
