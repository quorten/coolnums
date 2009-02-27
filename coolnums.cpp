/*//Finds a number which a sum of integers has an equivilant sum of squares

#define WIN32_LEAN_AND_MEAN //Exclude rarely used headers from windows.h
#include <windows.h>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

struct ThreadParameters
{
	unsigned long long intsDepth;
	unsigned long long intsSum;
	unsigned long long currentSquare;
	unsigned long long currentOddNum; //Next number for figuring next sqaure
	unsigned long long squaresSum;
	unsigned long long squaresDepth;
	unsigned threadNum;
};

void ThreadFunction(LPVOID tp);

bool* threadCompletions;
stringstream* threadOuts;
unsigned long long stride; //Difference between starting search numbers of different cores
clock_t maxTime;

int main()
{
	//Greet the user and interogate them
	unsigned numThreads;
	unsigned long long intsSeed, squaresSeed;
	cout << "Welcome to the cool number finder.\n\
			This program finds any numbers which has a sum of integers and\n\
			an equivilant sum of squares associated with it.\n\
			Good luck running!\n\
			How many seconds should this program run for? ";
	cin >> maxTime;
	cout << "How many processor cores do you have? ";
	cin >> numThreads;
	cout << "What numbers should we start at?\n\
		Follow this format: (integers_seed squares_seed squares_sum) ";
	cin >> intsSeed >> squaresSeed >> squaresSum;
	cout << "What should be the numeric stride between different cores? ";
	cin >> stride;
	cout << "Alright, you're good to go. Press CTRL+C any time you would like to\n\
			terminate this program before the given time elapses.\n";
	//Perform dynamic allocations
	threadCompletions = new bool[numThreads];
	threadOuts = new stringstream[numThreads];
	ThreadParameters* tps = new ThreadParameters[numThreads];
	HANDLE* auxThreads = new HANDLE[numThreads - 1];
	//Figure starting numerics
	tps[0].intsDepth = intsSeed;
	tps[0].squaresDepth = squaresSeed;
	tps[0].currentOddNum = (suqaresSeed) * 2 + 1;
	tps[0].currentSquare = squaresSeed * squaresSeed;
	tps[0].threadNum = 0;
	tps[0].squaresSum = 0;
	tps[0].intsSum = (intsSeed * intsSeed + intsSeed) / 2;
	//Configure primary thread
	HANDLE currentThread = GetCurrentThread();
	SetThreadAffinityMask(currentThread, &0);
	tps[0].currentOddNum = 0;
	//Configure auxiliary threads
	for (unsigned i = 1; i < numThreads; i++)
	{
		HANDLE auxThread = CreateThread(NULL, 0, ThreadFunction, tps[i], 0, NULL);
		DWORD threadAff = (1 << i);
		SetThreadAffinityMask(auxThread, &threadAff);
	}
	//Clean up dynamic allocations
	delete []threadCompletions;
	delete []threadOuts;
	delete []tps;
	delete []auxThreads;
	return 0;
}

void ThreadFunction(LPVOID tp)
{
	static_cast<ThreadParameters&>(tp);
	unsigned long long initNum = tp.intsSum;
	while (clock() < maxTime * CLOCKS_PER_SEC && tp.intsSum - initNum < stride)
	{
		if (tp.intsSum == tp.squaresSum)
		{
			threadOuts[tp.threadNum] << "Found a cool number! It is " << tp.intsSum <</* " and " << squaresSum <</* ".\n";
			threadOuts[tp.threadNum] << "Integer depth: " << tp.intsDepth << ". Squares depth: " << tp.squaresDepth << ".\n";
		}
		tp.intsDepth++;
		tp.intsSum += tp.intsDepth;
		if (intsSum > squaresSum)
		{
			tp.currentSquare += tp.currentOddNum; tp.squaresDepth++;
			tp.squaresSum += tp.currentSquare;
			tp.currentOddNum += 2;
		}
	}
	threadOuts[tp.threadNum].flush();
}
*/

//Simpler version

#define WIN32_LEAN_AND_MEAN //Exclude rarely used headers from windows.h
#include <windows.h> //Console handling
#include <iostream>
#include <fstream>
#include <ctime>
#include <gmpxx.h> //Bignum support

using namespace std;

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
bool timeToQuit = false;

int main()
{
	mpz_class intsDepth;
	mpz_class intsSum;
	mpz_class currentSquare;
	mpz_class currentOddNum; //Next number for figuring next sqaure
	mpz_class squaresSum;
	mpz_class squaresDepth;
	{ //Load our data
		ifstream inFile("coolnums.dat");
		inFile >> intsDepth;
		inFile >> intsSum;
		inFile >> currentSquare;
		inFile >> currentOddNum;
		inFile >> squaresSum;
		inFile >> squaresDepth;
	}
	bool overflow = false;

	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	cout << "This program is performing a 7 hour number search on (almost)\none CPU. To stop it safely, press CTRL+C. Do not close this window for safety\nreasons." << endl;
	cout << "Diagnostics:" <<
			 "\nintsDepth = "		<< intsDepth <<
			 "\nintsSum = "			<< intsSum <<
			 "\ncurrentSquare = "	<< currentSquare <<
			 "\ncurrentOddNum = "	<< currentOddNum <<
			 "\nsquaresSum = "		<< squaresSum <<
			 "\nsquaresDepth = "	<< squaresDepth << endl;
	while (clock() < 25200 * CLOCKS_PER_SEC && timeToQuit == false)
	{
		clock_t lastTime = clock();
		while (clock() - lastTime < 600 * CLOCKS_PER_SEC && timeToQuit == false)
		{
			if (intsSum == squaresSum)
			{
				cout << "Found a cool number! It is " << intsSum <</* " and " << squaresSum <<*/ ".\n";
				cout << "Integer depth: " << intsDepth << ". Squares depth: " << squaresDepth << ".\n";
				ofstream stats("coolnums.txt", ios_base::app);
				stats << "Found a cool number! It is " << intsSum <</* " and " << squaresSum <<*/ ".\n";
				stats << "Integer depth: " << intsDepth << ". Squares depth: " << squaresDepth << ".\n";
			}
			intsDepth++;
			//mpz_class oldSum = intsSum;
			intsSum += intsDepth;
			/*if (oldSum >= intsSum) //Don't check for overflow (because it shouldn't happen this way)
			{
				intsSum = oldSum;
				intsDepth--;
				overflow = true;
				break;
			}*/
			if (intsSum > squaresSum)
			{
				currentSquare += currentOddNum; squaresDepth++;
				squaresSum += currentSquare;
				currentOddNum += 2;
			}
		}
		//cout << "Ten minute save initiated.\n";
		{ //Save our data
			ofstream outFile("coolnums.dat", ios_base::trunc);
			outFile << intsDepth << "\n";
			outFile << intsSum << "\n";
			outFile << currentSquare << "\n";
			outFile << currentOddNum << "\n";
			outFile << squaresSum << "\n";
			outFile << squaresDepth;
		}
	}
	if (overflow == true)
		cout << "Overflow occured.\n";
	cout << "Diagnostics:" <<
			 "\nintsDepth = "		<< intsDepth <<
			 "\nintsSum = "			<< intsSum <<
			 "\ncurrentSquare = "	<< currentSquare <<
			 "\ncurrentOddNum = "	<< currentOddNum <<
			 "\nsquaresSum = "		<< squaresSum <<
			 "\nsquaresDepth = "	<< squaresDepth << endl;
	
	{ //Save our data
		ofstream outFile("coolnums.dat", ios_base::trunc);
		outFile << intsDepth << "\n";
		outFile << intsSum << "\n";
		outFile << currentSquare << "\n";
		outFile << currentOddNum << "\n";
		outFile << squaresSum << "\n";
		outFile << squaresDepth;
	}
	return 0;
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	timeToQuit = true;
	return TRUE;
}
