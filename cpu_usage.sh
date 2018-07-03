#!/bin/bash
##############################################################################################
#
# A bash script to print out the cpu(s) and gpu usage as a percentage for the Jetson TX2.
#                       Douglas Teeple July 2018
#
# Input taken from from /proc/stat:
#      user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
# cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
#
##############################################################################################
declare -a previdle
declare -a prevtotal
if [[ "$1" == "-h" ]]
then
    echo "usage: $(basename $0): [-h] [-p] [-v] -- -p means plot -v means verbose in plot mode"
    exit
fi
if [[ "$1" == "-p" ]]
then
    shift
    $0 | stripchart $*
    exit
fi
((n=0))
while true
do
	((i=0))
	cat /proc/stat | awk '/cpu/ {print}' | \
	while read -r cpu usertime nicetime systemtime idletime iowait irq softirq steal guest guestnice
	do
		((usertime=usertime-guest));                     # As you see here, it subtracts guest from user time
		((nicetime=nicetime-guestnice));                 # and guest_nice from nice time
		((idlealltime=idletime+iowait));                 # ioWait is added in the idleTime
		((nonidletime=usertime+nicetime+systemtime+irq+softirq+steal))
		((systemalltime=systemtime+irq+softirq));
		((virtaltime=guest+guestnice));
		((totaltime=usertime+nicetime+systemalltime+idlealltime+steal+virtaltime));

		if (( n > 0 ))
		then
			# differentiate: actual value minus the previous one
			((totaldelta=totaltime-prevtotal[i]))
			((idledelta=idlealltime-previdle[i]))
			cp_percentage=`bc -l -q <<< "scale=2; (500*($totaldelta-$idledelta))/$totaldelta"`
			printf "${cp_percentage}\t"
		else
			if (( i == 0 ))
			then
				printf "Avg\t"
			else
				printf "CPU_${i}\t"
			fi 
		fi
		((prevtotal[i]=totaltime))
		((previdle[i]=idlealltime))
		((i++))
	done
	((gpu=0))
	if (( n > 0 ))
	then
		#while [ -e "/sys/devices/gpu.${gpu}.load" ]
		#do
			gpuload=`cat /sys/devices/gpu.${gpu}/load`
			gpu_percentage=`bc -l -q <<< "scale=2; ${gpuload}/10.0"`
			printf "${gpu_percentage}\t"
		#	((gpu++))
		#done
	else
		printf "GPU_${gpu}"
	fi
	echo
	sleep 1
	((i=0))
	((n++))
done
exit


