#define LEFT_A1 15
#define LEFT_B1 2
#define RIGHT_A2 4
#define RIGHT_B2 5

const int trigPin = 19;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
// #define CM_TO_INCH 0.393701

long duration;
float distance;

int currState = 0;
int nextState = 1;

unsigned long int forwardStartTime;
unsigned long int stopStartTime;
unsigned long int backStartTime;
unsigned long int leftStartTime;
unsigned long int totalForwardTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(LEFT_A1, OUTPUT);
  pinMode(RIGHT_A2, OUTPUT);
  pinMode(LEFT_B1, OUTPUT);
  pinMode(RIGHT_B2, OUTPUT);
}


void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  distance = duration * SOUND_SPEED/2;
  Serial.print("\nDistance: ");
  Serial.println(distance);
  Serial.println(currState  );
  delay(5);  // very important code!
  // smallest possible value,
  // otherwise the sampling rate will be set to 1000 (1 second delay) 
  
  /* State Trnsitions
    0 --> 0 keep moving forward
    0 --> 1 obstacle detected < 10 cm
    
    1 --> 1 moving backward
    1 --> 2 backward timing achieved

    2 --> 2 turning to the left
    2 --> 0 turning timing achieved
  */

  int prevState = currState;
  currState = nextState;
  if (currState == 0) {
    if (distance <= 30) {
      nextState = 1;
    } else if(prevState != currState){  // just started
      forward();
      forwardStartTime = millis();
    }else if (millis() - forwardStartTime > 10000) {
      nextState = 2;
    }
  } else if (currState == 1) {
    if (prevState != currState) {  // start moving backward
      backward();
      backStartTime = millis();
    } else if (millis() - backStartTime > 200) {
      nextState = 2;
    }
  } else if (currState == 2) {
    if (prevState != currState) {  // start turning left
      left();
      leftStartTime = millis();
    } else if (millis() - leftStartTime > 400) {
      nextState = 0;
    }
  }
}

void forward() {
  Serial.println("=======Maju");
  digitalWrite(LEFT_A1, HIGH);
  digitalWrite(LEFT_B1, LOW);
  digitalWrite(RIGHT_A2, HIGH);
  digitalWrite(RIGHT_B2, LOW);
}
void backward() {
  Serial.println("Mundur=====");
  digitalWrite(LEFT_A1, LOW);
  digitalWrite(LEFT_B1, HIGH);
  digitalWrite(RIGHT_A2, LOW);
  digitalWrite(RIGHT_B2, HIGH);
}
void left() {
  Serial.println("++Putar kiri++");
  digitalWrite(LEFT_A1, LOW);
  digitalWrite(LEFT_B1, HIGH);
  digitalWrite(RIGHT_A2, HIGH);
  digitalWrite(RIGHT_B2, LOW);
}
void right() {
  Serial.println("Putar kanan");
  digitalWrite(LEFT_A1, HIGH);
  digitalWrite(LEFT_B1, LOW);
  digitalWrite(RIGHT_A2, LOW);
  digitalWrite(RIGHT_B2, HIGH);
}
void stop() {
  Serial.println("Berhenti");
  digitalWrite(LEFT_A1, LOW);
  digitalWrite(LEFT_B1, LOW);
  digitalWrite(RIGHT_A2, LOW);
  digitalWrite(RIGHT_B2, LOW);
}
