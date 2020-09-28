#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import pyaudio
import wave
from tqdm import tqdm
import time
import rospy
import os

class Speaker_SE(object):
    def __init__(self):
        self.pre_command = rospy.get_param("command")
        self.cur_command = self.pre_command

    def play_audio_callback(self, wave_path):
        CHUNK = 1024
        wf = wave.open(wave_path, 'rb')
        # instantiate PyAudio (1)
        p = pyaudio.PyAudio()
        def callback(in_data, frame_count, time_info, status):
            data = wf.readframes(frame_count)
            return (data, pyaudio.paContinue)
        # open stream (2)
        stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                        channels=wf.getnchannels(),
                        rate=wf.getframerate(),
                        output=True,
                        stream_callback=callback)
        # read data
        stream.start_stream()
        while stream.is_active():
            time.sleep(0.1)
        # stop stream (4)
        stream.stop_stream()
        stream.close()
        # close PyAudio (5)
        p.terminate()


if __name__ == "__main__":
    rospy.init_node('speaker_se', anonymous=True)
    speak = Speaker_SE()
    r = rospy.Rate(1)
    dir = os.getcwd()
    dir = dir[:len(dir)-7]
    while not rospy.is_shutdown():
        speak.cur_command = rospy.get_param("command")
        print("{}-{}".format(speak.pre_command,speak.cur_command))
        if speak.pre_command == "error" or speak.cur_command == "error":
            speak.play_audio_callback(dir+"/audio/error.wav")
        if speak.pre_command != speak.cur_command:
            try:
                speak.play_audio_callback(dir+"/audio/"+speak.pre_command+"-"+speak.cur_command+".wav")
            except:
                print("File doesn't exist!!!")
            speak.pre_command = speak.cur_command
        r.sleep()
