int Encoder_1              = 2;      
int Encoder_2              = 3;      

int power_p            = 5;     
int power_n            = 6;     

float Kp            = 10;    
float Ki            = 30;    

float elapsedTime    = 1000;   
float rpm_desired    = -25;
float error          = 0;
float PID            = 0;
float integrat       = 0;

long count           = 0;
float rpm_actual     = 0;
float rpm;

float previous_integrat        = 0;
float previous_error           = 0;
unsigned long previous_time    = 0;
long previous_count            = 0;

unsigned char pwm;

boolean dir; 


void setup()
{
  attachInterrupt(0, encoderInt, RISING); 
  pinMode(Encoder_2, INPUT);
  
  pinMode(power_p, OUTPUT); 
  pinMode(power_n, OUTPUT); 
  
  Serial.begin(115200);
}

void loop()
{ 
  rpm_actual = (count - previous_count);
  previous_count = count;
   
  rpm_actual = rpm_actual * 1000 / (1.1 * elapsedTime);
  
  error = rpm_desired*0.0033- rpm_actual;

  
  integrat = previous_integrat + (previous_error / 1000000) * elapsedTime;
  previous_integrat = integrat;
  previous_error = error;
  PID = error * Kp + Ki * integrat;

  
  if(PID > 9)
  {
    PID = 9;
  }
  if(PID < -9)
  {
    PID = -9;
  }

  
 
  
  
  PID = abs(PID);
  pwm = (PID / 9) * 255;

  rpm = pwm*0.588;// (150/255)

  Serial.print(rpm);
  Serial.print("\t");

  Serial.println(rpm_desired);
  if(rpm_desired > 0)
  {
    dir = true;
    }
  if(rpm_desired < 0)
  {
    dir = false;
    }  
  switch (dir){
  
  case true:  
  analogWrite(power_p, pwm);
  analogWrite(power_n, 0);
  break;
  
  case false:
  analogWrite(power_p, 0);
  analogWrite(power_n, pwm);
  break;
  }
  elapsedTime = micros() - previous_time;
  previous_time = micros();
}

void encoderInt()
{
  if(digitalRead(Encoder_2) == HIGH){count++;}
  else if(digitalRead(Encoder_2) == LOW){count--;}
}
