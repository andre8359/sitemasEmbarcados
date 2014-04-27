#! /usr/bin/env python

import sys
import os 

CC = "/opt/devel/raspberrypi/tools/tools-master/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi-gcc "
CFLAGS= "-I/usr/src/raspberrypi/rootfs/usr/xenomai/include -D_GNU_SOURCE -D_REENTRANT -D__XENO__ "
ARCH_FLAGS="-march=armv6 -mfpu=vfp -mfloat-abi=hard "
LDFLAGS="-lnative -L/usr/src/raspberrypi/rootfs/usr/xenomai/lib -lxenomai -lpthread -lrt -lm "


if len(sys.argv) == 1 :
	print "Erro com parametros!!!!"
	exit(0)

else :
	print "######################### COMPPILANDO #####################################"

	os.system(CC+CFLAGS+ARCH_FLAGS+" -c "+sys.argv[1]+".c")

	os.system(CC+LDFLAGS+ARCH_FLAGS+" -o "+ sys.argv[1]+" "+sys.argv[1]+".o")

	print "######################## ENVIANDO ########################################"

	os.system("rm -irf *.o")

	os.system("scp "+sys.argv[1]+ " pi@10.42.0.2:/home/pi")
	
	print "DONE!!"
