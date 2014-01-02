from slim_pb2 import SlimMessage
from google.protobuf.internal import encoder
import socket

def send(msg):
    string = msg.SerializeToString()
    string = encoder._VarintBytes(len(string)) + string
    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect("/tmp/slim_socket") 
    s.send(string)
    s.close()
