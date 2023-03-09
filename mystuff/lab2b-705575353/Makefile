# NAME: Nevin Liang
# EMAIL: nliang868@g.ucla.edu
# ID: 705575353

lab2_list: lab2_list.c SortedList.c
	gcc -Wall -Wextra -o lab2_list lab2_list.c SortedList.c -lpthread -g

tests: lab2_list listtest.sh
	chmod 755 listtest.sh
	./listtest.sh > lab2b_list.csv

graphs: lab2_list.gp tests
	chmod 755 lab2_list.gp
	./lab2_list.gp

profile: lab2_list
	LD_PRELOAD=/usr/lib64/libprofiler.so \
	CPUPROFILE=raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list raw.gperf > profile.out
	pprof --list=thread_list ./lab2_list raw.gperf >> profile.out
	rm raw.gperf

clean:
	rm -f lab2_list
	rm -f lab2b-705575353.tar.gz

dist: graphs profile
	tar -czf lab2b-705575353.tar.gz lab2_list.c \
	SortedList.h SortedList.c README Makefile \
	listtest.sh *.png *.csv lab2_list.gp profile.out
