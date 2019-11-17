PROGNAME = assembler
FLAGS = -pedantic -Wall -ansi
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

$(PROGNAME) : $(OBJS)
	gcc -o $(PROGNAME) $(OBJS) 
	
run: $(PROGNAME)
#	./$(PROGNAME) < test1 > output
#	./$(PROGNAME) < test2 >> output
#	./$(PROGNAME) < test3 >> output
	
-include depends

%.o : %.c
	gcc -g -c $(FLAGS) $<
	gcc -g -c $(FLAGS) -MM $< >> depends 
		
clean:
	rm -f *.o
	rm -f *.d
	rm -f depends
