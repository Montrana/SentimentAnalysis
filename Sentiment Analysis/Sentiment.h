#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<map>
using namespace std;

struct wordData {
	string word;
	char punctuationAfter;
	double value;
	int wordOrder;
};

void readFile(string filename, map<string, double>& allWords, map<string, double>& posWords,
	map<string, double>& negWords);
bool isValid(string str);
void printMap(string filename, map<string, double> wordMap);