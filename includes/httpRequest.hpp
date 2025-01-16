#ifndef REQUEST_HPP
# define REQUEST_HPP
# include "usefull.hpp"

class HTTPRequest
{
	protected:
		std::string _META;		
		std::string _OPTION;	
		std::string _method;	
		std::string	_httpv;		
		std::string _url;		
		int			_active;
		std::string _fLine;

	public:
		HTTPRequest();
		HTTPRequest(const HTTPRequest &request);
		virtual ~HTTPRequest();

		HTTPRequest	&operator=(const HTTPRequest &request);
		std::string		getFirstLine(void);
		std::string		getMethod() const;
		std::string		getUrl() const;
		void			clear(void);
};

#endif