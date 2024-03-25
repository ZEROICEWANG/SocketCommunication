import random

class point:
    def __init__(self,name, number_point, info):
        self.name=int(name)
        self.point_cost={}
        self.path=[]
        messagelist=[]
        for i_ in range(len(info)):
            a=info[i_][0]
            if info[i_][0] == name:
                messagelist.append(info[i_])
            if info[i_][1]==name:
                messagelist.append(info[i_])
        j = 0
        for i in range(number_point):
            if i+1==self.name:
                self.point_cost[i+1]=0
                self.path.append('')
                continue
            if j<len(messagelist):
                if i+1==(messagelist[j])[1]:
                    self.point_cost[i+1]=(messagelist[j])[2]
                    j+=1
                elif i+1==(messagelist[j])[0]:
                    self.point_cost[i + 1] = (messagelist[j])[2]
                    j += 1
                else:
                    self.point_cost[i+1] = float('inf')
            else:
                self.point_cost[i+1]= float('inf')
            self.path.append('')


def ini_point():
    print('请输入节点数')
    number_point = input()
    number_point = number_point.strip('\n')
    number_point = int(number_point)
    print('请输入初始节点信息个数')
    number_tricell = input()
    number_tricell = int(number_tricell.strip('\n'))
    print('请输入节点信息, 以逗号间隔')
    message_of_cells = []
    for i in range(number_tricell):
        message_of_cell = []
        message = input()
        messagelist = message.split(',')
        message_of_cell.clear()
        for message in messagelist:
            message = int(message.strip(' '))
            message_of_cell.append(message)
        message_of_cells.append(message_of_cell)

    point_list = []
    for i in range(number_point):
        temp_point = point(i+1, number_point, message_of_cells)
        point_list.append(temp_point)
        print(str(temp_point.name)+str(temp_point.point_cost))


    return point_list,number_point


def updata(point1, point2):
    if point1.point_cost[point2.name]<float('inf'):
        cost=point1.point_cost[point2.name]
        for i in range(len(point1.path)):
            if point1.point_cost[i+1]!=float('inf'):
                if point1.point_cost[i+1]+cost<point2.point_cost[i+1]:
                    point2.point_cost[i+1]=point1.point_cost[i+1]+cost
                    if i+1==point1.name:
                        point2.path[i]=point1.path[point2.name-1]
                    else:
                        point2.path[i]=(point1.path[i]+'<-'+str(point1.name)+'<-'+point2.path[point1.name-1]).strip('<-')


if __name__=='__main__':
    point_list, number=ini_point();

    for i in range(len(point_list)-1):
        updata(point_list[i], point_list[i+1])
    updata(point_list[number-1], point_list[0])
    for i in range(number - 1):
        updata(point_list[number-i-1], point_list[number-i-2])

    for i in range(number):
        for j in range(number):
            if i==j:
                continue
            else:
                updata(point_list[i], point_list[j])
    print('\n\n\n\n\n\n\n\n\n')
    for i in range(number):
        print(point_list[i].name)
        print(point_list[i].point_cost)
        print(point_list[i].path)



