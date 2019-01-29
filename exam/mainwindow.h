#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_SetIp_released();

    void on_Capture_released();

    void on_TurnLeft_released();

    void on_TurnRight_released();

private:
    Ui::MainWindow *ui;
    int sockServo;
    struct sockaddr_in serv_addrServo;
    int sockCamera;
    struct sockaddr_in serv_addrCamera;
    char buffCamera[1024];
};

#endif // MAINWINDOW_H
