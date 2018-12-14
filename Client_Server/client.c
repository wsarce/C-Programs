/*
 * Author  : Walker Arce
 * Date    : 12/7/2018
 * Purpose : Simple TCP client to communicate with TCP server to
 *		  manage student records.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <stddef.h>

#define PORT_NUM 20030

typedef struct student
{
    char lname[10]; 
    char initial; 
    char fname[10];
    unsigned long SID;
    float GPA;
} SREC;

void error(char *msg)
{
	perror(msg);
	exit(0);
}

char* strsep (char **stringp, const char *delim)
{
    char *begin, *end;
    begin = *stringp;
    if (begin == NULL)
    {
        return NULL;
    }
    /* Find the end of the token.  */
    end = begin + strcspn (begin, delim);
    if (*end)
    {
        /* Terminate the token and set *STRINGP past NUL character.  */
        *end++ = '\0';
        *stringp = end;
    }
    else
    {
        /* No more delimiters; this is the last token.  */
        *stringp = NULL;
    }
    return begin;
}
char* strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if (dup)
    {
        strcpy(dup, str);
    }
    return dup;
}

int main()
{
      const char delimiters[] = {',', ' '};
	/*set up ints for socket file descriptor
	  port number and return of read/write*/
	int sockfd, portno, n;
	/*structure for server info*/
	struct sockaddr_in serv_addr;
	/*used to hold the return of the function
	  that finds our server address, will
	  be copied into serv_addr*/
	struct hostent *server;
	
	/*for our message*/
	char buffer[256];
	/*create the socket*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	/*make sure it was made*/
	if (sockfd < 0) 
		error("ERROR opening socket");
	/*gethostbyname takes our host domain name and 
	  resolves it to an address, there is a similar
	  function, gethostbyaddr that takes an address
	  and returns the same struct, struct hostent*/
	server = gethostbyname("loki");
	/*make sure the host exists*/
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	/*0 out the server address stuct and set members*/
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	/*copy the data returned from gethostbyname
	  into the server address struct*/
	memcpy((char *)server->h_addr_list, 
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
    portno = PORT_NUM;
    portno = portno - 1;
    &serv_addr,sizeof(serv_addr)) < 0)

    for (portno = PORT_NUM; portno < PORT_NUM+10; portno++)
    {
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
        {
            printf("Failed: %d\n", portno);
            if (portno == 20039)
            {
                error("ERROR connecting");
            }
        }
        else
        {
            break;
        }
     }
	/*Request a connection to the server through the socket we set up
	  make sure it connected, this function will also do the binding
	  of our socket and server info*/

    while(1)
    {
	    printf("Enter next command> ");
	    /*Retrieve a message to send from the user*/
	    memset(buffer, 0, 256);
	    fgets(buffer,255,stdin);
	    /*send the message to the socket*/
	    	    
        if (strncmp("stop", buffer, 4) == 0)
        {
            n = write(sockfd,buffer,strlen(buffer));
            close(sockfd);
            exit(1);
        }
        else if (strncmp("get lname", buffer, 9) == 0)
        {
            memset(buffer, 0, 256);
            buffer[0] = 1;
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0) error("ERROR writing to socket");
            n = read(sockfd, buffer, 255);
            //Wait for entire table to write
            int i = atoi(buffer);
            n = write(sockfd, buffer, 255);
            n = read(sockfd, buffer, 255);
            printf("%s", buffer);
            while (i != 0)
            {
                n = read(sockfd, buffer, 255);
                if (n < 0) error("ERROR reading from socket");
                printf("%s\n", buffer);
                memset(buffer,0,256);
                i--;
            }
            n = read(sockfd, buffer, 255);
            printf("%s", buffer);
        }
        else if (strncmp("get fname", buffer, 9) == 0)
        {
            memset(buffer, 0, 256);
            buffer[0] = 2;
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0) error("ERROR writing to socket");
            n = read(sockfd, buffer, 255);
            //Wait for entire table to write
            int i = atoi(buffer);
            n = write(sockfd, buffer, 255);
            n = read(sockfd, buffer, 255);
            printf("%s", buffer);
            while (i != 0)
            {
                n = read(sockfd, buffer, 255);
                if (n < 0) error("ERROR reading from socket");
                printf("%s\n", buffer);                                                                      memset(buffer,0,256);
                i--;
            }
            n = read(sockfd,buffer, 255);
            printf("%s",buffer);
        }
        else if (strncmp("get SID", buffer, 7) == 0)
        {
            memset(buffer, 0, 256);
            buffer[0] = 3;
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) error("ERROR writing to socket");
            n = read(sockfd, buffer, 255);
            //Wait for entire table to write
            int i = atoi(buffer);
            n = write(sockfd, buffer, 255);
            n = read(sockfd, buffer, 255);
            printf("%s", buffer);
            while (i != 0)
            {
                n = read(sockfd, buffer, 255);
                if (n < 0) error("ERROR reading from socket");
                printf("%s\n", buffer);
                memset(buffer,0,256);
                i--;
            }
            n = read(sockfd,buffer,255);
            printf("%s", buffer);
        }
        else if (strncmp("get GPA", buffer, 7) == 0)
        {
            memset(buffer, 0, 256);
            buffer[0] = 4;
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) error("ERROR writing to socket");
            n = read(sockfd, buffer, 255);
            int i = atoi(buffer);
            n = write(sockfd, buffer, 255);
            n = read(sockfd,buffer,255);
            printf("%s", buffer);
            while (i != 0)
            {
                n = read(sockfd, buffer, 255);
                if (n < 0) error("ERROR reading from socket");
                printf("%s\n", buffer);
                memset(buffer,0,256);
                i--;
            }
            n = read(sockfd,buffer,255);
            printf("%s", buffer);
        }
        else if (strncmp("put", buffer, 3) == 0)
        {
            char *running = strdup(buffer);
            char *command = strsep(&running, delimiters);
            char *lName = strsep(&running, delimiters);
            char *fName = strsep(&running, delimiters);
            char *init = strsep(&running, delimiters);
            char *sid = strsep(&running, delimiters);
            char *gpa = strsep(&running, delimiters);
            
            long unsigned int sidn = atoi(sid);
            float gpan = atof(gpa);
             
            SREC newStu = {"", init[0], "", sidn, gpan};
            strncpy(newStu.lname, lName, 10);
            strncpy(newStu.fname, fName, 10);
            
            n = write(sockfd, command, strlen(command));
            if (n < 0) error("ERROR writing to socket");
            n = write(sockfd, &newStu, sizeof(struct student));
            if (n < 0) error("ERROR writing to socket");
            n = read(sockfd, buffer, 255);
            printf("%s\n", buffer);
            if (n < 0) error("ERROR writing to socket");
        }
        else if (strncmp("delete", buffer, 6) == 0)
        {
            char *running = strdup(buffer);
            char *command = strsep(&running, delimiters);
            char *sid = strsep(&running, delimiters);
            n = write(sockfd, command, strlen(command));
            if (n < 0) error("ERROR writing to socket");
            n = write(sockfd, sid, strlen(sid));
            if (n < 0) error("ERROR writing to socket");
            n = read(sockfd, buffer, 255);
            if (n < 0) error("ERROR writing to socket");
            printf("%s\n", buffer);
        }
        else
        {
            n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) error("ERROR writing to socket");
            n = read(sockfd, buffer, 255);
            printf("%s\n", buffer);
            if (n < 0) error("ERROR writing to socket");
        }
	    memset(buffer, 0, 256); 
    }
    close(sockfd);
    return 0;
}

