#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <queue>
using namespace std;

struct wordData {
	string word;
	string punctuationAfter;
	double value = 0.0;
	int wordOrder = 0;
};

void readFile(string filename, map<string, double>& allWords, map<string, double>& posWords,
	map<string, double>& negWords);
void readFile(string filename, queue<wordData> neutralWords,
	queue<wordData> posWords, queue<wordData> negWords, map<string, double>const& dictionary);
bool isValid(string str);
void printMap(string filename, map<string, double> wordMap);