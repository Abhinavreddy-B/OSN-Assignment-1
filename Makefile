all: q1 q2 q3

q1: q1.c
	gcc ./q1.c -o q1

q2: q2.c
	gcc ./q2.c -o q2

q3: q3.c
	gcc ./q3.c -o q3

clean:
	rm ./q1 ./q2 ./q3
	rm -r Assignment