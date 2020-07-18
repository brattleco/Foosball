//encoder
// blue player   shoot with brain ok
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
pinMode(13,OUTPUT);
pinMode(7,INPUT);   
pinMode(8,INPUT);
pinMode(12,INPUT);      // doctor's key
}

void loop(void) {
int right_blue = digitalRead(7);
int left_blue = digitalRead(8);
if (right_blue == HIGH && lr_off == 0 && ticksEncoder1 == 0)
{ 
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    
    out(0,255,255);
    delay(10);
    out(0,0,0);
  }
if (right_blue == LOW )
{
    out(0,0,0);
  }
if (left_blue == HIGH && lr_off == 0 &&ticksEncoder1 == 0)
{
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    
  out(255,255,0);
  delay(10);
  out(0,0,0);
  }
if (left_blue == LOW )
{ 
  out(0,0,0);
  }

if (right_blue == HIGH && left_blue == HIGH)
{ 
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    out(0,0,0);
    delay(1000);
  }
  
int permission_key = digitalRead(12);     //permission_key
if (flag_persist == 1 && left_blue == LOW && right_blue == LOW)
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
  
if (Serial.available() > 0) {
  incomingByte = Serial.read();   
  if (incomingByte == 'B' && permission_key == HIGH)  
{
  lr_off = 1;
  buzzer();
  time_flag3 = 0;
  flag3 = 0;
  flag_persist = 0;
  delay(200);
  if (flagEn == 0)
  {
    // mire aghab , ok
    analogWrite(motor1_PWM , 165);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B,LOW );
    flagEn = 1;
    }
  }
}
  
if (ticksEncoder1 <= -45)
{   
  analogWrite(motor1_PWM , 255);
  digitalWrite(motor1A,HIGH);
  digitalWrite(motor1B, HIGH);
  delay(100);
    
  if (flagEn == 1 && flag2 == 0 && flag3 == 0)
  {   // shoot
    shoot();
    flagEn = 0;
    flag2 = 1;
    }
  }
//----------------------------------------------------
if (ticksEncoder1 >= 50)
{
  if (flagEn == 0 && flag2 == 1 && flag3 == 0 )
  {    //come back
    analogWrite(motor1_PWM , 255);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B, HIGH);
    delay(200);
    for (int bi=0;bi<100; bi=bi+5){
    analogWrite(motor1_PWM , bi);
    digitalWrite(motor1A,HIGH);
    digitalWrite(motor1B, LOW);
    }
    flag2 = 0;
    flagEn = 0;
    flag3 = 1;
    }
  }
//----------------------------------------------------
if (ticksEncoder1 <= 15)
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
    analogWrite(motor1_PWM , 0);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, LOW);
    //delay(5);      
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
      //delay(5);  
      }  
  if (flag3 == 1 && ticksEncoder1 == 0)
  {
    time_flag3 = millis();
    if(time_flag3 > 750)
    {
      flag3 = 0;
      time_flag3 = 0;
      flag_persist = 1;
      lr_off = 0;
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
void shoot(){
  analogWrite(motor1_PWM , 0);
  digitalWrite(motor1A,LOW);
  digitalWrite(motor1B, LOW);
  delay(200);
    for (int s=0;s<=175;s=s+3){
    analogWrite(motor1_PWM , 255);
    digitalWrite(motor1A,LOW);
    digitalWrite(motor1B, HIGH);
    }
  }
  
