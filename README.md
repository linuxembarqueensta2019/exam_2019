# exam_2019 Linux Embarqué

Voici notre repository Github pour le projet de linux embarqué.
Ce dossier contient

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
La deuxième partition de 200Mo contient le système de fichier de la rasperry.

Il faut maintenant copier les fichiers ```start_x.elf``` et ```fixup_x.dat``` dans la première partition de la carte sd.

```
$ sudo docker cp <container_id>:/root/buildroot-precompiled-2017.08/output/images/rpi-firmware/start.elf /media/paul-antoine/sdc/ACOMPLETER
$ sudo docker cp <container_id>:/root/buildroot-precompiled-2017.08/output/images/rpi-firmware/fixup.data /media/paul-antoine/sdc/ACOMPLETER
```

Il faut ajouter dans le fichier `config.txt` de la 1ère partition:

````
start_x=1
gpu_mem=128
````

## Utilisation de V4L

### Prérequis

Pour utiliser ce projet il faut tout d'abord installer des librairies et des autotools.

````
sudo apt-get install libjpeg-dev libv4l-dev
git clone https://github.com/twam/v4l2grab.git
cd v4l2grab
./autogen.sh
./configure
make
sudo make install
````

### Utilisation

On récupère le projet v4l2grab:
````
$ git clone https://github.com/twam/v4l2grab
````
Ensuite on active les autotools:
````
$ sh autogen.sh
````
Puis on commente ````#undef malloc```` dans config.h.in
Ensuite on lance les commandes suivantes pour compiler pour la rasberry:
````
$ ./configure
$ ./configure CC=/root/buildroot-precompiled-2017.08/output/host/usr/bin/arm-linux-gcc --host=linux
$ make
````

## Servomoteur

Le servomoteur est controllé à l'aide du code python servo.py 
Pour lancer le code manuellement il suffit de lancer 
````
python servo.py
````

Ce code se connecte à la raspberry pi et plus précisement au GPIO 17 ainsi qu'en UDP avec la GUI.
Dans la boucle principale, on recoit les commandes via la socket et selon la commande reçue on envoie une consigne correspondante aux servomoteur.
