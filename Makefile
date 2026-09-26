build:
	gcc -Wall tema1.c check_availabilty.c incidents.c queues_stack.c init_system.c -o tema1
run:
	./tema1
clean:
	rm -f tema1 tema1.out