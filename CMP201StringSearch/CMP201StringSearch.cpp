#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <chrono>

// From MLS
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

/// <summary>
/// Clears the console screen and outputs the relevant header
/// </summary>
/// <param name="message">The header header to print</param>
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

std::string GetText() {
	//// get text file
	std::string input;
	do {
		std::cout << "USE ASCII CHARACTERS ONLY\nYOU HAVE BEEN WARNED\nEnter y when the text you want to search is in a text file named \"search.txt\" in the same folder as this .exe\n";
		std::getline(std::cin, input);
	} while (input[0] != 'y');
	std::cout << "\nReading file...\n";
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
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerStart).count();
	std::cout << "us\n";
}

std::string results = "sample size,pattern size, iteration, time taken\n";

void StoreTimeTaken(long long sampleSize, long long patternLength, long long sampleIteration) {
	// sample size out
	std::string output = std::to_string(sampleSize);
	// pattern length out
	switch (patternLength) {
	case 0:
		output.append(",3,");
		break;
	case 1:
		output.append(",5,");
		break;
	case 2:
		output.append(",7,");
		break;
	default:
		exit(1);
	}
	// current iteration out
	output.append(std::to_string(sampleIteration) + ",");
	// time taken out
	output.append(std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerStart).count()));
	// store for writing
	results.append(output + "\n");
}
void WriteTimeTaken(std::string algo) {
	std::ofstream f;
	f.open("benchmark-results-" + algo + ".csv", std::ios_base::binary);
	f << results;
	f.close();
}

std::vector<long long> Search_BoyerMoore(const std::string& text, const std::string& pattern) {
	StartClock();
	long long textLength = text.length();
	long long patternLength = pattern.length();

	std::vector<long long> matchingIndexes;

	// lookup table to store how many places the given position should skip
	// ascii extended codes
	long long skip[256] = {};

	// set all points to be max skip value
	for (long long i = 0; i < 256; ++i)
		// Not in the pattern.
		skip[i] = patternLength;

	// for each of the characters in the pattern
	for (long long i = 0; i < patternLength; ++i)
		// set that character to its length from the end of the pattern
		skip[pattern[i]] = (patternLength - 1) - i;

	// iterate through all the text, stopping patternLength positions from the end of the text
	for (long long i = 0; i < textLength - patternLength; ++i) {
		// check if the last character in the pattern is a match
		long long pos = i + patternLength - 1;
		long long distance = skip[text[pos]];

		// if no match, skip by distance to the next position
		if (distance != 0) {
			i += distance - 1;
			continue;
		}

		// there is a match

		long long j;

		// iterate through the text to check each character
		for (j = 0; j < patternLength; j++) {
			// if the current char in text being checked doesn't match that point in the pattern
			long long pos = i + j;
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

void ShowMatches(std::vector<long long> matchingIndexes, std::string& text, std::string& pattern) {
	ShowTimeTaken();
	if (matchingIndexes.empty()) std::cout << pattern << " is not in the given text!\n";
	else {
		std::cout << "\nThere ";
		(matchingIndexes.size() == 1) ? std::cout << "was 1 match!" : std::cout << "were " << matchingIndexes.size() << " matches!";
		if (!ShowMatchingIndexes()) return;

		std::cout << "These are the match positions:\n";

		std::string output = "";

		for (long long i = 0; i < matchingIndexes.size(); ++i)
			output.append(get_context(text, matchingIndexes[i], (i + 1)));

		std::cout << output;
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

	std::vector<long long> matchingIndexes = Search_BoyerMoore(text, pattern);
	ShowMatches(matchingIndexes, text, pattern);

	EndOfAlgorithm();
}

long long GetHashValue(long long patternLength, long long hashVal, long long alphabet, long long prime) {
	for (long long i = 0; i < patternLength - 1; ++i)
		hashVal = (hashVal * alphabet) % prime;
	return hashVal;
}

long long HashText(std::string text, long long textHash, long long patternLength, long long alphabet, long long prime) {
	for (long long i = 0; i < patternLength; ++i)
		textHash = (alphabet * textHash + text[i]) % prime;
	return textHash;
}

long long RollHash(std::string& text, long long textHashVal, long long hashVal, long long i, long long patternLength, long long alphabet, long long prime) {
	/* Get hash value of the next position
	 * Subtract hash value of text[i]
	 * Add value of text[i + patlen]
	 * Divide total by prime number
	 */

	long long pos = i + patternLength;
	textHashVal = (alphabet * (textHashVal - text[i] * hashVal) + text[pos]) % prime;

	// if textHash is below 0
	// add prime number to text hash

	if (textHashVal < 0)
		textHashVal += prime;

	return textHashVal;
}

std::vector<long long> Search_RabinKarp(std::string& text, std::string& pattern) {
	StartClock();
	// vector to hold returnable data
	std::vector<long long> matchingIndexes;
	// Get lengths
	long long patternLength = pattern.size();
	long long textLength = text.size();

	// Count of possible chars in input
	const long long alphabet = 256;
	// Hash value of the pattern
	long long patternHashVal = 0;
	// Hash value of the text
	long long textHashVal = 0;
	// prime number used to calculate hash
	const long long prime = 17;

	// Calculate the hash value
	// initialise
	long long hashVal = 1;
	// get hash value
	hashVal = GetHashValue(patternLength, hashVal, alphabet, prime);

	// Keep iterators in scope
	long long i = 0, j = 0;

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

				long long pos = i + j;
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

	std::vector<long long> matchingIndexs = Search_RabinKarp(text, pattern);

	ShowMatches(matchingIndexs, text, pattern);

	EndOfAlgorithm();
}

void benchmarkRK() {
	// iterate text sample sizes
	// 2^0 to 2^13 sets of sample text
	for (long long fullLoop = 1; fullLoop <= 8192; fullLoop *= 2)
	{
		// file to load
		std::string file = "search.txt";
		// text to search
		std::string text;
		// read once
		load_file(file, text);
		// countdown iterator for loop reading
		long long j = fullLoop;
		// if more than 1 set is to be loaded
		// loop until 1 is reached
		while (j > 1) {
			// append self
			text.append(text);
			j /= 2;
		}

		/* Patterns
		 * Small:	nec
		 * Medium:	dolor
		 * Large:	consectetur
		 */
		std::string patterns[] = { "nec","dolor","consectetur" };
		for (long long patternLoop = 0; patternLoop < 3; patternLoop++)
		{
			std::string pattern = patterns[patternLoop];
			for (long long algoLoop = 0; algoLoop < 100; algoLoop++) {
				std::cout << "Sample Size: " << fullLoop << " | Pattern: " << pattern << " | Iteration: " << algoLoop << " | Time taken: " << std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerStart).count() << std::endl;
				StartClock();
				// vector to hold returnable data
				std::vector<long long> matchingIndexes;
				// Get lengths
				long long patternLength = pattern.size();
				long long textLength = text.size();

				// Count of possible chars in input
				const long long alphabet = 256;
				// Hash value of the pattern
				long long patternHashVal = 0;
				// Hash value of the text
				long long textHashVal = 0;
				// prime number used to calculate hash
				const long long prime = 17;

				// Calculate the hash value
				// initialise
				long long hashVal = 1;
				// get hash value
				hashVal = GetHashValue(patternLength, hashVal, alphabet, prime);

				// Keep iterators in scope
				long long i, j;

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

							long long pos = i + j;
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
				StoreTimeTaken(fullLoop, patternLoop, algoLoop);
				WriteTimeTaken("Rabin-Karp");
			}
		}
	}
}

void benchmarkBM() {
	// iterate text sample sizes
	// 2^0 to 2^13 sets of sample text
	for (long long fullLoop = 1; fullLoop <= 8192; fullLoop *= 2)
	{
		// file to load
		std::string file = "search.txt";
		// text to search
		std::string text;
		// read once
		load_file(file, text);
		// countdown iterator for loop reading
		long long j = fullLoop;
		// if more than 1 set is to be loaded
		// loop until 1 is reached
		while (j > 1) {
			// append self
			text.append(text);
			j /= 2;
		}

		/* Patterns
		 * Small:	nec
		 * Medium:	dolor
		 * Large:	consectetur
		 */
		std::string patterns[] = { "nec","dolor","consectetur" };
		for (long long patternLoop = 0; patternLoop < 3; patternLoop++)
		{
			std::string pattern = patterns[patternLoop];
			for (long long algoLoop = 0; algoLoop < 100; algoLoop++)
			{
				std::cout << "Sample Size: " << fullLoop << " | Pattern: " << pattern << " | Iteration: " << algoLoop << " | Time taken: " << std::chrono::duration_cast<std::chrono::microseconds>(timerEnd - timerStart).count() << std::endl;
				StartClock();
				long long textLength = text.length();
				long long patternLength = pattern.length();

				std::vector<long long> matchingIndexes;

				// lookup table to store how many places the given position should skip
				// ascii extended codes
				long long skip[256] = {};

				// set all points to be max skip value
				for (long long i = 0; i < 256; ++i)
					// Not in the pattern.
					skip[i] = patternLength;

				// for each of the characters in the pattern
				for (long long i = 0; i < patternLength; ++i)
					// set that character to its length from the end of the pattern
					skip[long long(pattern[i])] = (patternLength - 1) - i;

				// iterate through all the text, stopping patternLength positions from the end of the text
				for (long long i = 0; i < textLength - patternLength; ++i) {
					// check if the last character in the pattern is a match
					long long pos = i + patternLength - 1;
					long long distance = skip[long long(text[pos])];

					// if no match, skip by distance to the next position
					if (distance != 0) {
						i += distance - 1;
						continue;
					}

					// there is a match

					long long j;

					// iterate through the text to check each character
					for (j = 0; j < patternLength; j++) {
						// if the current char in text being checked doesn't match that point in the pattern
						long long pos = i + j;
						if (text[pos] != pattern[j]) break; // break and move on
					}

					// the word in text matches the pattern
					if (j == patternLength)
						// add the index of the word to matchingIndexes
						matchingIndexes.push_back(i);
				}
				StopClock();
				StoreTimeTaken(fullLoop, patternLoop, algoLoop);
				WriteTimeTaken("Boyer-Moore");
			}
		}
	}
}

int main()
{
	Clear(MessageType::Menu);
	bool benchmark = false;
	while (true) {
		std::cout << "Select the algorithm to ";
		std::cout << ((benchmark) ? "benchmark" : "search with");
		std::cout << ":\n";
		std::cout << "1 - Rabin Karp\n";

		std::cout << "2 - Boyer Moore\n";

		std::cout << "3 - Switch to ";
		std::cout << ((benchmark) ? "search mode" : "benchmark mode");
		std::cout << "\n";

		std::cout << "4 - Quit\n";

		std::cout << ">";
		std::string input;
		std::getline(std::cin, input);

		char in = input[0];
		bool clear = true;
		switch (in)
		{
		case '1':
			(benchmark) ? benchmarkRK() : RabinKarp();
			break;
		case '2':
			(benchmark) ? benchmarkBM() : BoyerMoore();
			break;
		case '3':
			benchmark = !benchmark;
			break;
		case '4':
		case 'q':
		case 'Q':
			return 0;
			break;
		default:
			clear = false;
			std::cout << "\nInvalid input: '" << input << "'\n";
		}

		if (clear)
			Clear(MessageType::Menu);
	}
}