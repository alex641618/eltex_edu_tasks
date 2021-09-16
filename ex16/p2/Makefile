chat-server: chat-server.c iface.c
	gcc chat-server.c -o chat-server -lrt -lcurses -g
	gcc iface.c -o iface -g
	./chat-server
chat-client: chat-client.c
	gcc chat-client.c -o chat-client -lrt -g
	./chat-client chat-server
clean:
	rm -rf chat-client
	rm -rf chat-server
	rm -rf iface