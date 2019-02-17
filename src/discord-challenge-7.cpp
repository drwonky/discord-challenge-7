//============================================================================
// Name        : discord-challenge-7.cpp
// Author      : Perry Harrington
// Version     :
// Copyright   : Your copyright notice
// Description : Discord challenge 7
//============================================================================

#include <iostream>
#include <ctime>
#include <cstdint>
#include <cstdlib>
#include <algorithm>

const int gCodeLength = 4;                            // Length of code in digits
const int gCodeLetters = 6;                            // Variation of code eg 6 = [A..F], 8 = [A..H]
const int gMemoryRequired = 20;                    // Bytes of memory you require - you may change this
unsigned char gMemory[gMemoryRequired] = { 0 };        // Array of your memory!
unsigned char gMemory1[gMemoryRequired] = { 0 };        // Array of your memory!

//template<typename It>
//bool next_permutation(It begin, It end)
//{
//        if (begin == end)
//                return false;
//
//        It i = begin;
//        ++i;
//        if (i == end)
//                return false;
//
//        i = end;
//        --i;
//
//        while (true)
//        {
//                It j = i;
//                --i;
//
//                if (*i < *j)
//                {
//                        It k = end;
//
//                        while (!(*i < *--k)) /* pass */;
//
//                        iter_swap(i, k);
//                        reverse(j, end);
//                        return true;
//                }
//
//                if (i == begin)
//                {
//                        reverse(begin, end);
//                        return false;
//                }
//        }
//}

std::string codebreak(const int nCorrectDigits, const int nCorrectPositions)
{
	int permute[24][4] = {
		{0,1,2,3},
		{0,1,3,2},
		{0,2,1,3},
		{0,2,3,1},
		{0,3,1,2},
		{0,3,2,1},
		{1,0,2,3},
		{1,0,3,2},
		{1,2,0,3},
		{1,2,3,0},
		{1,3,0,2},
		{1,3,2,0},
		{2,0,1,3},
		{2,0,3,1},
		{2,1,0,3},
		{2,1,3,0},
		{2,3,0,1},
		{2,3,1,0},
		{3,0,1,2},
		{3,0,2,1},
		{3,1,0,2},
		{3,1,2,0},
		{3,2,0,1},
		{3,2,1,0}
	};

	struct MyData {
		char digits[gCodeLength+1];
		char lastchar;
		unsigned char count;
		int correctDigits;
	};

	MyData *data = (MyData *)gMemory;
	//std::cout<<sizeof(MyData)<<std::endl;

	std::string guess;

	auto nextpermutation = [&] (int p){
		guess += data->digits[permute[p][0]];
		guess += data->digits[permute[p][1]];
		guess += data->digits[permute[p][2]];
		guess += data->digits[permute[p][3]];
	};

	auto swap = [] (char *a, char *b) {
		char c;
		c = *b;
		*b = *a;
		*a = c;
	};

	auto reverse = [swap] (char *a, char *b) {
		while ((a!=b) && (a!=--b)) {
			swap (a, b);
			++a;
		}
	};

	auto next_permutation = [swap,reverse] (char *begin, char *end)
	{
	        if (begin == end)
	                return false;

	        char *i = begin;
	        ++i;
	        if (i == end)
	                return false;

	        i = end;
	        --i;

	        while (true)
	        {
	                char *j = i;
	                --i;

	                if (*i < *j)
	                {
	                        char *k = end;

	                        while (!(*i < *--k)) /* pass */;

	                        swap(i, k);
	                        reverse(j, end);
	                        return true;
	                }

	                if (i == begin)
	                {
	                        reverse(begin, end);
	                        return false;
	                }
	        }
	};

	auto padstring = [&] (char c) {
		guess.append(4,c);
	};

	if (data->correctDigits < gCodeLength) {
		if (nCorrectDigits) {
//			std::cout<<"Got a correct digit"<<std::endl;
			for (int i=0;i<nCorrectDigits;i++) {
				data->digits[data->correctDigits]=data->lastchar;
				data->correctDigits++;
//				std::cout<<"Correct digit "<<data->lastchar<<std::endl;
			}

		}

		if (data->correctDigits == gCodeLength) {
			data->digits[gCodeLength]='\0';
			data->count=0;
//			std::cout << "All digits guessed: " << data->digits << std::endl;
		} else {
			data->lastchar = data->count+65;
			padstring(data->lastchar);
		}
	}

	if (data->correctDigits == gCodeLength) {
//		nextpermutation(0,gCodeLength);
		//nextpermutation(data->count);
		next_permutation(data->digits, data->digits+gCodeLength);
		guess=(char *)data->digits;
	}

	data->count++;
 //   std::cout << "guess " << guess << std::endl;
    std::cout << guess << " ";
    return guess;
}

std::string codebreak1(const int nCorrectDigits, const int nCorrectPositions)
{
#include <algorithm>

	struct MyData {
		char digits[gCodeLength+1];
		char lastchar;
		char count;
		int correctDigits;
	};

	/*
	char g[4] = { 'A','B','C','D'};

	do {
		printf("{%d,%d,%d,%d},\n",g[0]-65,g[1]-65,g[2]-65,g[3]-65);
	} while (std::next_permutation(g,g+4));
	*/

	MyData *data = (MyData *)gMemory1;
	//std::cout<<sizeof(MyData)<<std::endl;

	std::string guess;

	auto nextpermutation = [&] {
		guess=(char *)data->digits;
		std::next_permutation(data->digits,data->digits+gCodeLength);
	};

	auto padstring = [&] (char c) {
		guess.append(4,c);
	};

//	std::cout << "nCorrectDigits "<<nCorrectDigits<<" nCorrectPositions "<<nCorrectPositions<<std::endl;
	if (data->correctDigits < gCodeLength) {
		if (nCorrectDigits) {
//			std::cout<<"Got a correct digit"<<std::endl;
			for (int i=0;i<nCorrectDigits;i++) {
				data->digits[data->correctDigits]=data->lastchar;
				data->correctDigits++;
//				std::cout<<"1 Correct digit "<<data->lastchar<<std::endl;
			}

		}

		if (data->correctDigits == gCodeLength) {
			data->digits[gCodeLength]='\0';
			data->count=0;
//			std::cout << "1 All digits guessed: " << data->digits << std::endl;
		} else {
			data->lastchar = data->count+65;
			padstring(data->lastchar);
		}
	}

	if (data->correctDigits == gCodeLength) {
		nextpermutation();
		guess=(char *)data->digits;
	}

	data->count++;
//    std::cout << "1 guess " << guess << std::endl;
    std::cout << guess << " ";
    return guess;
}

// No Editing Below Here!

int compo()
{
    std::string sCode;
    int nGuessCount = 0;
    int nCorrectDigits = 0;
    int nCorrectPositions = 0;
    std::string sGuess = "", sGuess1 = "";
    int retval=0;

    for (int i=0; i<gMemoryRequired;i++) {
    	gMemory[i]=0;
    	gMemory1[i]=0;
    }

    for (int i = 0; i < gCodeLength; i++)
        sCode.append(1, (rand() % gCodeLetters) + 65);

    // Guess loop
    do{
        sGuess = codebreak(nCorrectDigits, nCorrectPositions);
        sGuess1 = codebreak1(nCorrectDigits, nCorrectPositions);
        if (sGuess == sCode && sGuess1 != sCode && !retval) {
        	std::cout<<"OG won!"<<std::endl;
        	retval=1;
        } else if (sGuess1 == sCode && sGuess != sCode && !retval) {
        	std::cout<<"std won"<<std::endl;
        	retval=2;
        } else if (sGuess == sCode && sGuess1 == sCode && !retval) {
        	std::cout<<"TIE!!!"<<std::endl;
        	retval=3;
        }

        nGuessCount++;
        nCorrectDigits = 0;    nCorrectPositions = 0;

        if (sGuess.length() == gCodeLength){
            std::string sTemp = sCode;
            for (size_t i = 0; i < sCode.length(); i++){
                size_t n = sTemp.find(sGuess[i]);
                if (n != std::string::npos){
                    sTemp[n] = '.';
                    nCorrectDigits++;}
                nCorrectPositions += sGuess[i] == sCode[i];}}

        // Display progress
        std::cout << nGuessCount << ": " << sGuess.c_str() << " - " << sCode.c_str() << " Digits: " << nCorrectDigits << " Positions: " << nCorrectPositions << std::endl;
    } while (sGuess != sCode && nGuessCount < 100);

    // Display result
    if (nGuessCount >= 100)
        std::cout << "FAILED TO CRACK CODE" << std::endl;
    else
    {
        std::cout << "SUCCESSFULLY CRACKED CODE in " << nGuessCount << " attempts using " << gMemoryRequired << " bytes of memory" << std::endl;
        std::cout << "SCORE: " << nGuessCount * gMemoryRequired << std::endl;
    }

    return retval;
}

int main()
{
	int who[5]={0};

    srand((unsigned int)std::time(0));
//	return compo();

    // Generate Code
	for (int i=0; i<1000; i++)
		who[compo()]++;

	for (int i=0;i<4;i++)
		std::cout << "who ["<<i<<"] = "<<who[i]<<std::endl;

	return 0;
}
