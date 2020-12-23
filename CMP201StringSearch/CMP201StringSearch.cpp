// StringSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <string>
#include <filesystem>
#include <unordered_map>
#include <math.h>
#include <vector>

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
	RabinKarp,
	BoyerMoore
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

	case MessageType::BoyerMoore:
		std::cout << "--------------------------------\n";
		std::cout << "|String search with Boyer Moore|\n";
		std::cout << "--------------------------------\n";
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

void Search_BoyerMoore(const std::string& text, const std::string& pattern) {
	unsigned int textLength = text.length();
	unsigned int patternLength = pattern.length();
	unsigned int patternSkip = patternLength - 1;
	unsigned int patternLastChar = pattern[patternLength - 1];
	unsigned int lastPosition = textLength - patternLength;

	std::vector<unsigned int> indexesFound;

	for (size_t i = 0; i < lastPosition; i++)
	{
		// if the last character in the chunk doesnt match
		// the last char of the pattern, skip
		if (text[i + patternLength] != patternLastChar)
		{
			i += patternSkip;
			continue;
		}

		bool match = false;

		// the last character in the chunk matches the
		// last char of the pattern iterate pattern to
		// check each char
		for (size_t j = 0; j < patternLength; j++)
		{
			// i    = index of text chunk
			// j    = index of pattern
			// i+j  = relative position in the text chunk

			// if the chars don't match
			if (text[i + j] != pattern[j]) {
				// stop checking the pattern
				match = false;
				break;
			}

			match = true;
		}

		// if you made it here then the pattern and chunk are a match
		// add index to list of viable indexes
		if (match) indexesFound.push_back(i);

		// skip i to end of the match
		i += patternSkip;
	}
}

void BoyerMoore() {
	Clear(MessageType::BoyerMoore);
	// text to search
	std::string text = GetText();
	// get pattern to search for
	std::string pattern = GetPattern();
	Clear(MessageType::BoyerMoore);
	std::cout << "Searching for: " << pattern;

	Search_BoyerMoore(text, pattern);

	std::string x = "";
	std::getline(std::cin, x);
}

void Search_RabinKarp(std::string* text, std::string* pattern) {
	const int textLength = text->length();
	// bool matchFound[textLength]; define at runtime?
	// otherwise vector?
}

unsigned int RollingHash(const std::string& input) {
	unsigned int total = 0;
	unsigned int p = input.length() + 1;
	for (size_t i = 0; i < input.length(); i++)
		total += input[i] * (int)pow(p, i);

	return total;
}

void RabinKarp() {
	Clear(MessageType::RabinKarp);
	// text to search
	std::string text = GetText();
	// get pattern to search for
	std::string pattern = GetPattern();
	Clear(MessageType::RabinKarp);
	std::cout << "Searching for: " << pattern;

	// hash the pattern to compare
	unsigned int hashedPattern = RollingHash(pattern);

	std::cout << "\n\n" << hashedPattern << "\n\n";

	std::string x = "";
	std::getline(std::cin, x);
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