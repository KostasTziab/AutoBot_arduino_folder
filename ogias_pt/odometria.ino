/****************************
     ODOMETRY CALCULATION
*****************************/
void odometria () {

  //  ticks measured in each loop
  dl_tick = lTick - lTickPrev;        // for left wheel
  dr_tick = rTick - rTickPrev;        // for right wheel

  // calculation of distance
  left_dist_mm = (circumference * dl_tick ) / 1600;           // count distance travelled in mm
  right_dist_mm = (circumference * dr_tick ) / 1600 ;

  average_dist = (left_dist_mm + right_dist_mm) / 2;          // average distance travelled
  total_dist += average_dist;                                 // total distance travelled


  // calculation of angle
  th += ((right_dist_mm - left_dist_mm   ) / car_length)  ;   // measured in radians
  th = atan2(sin(th), cos(th));                               // transformation for -pi / pi

  degrees_th += ((right_dist_mm - left_dist_mm   ) / car_length) * (180 / PI) ;   // radians-to-degrees for easy handle in the program


  // calculating the x and y position that will be broadcasting with tf
  x += average_dist * cos(th) ;
  y += average_dist * sin(th) ;


  rTickPrev = rTick;
  lTickPrev = lTick;



}
