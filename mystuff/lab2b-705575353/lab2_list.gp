#! /usr/local/cs/bin/gnuplot
#
# NAME: Nevin Liang
# EMAIL: nliang868@g.ucla.edu
# ID: 705575353
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#         with the addition of contention calculations and options to fix
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#    8. average wait-for-lock time
#
# output:
#	lab2_list-1.png ... throughput vs. # of threads for mutex/spin-lock sync list ops.
#	lab2_list-2.png ... avg time/mutex wait & avg time/op for mutex-sync list ops.
#	lab2_list-3.png ... successful iterations vs. threads for each sync method.
#	lab2_list-4.png ... throughput vs. # of threads for mutex sync partitioned lists.
#    lab2_list-5.png ... throughput vs. # of threads for spin-lock-sync partitioned lists.
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","


set title "List-1: Throughput for Different Syncs"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Throughput in Ops/Sec"
set logscale y 10
set output 'lab2b_1.png'

plot \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'sync=mutex lock' with linespoints lc rgb 'red', \
     "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'sync=spin lock' with linespoints lc rgb 'green'


set title "List-2: Average wait-for-lock time and Average time per op"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Time"
set logscale y 10
set output 'lab2b_2.png'

plot \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
    title 'wait-for-lock time' with linespoints lc rgb 'red', \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
    title 'time per op' with linespoints lc rgb 'green'


set title "List-3: Iterations to Reliably Fail for Different Syncs"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Iterations"
set logscale y 10
set output 'lab2b_3.png'

plot \
     "< grep list-id-m lab2b_list.csv" using ($2):($3) \
    title 'mutex lock' with points lc rgb 'blue', \
     "< grep list-id-s lab2b_list.csv" using ($2):($3) \
    title 'spin lock' with points lc rgb 'red', \
     "< grep list-id-none lab2b_list.csv" using ($2):($3) \
    title 'no lock' with points lc rgb 'green'


set title "List-4: Throughput for Mutex Lock and Subset Lists"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Throughput in Ops/Sec"
set logscale y 10
set output 'lab2b_4.png'

plot \
     "< grep 'list-none-m,.*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'lists=1' with linespoints lc rgb 'blue', \
     "< grep 'list-none-m,.*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'lists=4' with linespoints lc rgb 'red', \
     "< grep 'list-none-m,.*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'lists=8' with linespoints lc rgb 'violet', \
     "< grep 'list-none-m,.*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'lists=16' with linespoints lc rgb 'green'


set title "List-5: Throughput for Spin Lock and Subset Lists"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Throughput in Ops/Sec"
set logscale y 10
set output 'lab2b_5.png'

plot \
     "< grep 'list-none-s,.*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'lists=1' with linespoints lc rgb 'blue', \
     "< grep 'list-none-s,.*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'lists=4' with linespoints lc rgb 'red', \
     "< grep 'list-none-s,.*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'lists=8' with linespoints lc rgb 'violet', \
     "< grep 'list-none-s,.*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
    title 'lists=16' with linespoints lc rgb 'green'