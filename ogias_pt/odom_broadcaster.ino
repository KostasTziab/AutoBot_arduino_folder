/****************************
     ODOMETRY BROADCASTER
*****************************/
void odom_broadcast() {
  nav_msgs::Odometry odom_msg;
  odom_msg.header.stamp = nh.now();
  odom_msg.header.frame_id = odom;

  odom_msg.pose.pose.position.x = x / 1000; //convert to m
  odom_msg.pose.pose.position.y = y / 1000; //convert to m
  odom_msg.pose.pose.position.z = 0.0;
  odom_msg.pose.pose.orientation = tf::createQuaternionFromYaw(th);

  odom_msg.child_frame_id = base_link;
  odom_msg.twist.twist.linear.x = ((left_dist_mm + right_dist_mm) / 2) / 10;
  odom_msg.twist.twist.linear.y = 0.0;
  odom_msg.twist.twist.angular.z = ((right_dist_mm - left_dist_mm ) / 300)*100;
}
