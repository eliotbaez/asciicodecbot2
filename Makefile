functions: functions.c
	gcc -fPIC -shared -o cfunctions.so functions.c

debug: functions.c
	gcc -fPIC -shared -o cfunctions.so -g -Og -std=gnu99 functions.c
