int status = 0;                         // Variable to hold traffic light status (currently unused)
int PIN_GREEN = 10;                     // Pin number for the green traffic light LED
int PIN_YELLOW = 12;                    // Pin number for the yellow traffic light LED
int PIN_RED = 8;                        // Pin number for the red traffic light LED
int BUTTON_PIN = 2;                     // Pin number for the pedestrian button input

int PED_GREEN = 4;                      // Pin number for the pedestrian green LED
int PED_RED = 6;                        // Pin number for the pedestrian red LED


// Initializing the class with the specific pins
class TrafficLight {
  int _greenLED;                        // Internal variable for green LED pin
  int _yellowLED;                       // Internal variable for yellow LED pin
  int _redLED;                          // Internal variable for red LED pin
  int _pedGreenLED;                     // Internal variable for pedestrian green LED pin
  int _pedRedLED;                       // Internal variable for pedestrian red LED pin
  int _buttonPin;                       // Internal variable for pedestrian button pin
  long TIMER_YELLOW = 5000;             // Duration for yellow light in milliseconds (5 seconds)
  unsigned long previousMillis;         // Variable to store last time the state changed
  bool buttonPressed = false;           // Flag to track if the pedestrian button was pressed

  // Enumeration for traffic light states
  enum State {
    GREEN,                              // Traffic light green state
    YELLOW,                             // Traffic light yellow state
    RED                                 // Traffic light red state
  };

  State currentState;                   // Current traffic light state
  State targetState;                    // Next state the light should transition to

public:
  // Constructor initializes all pins and sets initial state
  TrafficLight(int greenLED, int yellowLED, int redLED, int pedGreenLED, int pedRedLED, int buttonPin) {
    _greenLED = greenLED;               // Assign the green LED pin
    _yellowLED = yellowLED;             // Assign the yellow LED pin
    _redLED = redLED;                   // Assign the red LED pin
    _pedGreenLED = pedGreenLED;         // Assign the pedestrian green LED pin
    _pedRedLED = pedRedLED;             // Assign the pedestrian red LED pin
    _buttonPin = buttonPin;             // Assign the pedestrian button pin

    pinMode(_greenLED, OUTPUT);         // Set traffic green LED pin as output
    pinMode(_yellowLED, OUTPUT);        // Set traffic yellow LED pin as output
    pinMode(_redLED, OUTPUT);           // Set traffic red LED pin as output
    pinMode(_pedGreenLED, OUTPUT);      // Set pedestrian green LED pin as output
    pinMode(_pedRedLED, OUTPUT);        // Set pedestrian red LED pin as output
    pinMode(_buttonPin, INPUT_PULLUP);  // Set button pin as input with internal pull-up resistor

    // Initialize state to GREEN
    currentState = GREEN;               // Start traffic light in green state
    targetState = RED;                  // Next target state is red (after yellow)
    previousMillis = 0;                 // Reset timer
    green();                            // Turn on the green light at startup
  }

  // Main loop for traffic light control
  void loop() {
    unsigned long currentMillis = millis();  // Get current time in milliseconds

    // Check if the button is pressed
    if (digitalRead(_buttonPin) == LOW && currentState == GREEN) {
      buttonPressed = true;             // Mark that button was pressed if in green state
    }

    switch (currentState) {
      case GREEN:                       // If the light is currently green
        if (buttonPressed) {            // If pedestrian button was pressed
          buttonPressed = false;        // Reset button flag
          currentState = YELLOW;        // Move to yellow state
          targetState = RED;            // Next target after yellow is red
          previousMillis = currentMillis; // Save current time
          yellow();                     // Turn on yellow light
        }
        break;

      case YELLOW:                      // If the light is currently yellow
        if (currentMillis - previousMillis >= TIMER_YELLOW) { // Wait until yellow timer expires
          previousMillis = currentMillis; // Reset timer
          if (targetState == GREEN) {   // If target is green, switch to green
            currentState = GREEN;
            green();
          } else {                      // Otherwise, switch to red
            currentState = RED;
            red();
          }
        }
        break;

      case RED:                         // If the light is currently red
        // Pedestrian can cross safely when the light is red
        if (currentMillis - previousMillis >= TIMER_YELLOW) { // Wait until red duration is over
          previousMillis = currentMillis; // Reset timer
          currentState = YELLOW;        // Move to yellow state before green
          targetState = GREEN;          // Next target is green
          yellow();                     // Turn on yellow light
        }
        break;
    }
  }

private:
  // Function to turn on green light
  void green() {
    digitalWrite(_greenLED, HIGH);      // Turn on green LED
    digitalWrite(_yellowLED, LOW);      // Turn off yellow LED
    digitalWrite(_redLED, LOW);         // Turn off red LED
    digitalWrite(_pedGreenLED, LOW);    // Pedestrian green OFF (wait)
    digitalWrite(_pedRedLED, HIGH);     // Pedestrian red ON (don’t cross)
  }

  // Function to turn on yellow light
  void yellow() {
    digitalWrite(_greenLED, LOW);       // Turn off green LED
    digitalWrite(_yellowLED, HIGH);     // Turn on yellow LED
    digitalWrite(_redLED, LOW);         // Turn off red LED
    digitalWrite(_pedGreenLED, LOW);    // Pedestrian must still wait
    digitalWrite(_pedRedLED, HIGH);     // Pedestrian red stays ON
  }

  // Function to turn on red light
  void red() {
    digitalWrite(_greenLED, LOW);       // Turn off green LED
    digitalWrite(_yellowLED, LOW);      // Turn off yellow LED
    digitalWrite(_redLED, HIGH);        // Turn on red LED
    digitalWrite(_pedGreenLED, HIGH);   // Pedestrian green ON (can cross)
    digitalWrite(_pedRedLED, LOW);      // Pedestrian red OFF
  }
};

// Create TrafficLight instance
TrafficLight trafficLight(PIN_GREEN, PIN_YELLOW, PIN_RED, PED_GREEN, PED_RED, BUTTON_PIN); // Instantiate traffic light with defined pins

void setup() {}                        // Arduino setup function (currently empty)

void loop() {
  trafficLight.loop();                 // Continuously run traffic light logic
}
