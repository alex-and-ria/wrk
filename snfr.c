#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen
 
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include <net/if.h> //for ifreq
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

#define buf_sz 65536

unsigned int ffindip(struct sockaddr_in src,FILE *logfile){
	if(fseek(logfile,0L,SEEK_SET)){//find start of the file;
		printf("\nfseek_err: %s",strerror(errno)); return 1;
	}
	unsigned long int sstrt=SEEK_SET, send=SEEK_END;
	while((send-sstrt)/2>0){
		if(fseek(logfile,(send-sstrt)/2,SEEK_SET)){//find middle of the file;
			printf("\nfseek_err: %s",strerror(errno)); return 1;
		}
	}
	//TODO put to file (%S)"ipaddr"; (%ld (unsigned))"ipcnt";
	
	return 0;
}

void pprocess(unsigned char* buffer, int data_sz, struct ifreq ifr,FILE *logfile){
	struct sockaddr_in source,dest;
	struct iphdr *iph = (struct iphdr *)(buffer  + sizeof(struct ethhdr));
	source.sin_addr.s_addr = iph->saddr; dest.sin_addr.s_addr = iph->daddr;
	printf("\nsrc_addr=%s,",inet_ntoa(source.sin_addr)); printf(" dst_addr=%s",inet_ntoa(dest.sin_addr));
	if(source.sin_addr.s_addr==((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr){
		printf("\nsrc");//this packet is sent from interface's ip;
	}
	else{
		printf("\nsource.sin_addr=%s, ifr.sin_addr=%s\n",inet_ntoa(source.sin_addr),inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
		if(ffindip(source,logfile)){
			
		}
	}
}
 
int main(){
	FILE *logfile; int saddr_size , data_size; struct sockaddr saddr; int dcnt=50;
	char* if_name="eth0";
    unsigned char *buffer = (unsigned char *) malloc(buf_sz);
     
    logfile=fopen("log.txt","ab+");
    if(logfile==NULL){printf("Unable to create log.txt file.");}
    
    int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;
    if(setsockopt(sock_raw , SOL_SOCKET , SO_BINDTODEVICE , if_name , strlen(if_name))==-1){
    	printf("\nsetsockopt_err: %s",strerror(errno)); return 1;
    }
     
    if(sock_raw < 0){ perror("Socket Error"); return 1;}
    
    //get interface's ip;
	int fd= socket(AF_INET, SOCK_DGRAM, 0);
	struct ifreq ifr;
	ifr.ifr_addr.sa_family = AF_INET; /* I want to get an IPv4 IP address */
	strncpy(ifr.ifr_name, if_name, IFNAMSIZ-1); /* I want IP address attached to if_name */
	ioctl(fd, SIOCGIFADDR, &ifr); close(fd);
	printf("\nip(if_name)=%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    
    while(dcnt){
        saddr_size = sizeof(saddr);
        //Receive a packet
        data_size = recvfrom(sock_raw , buffer , buf_sz , 0 , &saddr , (socklen_t*)&saddr_size);
        if(data_size <0 ){
            printf("Recvfrom error , failed to get packets\n");
            return 1;
        }
        //Now process the packet
        pprocess(buffer,data_size,ifr,logfile);
        dcnt--;
        
    }
    close(sock_raw);
    printf("Finished");
    return 0;
}
