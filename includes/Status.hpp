#ifndef STATUS_HPP
# define STATUS_HPP
# include "usefull.hpp"
# include "httpResponse.hpp"

class Status : public HTTPResponse
{
	protected:
		std::string	_code;
		typedef void (Status::*ptr)(int);
		ptr			getStatus[3];

	public:
		Status();
		Status(const Status &status);
		virtual ~Status();

		Status	&operator=(const Status &status);

		std::string status(int code, int type);
		void        err4xx(int type);
		void        status2xx(int type);
		void        err5xx( int type);

		void		clear(void);
};

#endif
