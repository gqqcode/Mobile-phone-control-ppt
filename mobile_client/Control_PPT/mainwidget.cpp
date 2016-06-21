#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    flag = true;

    msgLabel = new QLabel;
    //msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);
    statusBar = new QStatusBar(this);
    statusBar->setFixedHeight(30);
    statusBar->setFixedWidth(this->width());
    statusBar->addWidget(msgLabel); //头文件要添加include<QStatusBar>才不会报错

    initIpLayout();
    initControlBtnLayout();

    mainVLayout = new QVBoxLayout;
    mainVLayout->addLayout(ipHLayout);
    mainVLayout->addLayout(controlBtnLayout);
    this->setLayout(mainVLayout);
    connect(connectBtn,SIGNAL(clicked()),this, SLOT(conncSlot()));
    connect(upBtn,SIGNAL(clicked()),this, SLOT(upSlot()));
    connect(downBtn,SIGNAL(clicked()),this, SLOT(downSlot()));
    connect(f5Btn,SIGNAL(clicked()),this, SLOT(f5Slot()));
    connect(leftBtn,SIGNAL(clicked()),this, SLOT(leftSlot()));
    connect(rightBtn,SIGNAL(clicked()),this, SLOT(rightSlot()));
    connect(escBtn,SIGNAL(clicked()),this, SLOT(escSlot()));


}
/*
    设置获取ip连接的布局
*/
void MainWidget::initIpLayout()
{
    ipLbl = new QLabel(tr("IP:"));
    ipLineEdit = new QLineEdit();
    ipLineEdit->setPlaceholderText(tr("127.0.0.1"));
    connectBtn = new QPushButton(tr("连接"));

    ipHLayout = new QHBoxLayout;
    ipHLayout->addWidget(ipLbl);
    ipHLayout->addWidget(ipLineEdit);
    ipHLayout->addWidget(connectBtn);

//    ipHLayout->setMargin(10);
}

void MainWidget::initControlBtnLayout()
{
    upBtn = new QPushButton(tr("上"));
    leftBtn = new QPushButton(tr("左"));
    f5Btn = new QPushButton(tr("f5"));
    rightBtn = new QPushButton(tr("右"));
    downBtn = new QPushButton(tr("下"));
    escBtn = new QPushButton(tr("esc"));

    controlBtnLayout = new QGridLayout();
    controlBtnLayout->addWidget(upBtn,0,1);

    controlBtnLayout->addWidget(leftBtn, 1, 0);
    controlBtnLayout->addWidget(f5Btn,1,1);
    controlBtnLayout->addWidget(rightBtn, 1, 2);
    controlBtnLayout->addWidget(downBtn, 2, 1);
    controlBtnLayout->addWidget(escBtn, 3, 1);
}

//void MainWidget::mouseMoveEvent(QMouseEvent *event)
//{


//    int x = event->x();
//    int y = event->y();
//    QPoint point = cursor().pos();
//    int x = point.x();
//    int y = point.y();
//    QString xy;
//    xy.clear();
//    xy = tr("%1%2%3").arg(x).arg("#").arg(y);
//    qDebug() << xy;
//    char* chxy;
//    QByteArray ba = xy.toLatin1();

//    chxy=ba.data();
//    if(flag)
//        client->write(chxy);

//}
void MainWidget::displayError(QAbstractSocket::SocketError)
{
    flag = false;
    qDebug() << client->errorString(); //输出错误信息
}

void MainWidget::conncSlot()
{
    QString ip = ipLineEdit->text();

    QString pattern("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])[.](\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])[.](\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])[.](\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$");
    QRegExp rx(pattern);

    if(ip.isEmpty())
    {
        QMessageBox::information(this,"请输入IP","请输入服务器的IP地址！");
        return ;
    }
    else if(!(rx.exactMatch(ip)))
    {
        QMessageBox::information(this,"格式错误","请输入正确的IP地址！");
        return ;
    }
    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress(ip), 5588);
    //connect(client, SIGNAL(connected()),this, SLOT(is_connect_ok()));
    connect(client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    if(flag)
    {
        msgLabel->setText(tr("连接成功！"));
        QMessageBox::information(this,tr("连接提示"),tr("恭喜你连接成功"));
        flag = false;
    }
    else
    {
        msgLabel->setText(tr("连接失败！"));
        QMessageBox::information(this,tr("连接提示"),tr("连接失败，请检查ip是否正确"));
    }

    ipLineEdit->clear();
}


void MainWidget::escSlot()
{
    client->write("esc");
    msgLabel->setText(tr("退出全屏显示"));
    //client->write("100#100");
}
void MainWidget::upSlot()
{
    client->write("up");
    msgLabel->setText(tr("上一页！"));

}

void MainWidget::downSlot()
{
    msgLabel->setText(tr("下一页！"));

    client->write("down");
}

void MainWidget::leftSlot()
{
    msgLabel->setText(tr("上一页！"));
    client->write("left");
}

void MainWidget::rightSlot()
{
    msgLabel->setText(tr("下一页！"));

    client->write("right");
}

void MainWidget::f5Slot()
{
    msgLabel->setText(tr("全屏显示！"));

    client->write("f5");
}

MainWidget::~MainWidget()
{

}
