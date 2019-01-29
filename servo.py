import RPi.GPIO as GPIO
import socket
import time

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



while True:
    direction, addr = sock.recvfrom(1) # buffer size is 1 bytes
    direction = int(direction)
    #direction=int(input("1 met a gauche, 0 a droite :/n " ))
    print(direction)
    if direction == 0 : # droite
                print("commande droite")
                rapport = rapport -5
                rapport = rapport * 10
                rapport = rapport +30
                rapport = rapport /10  +5
    if direction == 1 : # gauche
                print("commande gauche")
                rapport += -5
                rapport = rapport * 10
                rapport = rapport -30
                rapport = rapport/10 +5
    if rapport < 0 :
        rapport = 24
    if rapport > 25 :
        rapport = 0

    print "le rapport est de"+str(rapport)

    print("#####")
    print("")
    pwm.ChangeDutyCycle(rapport)
       
                             
                                          
    time.sleep(0.8)  # Pause apres avoir bouge
GPIO.cleanup()                          

