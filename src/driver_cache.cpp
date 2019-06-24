#include <iostream>
#include "../include/cache.h"

int main (int argc, char *argv[]){
    Cache::Cache cache(argc, argv); // read the commands

    std::string command;
    int number1, number2;
    while(true){

        std::cout << "\033[1;35m -------------------------------------- \033[0m\n";
        std::cout << "\033[1;35m >>> Choose a command:\n - Stop\n - Read\n - Write\n - Show\n \033[0m";
        
        std::cout << "\033[1;35m⇨ \033[0m";
        std::cin >> command;
        if(command == "Stop") break; // Stops running with an zero.
        if(command == "Read") std::cin >> number1;
        else if(command == "Write") std::cin >> number1 >> number2;
        
        if(command == "Show"){
            cache.Show();
        }
        else if(command == "Write"){
            cache.Write(command, number1, number2);
        }
        else if(command == "Read"){
            cache.Read(command, number1, number2);
        }

        std::cout << std::endl;
    }


    return 0;  
}