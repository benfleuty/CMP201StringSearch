// StringSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <string>
#include <filesystem>
#include <unordered_map>
#include <math.h>
#include <vector>

#include "utils.h"

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

std::string GetFileText(const std::string& fileToRead) {
	// read text file
	// file to read from
	std::ifstream file(fileToRead);
	// temp line storage
	std::string line;
	// output text storage
	std::string output;
	// loop through each line and append the line to the string
	while (std::getline(file, line)) output.append(line);
	//	while (file >> line) output.append(line);
		// return the contents of the file
	return output;
}

std::string GetText() {
	//// get text file
	std::string input;
	do {
		std::cout << "USE ASCII CHARACTERS ONLY\nYOU HAVE BEEN WARNED\nEnter y when the text you want to search is in a text file named \"search.txt\" in the same folder as this .exe\n";
		std::getline(std::cin, input);
	} while (input[0] != 'y');
	//return GetFileText("search.txt");
	load_file("search.txt", input);
	return input;
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

std::vector<unsigned long long int> Search_BoyerMoore(const std::string& text, const std::string& pattern) {
	unsigned int textLength = text.length();
	unsigned int patternLength = pattern.length();

	std::vector<unsigned long long int> matchingIndexes;

	// lookup table to store how many places the given position should skip
	// ascii extended codes
	int skip[256];

	// set all points to be max skip value
	for (int i = 0; i < 256; ++i)
		skip[i] = patternLength; // Not in the pattern.

	// for each of the characters in the pattern
	for (int i = 0; i < patternLength; ++i)
		// set that character to its length from the end of the pattern
		skip[int(pattern[i])] = (patternLength - 1) - i;

	// iterate through all the text, stopping patternLength positions from the end of the text
	for (int i = 0; i < textLength - patternLength; ++i) {
		// check if the last character in the pattern is a match
		int distance = skip[int(text[i + patternLength - 1])];

		// if no match, skip by distance to the next position
		if (distance != 0) {
			i += distance - 1;
			continue;
		}

		// there is a match

		int j;

		// iterate through the text to check each character
		for (j = 0; j < patternLength; j++) {
			// if the current char in text being checked doesn't match that point in the pattern
			if (text[i + j] != pattern[j]) break; // break and move on
		}

		// the word in text matches the pattern
		if (j == patternLength)
			// add the index of the word to matchingIndexes
			matchingIndexes.push_back(i);
	}

	return matchingIndexes;
}

void BoyerMoore() {
	Clear(MessageType::BoyerMoore);
	// text to search
	std::string text = GetText();
	// get pattern to search for
	std::string pattern = GetPattern();
	Clear(MessageType::BoyerMoore);
	std::cout << "Searching for: " << pattern;

	std::vector<unsigned long long int> indexes = Search_BoyerMoore(text, pattern);
	if (indexes.empty()) std::cout << "\r" << pattern << " is not in the given text!\n";
	else {
		std::cout << "\nThere ";
		(indexes.size() == 1) ? std::cout << "was 1 match!" : std::cout << "were " << indexes.size() << " matches!";
		std::string input;
		do {
			std::cout << "\nWould you like to see all the matching positions? [y/n]: ";
			std::getline(std::cin, input);
			if (input[0] == 'n') return;
		} while (input[0] != 'y');

		std::cout << "These are the match positions: ";

		for (unsigned int i = 0; i < indexes.size(); ++i)
			std::cout << "\n" << i << ") " << indexes[i];

		std::cout << "\n\n";
	}

	std::cout << "\nPress RETURN to return to the main menu...";
	std::getline(std::cin, text);
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
	while (true) {
		std::cout << "Select the algorithm to use:\n";
		std::cout << "1 - Rabin Karp\n";
		std::cout << "2 - Boyer Moore\n";
		std::cout << "3 - Quit application\n";
		std::cout << ">";
		std::string input;
		std::getline(std::cin, input);

		char in = input[0];
		bool clear = true;

		if (in == '1')
			RabinKarp();
		else if (in == '2')
			BoyerMoore();
		else if (in == '3' || in == 'q' || in == 'Q')
			return 0;
		else {
			clear = false;
			std::cout << "\nInvalid input: '" << input << "'\n";
		}
		if (clear) Clear(MessageType::Menu);
	}
}