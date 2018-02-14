#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include<stdlib.h>

#define NAME "sock_file"
#define info_str "q\n"


/*"The​ ​ cli​ ​ should​ ​ support​ ​ command:\
			\na. start​​ ​ (packets​ ​ are​ ​ being​ ​ sniffed​ ​ from​ ​ now​ ​ on​ ​ from​ ​ default​ ​ iface(eth0))\
			\nb. stop​​ ​ (packets​ ​ are​ ​ not​ ​ sniffed)\
			\nc. show​ ​ [ip]​ ​ count​ ​ (print​ ​ number​ ​ of​ ​ packets​ ​ received​ ​ from​ ​ ip​ ​ address)\
			\nd. select​ ​ iface​ ​ [iface]​ ​ (select​ ​ interface​ ​ for​ ​ sniffing​ ​ eth0,​ ​ wlan0,​ ​ ethN, wlanN...)\
			\ne. stat​​ ​ [iface]​​ ​ show​ ​ all​ ​ collected​ ​ statistics​ ​ for​ ​ particular​ ​ interface,​ ​ if​ ​ iface\
			omitted​ ​ - ​ ​ for​ ​ all​ ​ interfaces.\
			\nf. ​ ​ --help​ ​ (show​ ​ usage​ ​ information)\n"*/

int main(int argc, char** argv){//it is good idea to use here getopt_long() for parsing command-line options, but it will take time; so just limit number of possible options to one and parsing it;
	if(argc<2||argc>4||strcmp(argv[1],"--help")==0){
		printf(info_str);
		return 0;
	}
	int sock,rval;
    struct sockaddr_un server;
    char buf[1024]; 
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		return 1;
	}
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, NAME);
	if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
		close(sock);
		perror("connecting stream socket");
		return 1;
	}
    
	
	if(strcmp(argv[1],"start")==0){
		unsigned char* data_snd="start";
		printf("data_snd=%s\n",data_snd);
		if (write(sock, data_snd, sizeof(data_snd)) < 0)
        	perror("writing on stream socket");
	}
	else if(strcmp(argv[1],"stop")==0){
		unsigned char* data_snd="stop";
		printf("data_snd=%s\n",data_snd);
		if (write(sock, data_snd, sizeof(data_snd)) < 0)
        	perror("writing on stream socket");
	}
	else if(strcmp(argv[1],"show")==0&&argc==4 &&strcmp(argv[3],"count")==0){
		//printf("show ip=%s\n",argv[2]);
		unsigned char data_snd[strlen("show")+strlen(argv[2])+2];
		memcpy(data_snd,"show",strlen("show"));
		memcpy(data_snd+strlen("show")," ",1);
		memcpy(data_snd+strlen("show")+1,argv[2],strlen(argv[2])+1);
		printf("data_snd=%s\n",data_snd);
		if (write(sock, data_snd, sizeof(data_snd)) < 0)
        	perror("writing on stream socket");
	}
	else if(strcmp(argv[1],"select")==0 &&argc==4&&strcmp(argv[2],"iface")==0){
		//printf("select iface=%s\n",argv[3]);
		unsigned char data_snd[strlen("select")+strlen(argv[3])+2];
		memcpy(data_snd,"select",strlen("select"));
		memcpy(data_snd+strlen("select")," ",1);
		memcpy(data_snd+strlen("select")+1,argv[3],strlen(argv[3])+1);
		printf("data_snd=%s\n",data_snd);
		if (write(sock, data_snd, sizeof(data_snd)) < 0)
        	perror("writing on stream socket");
	}
	else if(strcmp(argv[1],"stat")==0&&(argc==2 || argc==3)){
		unsigned char *data_snd1;
		if(argc==3){
			data_snd1=(unsigned char *) malloc(strlen("stat")+strlen(argv[2])+2);
			memcpy(data_snd1,"stat",strlen("stat"));
			memcpy(data_snd1+strlen("stat")," ",1);
			memcpy(data_snd1+strlen("stat")+1,argv[2],strlen(argv[2])+1);
		}
		else{
			data_snd1=(unsigned char *) malloc(strlen("stat")+strlen("all")+2);
			memcpy(data_snd1,"stat",strlen("stat"));
			memcpy(data_snd1+strlen("stat")," ",1);
			memcpy(data_snd1+strlen("stat")+1,"all",strlen("all")+1);
		}
		printf("data_snd1=%s\n",data_snd1);
		if (write(sock, data_snd1, strlen(data_snd1)) < 0)
        	perror("writing on stream socket");
        free(data_snd1);
		/*if(argc==3){
			printf("stat iface=%s\n",argv[2]);
		}
		else{
			printf("stat all\n");
		}*/
	}
	else{
		printf("default: %s\n",argv[1]);
	}
	if ((rval = read(sock, buf, 1024)) < 0)
        perror("reading stream message");
    else if (rval == 0)
        printf("Ending connection\n");
    else{
    	printf("\nrecv_msg=%s",buf);
    }
	
	
	close(sock);
	
	return 0;
}
