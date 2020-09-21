#!/usr/bin/python
# -*- coding: UTF-8 -*-
import subprocess
import audioop
import rospy
from aip import AipSpeech
from std_msgs.msg import String


def record_audio():
    cmd = ['arecord', '-d', '5', '-r', '16000', '-c', '1', '-f', 'S16_LE', 'audio.wav']
    process = subprocess.Popen(cmd, stdout = subprocess.PIPE)
    for i in range(16):
        audio = process.stdout.read(160*2)
        # print(audioop.rms(audio,2))
    process.stdout.close()
    process.terminate()

# 读取文件
def get_file_content(filePath):
    with open(filePath, 'rb') as fp:
        return fp.read()

def play_audio():
    """ 你的 APPID AK SK """
    APP_ID = '18797850'
    API_KEY = 'QArLMWEmt0tpRQ6EBG9TNR47'
    SECRET_KEY = 'gtZL6OSYO9G32pQYF85iirCMaiV35U6O'
    FILE_PATH = 'audio.wav'
    client = AipSpeech(APP_ID, API_KEY, SECRET_KEY)
    # 识别本地文件
    result = client.asr(get_file_content(FILE_PATH), 'wav', 16000, {
        'dev_pid': 1537,
    })
    return result['result']

def talker():
    pub = rospy.Publisher('audio_pub', String, queue_size=10)
    rospy.init_node('audio', anonymous=True)
    while not rospy.is_shutdown():
        record_audio()
        result = play_audio()[0]
        print(result)
        pub.publish(result)


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass