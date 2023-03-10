#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <queue>
#include <limits>
#include "WordData.h"
using namespace std;

struct wordData {
	string word;
	string punctuationAfter;
	double value = 0.0;
	int wordOrder = INT_MAX;
	bool isCapitalized = false;
};

void readFile(string filename, map<string, double>& allWords, map<string, double>& posWords,
	map<string, double>& negWords);
void readReview(string filename, queue<wordData>& neutralWords,
	queue<wordData>& posWords, queue<wordData>& negWords, map<string, double>const& dictionary);
void pushWord(queue<wordData>& neutralWords,
	queue<wordData>& posWords, queue<wordData>& negWords,
	string wordStr, double wordVal, string punctuation, int order, bool isCapitalized);
double findValue(string word, map<string, double>const& dictionary);
bool isValid(string str);
void printMap(string filename, map<string, double> wordMap);
void printQueue(queue<wordData> words);
void printAdjustedReview(queue<wordData> posWords, queue<wordData> negWords, queue<wordData> neutralWords);