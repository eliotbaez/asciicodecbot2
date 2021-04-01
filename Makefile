# installation requires root permissions
install: functions.c
	gcc -fPIC -shared -o /usr/local/lib/libasciicodec.so functions.c

debug: functions.c
	gcc -fPIC -shared -o /usr/lib/libasciicodec.so -g -Og -std=gnu99 functions.c

uninstall:
	rm /usr/local/lib/libasciicodec.so
