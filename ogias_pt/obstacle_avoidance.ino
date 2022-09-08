void obstacle_avoidance () {

  //int mid = Mid_sonar.ping_cm();
  int left = Left_sonar.ping_cm();
  int right = Right_sonar.ping_cm();

  if ( left == 0 && right == 0 /*&& mid ==0*/) { // while there is no low obstacle
    FB.step(100);                                 //move forward
    lTick = leftEnc.read();                         //read encoder values
    rTick = rightEnc.read();
    odometria();                                    // caluclate odometry
    tf_broadcast();                                 // broadcast transformation

  }

  //mid = Mid_sonar.ping_cm();                    //read ultrasonic sensors again and repeat
  left = Left_sonar.ping_cm();
  right = Right_sonar.ping_cm();                                // for 150 milliseconds

  if ( (left <= 40 && left >= 1) && (right <= 40 && right >= 1) /*|| (mid<= 40 && mid >= 1)*/ )  {// check which sensor detected obstacle
    FB.step(0);
    delay(120);
    while (degrees_th < 90) {
      turnRight.step(100);                     //rotate robot 90 deg right
      lTick = leftEnc.read();                 //read encoder values
      rTick = rightEnc.read();
      odometria();                            // caluclate odometry
      tf_broadcast();                         // broadcast transformation
      odom_broadcast();
    }

    degrees_th = 0;                          //reset degrees
    FB.step(0);                              //stop robot
    delay(120);                              // wait for 150 millis
    //mid = Mid_sonar.ping_cm();             // read again sensors for obstacles
    left = Left_sonar.ping_cm();
    right = Right_sonar.ping_cm();

    if (((left <= 40 && left >= 1) && (right <= 40 && right >= 1)) /*|| (mid<= 40 && mid >= 1)*/ ) { // if there is an obstacle after the
      // the first roatation
      while (degrees_th < 180) {                               // then rotate again 180 degrees to check the other side for obstacles
        turnLeft.step(100);                                     // execute the same actions as previous
        leftEnc.read();
        rightEnc.read();
        odometria();
        tf_broadcast();
        odom_broadcast();
      }
    }

    degrees_th = 0;                                     // again execute the same actions as previous
    FB.step(0);
    delay(120);
    //mid = Mid_sonar.ping_cm();
    left = Left_sonar.ping_cm();
    right = Right_sonar.ping_cm();

    if (((left <= 40 && left >= 1) && (right <= 40 && right >= 1)) /*|| (mid<= 40 && mid >= 1)*/) { // now if there is still an obstacle
      //this means that there is a dead end for the robot and it has to turn arround
      while (degrees_th < 90 ) {
        turnLeft.step(100);
        leftEnc.read();
        rightEnc.read();
        odometria();
        tf_broadcast();
        odom_broadcast();
      }
    }
    degrees_th = 0;
    FB.step(0);
    delay(150);

  }

  //mid = Mid_sonar.ping_cm();
  left = Left_sonar.ping_cm();
  right = Right_sonar.ping_cm();

  FB.step(0);
  delay(120);

  if (left <= 40 && left >= 1) {
    while (left != 0 ) {
      turnRight.step(100);
      left = Left_sonar.ping_cm();
    }
  }

  if (right <= 40 && right >= 1) {
    while (right != 0 ) {
      turnLeft.step(100);
      right = Right_sonar.ping_cm();
    }
  }
}
