CC_ARM = arm-buildroot-linux-gnueabihf-gcc
CC_x86 = gcc
CFLAGS = 

msg_tcp_server: msg_tcp_server.c
	$(CC_ARM) $(CFLAGS) -o msg_tcp_server msg_tcp_server.c

msg_tcp_client: msg_tcp_client.c
	$(CC_x86) $(CFLAGS) -o msg_tcp_client msg_tcp_client.c


msg_udp_server: msg_udp_server.c
	$(CC_ARM) $(CFLAGS) -o msg_udp_server msg_udp_server.c

msg_udp_client: msg_udp_client.c
	$(CC_x86) $(CFLAGS) -o msg_udp_client msg_udp_client.c

msg_server_broadcast: msg_server_broadcast.c
	$(CC_ARM) $(CFLAGS) -o msg_server_broadcast msg_server_broadcast.c

msg_client_broadcast: msg_client_broadcast.c
	$(CC_x86) $(CFLAGS) -o msg_client_broadcast msg_client_broadcast.c

msg_server_muticast: msg_server_muticast.c
	$(CC_ARM) $(CFLAGS) -o msg_server_muticast msg_server_muticast.c

msg_client_muticast: msg_client_muticast.c
	$(CC_x86) $(CFLAGS) -o msg_client_muticast msg_client_muticast.c

clean:	
	rm -f msg_tcp_server msg_tcp_client msg_udp_server msg_udp_client msg_server_broadcast msg_client_broadcast
