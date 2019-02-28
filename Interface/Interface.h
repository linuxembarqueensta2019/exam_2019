#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <jpeglib.h>
#include <signal.h>
#include <sys/stat.h>

#define IMAGE_SIZE 921600
#define PORTSERVO 5005
#define PORTCAM 5006
#define CLEAR(x) memset (&(x), 0, sizeof (x))

namespace Ui {
class Interface;
}

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    explicit Interface(QWidget *parent = 0);
    ~Interface();

private slots:

    void on_Capture_released();

    void on_TurnLeft_released();

    void on_TurnRight_released();

    void on_SetIp_clicked();

private:
    Ui::Interface *ui;
    unsigned char *buffCamera;
    char ip [15];
    char nameImage[40] = "image.jpg";
};

#endif // INTERFACE_H
