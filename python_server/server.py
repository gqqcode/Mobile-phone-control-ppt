# Echo server program

from socket import*
from time import ctime
import win32com.client
import win32api
import win32con
import time
import pythoncom
from ctypes import *

class PowerPointControler:
    
    def __init__(self):
        pythoncom.CoInitialize()
        self.app = win32com.client.Dispatch("PowerPoint.Application")
    def fullScreen(self):
        #全屏播放
        if self.hasActivePresentation():
            #使用Run方法创建一个新的幻灯片放映窗口并将其添加到SlideShowWindows集合
            self.app.ActivePresentation.SlideShowSettings.Run()#该方法用于运行指定演示文稿的幻灯片放映。
            return self.getActivePresentationSlideIndex()
    def closeFull(self):#结束第一个幻灯片放映窗口中的幻灯片放映
        if self.app.ActivePresentation.SlideShowSettings.Run():
            self.app.SlideShowWindows(1).View.Exit()
    def gotoSlide(self,index):
        #跳转到指定的页面
        if self.hasActivePresentation():
                try:
                    self.app.ActiveWindow.View.GotoSlide(index)
                    return self.app.ActiveWindow.View.Slide.SlideIndex
                except:
                    self.app.SlideShowWindows(1).View.GotoSlide(index)
                    return self.app.SlideShowWindows(1).View.CurrentShowPosition
    def nextPage(self):#下一页ppt
        if self.hasActivePresentation():
            count = self.getActivePresentationSlideCount()
            index = self.getActivePresentationSlideIndex()
            return count if index >= count else self.gotoSlide(index+1)
    def prePage(self):#上一页ppt
        if self.hasActivePresentation():
            index =  self.getActivePresentationSlideIndex()
            return index if index <= 1 else self.gotoSlide(index-1)
    def drawLine(self, x, y):
        index =  self.getActivePresentationSlideIndex()
        windll.user32.SetCursorPos(x, y)
        #参数1位x的起点  第二个参数是y的起点   这两个参数决定了起始的位置
        #参数3是x的终点位置      第四个参数是 y的终点位置
        self.app.SlideShowWindows(index).View.DrawLine(x, y, x + 500, y)
    def getActivePresentationSlideIndex(self):
        #得到活跃状态的PPT当前的页数
        if self.hasActivePresentation():
            try:
                index = self.app.ActiveWindow.View.Slide.SlideIndex
            except:
                index = self.app.SlideShowWindows(1).View.CurrentShowPosition
        return index
    def getActivePresentationSlideCount(self):
        #返回处于活跃状态的PPT的页面总数
        return self.app.ActivePresentation.Slides.Count
    def getPresentationCount(self):
        #返回打开的PPT数目
        return self.app.Presentations.Count
    def hasActivePresentation(self):
        #判断是否有打开PPT文件
        return True if self.getPresentationCount() > 0 else False
if __name__ == '__main__':

    HOST = ''                 
    PORT = 5588              
    BUFSIZE = 1024
    ADDR = (HOST, PORT)

    tcpSerSock = socket(AF_INET, SOCK_STREAM)
    tcpSerSock.bind(ADDR)
    tcpSerSock.listen(5)
    ppt = PowerPointControler()

    while True:
        print('waiting for connection...')
        tcpCliSock, addr = tcpSerSock.accept()
        print('...connected from:', addr)

        while True:
            data = tcpCliSock.recv(BUFSIZE).decode()
            print(data)
            if not data:
                break
            if ppt.hasActivePresentation():
                if data == "f5":
                    ppt.fullScreen()
                elif data == "up":
                    ppt.prePage()
                elif data == "down":
                    ppt.nextPage()
                elif data == "left":
                    ppt.prePage()
                elif data == "right":
                    ppt.nextPage()
                elif data == "esc":
                    ppt.closeFull()
                    
                #else:未完成画线操作，只能话简单的直线
                    #xy = data.split('#')
                    #print(xy[0],xy[1])
                    #ppt.drawLine(int(xy[0]), int(xy[1]))
                    #xy.clear();
            else:
                tcpCliSock.send("close")
    tcpCliSock.close()
    tcpSerSock.close()

