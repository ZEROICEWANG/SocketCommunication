import socket
import uuid


def get_mac_address():
    mac=uuid.UUID(int = uuid.getnode()).hex[-12:]
    return ":".join([mac[e:e+2] for e in range(0, 11, 2)])


#获取本机电脑名
myname = socket.getfqdn(socket.gethostname())
#获取本机ip
myaddr = socket.gethostbyname(myname)
print(myname)
print(myaddr)
print(get_mac_address())

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # socket.AF_INET 指ipv4  socket.SOCK_STREAM 使用tcp协议
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.connect(('127.0.0.1', 10001))
while True:
    mess = input()
    sock.sendall(mess.encode('utf-8'))
    mess = sock.recv(1024)
    print(mess.decode('utf-8'))
