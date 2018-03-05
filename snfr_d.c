#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen
#include <stdbool.h>//for bool;
 
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include <netpacket/packet.h>//for sockaddr_ll;
#include <net/if.h> //for ifreq
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include <fcntl.h>//for fcntl (nonblocking socket);
#include <sys/un.h>
#include <ifaddrs.h>
#include <syslog.h>

#define buf_sz 65536
#define kb_sz 1024
#define line_len 21//n(255.255.255.255)=15; n(' ')=1; n(unsigned int)=4; n('\n')=1;
#define m_ip_len 15
#define flines_max 500
#define NAME "sock_file"

FILE * outlog;

void sps(){
	printf( "\npak...");
	getchar();
}

typedef union uint_u{
	unsigned int u_val;
	unsigned char u_byte[4];
} uint_f;

unsigned int getln(unsigned char* strff,unsigned char* g_ip, uint_f* g_ipcnt,FILE *logfile){
	unsigned int cpcnt=0,i=0;
	fread(strff,sizeof(unsigned char),line_len,logfile);
	for(i=0;i<m_ip_len;i++){
		if(strff[i]!=' '){
			memcpy(g_ip+cpcnt,strff+i,1);
			cpcnt++;
		}
	}
	memcpy(g_ip+cpcnt,"\0",1);
	g_ipcnt->u_byte[0]=strff[16]; g_ipcnt->u_byte[1]=strff[17]; g_ipcnt->u_byte[2]=strff[18]; g_ipcnt->u_byte[3]=strff[19];
	//printf( "\n g_ip=%s g_ipcnt->u_val=%d",/*cpcnt,strlen(g_ip),*/g_ip,g_ipcnt->u_val);
	return cpcnt;
}

int ffindip(struct sockaddr_in src,FILE *logfile,bool flread){//it would be easier to work with data loaded to RAM (with list or so) but as
//collected information may become too big to handle it, there used file to save data;
	//sps();
	unsigned char str_to_file[line_len],strff[line_len],g_ip[m_ip_len+1];
	uint_f curr_ipcnt,g_ipcnt; curr_ipcnt.u_val=1;
	unsigned int i; int cmpres; unsigned long int n_lines=0,sln=0,eln=0,curr_dif,curr_line; long int i_signed;
	for(i=0;i<line_len;i++)
		str_to_file[i]=' ';
	memcpy(str_to_file+15-strlen(inet_ntoa(src.sin_addr)),inet_ntoa(src.sin_addr),strlen(inet_ntoa(src.sin_addr)));
	memcpy(str_to_file+15," ",1);
	memcpy(str_to_file+16,curr_ipcnt.u_byte,sizeof(curr_ipcnt.u_val));
	memcpy(str_to_file+20,"\n",strlen("\n"));
	
	rewind(logfile);//find start of the file;
	unsigned long int sstrt=ftell(logfile);
	if(fseek(logfile,0L,SEEK_END)){
		printf( "\nfseek_err: %s",strerror(errno)); return -1;
	}
	unsigned long int send=ftell(logfile);
	rewind(logfile);// unsigned char ufchar;
	//printf( "send-sstrt=%ld",send-sstrt); //sps();
	if((send-sstrt)==0){//file is empty;
		if(flread) return -2;
		rewind(logfile);//to be sure;
		fwrite(str_to_file,sizeof(unsigned char),line_len,logfile);
		return 0;
	}
	n_lines=(send-sstrt)/line_len; sln=0;eln=n_lines-1;
	//printf( "n_lines=%ld",n_lines); //sps();
	
	rewind(logfile);
	while((eln-sln)>1){
		//sps();
		curr_dif=(unsigned int)((eln-sln)/2.+0.5); curr_line=sln+curr_dif;
		fseek(logfile, curr_line*line_len,SEEK_SET);//find line position;
		//printf( "\nsln=%ld, curr_dif=%ld curr_line=%ld pos=%ld",sln,curr_dif,curr_line,ftell(logfile));
		unsigned int f_ip_len=getln(strff,g_ip,&g_ipcnt,logfile);
		//sps();
		if((cmpres=strcmp(g_ip,inet_ntoa(src.sin_addr)))==0){//equal
			if(flread){
				//printf( "\nreturning: g_ipcnt.u_val=%d",g_ipcnt.u_val);
				return g_ipcnt.u_val;
			}
			g_ipcnt.u_val++;
			memcpy(strff+16,g_ipcnt.u_byte,sizeof(g_ipcnt.u_val));
			fseek(logfile, curr_line*line_len,SEEK_SET);//find line position;
			fwrite(strff,sizeof(unsigned char),line_len,logfile);
			//printf( "\ncmpres=%d",cmpres);
			return 0;
		}
		else if(cmpres<0){
			sln=curr_line;
			//printf( "\ncmpres=%d sln=%ld",cmpres,sln);
		}
		else{
			eln=curr_line;
			//printf( "\ncmpres=%d eln=%ld",cmpres,eln);
		}
	}//we have src.sin_addr, sln, eln;
	fseek(logfile, sln*line_len,SEEK_SET);//find start line position;
	getln(strff,g_ip,&g_ipcnt,logfile);
	cmpres=strcmp(g_ip,inet_ntoa(src.sin_addr)); //printf( "\ncmpres (sln,ni)=%d", cmpres);	
	if((cmpres=strcmp(g_ip,inet_ntoa(src.sin_addr)))==0){
		//printf( "\n(sln=%ld) cmpres=%d g_ip=%s inet_ntoa(src.sin_addr))=%s",sln,cmpres,g_ip,inet_ntoa(src.sin_addr));
		if(flread) return g_ipcnt.u_val;
		g_ipcnt.u_val++;
		memcpy(strff+16,g_ipcnt.u_byte,sizeof(g_ipcnt.u_val));
		fseek(logfile, sln*line_len,SEEK_SET);//find line position;
		fwrite(strff,sizeof(unsigned char),line_len,logfile);
		return 0;
	}
	if(cmpres>0){//insert line before sln;
		if(flread) return -2;
		for(i_signed=n_lines-1;i_signed>=(long int)sln;i_signed--){//shift lines;
			//printf( "\nib i_signed=%ld, n_lines=%ld, sln=%ld",i_signed,n_lines,sln);
			//sps();
			fseek(logfile, i_signed*line_len,SEEK_SET);//find line position;
			getln(strff,g_ip,&g_ipcnt,logfile);
			fwrite(strff,sizeof(unsigned char),line_len,logfile);
		}
		//printf( "\n((b)sln=%ld) cmpres=%d g_ip=%s inet_ntoa(src.sin_addr))=%s",sln,cmpres,g_ip,inet_ntoa(src.sin_addr)); //sps();
		fseek(logfile, sln*line_len,SEEK_SET);
		fwrite(str_to_file,sizeof(unsigned char),line_len,logfile);
		return 0;
	}
	else{
		fseek(logfile, eln*line_len,SEEK_SET);
		getln(strff,g_ip,&g_ipcnt,logfile);
		if((cmpres=strcmp(g_ip,inet_ntoa(src.sin_addr)))==0){
			//printf( "\n(eln=%ld) cmpres=%d g_ip=%s inet_ntoa(src.sin_addr))=%s",eln,cmpres,g_ip,inet_ntoa(src.sin_addr));
			if(flread) return g_ipcnt.u_val;
			g_ipcnt.u_val++;
			memcpy(strff+16,g_ipcnt.u_byte,sizeof(g_ipcnt.u_val));
			fseek(logfile, eln*line_len,SEEK_SET);
			fwrite(strff,sizeof(unsigned char),line_len,logfile);
			return 0;
		}
		if(cmpres>0){//insert line between sln and eln;
			//printf( "\ncmpres1 (eln,ni)=%d", cmpres);
			if(flread) return -2;
			for(i_signed=n_lines-1;i_signed>=(long int)eln;i_signed--){
				//printf( "\nibw i_signed=%ld, n_lines=%ld, eln=%ld",i_signed,n_lines,eln);
				//sps();
				fseek(logfile, i_signed*line_len,SEEK_SET);//find line position;
				getln(strff,g_ip,&g_ipcnt,logfile);
				fwrite(strff,sizeof(unsigned char),line_len,logfile);
			}
			//printf( "\n(eln=%ld sln=%ld) cmpres=%d g_ip=%s inet_ntoa(src.sin_addr))=%s",eln,sln,cmpres,g_ip,inet_ntoa(src.sin_addr)); //sps();
			fseek(logfile, eln*line_len,SEEK_SET);
			fwrite(str_to_file,sizeof(unsigned char),line_len,logfile);
			return 0;
		}
		else{//insert line after eln;
			//printf( "\ncmpres (eln,ni)=%d", cmpres);
			if(flread) return -2;
			for(i_signed=n_lines-1;i_signed>(long int)eln;i_signed--){
				//printf( "\niaeln i_signed=%ld, n_lines=%ld, eln=%ld",i_signed,n_lines,eln);
				//sps();
				fseek(logfile, i_signed*line_len,SEEK_SET);//find line position;
				getln(strff,g_ip,&g_ipcnt,logfile);
				fwrite(strff,sizeof(unsigned char),line_len,logfile);
			}
			//printf( "\n((a)eln=%ld) cmpres=%d g_ip=%s inet_ntoa(src.sin_addr))=%s",eln,cmpres,g_ip,inet_ntoa(src.sin_addr));
			fseek(logfile, (eln+1)*line_len,SEEK_SET);
			fwrite(str_to_file,sizeof(unsigned char),line_len,logfile);
			return 0;
		}
	}
	if(flread) return -3;
	return -1;
}

void pprocess(unsigned char* buffer, int data_sz, struct ifreq ifr,FILE *logfile,bool flread){
	struct sockaddr_in source,dest; int fip_res;
	struct iphdr *iph = (struct iphdr *)(buffer  + sizeof(struct ethhdr));
	source.sin_addr.s_addr = iph->saddr; dest.sin_addr.s_addr = iph->daddr;
	//printf( "\nsrc_addr=%s,",inet_ntoa(source.sin_addr)); printf( " dst_addr=%s",inet_ntoa(dest.sin_addr));
	if(source.sin_addr.s_addr==((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr){
		//printf( "\nsrc");//this packet is sent from interface's ip;
	}
	else{
		//printf( "\nsource.sin_addr=%s, ifr.sin_addr=%s\n",inet_ntoa(source.sin_addr),inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
		if((fip_res=ffindip(source,logfile,flread))){
			if(!flread){
				if(fip_res==-1){
					printf( "\nwrite_err!\n"); //sps();
				}
			}
			else{
				if(fip_res==-2){
					printf( "\nip_nfnd\n");
				}
				else if(fip_res==-3){
					printf( "\nread_err!\n"); //sps();
				}
			}
			//sps();
		}
	}
}

void prs_cli(char* cli_comm,int rval,int* argc_sock, unsigned char argv_sock[2][kb_sz]){
	unsigned int i=0,ln_cnt=0; *argc_sock=1;
	for(i=0;i<rval;i++){
		if(cli_comm[i]!=' '){
			argv_sock[(*argc_sock)-1][ln_cnt]=cli_comm[i];
			ln_cnt++;
		}
		else{
			argv_sock[(*argc_sock)-1][ln_cnt]='\0';
			(*argc_sock=2); ln_cnt=0;
		}
	}
	argv_sock[(*argc_sock)-1][ln_cnt]='\0';
	if((*argc_sock)==1){
		argv_sock[1][0]='\0';
	}
	printf( "\n*argc_sock=%d argv_sock[0]=%s argv_sock[1]=%s",*argc_sock,argv_sock[0],argv_sock[1]);
}

int checkifnm(char* ifname){
	struct ifaddrs *ifaddr,*ifa; int i,cmpres,family;
	if (getifaddrs(&ifaddr) == -1) {
       perror("getifaddrs");
       return -1;
   	}
   	for (ifa = ifaddr, i = 0; ifa != NULL; ifa = ifa->ifa_next, i++){
   		family = ifa->ifa_addr->sa_family;
   		if (ifa->ifa_addr == NULL||family != AF_PACKET)
			continue;
   		cmpres=strcmp(ifname,ifa->ifa_name);
   		printf( "\ni=%d, ifa_name=%s cmpres=%d",i,ifa->ifa_name,cmpres);
   		if(cmpres==0){
   			printf( "\nif_fnd");
   			return 0;
   		}
   	}
   	return -1;
}

void showstt(unsigned char* cli_snd,int msgsock, unsigned char* ifacenm){
	unsigned char flnm[strlen("log")+strlen(ifacenm)+strlen(".txt")+1], strff[line_len], g_ip[m_ip_len+1];
	uint_f g_ipcnt;	unsigned long i,sstrt,send,n_lines;
	sprintf(flnm,"log%s.txt",ifacenm);
	if(access(flnm,F_OK)!=-1){
		FILE* ftsw=fopen(flnm,"r");
		if(ftsw==NULL){
			sprintf(cli_snd,"%d",-1);
			if(write(msgsock,cli_snd,strlen(cli_snd)+1)<0){
				perror("writing stream message");
			}
			sprintf(cli_snd,"file log%s.txt nopn",ifacenm);
		}
		else{
			rewind(ftsw);
			sstrt=ftell(ftsw);
			if(fseek(ftsw,0L,SEEK_END)){
				printf( "\nfseek_err: %s",strerror(errno)); return;
			}
			send=ftell(ftsw);
			if((send-sstrt)==0){//file is empty;
				sprintf(cli_snd,"%d",-1);
				if(write(msgsock,cli_snd,strlen(cli_snd)+1)<0){
					perror("writing stream message");
				}
				sprintf(cli_snd,"file log%s.txt is empty",ifacenm);
			}
			else{
				n_lines=(send-sstrt)/line_len;
				sprintf(cli_snd,"%ld",n_lines); printf("\nn_lines=%ld",n_lines);
				if(write(msgsock,cli_snd,strlen(cli_snd)+1)<0){
					perror("writing stream message");
				}
				rewind(ftsw);
				for(i=0;i<n_lines;i++){
					getln(strff,g_ip,&g_ipcnt,ftsw);
					sprintf(cli_snd,"%s %d",g_ip,g_ipcnt.u_val);
					printf("\nsnd=%s",cli_snd);
					sleep(1);//TODO: replace sleep() with write/read pairs;
					if(write(msgsock,cli_snd,strlen(cli_snd)+1)<0){
						perror("writing stream message");
					}
				}
				sprintf(cli_snd,"ok");
			}
		}
	}
	else{
		sprintf(cli_snd,"%d",-1);
		if(write(msgsock,cli_snd,strlen(cli_snd)+1)<0){
    		perror("writing stream message");
    	}
    	sprintf(cli_snd,"file log%s.txt nfnd",ifacenm);
	}
}

int showstall(unsigned char* cli_snd,int msgsock){
	struct ifaddrs *ifaddr,*ifa; int i,cmpres,family;
	if (getifaddrs(&ifaddr) == -1) {
       perror("getifaddrs");
       return -1;
   	}
   	for (ifa = ifaddr, i = 0; ifa != NULL; ifa = ifa->ifa_next, i++){
   		family = ifa->ifa_addr->sa_family;
   		if (ifa->ifa_addr == NULL||family != AF_PACKET)
			continue;
		sleep(1);
		sprintf(cli_snd,"%d",1);
		if(write(msgsock,cli_snd,strlen(cli_snd)+1)<0){
			perror("writing stream message");
		}
		printf("\nifn=%s",ifa->ifa_name);
		sleep(1);
   		showstt(cli_snd,msgsock,ifa->ifa_name);
   	}
   	sprintf(cli_snd,"%d",0);
   	if(write(msgsock,cli_snd,strlen(cli_snd)+1)<0){
		perror("writing stream message");
	}
	sprintf(cli_snd,"ok");
}
 
int main(int argc, char** argv){
	pid_t process_id = 0; pid_t sid = 0;
	//printf("here0\n");
	printf("here\n");
	//printf( "here\n");
	if(argc==2 && !strcmp(argv[1],"strt")){
		//sps();
	}
	else{
		process_id = fork(); // Create child process
		if (process_id < 0){ printf( "fork failed!\n"); return 1;}
		if (process_id > 0){// PARENT PROCESS. Need to kill it.
			printf("process_id of child process %d \n", process_id); 
			//printf( "here0.01\n");
			return 0;
		}
	}
	umask(0); //unmask the file mode
	//printf( "here0.2\n");
	sid = setsid(); //set new session
	//printf( "here0.3\n");
	if(sid < 0) return 1;
	//printf( "here2\n");
	chdir("/"); // Change the current working directory to root.
	outlog=fopen("outlog","w+");
	if(outlog==NULL){
		fprintf(outlog,"outlog==NULL\n");
	 	return 1;
	}
	close(STDIN_FILENO); close(STDOUT_FILENO); close(STDERR_FILENO);// Close stdin. stdout and stderr;
	fprintf(outlog, "here3\n");
	chdir("/"); // Change the current working directory to root.;
	
	FILE *logfile; bool flread=0; unsigned int itr=0; int saddr_size, data_size, cli_sock, msgsock, rval; int dcnt=flines_max;
    struct sockaddr saddr; struct sockaddr_un server; struct ifreq ifr; struct sockaddr_ll sock_address;
	char cli_comm[kb_sz],cli_snd[kb_sz]; char if_name[10]; char fname[sizeof("log")+10+sizeof(".txt")];
	memcpy(if_name,"wlan0\0",strlen("wlan0")+1);
	memcpy(fname,"log",strlen("log"));
	memcpy(fname+strlen("log"),if_name,strlen(if_name));
	//memcpy(fname+strlen("log")+strlen(if_name),".txt\0",strlen(".txt\0")+1); fprintf(outlog, "%s",fname);
	sprintf(fname,"log%s.txt",if_name);
    unsigned char *buffer = (unsigned char *) malloc(buf_sz);
    unsigned int argc_sock; unsigned char argv_sock[2][kb_sz];
    logfile=fopen(fname,"rb+");
    if(logfile==NULL){
    	logfile=fopen(fname,"wb+");//if file not exist, create it;
    	if(logfile==NULL){
    		printf( "Unable to create log.txt file."); return 1;
    	}
    }
    
    //create and configure socet for sniffing; (sock_raw -- for sniffing; fd -- to get NIC's IPv4 address;)
    int sock_raw = socket(AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;
    if(sock_raw < 0){ perror("Socket Error"); return 1;}
    
	int fd= socket(AF_INET, SOCK_DGRAM, 0);//fd -- temporary file descriptor; AF_INET addresses are accepted;
	ifr.ifr_addr.sa_family = AF_INET; /* I want to get an IPv4 IP address */
	strncpy(ifr.ifr_name, if_name, IFNAMSIZ-1); /* I want IP address attached to if_name */
	ioctl(fd, SIOCGIFADDR, &ifr); close(fd);
	printf( "\nip(if_name)=%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	
	ioctl(sock_raw, SIOCGIFFLAGS, &ifr);//Set interface to promiscuous mode for sniffing all received packets;
	ifr.ifr_flags |= IFF_PROMISC;
	ioctl(sock_raw, SIOCSIFFLAGS, &ifr);	
	if (setsockopt(sock_raw, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1) {//Allow the socket to be reused;
	  perror("setsockopt");
	  close(sock_raw);
	  return 1;
	}
	memset(&sock_address, 0, sizeof(sock_address));//bind socket to specific NIC (if_name); (SO_BINDTODEVICE  is not supported for packet sockets);
	sock_address.sll_family = AF_PACKET;
	sock_address.sll_protocol = htons(ETH_P_ALL);
	sock_address.sll_ifindex = if_nametoindex(if_name);
	if (bind(sock_raw, (struct sockaddr*) &sock_address, sizeof(sock_address)) < 0) {
	  perror("bind failed\n");
	  close(sock_raw);
	  return 1;
	}
	
	
	//create socket for cli communication;
    cli_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (cli_sock < 0) {
        perror("opening stream socket");
        return 1;
    }
	if (setsockopt(cli_sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1) {//Allow the socket to be reused;
		perror("setsockopt");
		close(sock_raw); close(cli_sock);
		return 1;
	}
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, NAME);
    if (bind(cli_sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
        perror("binding stream socket");
        return 1;
    }
	listen(cli_sock, 3);
	fcntl(cli_sock, F_SETFL, O_NONBLOCK);//nonblocking socket;
    
    while(1){
    	//sps();
    	msgsock = accept(cli_sock, 0, 0);
		fcntl(msgsock, F_SETFL, O_NONBLOCK);
		if(dcnt>=0&&msgsock==-1&&(!flread)){
		    saddr_size = sizeof(saddr);
		    //Receive a packet
		    data_size = recvfrom(sock_raw , buffer , buf_sz , 0 , &saddr , (socklen_t*)&saddr_size);
		    if(data_size <0 ){
		        printf( "Recvfrom error , failed to get packets\n");
		        return 1;
		    }
		    //Now process the packet
		    pprocess(buffer,data_size,ifr,logfile,flread);
		    dcnt--;
		}
		else if(msgsock!=-1){
			if ((rval = read(msgsock, cli_comm, kb_sz)) < 0)
                perror("reading stream message");
            else if (rval == 0)
                printf( "Ending connection\n");
            else{
            	//handle request;
            	printf( "\n-->%s\n", cli_comm);
            	prs_cli(cli_comm,rval,&argc_sock,argv_sock);
            	if(argc_sock==1){
            		if(strcmp(argv_sock[0],"start")==0){
		        		fprintf(outlog, "\nstrt fnd"); dcnt=flines_max; flread=0;
		        		fclose(logfile);
		        		logfile=fopen(fname,"wb+");//clear existing file;
		        		if(logfile==NULL){
		        			fprintf(outlog, "Unable to create log[iface].txt file.");
		        			char* tmp_msg="start: Unable to create log[iface].txt file\0";
		        			memcpy(cli_snd,tmp_msg,strlen(tmp_msg)+1);
		        			//cli_snd="start: Unable to create log[iface].txt file";
		        		}
		        		else{
		        			char* tmp_msg="started log\0";
		        			memcpy(cli_snd,tmp_msg,strlen(tmp_msg)+1);
		        			//cli_snd="started log";
		        		}
		        	}
		        	else if(strcmp(argv_sock[0],"stop")==0){
		        		fprintf(outlog, "\nstop fnd");
		        		fclose(logfile);
		        		logfile=fopen(fname,"rb");//open for reading existing file;
		        		if(logfile==NULL){
		        			fprintf(outlog, "Unable to read log[iface].txt file.");
		        			char* tmp_msg="stop: Unable to read log[iface].txt file\0";
		        			memcpy(cli_snd,tmp_msg,strlen(tmp_msg)+1);
		        			//cli_snd="stop: Unable to read log[iface].txt file";
		        		}
		        		else{
		        			flread=1;
		        			char* tmp_msg="stopped log\0";
		        			memcpy(cli_snd,tmp_msg,strlen(tmp_msg)+1);
		        			//cli_snd="stopped log";
		        		}
		        	}
		        	else if(strcmp(argv_sock[0],"q")==0){
		        		fprintf(outlog, "\nq");
		        		memcpy(cli_snd,"q\0",2);
		        		break;
		        	}
		        	else{
		        		cli_snd[0]='\0';
		        		fprintf(outlog, "\ndefault");
		        	}
            	}
            	else{
            		if(strcmp(argv_sock[0],"show")==0){
		        		fprintf(outlog, "\nshow %s",argv_sock[1]);
		        		struct sockaddr_in iptf; int fip_res;
		        		if(inet_aton(argv_sock[1],&(iptf.sin_addr))){
		        			if((fip_res=ffindip(iptf,logfile,1))<0){
		        				if(fip_res==-2){
									fprintf(outlog, "\nip_nfnd\n");
									memcpy(cli_snd,"ip not found\0",strlen("ip not found")+1);
									//sps();
								}
								else if(fip_res==-3){
									fprintf(outlog, "\nread_err!\n");
									memcpy(cli_snd,"read_err!\0",strlen("read_err!")+1);
									//sps();
								}
		        			}
		        			else{
		        				printf( "\nfnd_cnt=%d\n",fip_res);
		        				unsigned char str_num_res[12];//enough for 4 byte number;
		        				sprintf(str_num_res,"%d",fip_res);
								memcpy(cli_snd,"fip_res=",strlen("fip_res="));
								memcpy(cli_snd+strlen("fip_res="),str_num_res,strlen(str_num_res)+1);
		        			}
		        		}
		        		else{
	        				perror("\nip_convertion");
	        				memcpy(cli_snd,"check ip\0",strlen("check ip")+1);
							//sps();
	        			}
		        	}
		        	else if(strcmp(argv_sock[0],"select")==0){
		        		printf( "\nselect %s",argv_sock[1]);
		        		int isifname;
		        		if((isifname=checkifnm(argv_sock[1]))<0){//no such interface;
		        			printf( "\niface not found");
		        			memcpy(cli_snd,"iface not found\0",strlen("iface not found")+1);
		        		}
		        		else{
		        			fclose(logfile);//finishing write to previous log file;
		        			memcpy(if_name,argv_sock[1],strlen(argv_sock[1])+1);//new file's name;
							memcpy(fname,"log",strlen("log"));
							memcpy(fname+strlen("log"),if_name,strlen(if_name));
							memcpy(fname+strlen("log")+strlen(if_name),".txt\0",strlen(".txt\0")+1); printf( "%s",fname);
		        			logfile=fopen(fname,"rb+");
							if(logfile==NULL){
								logfile=fopen(fname,"wb+");//if file not exist, create it;
								if(logfile==NULL){
									printf( "Unable to create new log.txt file.");
									memcpy(cli_snd,"file_err\0",strlen("file_err")+1);
								}
							}
							if(logfile!=NULL){
								memcpy(cli_snd,fname,strlen(fname));
								memcpy(cli_snd+strlen(fname)," selected\0",strlen(" selected")+1);
							}
		        		}
		        	}
		        	else if(strcmp(argv_sock[0],"stat")==0){//shoud be rewriten (probably with changes in cli_cnfr.c code);
		        		if(strcmp(argv_sock[1],"all")==0){
		        			printf( "\nstat: full");
		        			showstall(cli_snd,msgsock);
		        			/*unsigned char* tpm_msg="see log[iface_name].txt files\0";
		        			memcpy(cli_snd,tpm_msg,strlen(tpm_msg)+1);*/
		        		}
		        		else{
		        			printf( "\nstat %s",argv_sock[1]);
		        			int chck_stat,flns=0; unsigned char tpm_msg[kb_sz/2];
		        			if((chck_stat=checkifnm(argv_sock[1]))<0){//no such iface;
		        				sprintf(tpm_msg,"%d",-1);
		        				memcpy(cli_snd,tpm_msg,strlen(tpm_msg)+1);
		        				if(write(msgsock,cli_snd,strlen(cli_snd)+1)<0){
									perror("writing stream message");
								}
								sprintf(tpm_msg,"select another iface");
								printf( "\nselect another iface");
		        				memcpy(cli_snd,tpm_msg,strlen(tpm_msg)+1);
		        			}
		        			else if(chck_stat==0){
		        				showstt(cli_snd,msgsock,argv_sock[1]);
		        			}
		        			
		        			/*char tpm_msg[kb_sz/2];
		        			sprintf(tpm_msg,"try to find log%s.txt file",argv_sock[1]);
		        			memcpy(cli_snd,tpm_msg,strlen(tpm_msg)+1);*/
		        		}
		        	}
		        	else{
		        		cli_snd[0]='\0';
		        		printf( "\ndefault");
		        	}
            	}
				sleep(1);
            	printf( "\nsnd_msg=%s",cli_snd); int wrtsz=0;
            	printf("\n wrtsz=%d, srtlen=%d",wrtsz=write(msgsock,cli_snd,strlen(cli_snd)+1),strlen(cli_snd));
            	if(wrtsz<0){
            		perror("writing stream message");
            	}
            	printf("\n q");
            }
            close(msgsock);
		}
		//sps();
		//printf("sleep; itr=%d",itr); itr++;
		sleep(1);
		fclose(outlog);//keep outlog's size by reopening each iteration;
		outlog=fopen("outlog","w+");
		if(outlog==NULL){
			printf( "\noutlg");
		 	return 1;
		}
    }
    free(buffer);
    close(sock_raw);
    close(cli_sock);
    unlink(NAME);
    printf( "Finished");
    fclose(logfile);
    fclose(outlog);
    return 0;
}

//currently 'printf' statements are used for debugging; later they will be deleted for daemon code; upd: printf->fprintf;
//files "log%interface_name%.txt will be saved at '/' (perhaps place them in separate directory: '/%dir_name%/%files%');
//communication between cli and daemon will be implemented through file (AF_UNIX socket); may d-bus be used, but it will take more time;
//because of lines' shifts complexity is approximately O(N*log N);
//this code needs debagging; (snfr.c and cli1.c working, but snfr not a service);
