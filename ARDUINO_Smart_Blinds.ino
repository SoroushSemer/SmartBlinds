/* SMART BLINDS ARDUINO NANO
 * CODE WRITTEN BY: SOROUSH SEMERKANT
 * LINKEDIN: https://www.linkedin.com/in/soroush-semerkant/
 * GITHUB: https://github.com/SoroushSemer
 * PUBLISHED ON: 6/8/2021
 */

//declare variables for the motor pins
int motorPin1 = 3;    // stepper motor driver pin 1
int motorPin2 = 4;    // stepper motor driver pin 2
int motorPin3 = 5;    // stepper motor driver pin 3
int motorPin4 = 6;    // stepper motor driver pin 4

int espInput1 = 8; // input 1 from esp8266 
int espInput2 = 9; // input 2 from esp8266

int motorSpeed = 1000; //variable to set stepper speed
int count = 0;          // count of steps made
int countsperrev = 5200; // number of steps per full revolution

int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

int updown = 1; //starts up
int msg = "";

void setup() {
  //initializes the stepper motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  //initializes the esp to arduino pins
  pinMode(espInput1, INPUT);
  pinMode(espInput2, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  if(!digitalRead(espInput1) && digitalRead(espInput2))     //if received the up signal (01)
  {
    msg="UP";                                               //sets the msg to be up
    Serial.println(msg);
  }
  else if(digitalRead(espInput1) && !digitalRead(espInput2))//if received the down signal (10)
  {
    msg="DOWN";                                             //sets the msg to be down
    Serial.println(msg);
  }
  else if (digitalRead(espInput1) && digitalRead(espInput2))//if received the half signal (11)
  {
    msg="HALF";                                             //sets the msg to be half
    Serial.println(msg);
  }
  else
  {
    msg="";                                                 //if there is no msg (00) set the msg to blank
  }
  
  if (updown == 3 && msg == "UP") {                         //down position to up
    stepper(2 * countsperrev);                              //down to up distance
    updown = 1;                                             //sets the current position to be up (code 1)
  }
  else if (updown == 3 && msg == "HALF") {                  //down to half
    stepper(1 * countsperrev);                              //down to half distance
    updown = 2;                                             //sets the current position to be half (code 2)
  }
  else if (updown == 2 && msg == "UP") {                    //half to up
    stepper(1 * countsperrev);                              //half to up distance
    updown = 1;                                             //sets the current position to be up (code 1)
  }
  else if (updown == 2 && msg == "DOWN") {                  //half to down
    stepper(-0.5*countsperrev);                             //half to down distance
    updown = 3;                                             //sets the current position to be down (code 3)
  }
  else if (updown == 1 && msg == "DOWN") {                  //up to down
    stepper(-1*countsperrev);                               //up to down distance
    updown = 3;                                             //sets the current position to be down (code 3)    
  }
  else if (updown == 1 && msg == "HALF") {                  //up to half
    stepper(-0.5*countsperrev);                             //up to half distance
    updown = 2;                                             //sets the current position to be half (code 2)
  }
  else {
    Serial.print("The blind is already ");                  //if the goal position is the same as the current don't change anything
    Serial.println(updown);                                 //(ie. half to half, up to up, down to down)
  }

}

void stepper(int count) {
  for (int i = 0; i < abs(count); i++) {                    //repeats the steps the based on the given distance
    if (count < 0) {                                        //checks the direction of the movement
      counterclockwise();                                   //one counter clockwise step
      Serial.println("Counter-Clockwise");
    }
    else {
      clockwise();                                          //one clockwise step
      Serial.println("Clockwise");
    }
  }
  digitalWrite(motorPin1,LOW);                              //turns off stepper motor driver completely
  digitalWrite(motorPin2,LOW);
  digitalWrite(motorPin3,LOW);
  digitalWrite(motorPin4,LOW);
}
void counterclockwise(){                                    //one counterclockwise step code for driver
  for (int i = 0; i < 8; i++)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise(){                                           //one clockwise step code for driver
  for (int i = 7; i >= 0; i--)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput(int out){
  digitalWrite(motorPin1, bitRead(lookup[out], 0));         //ouput to pins for stepper motor driver
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
}
