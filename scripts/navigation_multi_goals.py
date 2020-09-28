#!/usr/bin/env python
import rospy

import actionlib
from actionlib_msgs.msg import *
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseWithCovarianceStamped
from tf_conversions import transformations
from math import pi
from std_msgs.msg import String

class navigation_demo:
    def __init__(self):
        self.set_pose_pub = rospy.Publisher('/initialpose', PoseWithCovarianceStamped, queue_size=5)
        self.arrive_pub = rospy.Publisher('/robot_voice/tts_topic',String,queue_size=10)
        self.move_base = actionlib.SimpleActionClient("move_base", MoveBaseAction)
        self.move_base.wait_for_server(rospy.Duration(60))

    def set_pose(self, p):
        if self.move_base is None:
            return False

        x, y, th = p

        pose = PoseWithCovarianceStamped()
        pose.header.stamp = rospy.Time.now()
        pose.header.frame_id = 'map'
        pose.pose.pose.position.x = x
        pose.pose.pose.position.y = y
        q = transformations.quaternion_from_euler(0.0, 0.0, th/180.0*pi)
        pose.pose.pose.orientation.x = q[0]
        pose.pose.pose.orientation.y = q[1]
        pose.pose.pose.orientation.z = q[2]
        pose.pose.pose.orientation.w = q[3]

        self.set_pose_pub.publish(pose)
        return True

    def _done_cb(self, status, result):
        rospy.loginfo("navigation done! status:%d result:%s"%(status, result))
        arrive_str = "arrived"
        self.arrive_pub.publish(arrive_str)

    def _active_cb(self):
        rospy.loginfo("[Navi] navigation has be actived")

    def _feedback_cb(self, feedback):
        rospy.loginfo("[Navi] navigation feedback\r\n%s"%feedback)

    def goto(self, p):
        rospy.loginfo("[Navi] goto %s"%p)
        arrive_str = "going to next point"
        self.arrive_pub.publish(arrive_str)
        goal = MoveBaseGoal()

        goal.target_pose.header.frame_id = 'map'
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = p[0]
        goal.target_pose.pose.position.y = p[1]
        goal.target_pose.pose.position.z = p[2]
        # q = transformations.quaternion_from_euler(0.0, 0.0, p[2]/180.0*pi)
        goal.target_pose.pose.orientation.x = p[3]
        goal.target_pose.pose.orientation.y = p[4]
        goal.target_pose.pose.orientation.z = p[5]
        goal.target_pose.pose.orientation.w = p[6]

        self.move_base.send_goal(goal, self._done_cb, self._active_cb, self._feedback_cb)
        result = self.move_base.wait_for_result(rospy.Duration(60))
        if not result:
            self.move_base.cancel_goal()
            rospy.loginfo("Timed out achieving goal")
        else:
            state = self.move_base.get_state()
            if state == GoalStatus.SUCCEEDED:
                rospy.loginfo("reach goal %s succeeded!"%p)

        return True

    def cancel(self):
        self.move_base.cancel_all_goals()
        return True


if __name__ == "__main__":
    rospy.init_node('navigation_demo', anonymous=True)
    navi = navigation_demo()
    r = rospy.Rate(1)
    rospy.set_param('command', "wait")
    plists = []
    plists.append([6.918, 1.723, 0, -0.006, 0.006, 0.658, 0.753])
    backlists = []
    backlists.append([6.198, 0.784, 0, 0, 0.004, 0.948, -0.317])
    backlists.append([3.056, 0.592, 0, -0.001, 0.00, 0.999, -0.0464])
    backlists.append([0.817, -0.819, 0, 0, 0.002, 0.969, 0.246])
    while not rospy.is_shutdown():
        if rospy.get_param('command') == "start":
            for p in plists:
                navi.goto(p)
            rospy.set_param("command", "pitch")
            rospy.loginfo("next step pitching")
        if rospy.get_param('command') == "back":
            for p in backlists:
                navi.goto(p)
            break
        r.sleep()
