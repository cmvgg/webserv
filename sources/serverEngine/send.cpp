#include "ServerEngine.hpp"

static int	sendHeader(int fde, Client &client, Socket &sock, bool redir, int sockNbr)
{
	HTTPResponse	&response = client.getResponse();
	std::string		header = response.getHeader();

	if (sock.isCgi(sockNbr, response.getUrl()) && !redir)
		header = Cgiheader(header, client);
	else
		header += "\r\n";
	if (LOG_LEVEL >= LVL_RESPONSE)
	{
		std::cout << std::endl << "<----------- Response ----------->" << std::endl;
		std::cout << header.substr(0, header.size()) << std::endl;
	}
	if (send(fde, header.c_str(), header.size(), 0) < 0)
	{
		perror("send()");
		return (ERROR);
	}
	return (OK);
}

static size_t chunkSize = 0;

static int	sendData(int fde, Client &client, bool isCgi)
{
	HTTPResponse		&response = client.getResponse();
	std::string			fileName(response.getFileName());
	std::string			tmpname("html/tmp.html");
	std::stringstream	ss;
	size_t				size;
	std::string			line;
	ss << response.getContentLen();
	ss >> size;
	if (isCgi)
	{
		if (response.isChunked())
		{
			size_t				sizeOfChunk;
			std::ostringstream	os;
			std::string			body = client.getCgiBody();
			size = body.length();

			if (response.getMaxSizeC() < size)
				sizeOfChunk = response.getMaxSizeC();
			else
				sizeOfChunk = size;
			if (size - chunkSize < sizeOfChunk)
				sizeOfChunk = size - chunkSize;
			os << std::hex << sizeOfChunk;
			line = os.str() + "\r\n";
			line += body.substr(chunkSize, sizeOfChunk);
			chunkSize += sizeOfChunk;
			if (chunkSize >=  size)
				line +=  "0\r\n";
			line = body;
		}
		else
			line = client.getCgiBody();
		std::stringstream	fileStream(line, std::ios::in | std::ios::binary);
		char				buf[BUFFER_SIZE];
		int					i;
		char				c;

		if (response.getMethod() == "HEAD" || response.getMethod() == "OPTIONS")
			return (OK);
		while (fileStream.get(c))
		{
			memset(buf, 0, BUFFER_SIZE);
			buf[0] = c;
			i = 1;
			while (fileStream.get(c) && i + 1 < BUFFER_SIZE)
			{
				buf[i] = c;
				i++;
			}
			if (i + 1 == BUFFER_SIZE)
			{
				buf[i] = c;
				i++;
			}
			if (send(fde, buf, i, 0) < 0)
			{
				perror("send()");
				fileStream.str(std::string());
				fileStream.clear();
				return (ERROR);
			}
		}
		fileStream.str(std::string());
		fileStream.clear();
	}
	else
	{
		if (response.isChunked())
		{
			std::ifstream	fileS(fileName.c_str(), std::ios::in | std::ios::binary);
			std::ofstream	tmpfile(tmpname.c_str());
			std::string		line;
			size_t			i;
			size_t			sizeOfChunk;

			if (response.getMaxSizeC() < size)
				sizeOfChunk = response.getMaxSizeC();
			else
				sizeOfChunk = size;

			char *buff = new char [sizeOfChunk + 1];
			for (i = 0; i < chunkSize; i += sizeOfChunk )
				fileS.read(buff, sizeOfChunk);

			if (size - chunkSize < sizeOfChunk)
				sizeOfChunk = size - chunkSize;
			tmpfile << std::hex << sizeOfChunk << "\r\n";
			fileS.read(buff, sizeOfChunk);
			buff[sizeOfChunk] = '\0';
			tmpfile << buff << "\r\n";
			delete [] buff;
			chunkSize += sizeOfChunk;
			if (chunkSize >=  size)
				tmpfile << "0" << "\r\n";
			fileName = tmpname;
			response.setUrl(tmpname);
			tmpfile.close();
			fileS.close();
		}
		std::ifstream	fileStream(fileName.c_str(), std::ios::in | std::ios::binary);
		char			buf[BUFFER_SIZE];
		int				i;
		char			c;
		if (response.getMethod() == "HEAD" || response.getMethod() == "OPTIONS")
			return (OK);
		while (fileStream.get(c))
		{
			memset(buf, 0, BUFFER_SIZE);
			buf[0] = c;
			i = 1;
			while (fileStream.get(c) && i + 1 < BUFFER_SIZE)
			{
				buf[i] = c;
				i++;
			}
			if (i + 1 == BUFFER_SIZE)
			{
				buf[i] = c;
				i++;
			}
			if (send(fde, buf, i, 0) < 0)
			{
				perror("send()");
				fileStream.close();
				return (ERROR);
			}
		}
		fileStream.close();
	}
	if (response.isChunked() && chunkSize < size)
		sendData(fde, client, isCgi);
	return (OK);
}

int		sendResponse(Client &client, Socket &sock, int sockNbr)
{
	HTTPHeader		&header = client.getHeader();
	HTTPResponse	&response = client.getResponse();
	int				fde = client.getFd();

	if (response.statusIsOk() && !sock.isAllowedMethod(sockNbr, response.getUrl(), getMethodNb(header.getMethod())))
		response.setStatus("405", " Method Not Allowed", header);

	if (getFile(sock, sockNbr, client))
	{
		if (response.getRedir() == 1)
		{
			response.rendering(client.getHeader());
			response.setRedir(0);
			if (sendHeader(fde, client, sock, true, sockNbr))
				return (ERROR);
			return (OK);
		}
		else if (response.getNeedAutoindex())
			return (autoindexPageSend(fde, response, response.getUrl(), sock.getRoot(sockNbr, response.getUrl())));
		return (defaultPageSend(fde, response));
	}
	
	if (sock.isCgi(sockNbr, response.getUrl()))
	{
        header.setContentTypeResponse("text/html");
        response.rendering(header);
		cgiSetEnv(sock, sockNbr, client);
		if (execCGI(sock.getCgiPass(sockNbr, response.getUrl()), client) < 0)
			return (ERROR);
	}
	if (LOG_LEVEL >= LVL_INFO)
	{
		std::cout << "[Sending] data to " << fde;
		std::cout << " from " << sock.getRealUrl(sockNbr, response.getUrl()) << std::endl;
	}

	downloading(header, response);

	if (sendHeader(fde, client, sock, false, sockNbr))
		return (ERROR);
	if (sendData(fde, client, sock.isCgi(sockNbr, response.getUrl())))
		return (ERROR);

	chunkSize = 0;
	remove("html/tmp.html");
	return (OK);
}
