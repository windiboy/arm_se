#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import sys
reload(sys)
sys.setdefaultencoding('utf8')
import os
import rospy

from respeaker_audio import RespeakerAudio
from std_msgs.msg import String


class VoiceController(object):
    def __init__(self, node_name):
        self.node_name = node_name
        rospy.init_node(node_name)
        rospy.on_shutdown(self.shutdown)
        # self.respeaker_interface = RespeakerInterface()
        self.respeaker_audio = RespeakerAudio()
        # self.ask_pub = rospy.Publisher('cmd_msg', String, queue_size=5)

    def shutdown(self):
        # self.respeaker_interface.close()
        self.respeaker_audio.stop()


if __name__ == '__main__':
    voice_controller = VoiceController("voice_controller")
    rate = rospy.Rate(100)

    while not rospy.is_shutdown():
        text = voice_controller.respeaker_audio.record()
        if text.find("办公室") >= 0:
            rospy.set_param('command', "start")
        if text.find("抓取") >= 0:
            rospy.set_param('command', "pitch")
        if text.find("停止行动") >= 0:
            rospy.set_param('command', "wait")
        # direction = voice_controller.respeaker_interface.direction
        print(text)
        # print(direction)
        rate.sleep()






