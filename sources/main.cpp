#include "../includes/ServerEngine.hpp"

static bool validateArguments(int argc) {
    if (argc > 2) {
        std::cerr << "Usage: ./webserv \"config.conf\"" << std::endl;
        return false;
    }
    return true;
}

static int runServer(int argc, char** argv) {
    try {
        Config serverConfig(fileName(argc, argv));
        if (!serverConfig.wellFormatted()) {
            std::cerr << "Error: Configuration file is not well formatted." << std::endl;
            return EXIT_FAILURE;
        }

        Socket serverSocket(serverConfig);
        if (serverSocket.getCheck() < 0) {
            std::cerr << "Error: Failed to initialize server socket." << std::endl;
            return EXIT_FAILURE;
        }

        if (initEpoll(serverSocket) < 0) {
            std::cerr << "Error: Failed to initialize epoll." << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Unhandled exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}

int main(int argc, char** argv) {
    if (!validateArguments(argc)) {
        return EXIT_FAILURE;
    }
    return runServer(argc, argv);
}
