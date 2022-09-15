// thermistor on pin A0
#define THERMISTORPIN A0          
//thermistor on pin A1
#define THERMISTORPIN2 A1
// resistance at 25 degrees C 
#define THERMISTORNOMINAL 10000       
// temp. for nominal resistance (almost always 25 C) 
#define TEMPERATURENOMINAL 25    
// how many samples to take and average, more takes longer 
// but is more 'smooth' 
#define NUMSAMPLES 5 
// The beta coefficient of the thermistor (usually 3000-4000) 
#define BCOEFFICIENT 3950 
// the value of the 'other' resistor 
#define SERIESRESISTOR 10000     
 
int samples[NUMSAMPLES];
int samples2[NUMSAMPLES]; 
 
void setup(void) { 
  Serial.begin(9600); 
  analogReference(EXTERNAL); 
} 
 
 
//MEMS 1042  University of Pittsburgh 
void loop(void) { 
  uint8_t i; 
  float average; 
  float average2;
 
  // take N samples in a row, with a slight delay 
  for (i=0; i< NUMSAMPLES; i++) { 
   samples[i] = analogRead(THERMISTORPIN); 
   samples2[i] = analogRead(THERMISTORPIN2);
   delay(10); 
  } 
   
  // average all the samples out 
  average = 0;
  average2 = 0; 
  for (i=0; i< NUMSAMPLES; i++) { 
     average += samples[i];
     average2 += samples2[i]; 
  } 
  average /= NUMSAMPLES;
  average2 /= NUMSAMPLES; 
 
   
  // convert the value to resistance for thermistor 1
  average = 1023 / average - 1; 
  average = SERIESRESISTOR / average;  

  // convert the value to resistance for thermistor 2
  average2 = 1023 / average2 - 1; 
  average2 = SERIESRESISTOR / average2;

  // temperature of thermistor 1
  float steinhart; 
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro) 
  steinhart = log(steinhart);                  // ln(R/Ro) 
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro) 
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To) 
  steinhart = 1.0 / steinhart;                 // Invert 
  steinhart -= 273.15;                         // convert absolute temp to C 

  // temperature of thermistor 2
  float steinhart2; 
  steinhart2 = average2 / THERMISTORNOMINAL;     // (R/Ro) 
  steinhart2 = log(steinhart2);                  // ln(R/Ro) 
  steinhart2 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro) 
  steinhart2 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To) 
  steinhart2 = 1.0 / steinhart2;                 // Invert 
  steinhart2 -= 273.15;                         // convert absolute temp to C 
   
  Serial.print("Temperature at thermistor 1");  
  Serial.print(steinhart); 
  Serial.println(" *C"); 

  Serial.print("Temperature at thermistor 2");  
  Serial.print(steinhart2); 
  Serial.println(" *C"); 
   
  delay(1000); 
}
