# !/bin/sh

# part 1
./lab2_list --iterations=1000 --threads=1 --sync=m
./lab2_list --iterations=1000 --threads=2 --sync=m
./lab2_list --iterations=1000 --threads=4 --sync=m
./lab2_list --iterations=1000 --threads=8 --sync=m
./lab2_list --iterations=1000 --threads=12 --sync=m
./lab2_list --iterations=1000 --threads=16 --sync=m
./lab2_list --iterations=1000 --threads=24 --sync=m
./lab2_list --iterations=1000 --threads=1 --sync=s
./lab2_list --iterations=1000 --threads=2 --sync=s
./lab2_list --iterations=1000 --threads=4 --sync=s
./lab2_list --iterations=1000 --threads=8 --sync=s
./lab2_list --iterations=1000 --threads=12 --sync=s
./lab2_list --iterations=1000 --threads=16 --sync=s
./lab2_list --iterations=1000 --threads=24 --sync=s

# part 2
./lab2_list --iterations=1000 --threads=1 --sync=m
./lab2_list --iterations=1000 --threads=2 --sync=m
./lab2_list --iterations=1000 --threads=4 --sync=m
./lab2_list --iterations=1000 --threads=8 --sync=m
./lab2_list --iterations=1000 --threads=16 --sync=m
./lab2_list --iterations=1000 --threads=24 --sync=m

# part 3
./lab2_list --yield=id --iterations=1 --threads=1
./lab2_list --yield=id --iterations=1 --threads=4
./lab2_list --yield=id --iterations=1 --threads=8
./lab2_list --yield=id --iterations=1 --threads=12
./lab2_list --yield=id --iterations=1 --threads=16
./lab2_list --yield=id --iterations=2 --threads=1
./lab2_list --yield=id --iterations=2 --threads=4
./lab2_list --yield=id --iterations=2 --threads=8
./lab2_list --yield=id --iterations=2 --threads=12
./lab2_list --yield=id --iterations=2 --threads=16
./lab2_list --yield=id --iterations=4 --threads=1
./lab2_list --yield=id --iterations=4 --threads=4
./lab2_list --yield=id --iterations=4 --threads=8
./lab2_list --yield=id --iterations=4 --threads=12
./lab2_list --yield=id --iterations=4 --threads=16
./lab2_list --yield=id --iterations=8 --threads=1
./lab2_list --yield=id --iterations=8 --threads=4
./lab2_list --yield=id --iterations=8 --threads=8
./lab2_list --yield=id --iterations=8 --threads=12
./lab2_list --yield=id --iterations=8 --threads=16
./lab2_list --yield=id --iterations=16 --threads=1
./lab2_list --yield=id --iterations=16 --threads=4
./lab2_list --yield=id --iterations=16 --threads=8
./lab2_list --yield=id --iterations=16 --threads=12
./lab2_list --yield=id --iterations=16 --threads=16

./lab2_list --yield=id --sync=s --iterations=10 --threads=1
./lab2_list --yield=id --sync=s --iterations=10 --threads=4
./lab2_list --yield=id --sync=s --iterations=10 --threads=8
./lab2_list --yield=id --sync=s --iterations=10 --threads=12
./lab2_list --yield=id --sync=s --iterations=10 --threads=16
./lab2_list --yield=id --sync=s --iterations=20 --threads=1
./lab2_list --yield=id --sync=s --iterations=20 --threads=4
./lab2_list --yield=id --sync=s --iterations=20 --threads=8
./lab2_list --yield=id --sync=s --iterations=20 --threads=12
./lab2_list --yield=id --sync=s --iterations=20 --threads=16
./lab2_list --yield=id --sync=s --iterations=40 --threads=1
./lab2_list --yield=id --sync=s --iterations=40 --threads=4
./lab2_list --yield=id --sync=s --iterations=40 --threads=8
./lab2_list --yield=id --sync=s --iterations=40 --threads=12
./lab2_list --yield=id --sync=s --iterations=40 --threads=16
./lab2_list --yield=id --sync=s --iterations=80 --threads=1
./lab2_list --yield=id --sync=s --iterations=80 --threads=4
./lab2_list --yield=id --sync=s --iterations=80 --threads=8
./lab2_list --yield=id --sync=s --iterations=80 --threads=12
./lab2_list --yield=id --sync=s --iterations=80 --threads=16

./lab2_list --yield=id --sync=m --iterations=10 --threads=1
./lab2_list --yield=id --sync=m --iterations=10 --threads=4
./lab2_list --yield=id --sync=m --iterations=10 --threads=8
./lab2_list --yield=id --sync=m --iterations=10 --threads=12
./lab2_list --yield=id --sync=m --iterations=10 --threads=16
./lab2_list --yield=id --sync=m --iterations=20 --threads=1
./lab2_list --yield=id --sync=m --iterations=20 --threads=4
./lab2_list --yield=id --sync=m --iterations=20 --threads=8
./lab2_list --yield=id --sync=m --iterations=20 --threads=12
./lab2_list --yield=id --sync=m --iterations=20 --threads=16
./lab2_list --yield=id --sync=m --iterations=40 --threads=1
./lab2_list --yield=id --sync=m --iterations=40 --threads=4
./lab2_list --yield=id --sync=m --iterations=40 --threads=8
./lab2_list --yield=id --sync=m --iterations=40 --threads=12
./lab2_list --yield=id --sync=m --iterations=40 --threads=16
./lab2_list --yield=id --sync=m --iterations=80 --threads=1
./lab2_list --yield=id --sync=m --iterations=80 --threads=4
./lab2_list --yield=id --sync=m --iterations=80 --threads=8
./lab2_list --yield=id --sync=m --iterations=80 --threads=12
./lab2_list --yield=id --sync=m --iterations=80 --threads=16

# part 4
./lab2_list --sync=m --iterations=1000 --threads=1 --lists=1
./lab2_list --sync=m --iterations=1000 --threads=1 --lists=4
./lab2_list --sync=m --iterations=1000 --threads=1 --lists=8
./lab2_list --sync=m --iterations=1000 --threads=1 --lists=16
./lab2_list --sync=m --iterations=1000 --threads=2 --lists=1
./lab2_list --sync=m --iterations=1000 --threads=2 --lists=4
./lab2_list --sync=m --iterations=1000 --threads=2 --lists=8
./lab2_list --sync=m --iterations=1000 --threads=2 --lists=16
./lab2_list --sync=m --iterations=1000 --threads=4 --lists=1
./lab2_list --sync=m --iterations=1000 --threads=4 --lists=4
./lab2_list --sync=m --iterations=1000 --threads=4 --lists=8
./lab2_list --sync=m --iterations=1000 --threads=4 --lists=16
./lab2_list --sync=m --iterations=1000 --threads=8 --lists=1
./lab2_list --sync=m --iterations=1000 --threads=8 --lists=4
./lab2_list --sync=m --iterations=1000 --threads=8 --lists=8
./lab2_list --sync=m --iterations=1000 --threads=8 --lists=16
./lab2_list --sync=m --iterations=1000 --threads=12 --lists=1
./lab2_list --sync=m --iterations=1000 --threads=12 --lists=4
./lab2_list --sync=m --iterations=1000 --threads=12 --lists=8
./lab2_list --sync=m --iterations=1000 --threads=12 --lists=16

# # part 5
./lab2_list --sync=s --iterations=1000 --threads=1 --lists=1
./lab2_list --sync=s --iterations=1000 --threads=1 --lists=4
./lab2_list --sync=s --iterations=1000 --threads=1 --lists=8
./lab2_list --sync=s --iterations=1000 --threads=1 --lists=16
./lab2_list --sync=s --iterations=1000 --threads=2 --lists=1
./lab2_list --sync=s --iterations=1000 --threads=2 --lists=4
./lab2_list --sync=s --iterations=1000 --threads=2 --lists=8
./lab2_list --sync=s --iterations=1000 --threads=2 --lists=16
./lab2_list --sync=s --iterations=1000 --threads=4 --lists=1
./lab2_list --sync=s --iterations=1000 --threads=4 --lists=4
./lab2_list --sync=s --iterations=1000 --threads=4 --lists=8
./lab2_list --sync=s --iterations=1000 --threads=4 --lists=16
./lab2_list --sync=s --iterations=1000 --threads=8 --lists=1
./lab2_list --sync=s --iterations=1000 --threads=8 --lists=4
./lab2_list --sync=s --iterations=1000 --threads=8 --lists=8
./lab2_list --sync=s --iterations=1000 --threads=8 --lists=16
./lab2_list --sync=s --iterations=1000 --threads=12 --lists=1
./lab2_list --sync=s --iterations=1000 --threads=12 --lists=4
./lab2_list --sync=s --iterations=1000 --threads=12 --lists=8
./lab2_list --sync=s --iterations=1000 --threads=12 --lists=16
