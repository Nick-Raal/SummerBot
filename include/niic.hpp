#pragma once

#include "pros/motors.hpp"
#include "pros/distance.hpp"
#include "pros/adi.hpp"

static pros::Motor l_fly(4, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_DEGREES);
static pros::Motor r_fly(5, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_DEGREES);
static pros::Distance index_distance(1);
static pros::Motor indexMotor(19, pros::E_MOTOR_GEAR_200, false, pros::E_MOTOR_ENCODER_DEGREES);
static pros::ADIDigitalOut wingR ('C', false);
static pros::ADIDigitalOut wingL ('F', false);

static void setWing(bool state){
    wingR.set_value(state);
    wingL.set_value(state);
}