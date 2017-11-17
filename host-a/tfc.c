#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

 
int main(int argc,char **argv)
{
    clock_t start = clock();
    clock_t diff;

	if(argc != 4){
        printf("Augments format: \"<filename executable> <IP addr> <port #> <zip_file>\"\n");
        return 1;
    	}

    char sendbf[1024];     
 	//What is the maximum # of bytes I can send over tcp? 
	//Let me set it to 1024 at this time...
    char recvbf[1024];
    long sz;
    int sock;
    struct sockaddr_in servaddr;
							printf("get here #1\n");
    FILE* fp;
    if((fp = fopen(argv[3], "rb+")) == NULL){
	    puts("File could not be opened.");
	    return 1;
     }
	
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fclose(fp);
	
    char s[100]; 
    sprintf(s, "%ld", sz);
 		
    sock = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);
 
    int x = strtol(argv[2], NULL, 0);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(x);
 
    inet_pton(AF_INET, argv[1], &(servaddr.sin_addr)); // what is servaddr.sin_addr ?
 	
    
    //(struct sockaddr *) is the type of a pointer to data type sockaddr,
    // and "&servaddr is the address of the variable of type sockaddr."
    connect(sock,(struct sockaddr *)&servaddr,sizeof(servaddr)); 			
    // just fount if there is no connection setup at server side,
    // the connect cause program exit directly, WHY?
							printf("get here #2\n");

    int filesize = write(sock, s, strlen(s)+1);
    //5866 = "5866\0"    
    printf("%s\n", s);
    //5 filezise, number of bytes reads from s: "5866\0"
    printf("%d\n", filesize);
							printf("get here \(GGGG)\n"); 	
    
    int count = 1;  
    fp = fopen(argv[3], "rb+");
	do{
		bzero(sendbf, 1024);
		fread(sendbf, 1024, 1, fp);
		printf("PACKET %d SENT ", count); count++;
        int i=0;		
        for(; i<10; i++){
			printf("%X", sendbf[i]);
		}
		printf("\n");
		write(sock, sendbf, 1024);
		if(ferror(fp)){
			puts("File reading error");		
		}			
	}while(!feof(fp));   //Or while(result == 0)
							printf("get here #3\n");
	fclose(fp);
	
	/* round 2 */

	bzero(recvbf, 1024);
    //read for file size in #of bytes
	read(sock, recvbf, 100);         
	int LEN = strtol(recvbf, NULL, 0);
	fp = fopen("received.txt","w");
    
    int countX = 1;
	for(; LEN >= 1024; LEN -= 1024 ){
        bzero(recvbf, 1024);
		read(sock, recvbf, 1024);
        usleep(300);
        if(countX > 610) printf("PACKET %d RECV'd ", countX); countX++;
        int i=0;        
        if(countX > 610){for(; i<10; i++){
			printf("%X", recvbf[i]);
		}
		printf("\n");}
		fwrite(recvbf, 1024, 1, fp);
	}
    bzero(recvbf, 1024);
	read(sock, recvbf, 1024);
    usleep(300);
    printf("PACKET %d RECV'd ", countX); countX++;
    int i=0;    
    for(; i<10; i++){
			printf("%X", recvbf[i]);
		}
	printf("\n");
    
	size_t last = LEN %1024;	
    //LAST = LEN %1024 = 773 
	printf("%lu\n", (unsigned long)last);
	fwrite(recvbf, last, 1, fp);
	fclose(fp);
	close(sock);
							printf("get here #4\n");
    printf("%s\n", s);
	// system("rm bi.txt");

     
    diff = clock() - start;
    int micsec = diff *1000/CLOCKS_PER_SEC + 0.3 *countX;
    printf("Time taken %d seconds %d milliseconds\n\n", micsec/1000, micsec%1000);    
}




 	/*		
    while(1)
    {
        bzero( sendbf, 1024);
        bzero( recvbf, 1024);
        fgets(sendbf,100,stdin); //stdin = 0 , for standard input 
 
        write(sock,sendbf,strlen(sendbf)+1); //Why is there a +1?????
        read(sock,recvbf,100);
        printf("%s",recvbf);
    }
    */
 

