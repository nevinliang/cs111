# NAME: Nevin Liang
# EMAIL: nliang868@g.ucla.edu
# ID: 705575353

lab1a: lab1a.c
	gcc lab1a.c -o lab1a -Wall -Wextra

clean:
	rm -f lab1a
	rm -f lab1a-705575353.tar.gz

dist: lab1a.c README Makefile
	tar -czf lab1a-705575353.tar.gz lab1a.c README Makefile

