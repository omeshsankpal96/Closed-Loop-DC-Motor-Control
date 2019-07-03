
// PID for motor position control

unsigned long currentTime;
double derivative_e, integral_e;
double e, elapsedTime,  theta_actual, PID_output, PID;

int EA = 2;//white
int EB = 3;//red
double pwm1 = 5;//blue
double pwm2 =6;//yellow

double theta_desired = -360;
double e_previous = 0;
unsigned long lastTime =0;
int count =0;
double Kp = 1;
double Kd = 0.001;
double Ki = 0.0015 ;

void setup(){
  pinMode(EA,INPUT);
  pinMode(EB,INPUT);

  attachInterrupt(digitalPinToInterrupt(EA),seqA , CHANGE);
  attachInterrupt(digitalPinToInterrupt(EB),seqB , CHANGE);
  Serial.begin(115200);
  
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
    
}

void seqA(){
  if (digitalRead(EB) == 0){
    if (digitalRead(EA) == 1){
      count = count+1;} 
    else{
      count = count-1;}
  }
  else{
    if(digitalRead(EA) == 1){ 
      count = count-1;}
    else {
      count = count+1;}      
    }
}


void seqB(){
  if (digitalRead(EA) == 0){
    if (digitalRead(EB) == 0){
      count = count+1;} 
    else{
      count = count-1;}
  }
  else{
    if(digitalRead(EB) == 0){ 
      count = count-1;}
    else {
      count = count+1;}      
    }
}

void loop(){

  theta_actual = 0.3214* count ; // 360/1100
  
  e = theta_desired - theta_actual;

  currentTime = millis();
  elapsedTime = currentTime -lastTime;

  derivative_e = (e - e_previous) / elapsedTime;

  integral_e = integral_e + e* elapsedTime;

  PID_output = Kp*e + Ki*integral_e + Kd*derivative_e;

  PID_output = constrain(PID_output,-255,255);

  PID = abs(PID_output);

  lastTime = currentTime;
  e_previous = e;

  
  if (theta_actual < theta_desired){
    digitalWrite(pwm1,LOW);
    analogWrite(pwm2,PID);
     }
  else {
    digitalWrite(pwm2,LOW);
    analogWrite(pwm1,PID);   
  }

  Serial.print(count);
  Serial.print("\t");
  Serial.print(theta_actual);
  Serial.print("\t");
  Serial.print(digitalRead(EA));
  Serial.println(digitalRead(EB));
  
  
}
