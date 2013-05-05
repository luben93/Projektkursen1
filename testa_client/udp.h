int lyssna(char *p);
int beejsconnect(char *portnr);

int create( char* portname);
int hear(int fd,char *buf);
int talk(char *s);
void *get_in_addr(struct sockaddr *sa);
