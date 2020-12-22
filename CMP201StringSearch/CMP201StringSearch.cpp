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

void RabinKarp() {
    Clear();
    std::cout << "-------------------------------\n";
    std::cout << "|String search with Rabin Karp|\n";
    std::cout << "-------------------------------\n";
    // get text file
    std::string input;
    do {
        std::cout << "Type y when the text you want to search is in a text file named \"search.txt\" in the folder that this .exe is in";
        std::getline(std::cin, input);
    } while (input[0] != 'y');


    // text to search
    std::string text;

    // temp file to store each line
    std::string line;

    std::ifstream fileToRead("search.txt");

    // iterate each line in the file and add the line to the text to search
    while (std::getline(fileToRead, line)) text.append(line);


    

}


int main()
{
    bool quit = false;
    do {

        std::cout << "Ben Fleuty | CMP 201 Assessment | String search\n";
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
