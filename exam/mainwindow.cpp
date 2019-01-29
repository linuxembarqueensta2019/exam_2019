#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SA struct sockaddr

void closefd(int sock){
    close(sock);
}

int connectfd(int sockfd, const struct sockaddr *addr,socklen_t addrlen){
    return connect(sockfd, addr,addrlen);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap image("../init.png");
    QPixmap pm = image.scaled(image.width()/2, image.height()/2, Qt::KeepAspectRatio);
    ui->capture->setPixmap(pm);

    sockServo = -1;
    sockCamera = -1;

}

MainWindow::~MainWindow()
{
    delete ui;
    closefd(sockServo);
    closefd(sockCamera);
}


void MainWindow::on_SetIp_released()
{
    QString ipQString = ui->IP->text();
    QByteArray ipQByte = ipQString.toLocal8Bit();
    char * ip = ipQByte.data(); //IP of the raspberry
    ui->IP->hide();
    ui->SetIp->hide();


    // !!!!!! Création socket Servo !!!!!!!
    if ((sockServo = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }

    memset(&serv_addrServo, '0', sizeof(serv_addrServo));

    serv_addrServo.sin_family = AF_INET;
    serv_addrServo.sin_port = htons(5005);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip, &serv_addrServo.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
    }
    connectfd(sockServo, (SA *)&serv_addrServo, sizeof(serv_addrServo));

    // !!!!!! Création socket Camera !!!!!!!
    if ((sockCamera = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }

    memset(&serv_addrCamera, '0', sizeof(serv_addrCamera));

    serv_addrCamera.sin_family = AF_INET;
    serv_addrCamera.sin_port = htons(5006);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip, &serv_addrCamera.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
    }
    connectfd(sockCamera, (SA *)&serv_addrCamera, sizeof(serv_addrCamera));
}

void MainWindow::on_Capture_released()
{

    if (sockCamera>=0) {
        send(sockCamera, "1", 1, 0);
        read(sockCamera, buffCamera, 1024);
        fprintf(stderr, buffCamera);
    }
    else {
        printf("\n Veuillez renseigner l'adresse ip ! \n");
    }




}

void MainWindow::on_TurnLeft_released()
{
    if (sockServo>=0) {
        send(sockServo, "0", 1, 0);
    }
    else {
        printf("\n Veuillez renseigner une bonne adresse ip ! \n");
    }
}

void MainWindow::on_TurnRight_released()
{
    if (sockServo>=0) {
        send(sockServo, "1", 1, 0);
    }
    else {
        printf("\n Veuillez renseigner une bonne adresse ip ! \n");
    }
}
