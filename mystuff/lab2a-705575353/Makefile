# NAME: Nevin Liang
# EMAIL: nliang868@g.ucla.edu
# ID: 705575353

lab2a: add list

add: lab2_add.c
	gcc lab2_add.c -o lab2_add -Wall -Wextra -pthread

list: lab2_list.c SortedList.c
	gcc lab2_list.c -o lab2_list SortedList.c -Wall -Wextra -pthread

tests: lab2a addtest.sh listtest.sh
	chmod 755 addtest.sh listtest.sh
	./addtest.sh > lab2_add.csv
	./listtest.sh > lab2_list.csv

graphs: lab2_add.gp lab2_list.gp tests
	chmod 755 lab2_add.gp lab2_list.gp
	./lab2_add.gp
	./lab2_list.gp

clean:
	rm -f lab2_list lab2_add
	rm -f lab2a-705575353.tar.gz

dist: graphs
	tar -czf lab2a-705575353.tar.gz lab2_add.c lab2_list.c \
	SortedList.h SortedList.c README Makefile \
	addtest.sh listtest.sh *.png *.csv *.gp