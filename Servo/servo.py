import RPi.GPIO as GPIO
import socket
import time
import signal
import sys

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setwarnings(False)

UDP_IP = "192.168.1.7"  # A changer pour la bonne ip
UDP_PORT = 5005       # A changer pour le bon port (5005)
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)


# Le Servo doit etre branche au pin 11 / GPIO 17
pwm=GPIO.PWM(17,100)
pwm.start(5)
rapport = (50/10)  +5
pwm.ChangeDutyCycle(rapport)
sock.bind(("", UDP_PORT))

def close(signal, frame):

    sock.close()

    pwm.stop()
    GPIO.cleanup()

    print 'Signal: '+ str(signal)
    sys.exit()

signal.signal(signal.SIGINT, close)
signal.signal(signal.SIGTERM, close)
signal.signal(signal.SIGQUIT, close)

if __name__=='__main__':

    while True:
        direction, addr = sock.recvfrom(1) # buffer size is 1 bytes
        direction = int(direction)
        if direction == 0 : # gauche
                    # print("commande gauche")
                    rapport = rapport -5
                    rapport = rapport * 10
                    rapport = rapport +30
                    rapport = rapport /10  +5
        if direction == 1 : # droite
                    # print("commande droite")
                    rapport += -5
                    rapport = rapport * 10
                    rapport = rapport -30
                    rapport = rapport/10 +5
        if rapport < 0 :
            rapport = 24
        if rapport > 25 :
            rapport = 0

        pwm.ChangeDutyCycle(rapport)
        time.sleep(0.8)  # Pause apres avoir bouge


    sock.close()

    pwm.stop()
    GPIO.cleanup()
