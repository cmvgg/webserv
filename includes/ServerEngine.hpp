#ifndef SERVER_ENGINE_HPP
#define SERVER_ENGINE_HPP

#define BUFFER_SIZE 4096
#define B_SIZE 5

/* Status Codes */
#define END_REQUEST 1
#define CLOSE_CONNECTION 2
#define BAD_REQUEST 3
#define WRONG_CHUNKED -2

/* Log Levels */
#define LVL_INFO 0
#define LVL_RESPONSE 1
#define LVL_REQUEST 2
#define LOG_LEVEL -1

# include "Socket.hpp"

/* epoll */
int			initConnect(Socket &sock, int i);
int			initEpoll(Socket &sock);


/* message */
int			requestResponse(int fde, Socket &sock);

/* cgi */
std::string	Cgiheader(std::string header, Client &client);
void        cgiSetEnv(Socket &sock, int sockNbr, Client &client);
int			execCGI(std::string cgi, Client &client);
int			getFile(Socket &sock, int sockNbr, Client &client);

/* env */
void    	freeEnv(char ***env, Client &client);
int     	createEnv(char ***env, Client &client);

/* send */
int			sendResponse(Client &client, Socket &sock, int sockNbr);
int			defaultPageSend(int fde, HTTPResponse &response);
int			autoindexPageSend(int fde, HTTPResponse &response, std::string path, std::string root);

/* utils */
void		downloading(HTTPHeader &header, HTTPResponse &response);
int			requestEnd(Client &client);
int			headerChecker(HTTPHeader &header, const std::string string);

#endif
