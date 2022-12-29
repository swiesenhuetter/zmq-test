import zmq
import msgpack
import io


context = zmq.Context()
print("Connecting to server...")
socket = context.socket(zmq.PAIR)
socket.bind("tcp://*:5555")
print("Bind to 5555")

for request in range(10):
    print("recv...")
    message = socket.recv()
    unpack_data = io.BytesIO(message)
    unpacker = msgpack.Unpacker(unpack_data, raw=False)
    for unpacked in unpacker:
        print(unpacked)    
    
