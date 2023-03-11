#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <queue>
#include <limits>
#include <iomanip>
using namespace std;

/// <summary>
/// category of word data that includes all of the data that is going to be used.
/// </summary>
struct wordData {
	string word;
	string punctuationAfter;
	double value = 0.0;
	int wordOrder = INT_MAX; // defaults to the max value so that the print output doesn't get bugged.
	bool isCapitalized = false;
};

const double dicThreshold = 1.5; // +/- this value is what is checked if the dictionary words are positive, negative, or neutral.
const double revThreshold = 1; // +/- this value is what is checked if the review words are positive, negative, or neutral.
const int MAX_OUTPUT_WIDTH = 80; //The max character width of any output to be outputted to cout or a file

/// <summary>
/// gets input from a user to chose a file, will loop until valid file is found
/// </summary>
/// <param name="inputType">used to output if we're wanting the file for the map or the review</param>
/// <returns>string of the name of the file the user chose</returns>
string getFile(string inputType);

/// <summary>
/// Edits the review input file to create the file name for the output file
/// </summary>
/// <param name="inFile">the review input file</param>
/// <returns>the string of the output file name</returns>
string findOutputFile(string inFile);

/// <summary>
/// reads in the dictionary file
/// </summary>
/// <param name="filename">the file name of the dictonary</param>
/// <param name="allWords">all of the words that are in the dictionary</param>
/// <param name="posWords">the positive words of the dictionary</param>
/// <param name="negWords">the negative words of the dictionary</param>
void readDictionary(string filename, map<string, double>& allWords, map<string, double>& posWords,
	map<string, double>& negWords);

/// <summary>
/// reads the input review and stores in into the appropriate files
/// </summary>
/// <param name="filename">the filename of the input review</param>
/// <param name="neutralWords">the neutral words of the review</param>
/// <param name="posWords">the positive words of the review</param>
/// <param name="negWords">the negative words of the review</param>
/// <param name="dictionary">the dictionary we reference to get the sentiment value for each word</param>
/// <returns>the total sentiment value of the review</returns>
double readReview(string filename, queue<wordData>& neutralWords,
	queue<wordData>& posWords, queue<wordData>& negWords, map<string, double>const& dictionary);

/// <summary>
/// pushes a word on to the appropriate queue
/// </summary>
/// <param name="neutralWords">the queue of neutral words</param>
/// <param name="posWords">queue of positive words</param>
/// <param name="negWords">queue of negative words</param>
/// <param name="wordStr">the string of the word we push</param>
/// <param name="wordVal">the value of the word that is pushed</param>
/// <param name="punctuation">the punctuation, if any, that comes after the word</param>
/// <param name="order">the order that the word comes in in the review</param>
/// <param name="isCapitalized">if the first letter of the word is capitalized or not</param>
void pushWord(queue<wordData>& neutralWords,
	queue<wordData>& posWords, queue<wordData>& negWords,
	string wordStr, double wordVal, string punctuation, int order, bool isCapitalized);

/// <summary>
/// finds the sentiment value of a word using a dictionary
/// </summary>
/// <param name="word">the string of the word to search for</param>
/// <param name="dictionary">the dictionary to search for the word</param>
/// <returns>the value of the word</returns>
double findValue(string word, map<string, double>const& dictionary);

/// <summary>
/// changes a queue to random words in taken from the dictionary
/// </summary>
/// <param name="words">the queue to change, passed as reference, so we can change it</param>
/// <param name="dictionary">the dictionary we check for values and find words using</param>
/// <returns>how much the sentiment value has changed</returns>
double changeQueue(queue<wordData>& words, map<string, double>const& dictionary);

/// <summary>
/// formats the new review into a single output string
/// </summary>
/// <param name="posWords">the queue of positive words, if it hasn't been changed</param>
/// <param name="negWords">the queue of negative words, if it hasn't been changed</param>
/// <param name="neutralWords">the queue of neutral words</param>
/// <returns>the review as an output string</returns>
string writeNewReview(queue<wordData> posWords, queue<wordData> negWords, queue<wordData> neutralWords);

/// <summary>
/// prints the review with correct formatting
/// </summary>
/// <param name="text">the text that gets formatted and output</param>
void printReview(string text);

/// <summary>
/// prints the review with correct formatting, and outputs it to a file
/// </summary>
/// <param name="filename">the file name to output to</param>
/// <param name="text">the text that gets formatted and output</param>
/// <param name="sentimentVal">the sentimental value to output, needed because it is used to output to the file as well.</param>
void printReview(string filename, string text, double sentimentVal);