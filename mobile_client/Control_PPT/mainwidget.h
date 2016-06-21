#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QtNetwork/QtNetwork>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStatusBar>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void initIpLayout();
    void initControlBtnLayout();
//    void mouseMoveEvent(QMouseEvent *event);
private:
    QLabel *ipLbl;
    QLineEdit *ipLineEdit;
    QPushButton *connectBtn;
    /*
    控制按钮  上一页、写一页等
    */
    QPushButton *upBtn;
    QPushButton *downBtn;
    QPushButton *leftBtn;
    QPushButton *rightBtn;
    QPushButton *f5Btn;
    QPushButton *escBtn;
    /*
    布局
    */
    QHBoxLayout *ipHLayout;
    QGridLayout *controlBtnLayout;
    QVBoxLayout *mainVLayout;

    QTcpSocket *client;
    QStatusBar *statusBar;
    QLabel *msgLabel;

    bool flag;

public slots:
    void conncSlot();
    void upSlot();
    void downSlot();
    void leftSlot();
    void rightSlot();
    void f5Slot();
    void escSlot();

    void displayError(QAbstractSocket::SocketError);
};

#endif // MAINWIDGET_H
