/**
 * I will not be using any libraries for this sketch. The `timer0` 
 * of Arduino Mega 2560 enables us to have an accurate time to 50 days
 * when used with `millis()`. Let's leverage on this fact. Machine time
 * is not accurate. So what I did was make the program work for seconds,
 * at least. 
 */

#define MOTOR_1_PIN 10

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
      ulong t1_on,  // How many milliseconds will t1 be turned on?
      ulong t1_off, // How many milliseconds will t1 be turned off?
      ulong t2_on,  // How many milliseconds will t2 be turned on?
      ulong t2_off  // How many milliseconds will t2 be turned off?
    ) : t_switch {t0}, 
        t1 {ms_to_s(t1_on), ms_to_s(t1_off)},
        t2 {ms_to_s(t2_on), ms_to_s(t2_off)},
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
        is_t1 = !is_t1;
        last_update = time;
        Serial.println(time);
      }

      // Timer::update has its own `last_update`.
      if (is_t1) {
        t1.update(time);
      } else {
        t1.update(time);
      }
    }
  private:
    /* The timer is a struct that contains information about one of the three timers. */
    struct Timer {
      ulong time_on;      // 32 bits
      ulong time_off;     // 32 bits
      ulong last_update;  // 32 bits
      bool is_on; // A flag // padded so 32 bits.
      Timer(ulong on, ulong off) : time_on{on}, time_off {off}, is_on {false} {}

      void update(ulong time) {
        if (is_on) {
          // Check how many seconds should this be turned on.
          
        } else {
          
        }
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

Motor m0 = m0 = Motor(MOTOR_1_PIN, 
    s_to_ms(10), // Switch every 10 seconds
    s_to_ms(3),  // t1 is on for 3 seconds
    s_to_ms(2),  // t1 is on for 2 seconds
    s_to_ms(4),  // t2 is on for 4 seconds
    s_to_ms(1)  // t2 is off for 1 second.
);

void setup() {
  Serial.begin(230400);
}

void loop() {
  // put your main code here, to run repeatedly:
  ulong time { ms_to_s(micros()) };
  m0.update(time);
}
