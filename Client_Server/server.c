/*
 * Author  : Walker Arce
 * Date    : 12/7/2018
 * Purpose : Simple TCP server to communicate with TCP client to
 *		  manage student records.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT_NUM 20030

typedef struct student
{
    char lname[10];
    char initial; 
    char fname[10];
    unsigned long SID;
    float GPA;
} SREC;

void error(char *msg);
void printStu(int sockfdnew);

void error(char *msg)
{
	perror(msg);
	exit(1);
}

struct node 
{
    SREC data;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//Debug utility for Linked List
void printList()
{
    struct node *ptr = head;
    while(ptr != NULL)
    {
        printf("%s\n", ptr->data.lname);
        ptr = ptr->next;
    }
    return;
}

int countList() 
{
    struct node *ptr = head;
    int i = 0;
    while(ptr != NULL) 
    {        
        ptr = ptr->next;
        i++;
    }

    return i;
}

//If empty creates new List
void insert(SREC data) 
{
    struct node *link = (struct node*) malloc(sizeof(struct node));
    
    link->data = data;
    link->next = head;
    head = link;

    return;
}

//Sorting algorithm
void sortLname(struct node *head)
{
    SREC k;

    struct node *temp1;
    struct node *temp2;

    for (temp1 = head; temp1 != NULL; temp1 = temp1->next)
    {
        for (temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next)
        {
            if (strcmp(temp1->data.lname, temp2->data.lname) > 0)
            {
                k = temp1->data;
                temp1->data = temp2->data;
                temp2->data = k;
            }
        }
    }
}
void sortFname(struct node *head)
{
    SREC k;

    struct node *temp1;
    struct node *temp2;

    for (temp1 = head; temp1 != NULL; temp1 = temp1->next)
    {
        for (temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next)
        {
            if (strcmp(temp1->data.lname, temp2->data.lname) > 0)
            {
                k = temp1->data;
                temp1->data = temp2->data;
                temp2->data = k;
            }
        }
    }
}
void sortSID(struct node *head)
{
    SREC k;

    struct node *temp1;
    struct node *temp2;

    for (temp1 = head; temp1 != NULL; temp1 = temp1->next)
    {
        for (temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next)
        {
            if (temp1->data.SID > temp2->data.SID)
            {
                k = temp1->data;
                temp1->data = temp2->data;
                temp2->data = k;
            }
        }
    }
}
void sortGPA(struct node *head)
{
    SREC k;

    struct node *temp1;
    struct node *temp2;

    for (temp1 = head; temp1 != NULL; temp1 = temp1->next)
    {
        for (temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next)
        {
            if (temp1-> data.GPA < temp2->data.GPA)
            {
                k = temp1->data;
                temp1->data = temp2->data;
                temp2->data = k;
            }
        }
    }
}

//Delete function (search)
void delete(unsigned long int checkSID, struct node *head)
{
    struct node *temp = head, *prev;

    if (temp != NULL && temp->data.SID == checkSID)
    {
        head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data.SID != checkSID)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return;
    }
    prev->next = temp->next;
    free(temp);

    return;    
}
//Save function
void save()
{
    FILE *fp;
    fp = fopen("List.txt", "w");

    struct node *ptr = head;
    while (ptr != NULL)
    {
        fwrite(ptr, sizeof(struct student),1, fp);
        ptr = ptr->next;
    }
    fclose(fp);

    return;
}


int main()
{
    FILE *fp;
    if (access("List.txt", F_OK) != -1)
    {    
        fp = fopen("List.txt", "rb");
        SREC newStudent;
        while((fread(&newStudent, sizeof(struct student), 1, fp) == 1))
        {
            insert(newStudent);
        }
    }
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0)
		error("ERROR opening socket");
 	
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	serv_addr.sin_port = htons(PORT_NUM);

    for (portno = PORT_NUM; portno < PORT_NUM+10; portno++)
    {
        if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
            printf("Failed: %d\n", portno);
            if (portno == 20039)
            {
                error("ERROR on binding");
            }
        }
        else
        {
            break;
        }
    }
    
    printf("Connected on: %d\n", portno);	
   
	listen(sockfd,5);
	
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd, 
			(struct sockaddr *) &cli_addr, 
			&clilen);
	if (newsockfd < 0) 
		error("ERROR on accept");
	memset(buffer, 0, 256);
    
    SREC newStu;

    while(1){
   /*Communicate************************************/
        n = read(newsockfd,buffer,255);
	    if (n < 0) error("ERROR reading from socket");
   	    if (strncmp("stop", buffer, 4) == 0)
        {
            close(sockfd);
            save();
            exit(1);
        }
        else if (buffer[0] == 1)
        {
            memset(buffer,0,256);
            int ma = countList();
            char str[100];
            sprintf(str, "%d", ma);
            n = write(newsockfd,str,100);
            if (n < 0) error("ERROR writing to socket");
            n = read(newsockfd, buffer, 255);
            printStu(newsockfd);
            sortLname(head);
            memset(buffer,0,256);
            struct node *ptr = head;
            while (ptr != NULL)
            {
                sprintf(buffer, "|");
                sprintf(buffer + strlen(buffer), "%05lu", ptr->data.SID);
                sprintf(buffer + strlen(buffer), "|"); 
                sprintf(buffer + strlen(buffer), "%-9s", ptr->data.lname);
                sprintf(buffer + strlen(buffer), "|");
                sprintf(buffer + strlen(buffer), "%-9s", ptr->data.fname);
                sprintf(buffer + strlen(buffer), "| ");
                sprintf(buffer + strlen(buffer), "%c", ptr->data.initial);
                sprintf(buffer + strlen(buffer), " | ");
                sprintf(buffer + strlen(buffer), "%.2f", ptr->data.GPA);
                sprintf(buffer + strlen(buffer), "|");
                n = write(newsockfd, buffer, 255);
                ptr = ptr->next;
                memset(buffer, 0, 256);
            }
            n = write(newsockfd, "+-----+---------+---------+---+-----+\n", 50);
            n = write(newsockfd, "endtable", 11);
            memset(buffer, 0, 256);
        }
        else if (buffer[0] == 2)
        {
            memset(buffer,0,256);
            int ma = countList();
            char str[100];
            sprintf(str, "%d", ma);
            n = write(newsockfd, str,100);
            if (n < 0) error("ERROR writing to socket");
            n = read(newsockfd, buffer, 255);
            printStu(newsockfd);
            sortFname(head);
            memset(buffer,0,256);
            struct node *ptr = head;
            while (ptr != NULL)
            {
                sprintf(buffer, "|");
                sprintf(buffer + strlen(buffer), "%05lu", ptr->data.SID);
                sprintf(buffer + strlen(buffer), "|");
                sprintf(buffer + strlen(buffer), "%-9s", ptr->data.lname);
                sprintf(buffer + strlen(buffer), "|");
                sprintf(buffer + strlen(buffer), "%-9s", ptr->data.fname);
                sprintf(buffer + strlen(buffer), "| ");
                sprintf(buffer + strlen(buffer), "%c", ptr->data.initial);
                sprintf(buffer + strlen(buffer), " | ");
                sprintf(buffer + strlen(buffer), "%.2f", ptr->data.GPA);
                sprintf(buffer + strlen(buffer), "|");
                n = write(newsockfd, buffer, 255);
                ptr = ptr->next;
                memset(buffer, 0, 256);
            }
            n = write(newsockfd,"+-----+---------+---------+---+-----+\n", 50);
            n = write(newsockfd, "endtable", 11);
            memset(buffer, 0, 256); 
        }
        else if (buffer[0] == 3)
        {
            memset(buffer,0,256);
            int ma = countList();
            char str[100];
            sprintf(str, "%d", ma);
            n = write(newsockfd, str,100);
            if (n < 0) error("ERROR writing to socket");
            n = read(newsockfd, buffer, 255);
            printStu(newsockfd);
            sortSID(head);
            memset(buffer,0,256);
            struct node *ptr = head;
            while (ptr != NULL)
            {
                sprintf(buffer, "|");
                sprintf(buffer + strlen(buffer), "%05lu", ptr->data.SID);
                sprintf(buffer + strlen(buffer), "|");
                sprintf(buffer + strlen(buffer), "%-9s", ptr->data.lname);
                sprintf(buffer + strlen(buffer), "|");
                sprintf(buffer + strlen(buffer), "%-9s", ptr->data.fname);
                sprintf(buffer + strlen(buffer), "| ");
                sprintf(buffer + strlen(buffer), "%c", ptr->data.initial);
                sprintf(buffer + strlen(buffer), " | ");
                sprintf(buffer + strlen(buffer), "%.2f", ptr->data.GPA);
                sprintf(buffer + strlen(buffer), "|");
                n = write(newsockfd, buffer, 255);
                ptr = ptr->next;
                memset(buffer, 0, 256);
            }
            n = write(newsockfd, "+-----+---------+---------+---+-----+\n", 50);
            n = write(newsockfd, "endtable", 11);
            memset(buffer, 0, 256);
        }
        else if (buffer[0] == 4)
        {            
		 memset(buffer,0,256);
            int ma = countList();
            char str[100];
            sprintf(str, "%d", ma);
            n = write(newsockfd, str,100);
            if (n < 0) error("ERROR writing to socket");
            n = read(newsockfd, buffer, 255);
            printStu(newsockfd);
            sortGPA(head);
            memset(buffer,0,256);
            struct node *ptr = head;
            while (ptr != NULL)
            {
                sprintf(buffer, "|");
                sprintf(buffer + strlen(buffer), "%05lu", ptr->data.SID);
                sprintf(buffer + strlen(buffer), "|");
                sprintf(buffer + strlen(buffer), "%-9s", ptr->data.lname);
                sprintf(buffer + strlen(buffer), "|");
                sprintf(buffer + strlen(buffer), "%-9s", ptr->data.fname);
                sprintf(buffer + strlen(buffer), "| ");
                sprintf(buffer + strlen(buffer), "%c", ptr->data.initial);
                sprintf(buffer + strlen(buffer), " | ");
                sprintf(buffer + strlen(buffer), "%.2f", ptr->data.GPA);
                sprintf(buffer + strlen(buffer), "|");
                n = write(newsockfd, buffer, 255);
                ptr = ptr->next;
                memset(buffer, 0, 256);
            }
            n = write(newsockfd, "+-----+---------+---------+---+-----+\n", 50);
            n = write(newsockfd, "endtable", 11);
            memset(buffer, 0, 256);
        }
        else if (strncmp("put", buffer, 3) == 0)
        {
            n = read(newsockfd, &newStu, sizeof(struct student));
            insert(newStu);
            n = write(newsockfd, "Received SREC.", 20);
            memset(buffer, 0, 256);
        }
        else if (strncmp("delete", buffer, 6) == 0)
        {
            n = read(newsockfd, buffer, 255);
            char *ptr;
            unsigned long int ret;

            ret = strtoul(buffer, &ptr, 10);
            delete(ret, head);
            n = write(newsockfd, "SREC Deleted.", 20);
            memset(buffer,0,256);
        }
        else
        {
            printf("Unrecognized command: %s", buffer);
            n = write(newsockfd, "Unrecognized command - Please try again.", 40);
            memset(buffer, 0, 256);
        }
    }
	return 0; 
}
void printStu(int sockfdnew)
{
    int n = write(sockfdnew,"| SID | Lname   | Fname   | M | GPA | \n+-----+---------+---------+---+-----+\n",100);
    if (n < 0) error("ERROR writing to socket");

    return;
}

