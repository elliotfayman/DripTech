#include <LiquidCrystal.h>

const int size = 12;
int prevbandpass = 0;
int values[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int index = 0;


float EMA_a_low = 0.1, EMA_a_high = 0;    
int EMA_S_low = 0, EMA_S_high = 0;       
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

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, V0 = 6, pinNoise = 9, pinADC = A0;


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int current_slct = 0; 
double waterDumped = 0.0;


void setup(){
  pinMode(pinNoise, OUTPUT);
  analogWrite(V0, 160);
  lcd.begin(16, 2);

  lcd.print(".......................");

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
  for(int x = 0; x<size; x++) values[x] = average1();
}

void loop(){
  lcd.setCursor(0, 1);
  lcd.print(totalOnTime);
  long sum = 0;
  for(int i = 0; i<32; i++){
    sum += analogRead(pinADC);
  }

  sum>>=5;
  values[index] = sum;
  index = (index + 1) % size; 
  double val = average1();
  EMA_S_low = (EMA_a_low*val) + ((1-EMA_a_low)*EMA_S_low); 
  EMA_S_high = (EMA_a_high*val) + ((1-EMA_a_high)*EMA_S_high);
  
  
  bandpass = EMA_S_high - EMA_S_low;  

  if(abs((total/totalTicks)-average1())<180){
    total+=average1();
    totalTicks++;

  }
  
  double curVal = abs(average1() - (total/totalTicks));
  Serial.println(curVal);

  if(curVal>50) {
    locations[locIndex] = totalClocks;
    locIndex = (locIndex+1)%3;
    int period1 = locations[(locIndex-1)%3] - locations[(locIndex-2)%3];
    int period2 = locations[locIndex] - locations[(locIndex-1)%3];
    int difference = abs(period1-period2);
    if(difference<40)  {
      positives++;
      totalOnTime+=0.3;
      waterDumped +=0.05*3;
    }
    else {
      analogWrite(pinNoise, 0);
    }
  }

  if(positives>5) {
    analogWrite(pinNoise, 20);
    lastTimeOn = millis() / 10000;
  }
  if(totalClocks%100==0) {
    positives = 0;
    analogWrite(pinNoise, 0);
  }
  lcd.setCursor(0, 0);
  current_slct = analogRead(A3);
  if((current_slct/520)>0) {
    lcd.print("Time Dripping:   ");
    lcd.setCursor(0, 1);
    lcd.print(totalOnTime);
    lcd.print("        ");
  }
  else {
    lcd.print("Water Used:   ");
    lcd.setCursor(0, 1);
    lcd.print(waterDumped);
    lcd.print("        ");
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
