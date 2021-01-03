#include <iostream>
#include <fstream>

#include <string>
#include <filesystem>
#include <unordered_map>
#include <math.h>
#include <vector>
#include <chrono>
#include <ctime>

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

std::chrono::steady_clock::time_point timerStart;
std::chrono::steady_clock::time_point timerEnd;

void StartClock() {
	timerStart = std::chrono::steady_clock::now();
}
void StopClock() {
	timerEnd = std::chrono::steady_clock::now();
}
void ShowTimeTaken() {
	std::cout << "\nTime taken: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(timerEnd - timerStart).count();
	std::cout << "ms\n";
}

std::vector<int> Search_BoyerMoore(const std::string& text, const std::string& pattern) {
	StartClock();
	unsigned int textLength = int(text.length());
	unsigned int patternLength = int(pattern.length());

	std::vector<int> matchingIndexes;

	// lookup table to store how many places the given position should skip
	// ascii extended codes
	int skip[256] = {};

	// set all points to be max skip value
	for (unsigned int i = 0; i < 256; ++i)
		skip[i] = patternLength; // Not in the pattern.

	// for each of the characters in the pattern
	for (unsigned int i = 0; i < patternLength; ++i)
		// set that character to its length from the end of the pattern
		skip[int(pattern[i])] = (patternLength - 1) - i;

	// iterate through all the text, stopping patternLength positions from the end of the text
	for (unsigned int i = 0; i < textLength - patternLength; ++i) {
		// check if the last character in the pattern is a match
		int pos = i + patternLength - 1;
		int distance = skip[int(text[pos])];

		// if no match, skip by distance to the next position
		if (distance != 0) {
			i += distance - 1;
			continue;
		}

		// there is a match

		unsigned int j;

		// iterate through the text to check each character
		for (j = 0; j < patternLength; j++) {
			// if the current char in text being checked doesn't match that point in the pattern
			int pos = i + j;
			if (text[pos] != pattern[j]) break; // break and move on
		}

		// the word in text matches the pattern
		if (j == patternLength)
			// add the index of the word to matchingIndexes
			matchingIndexes.push_back(i);
	}
	StopClock();
	return matchingIndexes;
}

bool ShowMatchingIndexes() {
	std::string input;
	do {
		std::cout << "\nWould you like to see all the matching positions? [y/n]: ";
		std::getline(std::cin, input);
		if (input[0] == 'n') return false;
	} while (input[0] != 'y');
	return true;
}

void ShowMatches(std::vector<int> matchingIndexes, std::string& text, std::string& pattern) {
	ShowTimeTaken();
	if (matchingIndexes.empty()) std::cout << pattern << " is not in the given text!\n";
	else {
		std::cout << "\nThere ";
		(matchingIndexes.size() == 1) ? std::cout << "was 1 match!" : std::cout << "were " << matchingIndexes.size() << " matches!";
		if (!ShowMatchingIndexes()) return;

		std::cout << "These are the match positions:\n";

		for (unsigned int i = 0; i < matchingIndexes.size(); ++i)
			show_context(text, matchingIndexes[i]);

		std::cout << "\n";
	}
}

void EndOfAlgorithm() {
	std::cout << "\nPress RETURN to return to the main menu...";
	auto _ignore = getchar();
}

void BoyerMoore() {
	Clear(MessageType::BoyerMoore);
	// text to search
	std::string text = GetText();
	// get pattern to search for
	std::string pattern = GetPattern();
	Clear(MessageType::BoyerMoore);
	std::cout << "Searching for: " << pattern;

	std::vector<int> matchingIndexes = Search_BoyerMoore(text, pattern);
	ShowMatches(matchingIndexes, text, pattern);

	EndOfAlgorithm();
}

int GetHashValue(int patternLength, int hashVal, int alphabet, int prime) {
	for (int i = 0; i < patternLength - 1; ++i)
		hashVal = (hashVal * alphabet) % prime;
	return hashVal;
}

int HashText(std::string& text, int& textHash, int patternLength, int alphabet, int prime) {
	int val = textHash;
	for (int i = 0; i < patternLength; i++)
		val = (alphabet * val + text[i]) % prime;
	return val;
}

int RollHash(std::string& text, int textHashVal, int hashVal, int i, int patternLength, int alphabet, int prime) {
	/* Get hash value of the next position
	 * Subtract hash value of text[i]
	 * Add value of text[i + patlen]
	 * Divide total by prime number
	 */

	int pos = i + patternLength;
	textHashVal = (alphabet * (textHashVal - text[i] * hashVal) + text[pos]) % prime;

	// if textHash is below 0
	// add prime number to text hash

	if (textHashVal < 0)
		textHashVal = textHashVal + prime;

	return textHashVal;
}

std::vector<int> Search_RabinKarp(std::string& text, std::string& pattern) {
	StartClock();
	// vector to hold returnable data
	std::vector<int> matchingIndexes;
	// Get lengths
	int patternLength = pattern.size();
	int textLength = text.size();

	// Count of possible chars in input
	const int alphabet = 256;
	// Hash value of the pattern
	int patternHashVal = 0;
	// Hash value of the text
	int textHashVal = 0;
	// prime number used to calculate hash
	const int prime = 17;

	// Calculate the hash value
	// initialise
	int hashVal = 1;
	// get hash value
	hashVal = GetHashValue(patternLength, hashVal, alphabet, prime);

	// Keep iterators in scope
	int i, j;

	// Get hash values of the pattern and text
	patternHashVal = HashText(pattern, patternHashVal, pattern.size(), alphabet, prime);
	textHashVal = HashText(text, textHashVal, pattern.size(), alphabet, prime);

	// iterate 0 through textlength - pattern length (last possible pos)
	for (i = 0; i <= textLength - patternLength; ++i) {
		// if the pattern's hash is the same as the text's hash
		// then it's likely a match
		if (patternHashVal == textHashVal) {
			// iterate each char for the length of the potential match
			for (j = 0; j < patternLength; j++) {
				// check each char

				int pos = i + j;
				if (text[pos] != pattern[j])
					// break if mismatch
					break;
			}

			//  j == patternLength when the two matching hashes have been compared char for char
			if (j == patternLength)
				// add the index to the list of matches
				matchingIndexes.push_back(i);
		}

		// if i is in range
		if (i < textLength - patternLength)
			// roll the hash to the next check
			textHashVal = RollHash(text, textHashVal, hashVal, i, pattern.size(), alphabet, prime);
	}
	StopClock();
	return matchingIndexes;
}

void RabinKarp() {
	Clear(MessageType::RabinKarp);
	// text to search
	std::string text = GetText();
	// get pattern to search for
	std::string pattern = GetPattern();
	Clear(MessageType::RabinKarp);
	std::cout << "Searching for: " << pattern;

	std::vector<int> matchingIndexs = Search_RabinKarp(text, pattern);

	ShowMatches(matchingIndexs, text, pattern);

	EndOfAlgorithm();
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