#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "usefull.hpp"

class Socket;
class HTTPHeader;
class Client;
class HTTPResponse
{
	protected:
		std::string 	_options;
		std::string 	_contentLen;
		std::string 	_protocol;
		std::string		_statusCode;
		std::string		_url;
		std::string		_header;
		std::string		_method;
		std::string		_fileName;
		std::string		_location;
		std::string		_serverName;
		int				_statusNb;
		int				_redir;
		bool			_needAutoindex;
		int				_chunked;
		size_t			_max_size_c;

		std::string		_returnErrPage(Socket &sock, int sockNbr);
		std::string		_returnSetErrPage(Socket &sock, int sockNbr, std::string code,
										std::string str, HTTPHeader &header);
		std::string		_manageDirectory(Socket &sock, int sockNbr, HTTPHeader &header);

	public:
		HTTPResponse(void);
		HTTPResponse(const HTTPResponse &response);
		virtual ~HTTPResponse();

		HTTPResponse	&operator=(const HTTPResponse &response);

		std::string	getUrl(void) const;
		std::string	getProtocol(void) const;
		std::string	getContentLen(void) const;
		std::string	getMethod(void) const;
		std::string	getHeader(void) const;
		std::string	getStatus(void) const;
		std::string	getFileName(void) const;
		int			getStatusNb(void) const;
		int			getMethodNbr(void) const;
		int			getRedir(void) const;
		size_t		getMaxSizeC(void) const;
		bool		getNeedAutoindex(void) const;
		bool		statusIsOk(void) const;

		void		rendering(HTTPHeader &header);

		void		setContentLen(int len);
		void		setStatusErr(int nbErr);
		int			setStatus(std::string code, std::string str, HTTPHeader &header);
		void		setFileName(const std::string &file);
		void		setStatusNb(int nb);
		void		setUrl(const std::string &url);
		void		setMethod(const std::string &method);
		void		setRedir(int r);
		void		setMaxSizeC(size_t value);
		void		setServerName(const std::string serv);
		void		setHeader(std::string header);

		int			isChunked(void);
		std::string	checkUrl(Socket &sock, int sockNbr, HTTPHeader &header);
		std::string	redirect(Socket &sock, int sockNbr, HTTPHeader &header, Client &client);
		void		statusCode(std::string status, std::string firstLine);

		void		clear(void);
};

#endif