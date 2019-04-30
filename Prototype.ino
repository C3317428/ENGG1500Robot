#include <Servo.h> //makes the sonar magic happen
#define ECHO 12
#define TRIG 7
#define SERVO A5
Servo servo;
int servoAngle = 0; //Servo intial position
unsigned int SonarL = 1;
unsigned int SonarF = 1;
unsigned int SonarR = 1;
unsigned int SonarFR = 1;
int Approximation = 0;
int ApproximationConstant = 4;
void setup() {
  
 
  //sonar pins
  servo.attach(SERVO); // Sonar motor
  pinMode(ECHO,INPUT); //set pin 12 as an input
  pinMode(TRIG,OUTPUT); //set pin 7 as an output
  Serial.begin(9600);
  
  delay(2000); //delay before starting loop

}

void loop() {


  for(servoAngle = 0; servoAngle < 90; servoAngle++){  //move the micro servo from 0 degrees to 90 degrees
                                   
    servo.write(servoAngle);              
    switch (servoAngle) {
      case 0:                 //Reading at 0 degrees
        SonarR = sonar_mm();
        break;
      case 45:                //Reading at 90 degrees
        SonarFR = sonar_mm();
        break;
      case 90:                //Reading at 90 degrees
        SonarF = sonar_mm();
        break;
      default:
        break;
    }
    
    if (SonarF < 15 || SonarFR < 15 || SonarR < 15){
      servo.write(180);
      SonarL = sonar_mm();
      
      if (-(ApproximationConstant) < (SonarL - SonarR) && (SonarL - SonarR) < ApproximationConstant){   //This checks to see if Left and Right are approximately equal 
        Approximation = 1;
      }
        
      if (SonarF < SonarR || SonarFR < SonarR){              //If there is something in front but not to the right
        TurnRight();
      }
      else if (2 * SonarR < SonarL|| 2 * SonarFR < SonarL){  //If there is something to the right but not in front
        TurnLeft();
      }
      else if (Approximation == 1 && SonarF < 3 * SonarL){   //If there is nothing in front but there is stuff to the left and right at roughly equal distances
        Corridor ();
        Approximation = 0;
      }
    }
    else {
      FollowLine();
    }
    delay(20);                  
  }

  for(servoAngle = 90; servoAngle > 0; servoAngle--){  //now move back the micro servo from 90 degrees to 0 degrees
                                    
      servo.write(servoAngle);          
      switch (servoAngle) {
        case 0: //Reading at 0 degrees
          SonarR = sonar_mm();
          break;
        case 45: //Reading at 45 degrees
          SonarFR = sonar_mm();
          break;
        case 90: //Reading at 90 degrees
          SonarF = sonar_mm();
          break;
        default:
          break;
      }
      
      if (SonarF < 15 || SonarFR < 15 || SonarR < 15){
        servo.write(180);
        SonarL = sonar_mm();
      
        if (-(ApproximationConstant) < (SonarL - SonarR) && (SonarL - SonarR) < ApproximationConstant){   //This checks to see if Left and Right are approximately equal 
          Approximation = 1;
        }
        
        if (SonarF < SonarR || SonarFR < SonarR){              //If there is something in front but not to the right
          TurnRight();
        }
        else if (2 * SonarR < SonarL|| 2 * SonarFR < SonarL){  //If there is something to the right but not in front
          TurnLeft();
        }
        else if (Approximation == 1 && SonarF < 3 * SonarL){   //If there is nothing in front but there is stuff to the left and right at roughly equal distances
          Corridor ();
          Approximation = 0;
        }
      }
      else {
        FollowLine();
      }
      
      delay(20);      
  }
}


unsigned int sonar_mm(void){
long duration = 0;
const float speed_sound = 340.29;
// m/s, "const" makes the compiler able to optimise the program where this variable is used, cool!
// Read in a distance from the ultrasonic distance sensor The ultrasonic burst is triggered by a HIGH pulse of 10 microseconds.
digitalWrite(TRIG, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG, LOW);
//read length of time pulse
duration = pulseIn(ECHO, HIGH); 
// This function measures a pulsewidth and returns the width in microseconds convert the time into a distance
// the code "(unsigned int)" turns the result of the distance calculation into an integer instead of a floating point.
return (unsigned int)(0.5 * duration * 1e-6 * speed_sound * 1e3);
//"unsigned" ensures we are returning an unsigned number, remember that there is no such thing as negative distance.
}
