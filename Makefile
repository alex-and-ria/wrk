CC=gcc 

snfr_d: snfr_d.o
#cli_snfr: cli_snfr.o

clean:
	sudo rm -f snfr snfr.o
	sudo rm -f /sock_file
	sudo rm -f /outlog
	sudo rm -f /logwlan0.txt
