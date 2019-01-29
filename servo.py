import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setwarnings(False)

ajoutAngle = 5


# Le Servo doit etre branche au pin 11 / GPIO 17 
 


# A changer ensuite par le serveur
angle = float(input("Entrez l'angle souhaite :\n"))
duree = int(input("Entrez la duree durant laquelle le Servo devra tenir sa position : ( en secondes )\n"))

pwm=GPIO.PWM(17,100)
pwm.start(5)

angleChoisi = angle/10 + ajoutAngle
pwm.ChangeDutyCycle(angleChoisi)
time.sleep(duree)
GPIO.cleanup()