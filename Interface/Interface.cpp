#include "Interface.h"
#include "ui_Interface.h"

#define SA struct sockaddr

int sockCamera = -1;
int sockServo = -1;

void closefd(int sock){
    if (sock != -1)
    {
        if (close(sock) < 0)
        fprintf(stderr,"Erreur fermeture socket\n");
        else
        fprintf(stderr,"fermeture de la socket réussie\n");
    }
}

void createSocket(int * sock, char * ip, int port, int sockType)
{
    struct sockaddr_in serv_addr;
    if ((*sock = socket(AF_INET, sockType, 0)) < 0)
    {
        fprintf(stdout,"Erreur de la création de la socket\n");
        exit(1);
    }
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
    {
        fprintf(stderr,"Adresse invalide ou non supportée\n");
        exit(1);
    }
    if (::connect(*sock, (SA *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr,"Connexion impossible\n");
        exit(1);
    }
}

void errno_exit(const char* s)
{
    fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
    exit(EXIT_FAILURE);
}

void jpegWrite(unsigned char* img, char* jpegFilename)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];
    FILE *outfile = fopen( jpegFilename, "wb" );

    // try to open file for saving
    if (!outfile) {
        errno_exit("jpeg");
    }

    // create jpeg data
    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    // set image parameters
    cinfo.image_width = 640;
    cinfo.image_height = 480;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_YCbCr;

    // set jpeg compression parameters to default
    jpeg_set_defaults(&cinfo);
    // and then adjust quality setting
    jpeg_set_quality(&cinfo, 70, TRUE);

    // start compress
    jpeg_start_compress(&cinfo, TRUE);

    // feed data
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &img[cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // finish compression
    jpeg_finish_compress(&cinfo);

    // destroy jpeg data
    jpeg_destroy_compress(&cinfo);

    // close output file
    fclose(outfile);
}

/**
SIGINT interput handler
*/
void StopCapture(int sig_id) {
    fprintf(stderr,"Signal Caught : %d", sig_id);

    if (sockServo >= 0)
    closefd(sockServo);

    if (sockCamera >= 0)
    closefd(sockCamera);

    exit(EXIT_SUCCESS);
}

void InstallSIGINTHandler() {
    struct sigaction sa;
    CLEAR(sa);

    sa.sa_handler = StopCapture;
    if(sigaction(SIGINT, &sa, 0) != 0)
    fprintf(stderr,"could not install SIGINT handler");
}

void InstallSIGQUITHandler() {
    struct sigaction sa;
    CLEAR(sa);

    sa.sa_handler = StopCapture;
    if(sigaction(SIGQUIT, &sa, 0) != 0)
    fprintf(stderr,"could not install SIGINT handler");
}

void InstallSIGKILLHandler() {
    struct sigaction sa;
    CLEAR(sa);

    sa.sa_handler = StopCapture;
    if(sigaction(SIGKILL, &sa, 0) != 0)
    fprintf(stderr,"could not install SIGINT handler");
}

Interface::Interface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Interface)
{
    InstallSIGINTHandler();
    ui->setupUi(this);
    QPixmap image("../init.png");
    QPixmap pm = image.scaled(image.width()/2, image.height()/2, Qt::KeepAspectRatio);
    ui->capture->setPixmap(pm);

    buffCamera = (unsigned char *) malloc(IMAGE_SIZE*sizeof(char));
}

Interface::~Interface()
{
    delete ui;

    if (sockServo > 0)
    closefd(sockServo);

    if (sockCamera)
    closefd(sockCamera);

    free(buffCamera);
}

void Interface::on_Capture_released()
{
    sockCamera = -1;
    createSocket(&sockCamera,ip,PORTCAM,SOCK_STREAM);
    int sum = 0;
    int receive = 0;
    if (sockCamera>=0) {
        send(sockCamera, "1", 1, 0);
          while (sum < IMAGE_SIZE)
          {
              receive = recv(sockCamera, buffCamera+sizeof(unsigned char) * sum, 20, 0);
              if (receive <= 0)
              {
                  fprintf(stderr,"Problème lors de la réception, erreur : %d \n", receive);
                  exit(1);
              }
              sum += receive;
          }
          fprintf(stderr,"Sortie de la réception\n %d bytes reçus\n",sum);
    }
    else {
        fprintf(stderr,"Veuillez renseigner l'adresse ip ! \n");
    }

    // Vérification de l'existance de l'image
    struct stat buffer;
    if (stat("image.jpg",&buffer) ==0)
    {
        remove("image.jpg");
    }

    jpegWrite(buffCamera,nameImage);
    memset(buffCamera,'\0',IMAGE_SIZE * sizeof(char));
    QPixmap image(nameImage);
    QPixmap pm = image.scaled(image.width()/2, image.height()/2, Qt::KeepAspectRatio);
    ui->capture->setPixmap(pm);

    closefd(sockCamera);
}

void Interface::on_TurnLeft_released()
{
    send(sockServo, "0", 1, 0);
}

void Interface::on_TurnRight_released()
{
    send(sockServo, "1", 1, 0);
}

void Interface::on_SetIp_clicked()
{
    QString ipQString = ui->IP->text();
    QByteArray ipQByte = ipQString.toLocal8Bit();
    strcpy(ip,ipQByte.data()); //IP of the raspberry

    createSocket(&sockServo,ip,PORTSERVO,SOCK_DGRAM);

    ui->IP->hide();
    ui->SetIp->hide();
}
