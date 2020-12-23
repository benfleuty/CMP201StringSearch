// StringSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <string>
#include <filesystem>


/// <summary>
/// Clear the console - should work on most OS
/// From https://stackoverflow.com/questions/6486289/how-can-i-clear-console
/// Answer by: Joma
/// </summary>
/*
void Clear()
{
#if defined _WIN32
    system("cls");
    //clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
    system("clear");
#endif
    std::cout << "Ben Fleuty | CMP 201 Assessment | String search\n";
}
*/

enum class MessageType {
    Menu,
    RabinKarp
};

void Clear(MessageType message)
{
#if defined _WIN32
    system("cls");
    //clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
    system("clear");
#endif

    switch (message) {
    case MessageType::Menu:
        std::cout << "Ben Fleuty | CMP 201 Assessment | String search\n";
        std::cout << "Main menu\n\n";
        break;

    case MessageType::RabinKarp:
        std::cout << "-------------------------------\n";
        std::cout << "|String search with Rabin Karp|\n";
        std::cout << "-------------------------------\n";
        break;

    default:
        std::cout << "Error in clear -> title printing";
    }
}

std::string GetFileText(std::string fileToRead) {
    // read text file
    // file to read from
    std::ifstream MyFile(fileToRead);
    // temp line storage
    std::string line;
    // output text storage
    std::string output;
    // loop through each line and append the line to the string
    while (std::getline(MyFile, line)) output.append(line);
    // return the contents of the file
    return output;
}

std::string GetText() {
    //// get text file
    std::string input;
    do {
        std::cout << "Enter y when the text you want to search is in a text file named \"search.txt\" in the same folder as this .exe\n";
        std::getline(std::cin, input);
    } while (input[0] != 'y');
    return GetFileText("search.txt");
}

std::string GetPattern() {
    //// get pattern
    std::string pattern;
    do {
        std::cout << "Enter the pattern you want to search for: ";
        std::getline(std::cin, pattern);
    } while (pattern.length() < 1);

    std::string input;
    do {
        std::cout << "\nYou are searching for: " << pattern << "\n";
        std::cout << "Is this correct? [y/n]: ";
        std::getline(std::cin, input);
        if (input[0] == 'n') return GetPattern();
    } while (input[0] != 'y');

    return pattern;
}

void RabinKarp() {
    Clear(MessageType::RabinKarp);    
    // text to search
    std::string text = GetText();
    // get pattern to search for 
    std::string pattern = GetPattern();
    Clear(MessageType::RabinKarp);
    std::cout << "Searching for: " << pattern;
}

int main()
{
    Clear(MessageType::Menu);
    bool quit = false;
    do {
        std::cout << "Select the algorithm to use:\n";
        std::cout << "1 - Rabin Karp\n";
        std::cout << "2 - Not implemented\n";
        std::cout << "3 - Quit application\n";
        std::cout << ">";
        std::string input;
        std::getline(std::cin, input);

        char in = input[0];

            if (in == '1')
                RabinKarp();
            else if (in == '3' || in == 'q' || in == 'Q')
                quit = true;
            else
                std::cout << "\nInvalid input: '" << input << "'\n";
    } while (!quit);
    return 0;
}