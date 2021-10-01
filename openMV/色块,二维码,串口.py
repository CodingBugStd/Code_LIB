import sensor, image, time , pyb

'''
 * 工程训练大赛智能搬运小车OpenMV代码
 * RGB 串口 色块识别 二维码识别
 * Creat by: 庞碧璋
 * 参考资料:https://docs.singtown.com/micropython/zh/latest/openmvcam/library/index.html#openmv-cam
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * date:    2021/2/??
'''

#初始化LED,UART,clock对象
RedLed = pyb.LED(1)
GreenLed = pyb.LED(2)
BlueLed = pyb.LED(3)
clock = time.clock()
usart = pyb.UART(3,115200,timeout_char=100)
usart.init(115200,bits=8,parity=None,stop=1,timeout_char=1000,read_buf_len=64)

#初始化感光元件
sensor.reset()
sensor.set_pixformat(sensor.RGB565)  #sensor.GRAYSCALE -> 灰度图(加快二维码扫描)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(50)
#sensor.set_contrast(2)              #对比度  -3~3
sensor.set_brightness(1)            #亮度
#sensor.set_auto_gain(True)         #自动增益
#sensor.set_auto_whitebal(False,(-5.243186, -6.02073, -2.498888))     #自动白平衡 实验室光照增益
#sensor.set_auto_whitebal(False,(-6.02073, -5.243186, -1.972561))       #实验产白天
#sensor.set_auto_whitebal(False,(-5.119987, -6.02073, -1.557143))
sensor.set_auto_whitebal(False)
sensor.set_auto_exposure(False)     #自动曝光


#色块阈值
Line_threshold = (0, 98)
''' 实验产白天

Red_threshold = (19, 49, 20, 93, 2, 127)
Green_threshold = (33, 61, -82, -25, 9, 107)
Blue_threshold = (28, 54, -11, 35, -119, -38)
'''
Red_threshold = (19, 56, 4, 76, 4, 127)
Green_threshold = (23, 66, -59, -19, 13, 49)
Blue_threshold = (19, 49, 0, 123, -119, -8)
Target_threshold = [Red_threshold,Green_threshold,Blue_threshold]
Target_List = [None,None,None,None] #过滤后的目标对象 前3元素为色块对象列表 第4元素为二维码对象(不是列表)
Target_proportion = (1.1,2.5)        #色块长宽比例过滤系数  H/W

#循迹取样矩形
'''
Rect1 = (125,15,30,30)
Rect2 = (155,15,30,30)
Rect3 = (185,15,30,30)
Rect4 = (125,45,30,30)
Rect5 = (155,45,30,30)
Rect6 = (185,45,30,30)
Rect7 = (125,75,30,30)
Rect8 = (155,75,30,30)
Rect9 = (185,75,30,30)
Rect10 = (95,45,30,30)
Rect11 = (215,45,30,30)
Rect12 = (215,45,30,30)
Rect13 = (215,45,30,30)
Rect14 = (215,45,30,30)
Rect15 = (215,45,30,30)
'''
StartPos = [70,15] #取样点起始坐标
Rect = [30,30]   #取样矩形大小 w h
NumRect = [7,7]     #取样矩阵的行和列

DataList_Len = NumRect[0]*NumRect[1]
RectData = [0,0]
while len(RectData) < DataList_Len+1:
    RectData.append(0)

OpenMVState = [0,0] #OpenMV状态第二位保留

#线训
def Find_Line():
    #img.histeq()
    #W:320 H:240
    Position = [0,0]
    RedLed.off()
    GreenLed.off()
    BlueLed.off()
    sensor.set_pixformat(sensor.GRAYSCALE) #设置为灰度图
    X_Num = 0
    Y_Num = 0
    temp = 0
    img.binary([Line_threshold]).mean(1)    #转化成位图 并 消除噪点
    while Y_Num < NumRect[1]:
        while X_Num < NumRect[0]:
            QY_Rect = [StartPos[0]+X_Num*Rect[0],StartPos[1]+Y_Num*Rect[1],Rect[0],Rect[1]]
            img.draw_rectangle(QY_Rect)
            vale = img.get_statistics(roi = QY_Rect).mean() #统计取样矩形的平均灰度值
            if vale > 40:
                RectData[temp] = 1
            else:
                RectData[temp] = 0
            temp+=1
            X_Num+=1
        X_Num=0
        Y_Num+=1
    temp1 = 0
    temp2 = 0
    #横线位置计算
    Usart_Send(bytearray(RectData))

def Usart_Read():
    if usart.any() != 0:
        sbuffer = int(usart.read(1))
        if sbuffer == 4:
            Target_List[3] = None
        else:
            if sbuffer != OpenMVState[0]:
                OpenMVState[0] = sbuffer
                if OpenMVState[0]==1 or OpenMVState[0]==2:
                    sensor.set_pixformat(sensor.RGB565)  #sensor.GRAYSCALE -> 灰度图(加快二维码扫描)
                elif OpenMVState[0]==3:
                    sensor.set_pixformat(sensor.GRAYSCALE) # use grayscale.

def QrCode_Find():
#RGB补光
    RedLed.on()
    GreenLed.on()
    BlueLed.on()
    if Target_List[3] == None :
        #img.binary([Light_threshold])   #根据阈值转化为位图,加快二维码识别
        QrCodes = img.find_qrcodes()
        if len(QrCodes)>0 :
            #扫描到二维码,载入Target_List[3]
            Target_List[3] = QrCodes[0]
            #RGB_Li()
    else:
        data = Target_List[3].payload()
        Usart_Send(data)

def TargetBlobs_Find():
#RGB补光
    RedLed.on()
    GreenLed.on()
    BlueLed.on()
    temp = 0
    while temp<3:
        Target_List[temp] = img.find_blobs([Target_threshold[temp]],merge = True,area_threshold = 600)
        if len(Target_List[temp]) > 0:
        #过滤列表中不满足长宽比的色块对象
            for Blob in Target_List[temp]:
                proportion = Blob.h()/Blob.w()
                #上物块可夹取范围 W:68~72
                if proportion < Target_proportion[0] or proportion > Target_proportion[1]:
                    Target_List[temp].remove(Blob)
                else:
                    img.draw_rectangle(Blob.rect(),thickness = 5,color = (175,0,175))
                    data = bytearray([temp,int(Blob.cx()/10)])
                    Usart_Send(data)
        temp += 1

def Usart_Send(Data):
    usart.write(bytearray([0xee]))
    usart.write(bytearray([OpenMVState[0]]))
    usart.write(Data)
    usart.write(bytearray([0xde]))

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.6)
    if OpenMVState[0] == 0:
        usart.write('A')
    elif OpenMVState[0] == 1:
        QrCode_Find()
    elif OpenMVState[0] ==2:
        TargetBlobs_Find()
    elif OpenMVState[0] == 3:
        Find_Line()
    Usart_Read()
    print(OpenMVState)
    print(clock.fps())

