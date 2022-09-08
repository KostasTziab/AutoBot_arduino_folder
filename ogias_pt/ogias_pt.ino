/*********************************************
             LIBRARIES & MSGS
*********************************************/
#include <NewPing.h>
#include <Stepper.h>
#include <Encoder.h>
#include <ros.h>
#include <ros/time.h>
#include <std_msgs/UInt16.h>
#include<std_msgs/Float32.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>


/*********************************************
             ROS CONFIGURATIONS
*********************************************/
ros::NodeHandle  nh;

geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;
nav_msgs::Odometry odom_msg;


char base_link[] = "/base_link";
char odom[] = "/odom";


/*********************************************
               VARIABLES
*********************************************/

//total ticks per revolutuon
int countsPerRev = 1600;

//distances
float car_length = 300; // in mm
float circumference = 310; // in mm
float left_dist_mm = 0  ;
float right_dist_mm = 0 ;
float average_dist = 0;
float total_dist = 0;

//XYZ position
float x = 0;
float y = 0;
float th = 0;
float degrees_th = 0;

// steps for turn
int steps = 100 ;

// encoders
int rTick = 0;
int rTickPrev = 0;
int dr_tick = 0;

int lTick = 0;
int lTickPrev = 0;
int dl_tick = 0;

// time
float currentMillis = 0;
float prevMillis = 0;

//velocities
float vel_x;
float vel_z;
float vel_th;

//variables for callback funcrions
boolean start_p = false ;
float laserL;
float laserR;
float laserM;


/***********************************************
   More variables in case they needed

  float r = 50 ; //radius of the wheel in mm
************************************************/


/**************************************
               CLASS
**************************************/
NewPing Right_sonar(14, 15, 40 ); // sonar = (tigPin, echoPin, cm_range_of_activation);
/*NewPing Mid_sonar(5, 4, 30);*/
NewPing Left_sonar (8, 7, 30);
Stepper FB(steps , 9 , 11 , 10 , 12); //  για να πηγαίνουν και οι δυο ρόδες ευθεία και πίσω
Stepper turnLeft(steps , 10 , 9 , 11 , 12); //  για να στρίβει αρτιστερά και
Stepper turnRight(steps , 11 , 10 , 12 , 9); //    δεξιά
Stepper rightWheel(steps, 10, 9);
Stepper leftWheel(steps, 11, 12);
Encoder leftEnc(20 , 21);
Encoder rightEnc(2 , 3);


/*********************************************
             START CALLBACK
*********************************************/
void answerCb ( std_msgs::UInt16& start) {
  delay(20);
  if (start.data == 1) {
    start_p = true ;
  }
  else {
    start_p = false ;
  }
}


/*********************************************
             LASER CALLBACK
*********************************************/
void laserCb_left (std_msgs::Float32& left, std_msgs::Float32& mid, std_msgs::Float32& right) {
  laserL = left.data;
}

void laserCb_mid ( std_msgs::Float32& mid) {
  laserM = mid.data;
}

void laserCb_right ( std_msgs::Float32& right) {
  laserR = right.data;
}


/*********************************************
            VELOCITY CALLBACK
*********************************************/

void velCB( const geometry_msgs::Twist& vel)
{
  vel_x = vel.linear.x * 10 ;
  vel_z = vel.angular.z * 10;
}



/*********************************************
          SUBSCRIBERS AND PUBLISHER
*********************************************/

//ros::Subscriber<std_msgs::UInt16> sub_start("start", &answerCb);
ros::Subscriber<std_msgs::Float32>sub_laser_left("/leftLaser", &laserCb_left);
ros::Subscriber<std_msgs::Float32>sub_laser_mid("/midLaser", &laserCb_mid);
ros::Subscriber<std_msgs::Float32>sub_laser_right("/rightLaser", &laserCb_right);
ros::Subscriber<geometry_msgs::Twist> sub_vel("cmd_vel", velCB);
ros::Publisher odom_pub("odom", &odom_msg);

/************************************************
                   SETUP
*************************************************/

void setup() {
  FB.setSpeed(400);
  turnLeft.setSpeed(400);
  turnRight.setSpeed(400);
  Serial.begin(57600);
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  broadcaster.init(nh);
  //  nh.subscribe(sub_start);
  nh.subscribe(sub_laser_left);
  nh.subscribe(sub_laser_mid);
  nh.subscribe(sub_laser_right);
  nh.subscribe(sub_vel);
  nh.advertise(odom_pub);

}


/************************************************
                   LOOP
*************************************************/
void loop() {

  nh.spinOnce();
  currentMillis = millis();

  if ((currentMillis - prevMillis) > 100) {
    prevMillis = currentMillis ;
    if ((laserL > 20 && laserL < 15) && (laserM > 30 && laserM < 15) && (laserR > 30 && laserR < 15)) {
      FB.step(100);
      lTick = leftEnc.read();                 //read encoder values
      rTick = rightEnc.read();
      odometria();
      tf_broadcast();
      odom_broadcast();
      obstacle_avoidance();
    }

    if (laserL < 20 && laserL > 15) {
      turnRight.step(100);
      lTick = leftEnc.read();                 //read encoder values
      rTick = rightEnc.read();
      odometria();
      tf_broadcast();
      odom_broadcast();
      obstacle_avoidance();
    }

    if (laserR < 20 && laserR > 15) {
      turnLeft.step(100);
      lTick = leftEnc.read();                 //read encoder values
      rTick = rightEnc.read();
      odometria();
      tf_broadcast();
      odom_broadcast();
      obstacle_avoidance();
    }

  }

}
