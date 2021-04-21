SERVER = Server/TCPserver
CLIENT = Client/TCPclient


all: $(SERVER).exec $(CLIENT).exec

$(SERVER).exec: $(SERVER).c makefile
	gcc -o server.exec $(SERVER).c

$(CLIENT).exec: $(CLIENT).c makefile
	gcc -o client.exec $(CLIENT).c 


test:
	./server.exec & 
	./client.exec


clean: 	
	rm *.exec output.txt