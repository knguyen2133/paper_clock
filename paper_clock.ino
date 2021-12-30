#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "RTClib.h"
#include "paper_clock.h"

#define SERVO_IN 150  // This is the 'minimum' pulse length count (out of 4096)
#define SERVO_OUT 600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// Component Declaration
static RTC_DS1307 RTC;

static Adafruit_PWMServoDriver pwm_minute_controller = Adafruit_PWMServoDriver(0x40);
static Adafruit_PWMServoDriver pwm_hour_controller = Adafruit_PWMServoDriver(0x41);

// Structure Declaration
static DateTime displayed_time;

void setup()
{
    // RTC Clock initialization
    Wire.begin();
    RTC.begin();
    if (!RTC.isrunning())
    {
        Serial.println("RTC is NOT running!");
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    Serial.begin(9600);

    // Servo motors initialization
    pwm_minute_controller.begin();
    pwm_minute_controller.setOscillatorFrequency(27000000);
    pwm_minute_controller.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

    pwm_hour_controller.begin();
    pwm_hour_controller.setOscillatorFrequency(27000000);
    pwm_hour_controller.setPWMFreq(SERVO_FREQ);

    delay(10);

    ResetTime();
}

void loop()
{
    DateTime current_time = RTC.now();

    // Move the servo motors on both hour and minute for a synchronized effect
    if ((current_time.hour() != displayed_time.hour()) && (current_time.minute() != displayed_time.minute()))
    {
        for (uint16_t positioned_inward = SERVO_IN, positioned_outward = SERVO_OUT; positioned_inward < SERVO_OUT && positioned_outward > SERVO_IN; positioned_inward++, positioned_outward--)
        {
            DisplayTime(current_time, positioned_inward, positioned_outward);
            delay(25);
        }
        displayed_time = current_time;
    }

    delay(500);
}

void ResetTime()
{
    DateTime zero_time;

    for (uint16_t positioned_inward = SERVO_IN, positioned_outward = SERVO_OUT; positioned_inward < SERVO_OUT && positioned_outward > SERVO_IN; positioned_inward++, positioned_outward--)
    {
        DisplayTime(zero_time, positioned_inward, positioned_outward);
        delay(25);
    }
    displayed_time = zero_time;
}

void PushOut(const Adafruit_PWMServoDriver &pwm_controller, const int servo_motor, uint16_t positioned_outward)
{
    if (positioned_outward < SERVO_IN)
    {
        pwm_controller.setPWM(servo_motor, 0, positioned_outward);
    }
}

void PushIn(const Adafruit_PWMServoDriver &pwm_controller, const int servo_motor, const uint16_t positioned_inward)
{
    if (positioned_inward > SERVO_OUT)
    {
        pwm_controller.setPWM(servo_motor, 0, positioned_inward);
    }
}

void Display(const PositionEnum &position, const unsigned int number, uint16_t positioned_inward, uint16_t positioned_outward)
{
    int servo_multiplier = 0;
    auto pwm_controller = pwm_minute_controller;
    switch (position)
    {
    // No break is intentional by design
    case HOUR_RIGHT:
        pwm_controller = pwm_hour_controller;
    case MINUTE_RIGHT:
        servo_multiplier = 0;
        break;

    // No break is intentional by design
    case HOUR_LEFT:
        pwm_controller = pwm_hour_controller;
    case MINUTE_LEFT:
        servo_multiplier = NUM_OF_SEGMENTS;
        break;
    default:
        Serial.println("Invalid position requested to display");
        break;
    }

    int motor_a = A + servo_multiplier;
    int motor_b = B + servo_multiplier;
    int motor_c = C + servo_multiplier;
    int motor_d = D + servo_multiplier;
    int motor_e = E + servo_multiplier;
    int motor_f = F + servo_multiplier;
    int motor_g = G + servo_multiplier;

    switch (number)
    {
    case 0:
        PushIn(pwm_controller, motor_a, positioned_inward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushOut(pwm_controller, motor_d, positioned_outward);
        PushOut(pwm_controller, motor_e, positioned_outward);
        PushOut(pwm_controller, motor_f, positioned_outward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    case 1:
        PushIn(pwm_controller, motor_a, positioned_inward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushIn(pwm_controller, motor_c, positioned_inward);
        PushIn(pwm_controller, motor_d, positioned_inward);
        PushIn(pwm_controller, motor_e, positioned_inward);
        PushIn(pwm_controller, motor_f, positioned_inward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    case 2:
        PushOut(pwm_controller, motor_a, positioned_outward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushIn(pwm_controller, motor_d, positioned_inward);
        PushOut(pwm_controller, motor_e, positioned_outward);
        PushOut(pwm_controller, motor_f, positioned_outward);
        PushIn(pwm_controller, motor_g, positioned_inward);
        break;
    case 3:
        PushOut(pwm_controller, motor_a, positioned_outward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushIn(pwm_controller, motor_d, positioned_inward);
        PushIn(pwm_controller, motor_e, positioned_inward);
        PushOut(pwm_controller, motor_f, positioned_outward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    case 4:
        PushIn(pwm_controller, motor_a, positioned_inward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushOut(pwm_controller, motor_d, positioned_outward);
        PushOut(pwm_controller, motor_e, positioned_outward);
        PushOut(pwm_controller, motor_f, positioned_outward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    case 5:
        PushIn(pwm_controller, motor_a, positioned_inward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushOut(pwm_controller, motor_d, positioned_outward);
        PushOut(pwm_controller, motor_e, positioned_outward);
        PushOut(pwm_controller, motor_f, positioned_outward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    case 6:
        PushOut(pwm_controller, motor_a, positioned_outward);
        PushIn(pwm_controller, motor_b, positioned_inward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushOut(pwm_controller, motor_d, positioned_outward);
        PushOut(pwm_controller, motor_e, positioned_outward);
        PushOut(pwm_controller, motor_f, positioned_outward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    case 7:
        PushIn(pwm_controller, motor_a, positioned_inward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushIn(pwm_controller, motor_d, positioned_inward);
        PushIn(pwm_controller, motor_e, positioned_inward);
        PushIn(pwm_controller, motor_f, positioned_inward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    case 8:
        PushOut(pwm_controller, motor_a, positioned_outward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushOut(pwm_controller, motor_d, positioned_outward);
        PushOut(pwm_controller, motor_e, positioned_outward);
        PushOut(pwm_controller, motor_f, positioned_outward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    case 9:
        PushOut(pwm_controller, motor_a, positioned_outward);
        PushOut(pwm_controller, motor_b, positioned_outward);
        PushOut(pwm_controller, motor_c, positioned_outward);
        PushOut(pwm_controller, motor_d, positioned_outward);
        PushIn(pwm_controller, motor_e, positioned_inward);
        PushOut(pwm_controller, motor_f, positioned_outward);
        PushOut(pwm_controller, motor_g, positioned_outward);
        break;
    default:
        Serial.print("Received incorrect number to display");
        Serial.println(number);
    }
}

void DisplayAmPm(const bool is_pm, const uint16_t positioned_inward, const uint16_t positioned_outward)
{
    constexpr int AM_SERVO_MOTOR = 28;
    constexpr int PM_SERVO_MOTOR = 29;

    if ((positioned_outward < (SERVO_IN / 2)) && (positioned_inward > (SERVO_OUT)))
    {
        if (!is_pm) //AM
        {
            pwm_hour_controller.setPWM(AM_SERVO_MOTOR, 0, positioned_inward);
            pwm_hour_controller.setPWM(PM_SERVO_MOTOR, 0, positioned_outward);
            delay(25);
        }
        else
        {
            pwm_hour_controller.setPWM(AM_SERVO_MOTOR, 0, positioned_outward);
            pwm_hour_controller.setPWM(PM_SERVO_MOTOR, 0, positioned_inward);
            delay(25);
        }
    }
}

void DisplayTime(const DateTime &time, const uint16_t positioned_inward, const uint16_t positioned_outward)
{
    // AM and PM on the colon
    DisplayAmPm(time.isPM(), positioned_inward, positioned_outward);

    // Display left hour
    Display(HOUR_LEFT, time.twelveHour() / 10, positioned_inward, positioned_outward); // Only displays 0 or 1
    // Display right hour
    Display(HOUR_RIGHT, time.twelveHour() % 10, positioned_inward, positioned_outward);
    // Display left minute
    Display(MINUTE_LEFT, time.minute() / 10, positioned_inward, positioned_outward);
    // Display right minute
    Display(MINUTE_RIGHT, time.minute() % 10, positioned_inward, positioned_outward);
}
