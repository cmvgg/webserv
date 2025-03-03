#include "ServerEngine.hpp"

int		getFile(Socket &sock, int sockNbr, Client &client)
{
	std::string 		filename;
	HTTPResponse		&response = client.getResponse();
	HTTPHeader			&header = client.getHeader();

	filename = response.redirect(sock, sockNbr, header, client);
	if (filename == "")
		return (ERROR);

	response.setFileName(filename);
	header.setContentTypeResponse(mimeContentType(header.getAccept(), filename));
	response.setContentLen(getFileSize(filename));
	response.rendering(header);
	return (OK);
}

int		headerChecker(HTTPHeader &header, const std::string string)
{
	std::string	copy = string;

	copy.erase(0, (getHead(copy)).length() + 2);
	while (1)
	{
		if (copy == "")
			break ;
		if (header.fillheader(copy) == -1)
			break ;
	}
	if (header.header() == -1)
		return (ERROR);
	return (OK);
}

static void	checkFirstAndEnd(int &end, Client &client)
{
	HTTPResponse	&response = client.getResponse();
	HTTPHeader		&header = client.getHeader();
	Status			&status = client.getStatus();

	if (!onlySpaces(client.getRequest()))
	{
		if (client.isFirstLine())
		{
			client.changeFirstLine();
			if (header.method(client.getRequest(), status, response) == -1)
				end = BAD_REQUEST;
			client.updateMethod();
		}
		if (requestEnd(client) && end == 0)
			end = END_REQUEST;
	}
}

static int	treatEndRequest(Client &client, int &end, Socket &sock, int sockNbr, int fde)
{
	HTTPResponse	&response = client.getResponse();
	HTTPHeader		&header = client.getHeader();
	Status			&status = client.getStatus();

	if (end == BAD_REQUEST)
		response.statusCode(status.status(4, 0), header.getFirstLine());
	if (end == BAD_REQUEST || end == END_REQUEST)
	{
		if (headerChecker(header, client.getRequest()))
			response.statusCode(status.status(4, 0), header.getFirstLine());
		else if (client.getBodySize() > sock.getMaxClientBodySize(sockNbr, response.getUrl()))
			response.statusCode(status.status(4, 13), header.getFirstLine());
		header.setContentTypeResponse(mimeContentType(header.getAccept(), header.getUrl()));
		response.setServerName(sock.getServerName(sockNbr));
		response.setMaxSizeC(sock.getMaxClientBodySize(sockNbr, response.getUrl()));
		if (sendResponse(client, sock, sockNbr))
			return (ERROR);
		if (!response.statusIsOk())
			end = CLOSE_CONNECTION;
		if (end != BAD_REQUEST && end != CLOSE_CONNECTION)
			client.clear();
	}
	if (end == BAD_REQUEST || end == CLOSE_CONNECTION)
	{
		if (LOG_LEVEL >= LVL_INFO)
			std::cout << "[Closing] connection with " << fde << std::endl;
		client.clear();
		sock.removeClient(fde);
		epoll_ctl(sock.getEpollFd(), EPOLL_CTL_DEL, fde, NULL);
		close(fde);
	}
	return (OK);
}

int		requestResponse(int fde, Socket &sock)
{
	char			buf[BUFFER_SIZE + 1];
	int				byteCount = 0;
	int				end = 0;
	int				sockNbr = sock.getConnection(fde);

	if (!sock.isConnectedClient(fde))
		sock.addClient(fde);
	Client			&client = sock.getClient(fde);
	
	if (client.getNeedReopen())
		client.openNewTmp();

	memset(buf, 0, BUFFER_SIZE);	
	byteCount = recv(fde, buf, BUFFER_SIZE, 0);
	if (byteCount > 0)
		buf[byteCount] = 0;
		
	if (LOG_LEVEL >= LVL_INFO)
		std::cout << "[Received] " << byteCount << " bytes from " << fde << std::endl;

	if (byteCount == 0)
		end = CLOSE_CONNECTION;
	else if (byteCount < 0)
	{
		checkFirstAndEnd(end, client);
		end = END_REQUEST;
	}
	else if (!onlySpaces(buf) || !client.isFirstLine())
	{
		client.addRecv(buf, byteCount);
		checkFirstAndEnd(end, client);
	}
	return (treatEndRequest(client, end, sock, sockNbr, fde));
}
