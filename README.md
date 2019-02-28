# Projet CAMERA IP

Voici notre dossier Github pour le projet de système embarqué que nous avons réalisé pendant notre dernière année de l'ENSTA Bretagne.

Voici ce que vous trouverez dans ce README :

1. Les objectifs
2. L'architecture
3. Les prérequis
4. Lancement du docker
5. Flashage de la carte SD
6. La caméra
7. Le servomoteur
8. Les réglages de la Raspberry
9. Lancement des serveurs
10. Lancement de l'interface

## Les objectifs

L'objectif est ici de mettre en place une caméra IP sur servomoteur à l'aide
d'une RPI3.

L'utilisateur doit donc pouvoir:

1. prendre 1 photo et la visualiser
2. contrôler la position de la caméra grâce au servomoteur

Pour cela, l'utilisateur possède simplement l'adresse IP de la RPI3.

## L'architecture

Voici les dossier que vous trouverez dans ce Github :

* ***Interface*** qui contient les fichier Qt de l'*interface client*
* ***Servo*** comprenant le script python permettant d'exécuter le serveur qui contrôle le *servomoteur*
* ***v4l2grab*** dans lequel vous trouverez les fichiers du serveur de capture de la *caméra*

## Les Prérequis

Pour pouvoir lancer ce magnifique projet, vous devez avoir quelques programmes indispensables :

* ***Docker*** qui va nous permettre de flasher la carte SD et de cross-compiler le code ***v4l2grab*** avant de le mettre sur la raspberry.
* ***GtkTerm*** qui sera utile pour exécuter les programmes sur la rapsberry depuis l'ordinateur. Utilisez cette commande :
```
$ sudo apt-get install gtkterm
```
* ***Qt*** qui permettra de compiler le programme d'interface sur n'importe quelle distribution Gnu/Linux. Utilisez cette commande :
```
$ sudo apt-get install qt-sdk
```

## Lancement du docker

Pour récupérer l'image déjà précompilée de la carte sd et pour effectuer la cross-compilation, nous utilisons un docker.

```
$ sudo docker pull pblottiere/embsys-rpi3-buildroot-video
$ sudo docker run -it pblottiere/embsys-rpi3-buildroot-video /bin/bash
# cd /root
# tar zxvf buildroot-precompiled-2017.08.tar.gz
```

## Flashage de la carte SD

Il faut récupérer l'image de la carte qui est sur le docker sur notre machine.

```
$ docker cp <container_id>:/root/buildroot-precompiled-2017.08/output/images/sdcard.img .
```
Puis on flashe la carte en ce plaçant dans le répertoire où est situé l'image sdcard.img. Pour trouver le nom de la carte sd, utiliser la commande ```lsblk```

```
$ sudo dd if=sdcard.img of=/dev/sdc
```

Maintenant, il y a normalement 2 partitions sur la carte sd.
La première partition contient toutes les configurations et les images nécessaire au lancement du système.
La deuxième partition de 200Mo contient le système de fichier de la raspberry.

Il faut maintenant copier les fichiers ```start_x.elf``` et ```fixup_x.dat``` dans la première partition de la carte sd.

```
$ sudo docker cp <container_id>:/root/buildroot-precompiled-2017.08/output/images/rpi-firmware/start.elf .
$ sudo docker cp <container_id>:/root/buildroot-precompiled-2017.08/output/images/rpi-firmware/fixup.data .
```

Pour simplifier l'opération de copie de ces deux fichiers sur la carte SD, veuillez la monter. Récupérez ces deux fichiers et placez les dans la première partition de la carte SD. De plus ajoutez dans le fichier `config.txt` :

````
start_x=1
gpu_mem=128
````

C'est aussi le moment de cross-compiler le code ```v4l2grab``` sur le docker pour le copier aussi sur la raspberry.

## La caméra

### Prérequis

Pour utiliser ce projet il faut tout d'abord installer des librairies et des autotools sur le docker

````
# sudo apt-get install libjpeg-dev libv4l-dev
````

### Utilisation

On récupère le projet v4l2grab:
````
$ git clone https://github.com/linuxembarqueensta2019/exam_2019
$ cd exam_2019/v4l2grab
````

Ensuite on active les autotools:

````
$ sh autogen.sh
````

Puis on commente ````#undef malloc```` dans config.h.in
Ensuite on lance les commandes suivantes pour compiler pour la rasberry:

````
$ ./configure CC=/root/buildroot-precompiled-2017.08/output/host/usr/bin/arm-linux-gcc --host=linux
$ make
````

En dehors du docker, copiez le programme cross-compilé depuis le docker vers l'ordinateur puis sur la carte SD.

````
$ sudo docker cp <container_id>:/root/buildroot-precompiled-2017.08/exam_2019/v4l2grab/v4l2grab .
````

Comme la carte est toujours montée, vous pouvez copier facilement ce programme sur la carte SD dans le dossier ```/home/user/```.

## Le servomoteur

Le servomoteur est controlé à l'aide du code python servo.py dans le dossier ***Servo***
Pour lancer le code manuellement il suffit de lancer

````
python servo.py
````

Ce code se connecte à la raspberry pi et plus précisement au GPIO 17 ainsi qu'en UDP avec la GUI.
Dans la boucle principale, on reçoit les commandes via la socket et selon la commande reçue on envoie une consigne correspondante aux servomoteur.

Copiez ce code également dans le dossier ```/home/user ``` de la raspberry.

## Les réglages de la Raspberry

Il ne reste plus qu'à configurer l'adresse ip et activer le module de la caméra de la Raspberry pour pouvoir tout exécuter.
Pour ce faire, voici à quoi doit ressembler le fichier ``` /etc/network/interfaces``` :

```
# interface file auto-generated by buildroot

auto lo
iface lo inet loopback

auto eth0
iface eth0 inet static
  address 192.168.1.X
  netmask 255.255.255.0
  gateway 192.168.1.1
```

Pour le chargement du module de la caméra, veuillez exécuter ce code :
```
modprobe bcm2835-v4l2
```

Vous pouvez choisir n'importe quelle adresse ip pour la raspberry, vous n'aurez qu'à reporter l'adresse choisie dans l'interface.

Il reste à lancer les serveurs !

## Lancement des serveurs

Pour lancer les serveurs, on peut exécuter le serveur du servomoteur avec la commande suivante :
```
$ python servo.py &
```

Ensuite vous pouvez exécuter le serveur de la caméra en exécutant la commande suivante :

```
$ ./v4l2grab -d /dev/video0
```

Pour quitter ce serveur, il suffira de faire un ``` ctrl + c```.

## Lancement de l'interface

Placez vous dans le dossier ***Interface*** qui contient le programme du même nom. Il suffit de le lancer avec la commande suivante :
```
./Interface
```

Vous verrez une fenêtre s'ouvrir et je pense pouvoir vous laisser utiliser tranquillement cette interface suffisamment simple pour que vous l'utilisiez sans "guide".
