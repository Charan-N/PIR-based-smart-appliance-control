/*
 Industry Oriented Mini Project, EIE Dept.
 Students: Akhila Sanathana :   18B81A1003
           Siva Charan N    :   18B81A1049
           K.V.M Vaishnavi  :   18B81A1058
 Faculty : G. Venkateswarulu, Asst.Professor, EIE Dept.

 Date Created : 22 May, 2021
 Last Modified: 24 May, 2021 
 */


//Trigger pin is the main switch for all PIRs in the hall except the entrance.
//The pin is connected to D2 in Arduino. 
int trigger = 2;

//The LEDs are used temporarily will be replaced with relays in final source code
int led_1 = 8;
int led_2 = 9;
int led_3 = 10;
int led_4 = 11;
int entrance_led = 12;

//Pir connections, only 5 for Arduino UNO
int  entrance_pir = A0;
int pir_1 = A1;
int pir_2 = A2;
int pir_3 = A3;
int pir_4 = A4;

//This variable is used to store which PIR gives the least voltage,
//i.e to which PIR the person is close to
//Change to max_id if the PIR gives higher voltage when the person is close 
int min_id;

//these set of arrays are used for simplicity and eliminate use of extensive if-else loops
//more PIRs can be easily added: simply update in Setup() and in these arrays!
int pir_array [4] = {pir_1, pir_2, pir_3, pir_4};
int led_array [4] = {led_1, led_2, led_3, led_4};

//to store PIR values
int values [4];

bool person_in = false;

//function declarations
void scan_area();
int min_max(int, String);

void setup() {
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(entrance_pir, INPUT);
  pinMode(pir_1, INPUT);
  pinMode(pir_2, INPUT);
  pinMode(pir_3, INPUT);
  pinMode(pir_4, INPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(entrance_led, OUTPUT);
} 

void loop() {
  if(analogRead(entrance_pir) == HIGH){
    digitalWrite(trigger, HIGH);  //this trigger will now provide the power supply to all other PIRs
    digitalWrite(entrance_led, HIGH);
    person_in = true;
  }

  while(person_in){
    scan_area();
  }
  
 delay(5);//for stability
}

void scan_area(){
  for(int i=0; i<(sizeof(pir_array)/sizeof(pir_array[0])); i++){
    values[i] = analogRead(pir_array[i]);
  }
  min_id = min_max(values, "min");//use "max" if the PIR returns higher voltage if the person is nearer.
  //min_id is -1 means the room is empty, i.e no person is in the room.
  if(min_id == -1){
    digitalWrite(trigger, LOW);//cut off power to PIRs since scanning empty room is energy wasting.
    person_in = false;//breakout of the while loop above
  }
  else{
    digitalWrite(led_array[min_id], HIGH);
  }
}

int min_max(int arr[], String metric){
  int minx = arr[0], maxx = arr[0], minid, maxid;
  for(int i=0; i<(sizeof(arr)/sizeof(arr[0])); i++){
     //the below statement is to ignore the values of PIR where person is not present
    //change the value from 0 to 1024(highest value of any analog o/p) based on PIR characteristics
    if (arr[i] == 0){
      continue;
    }
    //we're finding both min and max values to make the code easy to use with any PIR
    if(arr[i] > maxx){
      maxx = arr[i];
      maxid = i;
    }
    else if(arr[i] < minx){
      minx = arr[i];
      minid = i;
    }
  }
  //this statement means that both min, max = 0;
  //i.e the room is empty, return -1
  //again, change the 0s to 1024 if thats the PIR's nature.
  if(minx == 0 && maxx == 0){
    return -1;
  }
  
  //why return the ID and not value is pretty self explanatory, so I won't explain.
  if(metric == "min"){
    return minid;
  }
  else if(metric == "max"){
    return maxid;
  }
  else{
    return 0;
  }
}
