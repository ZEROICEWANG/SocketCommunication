import socket
#socket.AF_INET, socket.SOCK_STREAM
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # socket.AF_INET 指ipv4  socket.SOCK_STREAM 使用tcp协议
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # 设置端口
sock.bind(('192.168.137.1', 10001))  # 绑定端口
sock.listen(100)
print('waiting')

while True:
    print('waiting...')
    connect, adr = sock.accept()
    print('get a connection')
    while True:
        mess=connect.recv(1024)
        mess=mess.decode('utf-8')
        print(mess)
        if mess=="":
            print("lost a connection")
            break


