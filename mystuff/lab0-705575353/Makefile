# NAME: Nevin Liang
# EMAIL: nliang868@g.ucla.edu
# ID: 705575353

lab0: lab0.c
	gcc lab0.c -o lab0 -Wall -Wextra
	
check: lab0
	@echo "Creating Files..."
	
	@touch INFILE OUTFILE
	@echo "blah bloh bleh" > INFILE
	@echo ""

	@echo "Testing input/output cases..."
	
	@echo "    FROM HERE"
		
	-@./lab0 --input INFILE --output OUTFILE
	@diff INFILE OUTFILE
	-@./lab0 --input INFILE > OUTFILE
	@diff INFILE OUTFILE
	-@cat INFILE | ./lab0 --output OUTFILE
	@diff INFILE OUTFILE
	-@cat INFILE | ./lab0 > OUTFILE
	@diff INFILE OUTFILE

	@echo "    TO HERE, nothing should be printed."

	@echo ""
	@echo "Testing error codes..."

	-./lab0 --badarg INFILE
	@echo ""
	-./lab0 --input BADINFILE
	@echo ""
	-./lab0 --input INFILE --output BADFOLDER/BADOUTFILE
	@echo ""
	-./lab0 --segfault --catch

	@echo ""
	@echo "Testing segfault without catch..."
	-./lab0 --segfault
	
	@echo ""
	@echo "Testing no argument to flag..."
	-./lab0 --input=

clean:
	rm -f lab0
	rm -f lab0-705575353.tar.gz
	rm -f INFILE OUTFILE

dist: lab0.c README Makefile
	tar -czf lab0-705575353.tar.gz lab0.c README Makefile backtrace.png breakpoint.png

