/**
 * I will not be using any libraries for this sketch. The `timer0` 
 * of Arduino Mega 2560 enables us to have an accurate time to 50 days
 * when used with `millis()`. Let's leverage on this fact. Machine time
 * is not accurate. So what I did was make the program work for seconds,
 * at least. 
 */

#define MOTOR_0_PIN 10
#define MOTOR_1_PIN 8
#define MOTOR_2_PIN 7

// Comment this out to turn off all SERIAL printlns.
// #define DEBUG

using ulong = unsigned long;

/**
 * A function to convert from milliseconds to seconds.
 */
inline ulong ms_to_s(ulong ms) {
  return ms / 1000;
}

/**
 * A function that converts from seconds to ms.
 */
inline ulong s_to_ms(ulong s) {
  return s * 1000;
}

class Motor {
  public:
    Motor(int pin, 
      ulong t0,     // Switch time
      ulong t1_on,  // How many seconds will t1 be turned on?
      ulong t1_off, // How many seconds will t1 be turned off?
      ulong t2_on,  // How many seconds will t2 be turned on?
      ulong t2_off  // How many seconds will t2 be turned off?
    ) : t_switch {t0}, 
        t1 {t1_on, t1_off, pin},
        t2 {t2_on, t2_off, pin},
        last_update {0}
    {
      pinMode(pin, OUTPUT);
    }

    /**
     * Update timer status and the pin.
     * Note that this time is in seconds.
     */
    void update(ulong time) {
      // Poll t_switch
      if (time % t_switch == 0 && last_update != time) {
        is_t1 = !is_t1; // switch.
        last_update = time;
        #ifdef DEBUG
        Serial.print("Updating t0: ");
        Serial.print(time);
        Serial.print(" seconds. Timer is now: ");

        if (is_t1) {
          Serial.println("t1");
        } else {
          Serial.println("t2");
        }
        #endif
      }

      // Timer::update has its own `last_update`.
      if (is_t1) {
        t1.update(time);
      } else {
        t2.update(time);
      }
    }
  private:
    /* The timer is a struct that contains information about one of the three timers. */
    struct Timer {        // I'm just checking for struct padding.
      ulong time_on;      // 32 bits
      ulong time_off;     // 32 bits
      ulong last_update;  // 32 bits
      int pin;            // 16 bits
      bool is_on;         // A flag
      Timer(ulong on, ulong off, int _pin) 
        : time_on{on}, time_off {off}, is_on {false}, pin {_pin}, last_update {0} {}

      void update(ulong time) {
        if (is_on) {
          // Check how many seconds should this be turned on.
          if (time % time_on == 0 && time != last_update) {
            // Then we should switch.
            is_on = false;
            last_update = time;
            digitalWrite(pin, LOW);
            #ifdef DEBUG
            Serial.print("Switched to off at time ");
            Serial.print(time);
            Serial.println(" seconds");
            #endif
          }
        } else {
          if (time % time_off == 0 && time != last_update) {
            is_on = true; // switch.
            last_update = time;
            digitalWrite(pin, HIGH);
            #ifdef DEBUG
            Serial.print("Switched to on at time ");
            Serial.print(time);
            Serial.println(" seconds");
            #endif
          }
        }
      }

      void reset() {
        last_update = 0;
      }
    };
    
    /* Timer to switch between t1 and t2. */
    ulong t_switch;
    /* Last time update. This is to prevent multiple updates in the same second. */
    ulong last_update; 
    /* Flag whether t1 is currently in use. */
    bool is_t1;
    Timer t1;
    Timer t2;

    
};

Motor m0 = m0 = Motor(MOTOR_0_PIN, 
    15, // Switch every 15 seconds
    5,  // t1 is on for 5 seconds
    3,  // t1 is on for 3 seconds
    1,  // t2 is on for 1 second
    1   // t2 is off for 1 second.
);

Motor m1 = Motor(MOTOR_1_PIN, 
    30, // Switch every 10 seconds
    15, // t1 is on for 15 seconds
    2,  // t1 is on for 2 seconds
    4,  // t2 is on for 4 seconds
    8   // t2 is off for 8 seconds.
);

Motor m2 = Motor(MOTOR_2_PIN, 
    24, // Switch every 24 seconds
    3,  // t1 is on for 3 seconds
    7,  // t1 is on for 7 seconds
    9,  // t2 is on for 9 seconds
    2   // t2 is off for 2 seconds.
);

void setup() {
  #ifdef DEBUG
  Serial.begin(230400);
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  ulong time { ms_to_s(millis()) };
  m0.update(time);
  m1.update(time);
  m2.update(time);
}
