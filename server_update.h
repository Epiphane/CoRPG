#ifndef SERVER_UPDATE_H
#define SERVER_UPDATE_H 

#define PACKET_SIZE 64

typedef enum { PLAYER_UPDATE, MESSAGE } UpdateType;
typedef struct ServerUpdate {
	UpdateType method;
	char descriptor[PACKET_SIZE];
	char data      [PACKET_SIZE];
} ServerUpdate;

#endif /* SERVER_UPDATE_H */
