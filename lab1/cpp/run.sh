rm -rf lab1 															# remove the existing executable file
g++ lab1.cpp GPIO.cpp -o lab1 -pthread 		# compile and create a new executable file
./lab1 																		# run the new executable file
