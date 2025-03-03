#ifndef BLOCK_HPP
# define BLOCK_HPP
# include "usefull.hpp"

class Block
{
    protected:
        std::string     _root;
        vecStr          _index;
        size_t          _maxClientBodySize;
        vecInt          _methods;
        mapErr          _errorPages;
        std::string     _redirUrl;
        bool            _autoindex;

        bool            _rootSet;
        bool            _indexSet;
        bool            _maxClientBodySizeSet;
        bool            _methodsSet;
        bool            _errorPagesSet;
        bool            _redirUrlSet;
        bool            _autoindexSet;

        bool            _formatOk;
        std::string     _formatErr;

        void            _setRoot(vecStr words);
        void            _setIndex(vecStr words);
        void            _setMaxClientBody(vecStr words);
        void            _setMethods(vecStr words);
        void            _setErrorPages(vecStr words);
        void            _setAutoIndex(vecStr words);
        void            _setRedirUrl(vecStr words);

        void            _setWrongFormat(void);
        void            _setWrongFormat(std::string str);

    public:
        Block(void);
        Block(std::string str);
        Block(const Block &block);
        virtual ~Block();

        Block&         operator=(const Block &block);

        bool           wellFormatted(void) const;

        // getters
        const vecStr    getServerNames(void) const;
        const std::string  getRoot(void) const;
        const vecStr    getIndex(void) const;
        size_t          getMaxClientBody(void) const;
        const vecInt    getMethods(void) const;
        const mapErr    getErrorPages(void) const;
        const std::string  getRedirUrl(void) const;
        bool            getAutoIndex(void) const;
        const std::string  getFormatErr(void) const;

        bool            isRootSet(void) const;
        bool            isIndexSet(void) const;
        bool            isMaxClientBodySet(void) const;
        bool            isMethodsSet(void) const;
        bool            isErrorPagesSet(void) const;
        bool            isRedirUrlSet(void) const;
        bool            isAutoindexSet(void) const;
};

#endif
