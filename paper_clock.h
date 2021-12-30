//////////////////////////////////////////////////////////////
//
//                  C+(SERVO_MULTIPLIER*7)
//
//    D+(SERVO_MULTIPLIER*7)       B+(SERVO_MULTIPLIER*7)
//
//                  A+(SERVO_MULTIPLIER*7)
//
//    E+(SERVO_MULTIPLIER*7)       G+(SERVO_MULTIPLIER*7)
//
//                  F+(SERVO_MULTIPLIER*7)
//
//////////////////////////////////////////////////////////////
enum SegmentEnum
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    NUM_OF_SEGMENTS
};

enum PositionEnum
{
    MINUTE_RIGHT,
    MINUTE_LEFT,
    HOUR_RIGHT,
    HOUR_LEFT
};

void setup();

void loop();

void ResetTime();

void PushOut(const Adafruit_PWMServoDriver &pwm_controller, const int servo_motor, uint16_t positioned_outward);

void PushIn(const Adafruit_PWMServoDriver &pwm_controller, const int servo_motor, const uint16_t positioned_inward);

void Display(const PositionEnum &position, const unsigned int number, uint16_t positioned_inward, uint16_t positioned_outward);

void DisplayAmPm(const bool is_pm, const uint16_t positioned_inward, const uint16_t positioned_outward);

void DisplayTime(const DateTime &time, const uint16_t positioned_inward, const uint16_t positioned_outward);
