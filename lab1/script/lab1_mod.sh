echo 17 > /sys/class/gpio/export								# generate the gpio17 directory
sleep 0.5																				# delay
echo "out" > /sys/class/gpio/gpio17/direction		# set gpio17/direction to out
COUNTER=0																				# set the initial COUNTER value to 0
while [ $COUNTER -lt 100000 ]; do								# while loop (COUNTER < 100,000)
    echo 1 > /sys/class/gpio/gpio17/value				# set gpio17/value to 1
    let COUNTER=COUNTER+1												# COUNTER = COUNTER + 1
		sleep 0.1																		# delay
    echo 0 > /sys/class/gpio/gpio17/value				# set gpio17/value to 0
		sleep 0.1																		# delay
done			
echo 17 > /sys/class/gpio/unexport							# delete the gpio17 directory
