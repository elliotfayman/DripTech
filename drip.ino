const int pinADC = A0;
const int pinNoise = 9;
const int size = 12;
int prevbandpass = 0;
int values[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int index = 0;
float rate[] = {0, 0, 0, 0, 0};
int kindex = 0;
float EMA_a_low = 0.1;    //initialization of EMA alpha
float EMA_a_high = 0.4;
int EMA_S_low = 0;        //initialization of EMA S
int EMA_S_high = 0;
int highpass = 0;
int bandpass = 0;
double total = 0;
long totalTicks = 0;
long totalClocks = 0;
long locations[] =  {0, 0, 0};
int locIndex = 0;
int positives;
long totalOnTime = 0;
long lastTimeOn = 0;
//**********************************
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, V0 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup(){
  pinMode(pinNoise, OUTPUT);
  analogWrite(V0, 160);
  lcd.begin(16, 2);

  lcd.print("hello, world!");

  Serial.begin(115200);
  long sum = 0;
  for(int i = 0; i<32; i++){
    sum += analogRead(pinADC);
  }

  sum>>=5;
  EMA_S_low = sum;

  EMA_S_high = sum;
  total+=average1();
  totalTicks++;
  
}

void loop(){
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(totalOnTime);
  long sum = 0;
  for(int i = 0; i<32; i++){
    sum += analogRead(pinADC);
  }

  sum>>=5;
  values[index] = sum;
  index = (index + 1) % size; 
  double val = average1();
  EMA_S_low = (EMA_a_low*val) + ((1-EMA_a_low)*EMA_S_low);  //run the EMA
  EMA_S_high = (EMA_a_high*val) + ((1-EMA_a_high)*EMA_S_high);
  
  
  bandpass = EMA_S_high - EMA_S_low;      //find the band-pass
  //Serial.print("Value is: ");
  //Serial.println(bandpass);
  // Serial.print("Rate of Change is: ");
  //rate[kindex] = abs((bandpass-prevbandpass)/30.0);
  //kindex = (kindex + 1) % 5; 
  
  //prevbandpass = bandpass;
  if((total/totalTicks)<(average1()-30)){
    total+=average1();
    totalTicks++;
    //Serial.println("))))))))))))))))))))))))))))))))))))))");
    //Serial.println(average1());
    //Serial.println("))))))))))))))))))))))))))))))))))))))");
  }
  
  //Serial.println(abs(average2() - (total/totalTicks)));
  double curVal = abs(average1() - (total/totalTicks));
  //Serial.println(total/totalTicks);
  Serial.println(curVal);

  if(curVal>40) {
    locations[locIndex] = totalClocks;
    locIndex = (locIndex+1)%3;
    int period1 = locations[(locIndex-1)%3] - locations[(locIndex-2)%3];
    int period2 = locations[locIndex] - locations[(locIndex-1)%3];
    int difference = abs(period1-period2);

    if(difference<20)  positives++;
    else {
      analogWrite(pinNoise, 0);
    //positives--;
    }
  }

  if(positives>20) {
    analogWrite(pinNoise, 3);
    lastTimeOn = millis() / 10000;
  }
  if(totalClocks%100==0) {
    if(positives>0) totalOnTime += millis() / 1000 - lastTimeOn;
    positives = 0;
  }
  totalClocks++;
  delay(30);
}

double average1(){
  int sum = 0;
  for(int j = 0; j<size; j++) sum+=values[j];
  return (sum + 0.0)/size;
}
double average2(){
  double sum = 0;
  for(int j = 0; j<5; j++) sum+=rate[j];
  return (sum + 0.0)/3.0;
}
