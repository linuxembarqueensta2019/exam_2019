# exam_2019 Linux Embarqué

Voici notre repository Github pour le projet de linux embarqué.
Ce dossier contient

## Lancement du docker


```
$ sudo docker pull pblottiere/embsys-rpi3-buildroot-video
$ sudo docker run -it pblottiere/embsys-rpi3-buildroot-video /bin/bash
# cd /root
# tar zxvf buildroot-precompiled-2017.08.tar.gz
```

## Flashage de la carte SD

```
$ docker cp <container_id>:/root/buildroot-precompiled-2017.08/output/images/sdcard.img .
$ sudo dd if=sdcard.img of=/dev/sdb
```

Pour trouver le nom de la carte sd, utiliser la commande ```lsblk```
