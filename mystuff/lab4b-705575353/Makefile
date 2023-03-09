# NAME Nevin Liang
# EMAIL: nliang868@g.ucla.edu
# ID: 705575353

lab4b: lab4b.c
	@if [ `uname -a | grep -c "armv7l"` -ne 0 ]; then\
		gcc -lmraa -Wall -Wextra -lm -o lab4b lab4b.c;\
	else\
		gcc -DDUMMY -Wall -Wextra -lm -o lab4b lab4b.c;\
	fi
	
check: lab4b
	./lab4b --period=1 --log=temp --scale=F &\
	echo $$! > pid
	sleep 5
	kill `cat pid`
	grep -E "[0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9]+\.[0-9]" > temp2 < temp

	@if [ -s ./temp2 ]; then\
		echo "Test Script PASSED";\
	else\
		echo "Test Script FAILED";\
	fi
	rm temp2 temp pid

clean:
	rm -f lab4b
	rm -f lab4b-705575353.tar.gz

dist: lab4b.c README Makefile
	tar -czf lab4b-705575353.tar.gz lab4b.c README Makefile
