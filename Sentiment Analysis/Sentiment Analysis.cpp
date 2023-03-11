// Sentiment Analysis.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Sentiment.h"
#include "WordData.h"


int main()
{
    map<string, double> negWords;
    map<string, double> allWords;
    map<string, double> posWords;
    
    queue<wordData> neutralReviewWords;
    queue<wordData> posReviewWords;
    queue<wordData> negReviewWords;

    ifstream inFileDic; //dictionary infile
    ifstream inFileRev; //review infile
    string inFileNameDic;
    string inFileNameRev;
    string outFileNameRev;
    double sentimentValTot;

    int maxOutputWidth = 80;

    inFileNameDic = getFile(inFileDic, "dictionary");
    inFileNameRev = getFile(inFileRev, "review");

    readFile(inFileNameDic, allWords, posWords, negWords);
    sentimentValTot = readReview(inFileNameRev, neutralReviewWords, posReviewWords, negReviewWords, allWords);
    cout << "The original text is:\n";
    printReview(writeNewReview(posReviewWords, negReviewWords, neutralReviewWords), maxOutputWidth);
    cout << "The original sentiment of this file is: " << sentimentValTot << endl << endl;

    string input;
    cout << "What would you like to do?\n\n";
    cout << "P - Change from Negative to Positive\n";
    cout << "N - Change from Positive to Negative\n";
    cout << "Q - to Quit\n";
    cout << "Choice: ";

    cin >> input;
    if (input == "P")
    {
        sentimentValTot += changeQueue(negReviewWords, posWords);
    }
    else if (input == "N")
    {
        sentimentValTot += changeQueue(posReviewWords, negWords);
    }
    outFileNameRev = findOutputFile(inFileNameRev);
    printReview(writeNewReview(posReviewWords, negReviewWords, neutralReviewWords), maxOutputWidth);
    cout << "New Sentiment Value = " << sentimentValTot;
    cout << "The new output file will be stored in: " << outFileNameRev;
    
    //printQueue(neutralReviewWords);
    //printQueue(posReviewWords);
    //printQueue(negReviewWords);
    cout << endl;
    
    //printMap("allWords.txt", allWords); 
    //printMap("posWords.txt", posWords);
    //printMap("negWords.txt", negWords);

    inFileDic.close();
    inFileRev.close();
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
