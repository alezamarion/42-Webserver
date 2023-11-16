#include "Libs.hpp"
#include "ConfigParser.hpp" 



void stopAndClean(int signal)
{
    std::cout << '\n';
    std::cout << "CTRL + C received: freeing up resources" << std::endl;
    (void)signal;
    //webConfigSpec.stop()
    exit(0);
}

void setupSignalHandler()
{
    struct sigaction signalHandler;
    signalHandler.sa_handler = stopAndClean;
    sigemptyset(&signalHandler.sa_mask);       // Clear the signalHandler.sa_mask, meaning no additional signals are blocked during the execution of the signal handler.
    signalHandler.sa_flags = 0;                // Set sa_flags to 0, indicating no special flags for the signal handler.
    sigaction(SIGINT, &signalHandler, 0);
}

int main(int argc, char **argv)
{
    ConfigParser ConfigParser;
    
    if (argc != 2)
    {
        if (argc < 2)
            std::cerr << "Error: missing ConfigParseruration file" << std::endl;
        if (argc > 2)
            std::cerr << "Error: too many arguments" << std::endl;
        return (1);
    }

    setupSignalHandler();
    ConfigParser.handleConfigParserFile(argv[1]);

    (void)**argv;

    std::cout << "run webserver" << std::endl;

    while (true){}

    /* return(webConfigSpec.run(argv[1])); */
}




