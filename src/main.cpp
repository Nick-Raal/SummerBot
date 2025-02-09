#include "main.h"
#include "iostream"
#include "niic.hpp"

/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
/////


// Chassis constructor
Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {11, 2}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{-3, -6}

  // Inertial Port
  ,7

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,3.25

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,200

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,1

  // Uncomment if using tracking wheels
  /*
  // Left Tracking Wheel Ports (negative port will reverse it!)
  // ,{1, 2} // 3 wire encoder
  // ,8 // Rotation sensor

  // Right Tracking Wheel Ports (negative port will reverse it!)
  // ,{-3, -4} // 3 wire encoder      
  // ,-9 // Rotation sensor
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
);

//PID Tuning for the flywheel (unneccessary)
// l_fly(4, pros::E_MOTOR_GEAR_600, false, pros::E_MOTOR_ENCODER_DEGREES);
// r_fly(5, pros::E_MOTOR_GEAR_600, true, pros::E_MOTOR_ENCODER_DEGREES);
void set_fly(int input) {
  l_fly = input;
  r_fly = input;
}
// PID flyPID{0.45, 0, 0, 0, "Fly"};



// void fly_auto(double target) {
//   flyPID.set_target(target);
//   ez::exit_output exit = ez::RUNNING;
//   while (flyPID.exit_condition({l_fly, r_fly}, true) == ez::RUNNING) {
//     double output = flyPID.compute(l_fly.get_position());
//     set_fly(output);
//     target *= 2;
//     pros::delay(ez::util::DELAY_TIME);
//   }
//   // set_fly(0);
// }


//  void setWing(bool state){
//   wingR.set_value(state);
//   wingL.set_value(state);
// }

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  // ez::print_ez_template();
  
  pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  // Configure your chassis controls
  chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0); // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!
  exit_condition_defaults(); // Set the exit conditions to your own constants from autons.cpp!

  
  //p_small_exit_time time, in ms, before exiting p_small_error | p_small_error small error threshold| p_big_exit_time time, in ms, before exiting p_big_error | p_big_error big error threshold |p_velocity_exit_time time, in ms, for velocity to be 0 | p _mA_timeout time, in ms, for is_over_current to be true
  // flyPID.set_exit_condition(0, 0, 0, 0, 0, 00);
  l_fly.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  l_fly.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);


  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.set_left_curve_buttons (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used. 
  // chassis.set_right_curve_buttons(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({
    Auton("red side", drive_example),
    Auton("blue side", blue_auton),
    Auton("drive forward (near side)", drive_fwd),
    Auton("ONLY RUN FOR SKILLS", skills),
    // Auton("Example Turn\n\nTurn 3 times.", turn_example),
    // Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
    // Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
    // Auton("Swing Example\n\nSwing, drive, swing.", swing_example),
    // Auton("Combine all 3 movements", combining_movements),
    // Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
    // Auton("red side", red_side)
  });
  

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();

  //multithreading potential, idk if it's needed yet
  // Task drive(chassis.tank);
}



/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}



/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
  l_fly.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  l_fly.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}



/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.reset_pid_targets(); // Resets PID targets to 0
  chassis.reset_gyro(); // Reset gyro position to 0
  chassis.reset_drive_sensor(); // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency.

  ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.
}


//the method to do one button indexing

// pros::ADIDigitalIn checkTri('G');
//  index_distance(1);
//  indexMotor (19, pros::E_MOTOR_GEAR_200, true, pros::E_MOTOR_ENCODER_DEGREES);

int index_min = 15;

void doIndex(){
  //while the indexer is still moving back
  //while the indexer is cocked but there's no triball

  //to make sure this doesn't softlock the program
  int failsafe  = 0;
  //to move the indexer until it shoots
  while(failsafe < 3000 && index_distance.get() < index_min + 5){
    indexMotor.move_velocity(200);
    pros::delay(2);
    failsafe += 2;
  }
  pros::delay(20);
  failsafe = 0;
  while(failsafe < 3000 &&index_distance.get() >= index_min + 3){;
    indexMotor.move_velocity(200);
    failsafe += 2;
    pros::delay(2);
  }
  indexMotor.brake();
}

//task handler
void indexler(){
  while(1){
    if(master.get_digital_new_press(DIGITAL_R2)){
      doIndex();
    }
    pros::delay(2);
  }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

  pros::Task indexTask(indexler);
  indexMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  // This is preference to what you like to drive on.
  chassis.set_drive_brake(MOTOR_BRAKE_COAST);
  bool toggle { true }; //This variable will keep state between loops or function calls

  bool flyToggle = false; //same as above but for the flywheel
  int failsafe = 0;
  while(failsafe < 4000 && index_distance.get() >= index_min + 5){
    indexMotor.move_velocity(200);
    pros::delay(2);
    failsafe += 2;
  }
  indexMotor.brake();
  while (true) {
    //master.print(0, 0, "BRUH");

    
    chassis.tank(); // Tank control
    // chassis.arcade_standard(ez::SPLIT); // Standard split arcade
    // chassis.arcade_standard(ez::SINGLE); // Standard single arcade
    // chassis.arcade_flipped(ez::SPLIT); // Flipped split arcade
    // chassis.arcade_flipped(ez::SINGLE); // Flipped single arcade

    // . . .
    // Put more user control code here!
    // . . .

    //i still don't know if these control statements work (they do)
    // if (master.get_digital(DIGITAL_L1)) {
    //   l_fly.move_velocity(600);
    //   r_fly.move_velocity(600);
    // }
    // else /*if (master.get_digital(DIGITAL_L2))*/ {
    //   r_fly.brake();
    //   l_fly.brake();
    // }

    if(master.get_digital_new_press(DIGITAL_R1)){
      if(toggle){
        set_fly(400);
      }else{
        set_fly(0);
      }
      toggle = !toggle;
    }

    // if(master.get_digital_new_press(DIGITAL_R2)){
    //   // indexTask.join();
    //   doIndex();
    // }

    // set_fly(flyPID.compute(l_fly.get_position()));



    if(master.get_digital_new_press(DIGITAL_L1)) {
      setWing(!toggle);    //When false go to true and in reverse
      toggle = !toggle;    //Flip the toggle to match piston state
    } 
    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
