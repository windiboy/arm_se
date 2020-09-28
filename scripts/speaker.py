import pyaudio
import wave
from tqdm import tqdm
import time
def play_audio_callback(wave_path):
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
play_audio_callback("../audio/111.wav")