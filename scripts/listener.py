#!/usr/bin/env python
import rospy
from std_msgs.msg import String

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)
    if("办公室" in data.data):
        print("heared!!!")

def listener():

    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("audio_pub", String, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()