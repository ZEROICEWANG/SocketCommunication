import socket
import multiprocessing
from multiprocessing import Manager
import select
import os
import time

#LOCAL_IP=socket.gethostbyname(socket.gethostname())
LOCAL_IP='127.0.0.1'

class client:
    def __init__(self, connect, account, IP, mac, username):
        self.account=account
        self.username=username
        self.connect=connect
        self.ip=IP
        self.mac=mac


    def get_friends(self):
        if os.path.exists(os.path.join(os.getcwd(), self.account,'friendlist.txt')):
            with open(os.path.join(os.getcwd(),self.account,'friendlist.txt'),'r+') as file:
                message=file.read()
                if len(message)==0:
                    message=''
                return message
        else:
            file=open(os.path.join(os.getcwd(),self.account,'friendlist.txt'),'w+')
            file.close()
            return ''

    def save_friends(self,name,account):
        friends=self.get_friends()
        friends+=('\n'+name+'='+account)
        friends=friends.strip('\n')
        if os.path.exists(os.path.join(os.getcwd(),self.account,'friendlist.txt')):
            with open(os.path.join(os.getcwd(),self.account,'friendlist.txt'),'w+') as file:
                file.write(friends)
                file.flush()

    def sendMes(self, sender_name,sender_account, message,):
        try:
            data=str('chat|friendname='+sender_name+'\'account='+sender_account+'\'message='+message).encode('utf-8')
            self.connect.sendall(data)
            return True
        except:
            return False;



def save_message():
    print('save message')
    part_info = ''
    for info in unsending_list:
        part_info += unsending_message[info]
        part_info += '\n'
    file = open(os.path.join(os.getcwd(), 'waiting_for_send_message.txt'), 'w')
    file.write(part_info)
    file.close()


def load_user_list():
    os.chdir('E://')
    if not os.path.exists(os.path.join(os.getcwd(),'CS')):
        os.mkdir(os.path.join(os.getcwd(),'CS'))

    for path, floderlist, filelist in os.walk('E://CS'):
        for floder in floderlist:
            userlist.append(floder)
        break


def load_unsending_message():
    os.chdir('E://CS')
    if os.path.exists(os.path.join(os.getcwd(),'waiting_for_send_message.txt')):
        with open(os.path.join(os.getcwd(),'waiting_for_send_message.txt'),'r') as file:
            messagelist=file.read();
            if len(messagelist) != 0:
                messagelist=messagelist.split('\n')
                for message in messagelist:
                    unsending_message[message.split('|')[0].split('=')[1]] = message.split('|')[1]
                    unsending_list.append(message.split('|')[0].split('=')[1])
    else:
        file=open(os.path.join(os.getcwd(),'waiting_for_send_message.txt'),'w')
        file.close()


def is_port_availabel(ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((ip, port))
        s.close()
        return False
    except:
        s.close()
        return True


def check_port_id(PORT_ARRAY_):
    global  LOCAL_IP
    temparray = []
    for i in range(1, len(PORT_ARRAY_)):
        if not is_port_availabel(LOCAL_IP, PORT_ARRAY_[i]):
            temparray.append(PORT_ARRAY_[i])
    for i in range(len(temparray)):
        PORT_ARRAY_.remove(temparray[i])


def waiting_for_connection(port, connectlist_):
    global LOCAL_IP
    print('waiting')
    sock=socket.socket()
    sock.bind((LOCAL_IP,port))
    sock.listen(2)
    connect,(host, IP)=sock.accept()
    connectlist_.append(connect)


def regist_process(message, connect, userlist_):
    username=message.split('\'')[0].split('=')[1]
    password=message.split('\'')[1].split('=')[1]
    print(username+'请求注册')
    for i in range(pow(10, 5)):
        account_int=10000+len(userlist_)+i
        account=str(account_int)
        if account in userlist_:
            continue
        else:
            if os.path.exists(os.path.join(os.getcwd(),account)):
                continue
            else:
                os.mkdir(os.path.join(os.getcwd(),account))
                with open(os.path.join(os.getcwd(),account,'info.txt'),'w+') as file:
                    file.write(username+'\n'+account+'\n'+password)
                    file.flush()
                connect.sendall(('register|statue=permit\'account=' + account).encode('utf-8'))
                userlist_.append(account)
                return True
    connect.sendall(('register|statue=deny').encode('utf-8'))
    return False


def login_prcess(message, connect, port_account_, userlist_, account_clientDic_, logined_account_, unsending_list_, unsending_message_):
    messagelist=message.split('\'')
    account=messagelist[0].split('=')[1]
    password=messagelist[1].split('=')[1]
    mac=messagelist[2].split('=')[1]
    if account in userlist_:
        with open(os.path.join(os.getcwd(),account,'info.txt')) as file:
            inf=file.read()
            if account in logined_account_:
                connect.sendall(('login|statue=deny').encode('utf-8'))
                return 0
            if  account == inf.split('\n')[1]:
                if password == inf.split('\n')[2]:
                    port_account_[connect.getsockname()[1]] = account
                    cli=client(connect, account, connect.getpeername()[0], mac, inf.split('\n')[0])
                    account_clientDic_[account] = cli
                    logined_account_.append(account)
                    infotest=account_clientDic_[account].get_friends()
                    infotest=infotest.replace('\n',':')
                    if len(infotest)==0:
                        infotest+='end'
                    else:
                        infotest += ':end'
                    for account in logined_account_:
                        print('已登录账号：')
                        print(account)
                        print('--------------------------')
                    connect.sendall(('login|statue=permit\'username='+inf.split('\n')[0]+'|friends\''+infotest).encode('utf-8'))
                    print('login|statue=permit|friends\''+infotest)
                    if account in unsending_list_:
                        messlist=unsending_message_[account].split('\'')
                        for mess in messlist:
                            time.sleep(1)
                            action =mess.split(':')[0].split('=')[1]
                            sender=mess.split(':')[1].split('=')[1]
                            messtosend=mess.split(':')[2].split('=')[1]
                            if action == 'chat':
                                connect.sendall(('chat|friendname='+account_clientDic_[sender].username+'\'account='+sender+'\'message='+messtosend).encode('utf-8'))
                            elif action == 'add':
                                connect.sendall(('add|account=' + sender + '\'username=' + messtosend).encode('utf-8'))
                        for i in range(len(unsending_list_)):
                            if account ==unsending_list_[i]:
                                unsending_list_.pop(i)
                                break
                        del unsending_message_[account]
                else:
                    connect.sendall(('login|statue=deny').encode('utf-8'))
                    print('login|statue=deny')
            else:
                connect.sendall(('login|statue=deny').encode('utf-8'))
                print('login|statue=deny')
    else:
        connect.sendall(('login|statue=deny').encode('utf-8'))
        print('login|statue=deny')


def chat_process(message, account_clientDic_,  logined_account_, unsending_list_, unsending_message_):
    #chat|sender=sender’account=account’message=message
    messagelist = message.split('\'')
    sender=messagelist[0].split('=')[1]#senderaccount
    recv=messagelist[1].split('=')[1]#recvaccount
    mess=messagelist[2].split('=')[1]
    if recv in logined_account_:
        account_clientDic_[recv].sendMes(account_clientDic_[sender].username, sender, mess)
    else:
        if recv in unsending_list_:
            unsending_message_[recv]=unsending_message_[recv]+'\'action=chat:from='+sender+':message='+mess
        else:
            unsending_list_.append(recv)
            unsending_message_[recv]='action=chat:from='+sender+':message='+mess


def IP_process(message,connect, account_clientDic_, logined_account_):
    account=message.split('=')[1]
    if account in logined_account_:
        ip=account_clientDic_[account].ip
        mess='IP|account='+account+'\'ip='+ip+'\'mac='+account_clientDic_[account].mac
    else:
        mess = 'IP|account=' + account + '\'ip=对方未登录\'mac=none'
    connect.sendall(mess.encode('utf-8'))


def search_process(message, connect, account_clientDic_, userlist_,logined_account_):
    account=message.split('=')[1]
    if account in userlist_:
        if account in logined_account_:
            cli=account_clientDic_[account]
            mess='search|username='+cli.username+'\'account='+cli.account
            connect.sendall(mess.encode('utf-8'))
        else:
            with open(os.path.join(os.getcwd(), account, 'info.txt')) as file:
                inf = file.read()
                username=inf.split('\n')[0]
                account == inf.split('\n')[1]
                mess = 'search|username=' + username + '\'account=' + account
                connect.sendall(mess.encode('utf-8'))

    else:
        mess1 = 'search|username=查无此人\'account=00000'
        connect.sendall(mess1.encode('utf-8'))


def add_process(message, account_clientDic_,  logined_account_, unsending_list_, unsending_message_):
    sender=message.split('\'')[0].split('=')[1]
    recv=message.split('\'')[1].split('=')[1]
    sender_username=account_clientDic_[sender].username
    print('add:sender='+sender+'username:'+sender_username)
    if recv in logined_account_:
        account_clientDic_[recv].connect.sendall(('add|account='+sender+'\'username='+sender_username).encode('utf-8'))
    else:
        mess='action=add:from='+sender+':username='+sender_username
        if recv in unsending_list_:
            unsending_message_[recv]=unsending_message_[recv]+'\''+mess
        else:
            unsending_list_.append(recv)
            unsending_message_[recv]=mess


def result_process(message, account_clientDic_, logined_account_):
    statue=message.split('\'')[0].split('=')[1]
    if statue == 'permit':
        asker = message.split('\'')[1].split('=')[1]
        os.chdir('E://CS')
        with open(os.path.join(os.getcwd(),asker,'info.txt'),'r') as file:
            inf = file.read()
            asker_name = inf.split('\n')[0]
        recv = message.split('\'')[2].split('=')[1]
        recv_name = account_clientDic_[recv].username
        account_clientDic_[recv].save_friends(asker_name, asker)
        if asker in logined_account_:
            account_clientDic_[asker].save_friends(recv_name, recv)
            account_clientDic_[asker].connect.sendall(('result|username='+recv_name+'\'account='+recv).encode('utf-8'))
        else:
            os.chdir('E://CS')
            with open(os.path.join(os.getcwd(), asker, 'friendlist.txt'), 'r+') as file:
                message = file.read()
                message+=('\n'+recv_name + '=' + recv)
            message=message.strip('\n')
            with open(os.path.join(os.getcwd(), asker, 'friendlist.txt'), 'w+') as file:
                file.write(message)
                file.flush()


def delete_process(message):
    #delete|sender=account’recv= account
    sender=message.split('\'')[0].split('=')[1]
    with open(os.path.join(os.getcwd(), sender,'info.txt'),'r') as file:
        inf=file.read()
        sender_name = inf.split('\n')[0]

    recv=message.split('\'')[1].split('=')[1]
    with open(os.path.join(os.getcwd(),recv,'info.txt'),'r') as file:
        inf=file.read()
        recv_name = inf.split('\n')[0]
    sender_friend=sender_name+'='+sender
    recv_friend=recv_name + '=' + recv

    with open(os.path.join(os.getcwd(), sender, 'friendlist.txt'), 'r+') as file:
        message = file.read()
        friendslist = message.split('\n')
    newfriends = ''
    for friend in friendslist:
        if friend == recv_friend:
            continue
        newfriends += ('\n' + friend)
        newfriends=newfriends.strip('\n')
    with open(os.path.join(os.getcwd(), sender, 'friendlist.txt'), 'w') as file:
        file.write(newfriends)
        file.flush()

    with open(os.path.join(os.getcwd(), recv, 'friendlist.txt'), 'r') as file:
        message = file.read()
        friendslist = message.split('\n')
    newfriends = ''
    for friend in friendslist:
        if friend == sender_friend:
            #friendslist.remove(friend)
            continue
        newfriends += ('\n' + friend)
    newfriends=newfriends.strip('\n')
    with open(os.path.join(os.getcwd(), recv, 'friendlist.txt'), 'w') as file:
        file.write(newfriends)
        file.flush()


def refresh_process(message, connect, account_clientDic_):
    account=message.split('=')[1]
    infotest = account_clientDic_[account].get_friends()
    infotest = infotest.replace('\n', ':')
    if len(infotest) == 0:
        infotest += 'end'
    else:
        infotest += ':end'
    connect.sendall(('refresh|friends\'' + infotest).encode('utf-8'))


def analyze_message(message, connect, port_account_,account_clientDic_, logined_account_, unsending_list_, unsending_message_, userlist_):
    messagelist=message.split('|')
    #print(connect.getpeername()[1])
    if messagelist[0] == 'register':
        regist_process(messagelist[1], connect, userlist_)
    elif messagelist[0] == 'login':
        login_prcess(messagelist[1],connect,port_account_, userlist_,  account_clientDic_, logined_account_, unsending_list_, unsending_message_)
    elif messagelist[0] == 'chat':
        chat_process(messagelist[1], account_clientDic_,  logined_account_, unsending_list_, unsending_message_)
    elif messagelist[0] == 'IP':
        IP_process(messagelist[1],connect, account_clientDic_, logined_account_)
    elif messagelist[0] == 'search':
        search_process(messagelist[1],connect, account_clientDic_, userlist_, logined_account_)
    elif messagelist[0] == 'add':
        add_process(messagelist[1], account_clientDic_, logined_account_, unsending_list_, unsending_message_)
    elif messagelist[0] == 'result':
        result_process(messagelist[1], account_clientDic_, logined_account_)
    elif messagelist[0] == 'delete':
        delete_process(messagelist[1])
    elif messagelist[0]=='refresh':
        refresh_process(messagelist[1], connect, account_clientDic_)
    print('unsending:')
    print(str(unsending_message_))


def delete_connect(connect):
    port=connect.getsockname()[1]
    print('clear connect!')
    if port in port_account.keys():
        account = port_account[port]
        try:
            del account_clientDic[account]
            del port_account[port]
        except:
            print('get a error')
        for i in range(len(logined_account)):
            if account == logined_account[i]:
                logined_account.pop(i)
                break
        for account in logined_account:
            print('已登录账号：')
            print(account)
            print('--------------------------')


    for i in range(len(connectlist)):
        if port ==connectlist[i].getsockname()[1]:
            connectlist.pop(i)
            break
    for i in range(len(PORT_ARRAY)):
        if port == PORT_ARRAY[i]:
            PORT_ARRAY.pop(i)
            break


def server():
    global LOCAL_IP
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # socket.AF_INET 指ipv4  socket.SOCK_STREAM 使用tcp协议
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # 设置端口
    sock.bind((LOCAL_IP, PORT_ARRAY[0]))  # 绑定端口
    sock.listen(100)
    connectlist.append(sock)
    print('服务端启动，等待链接')
    while True:
        readlist ,writelist ,errorlist =select.select(connectlist, [], [])
        print('readlist:'+str(len(readlist)))
        for connect_i in readlist:
            laddr=connect_i.getsockname()
            if laddr[1] == PORT_ARRAY[0]:
                connect_main, (host, port) = sock.accept()
                personalport = 10001
                print(str(host) + ' ' + str(port) + ': is coming')
                for i in range(55535):
                    personalport += i
                    if personalport in PORT_ARRAY:
                        continue
                    elif not is_port_availabel(LOCAL_IP, personalport):
                        continue
                    else:
                        try:
                            PORT_ARRAY.append(personalport)
                            p=multiprocessing.Process(target=waiting_for_connection,args = (personalport, connectlist))
                            p.start()
                            connect_main.sendall(str(personalport).encode('utf-8'))
                            print('端口已发送:' + str(personalport))
                            time.sleep(2)
                            break
                        except:
                            print('get a error on recv a host request')
            else:
                try:
                    message = connect_i.recv(1024).decode('utf-8')
                    if len(message) ==0:
                        print('get null message:'+message+':')
                        delete_connect(connect_i)
                    else:
                        print('get a message:'+message)
                    p = multiprocessing.Process(target=analyze_message, args=(
                        message, connect_i, port_account, account_clientDic, logined_account, unsending_list,
                        unsending_message, userlist))
                    p.start()
                except:
                    delete_connect(connect_i)
                    print('lost a connect')


if __name__=='__main__':
    print('LOCAL_IP: ' + LOCAL_IP)
    with Manager() as manager:
        PORT_ARRAY = manager.list([10001])
        port_account=manager.dict()
        account_clientDic = manager.dict()  # account:client
        connectlist = manager.list()  # connct
        logined_account = manager.list()  # account
        unsending_list = manager.list()
        unsending_message = manager.dict()
        userlist = manager.list()

        load_user_list()
        load_unsending_message()
        server()




