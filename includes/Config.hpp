#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <fstream>
# include "Server.hpp"

typedef	std::vector<Server>	vecSrv;

class Config
{
	private:
		vecSrv	_servers;
		bool	_formatOk;

		Config(void);
		
	public:
		Config(const std::string file);
		Config(const Config &config);
		virtual ~Config();

		Config	&operator=(const Config &config);

		vecSrv	getServers(void) const;
		bool	wellFormatted(void) const;
};

std::ostream	&operator<<(std::ostream &o, const Config &config);

#endif
