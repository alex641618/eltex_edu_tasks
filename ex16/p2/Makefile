chat-server: chat-server.c
	gcc chat-server.c -o chat-server -lrt -g
	./chat-server
	rm -tf chat-server
chat-client: chat-client.c
	gcc chat-client.c -o chat-client -lrt -g
	./chat-client chat-server
	rm -rf chat-client
