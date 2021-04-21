/*
Due Date:       April 5th, 2021
Creators:       Casey Rock & Malachi Parks
Section:        CISC450-010
Assignment:     Programming Assignment 1

File Description: TCP server 
*/

#include "../TCP.h"



int main(int argc, char const * argv[]){
    struct sockaddr_in serverAddr ;
    int serverSocketID ;
    int objectSockerID;
    char recvBuff[MAX_FILE_NAME];
    char filepath[MAX_FILE_NAME]= "./Server/" ;
    FILE *fp ;
    char fileBuffer[MESSMAX];
    struct packet sendPacket;
    struct packet final ;
    int sequenceNumberCount = 0;
    int totalBytes = 0;
    char echo[MESSMAX];
    
    
    

    
    // Create a socket, if failed exit with failure
    if((serverSocketID = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))< 0){
        perror("Server Socket Failed");
        exit(EXIT_FAILURE);
    }
 
    serverAddr.sin_family = AF_INET ;  // IPv4 Family
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Make sure it can take any IPv4 address
    serverAddr.sin_port = htons(PORT); // Change for network byte order to host byte order
  
    // Bind a socket to server
    if(bind(serverSocketID, (struct sockaddr * ) &serverAddr, sizeof(serverAddr)) < 0){
        perror("Server Bind Failed");
        exit(EXIT_FAILURE);
    }

    // Use binded socket for listening
    if(listen(serverSocketID, SERVER_LISTEN_QUEUE) < 0){
        perror("Server Listen Failed");
        exit(EXIT_FAILURE);
    }

    // Accept connection from client to sync
    if((objectSockerID = accept(serverSocketID,(struct sockaddr * ) &serverAddr, (socklen_t*)&serverAddr)) < 0){
        perror("Server Accept Failed");
        exit(EXIT_FAILURE);
    }

    // File name recieved 0 then kill connection
    if(recv(objectSockerID,recvBuff, MAX_FILE_NAME, 0 ) < 0){
        perror("Server Accept Failed");
        exit(EXIT_FAILURE);
    }

    // Concat buff to filepath so file can be found in Server dir
    strcat(filepath,recvBuff) ;
    
    // If can't open then push out error
    if((fp = fopen(filepath, "r")) < 0){
        perror("Failed to open");
        exit(EXIT_FAILURE);
    };
    
    //clear buffer of received file
    memset(recvBuff, 0, sizeof recvBuff);

    if(fp > 0){ // make sure file exists, so it doesn't kick back seg fault
        while(fread(sendPacket.data, MESSMAX, 1, fp)){
            sequenceNumberCount++;
            sendPacket.pktSequenceNumber = sequenceNumberCount; // this needs to be a VAR 
            sendPacket.count = sizeof sendPacket.data + 4; 
            totalBytes += sendPacket.count ;
            
	    // Send packet to client
            printf("Packet: %d transmitted with %d data bytes \n", sendPacket.pktSequenceNumber, sendPacket.count);
            if(send(objectSockerID , &sendPacket, sendPacket.count, 0) < 0 ){
                perror("Server Send Failed");
                exit(EXIT_FAILURE);
            };

	    // Ensuring packet recieved, echo same information to Server console
            if(recv(objectSockerID, echo , MAX_FILE_NAME, 0 ) < 0){
                perror("Server Echo Failed");
                exit(EXIT_FAILURE);
            }

            // Clear buffer in case
            memset(sendPacket.data, 0, sizeof sendPacket.data);
        }
    }else{
        printf("File does not exist");
    }

    // Final send of information in pkt
    sequenceNumberCount++;
    sendPacket.pktSequenceNumber = sequenceNumberCount; // this needs to be a VAR 
    sendPacket.count = 0; 
    totalBytes += sendPacket.count ;
    
    if(send(objectSockerID ,&sendPacket, sizeof sendPacket.data + 4, 0) < 0 ){
            perror("Server end of transission Send Failed");
            exit(EXIT_FAILURE);
    };

    printf("End of Transmission Packet with sequence number %d transmitted with %d data bytes \n\n", sequenceNumberCount, sendPacket.count);

    printf("Number of data packets transmitted: %d \n", sequenceNumberCount);
    printf("Total number of data bytes transmitted: %d\n", totalBytes) ;
    
    fclose(fp); // close file
    close(objectSockerID); // close object socket ID
    close(serverSocketID); // close Server connection


}
