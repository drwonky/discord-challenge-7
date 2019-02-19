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
const int gMemoryRequired = 2;                    // Bytes of memory you require - you may change this
unsigned char gMemory[gMemoryRequired] = { 0 };        // Array of your memory!

std::string codebreak(const int nCorrectDigits, const int nCorrectPositions)
{
	struct MyData {
		char digits[gCodeLength];
		unsigned char count			:4;
	};

	MyData data = {0};

	struct PackedData {
		unsigned short a:3;
		unsigned short b:3;
		unsigned short c:3;
		unsigned short d:3;
		unsigned short count:4;
	};

	PackedData *packed = (PackedData *)gMemory;

	auto unpack = [&] {
		data.digits[0]=packed->a+65;
		data.digits[1]=packed->b+65;
		data.digits[2]=packed->c+65;
		data.digits[3]=packed->d+65;
		data.count=packed->count;
	};

	auto pack = [&] {
		packed->a=data.digits[0]-65;
		packed->b=data.digits[1]-65;
		packed->c=data.digits[2]-65;
		packed->d=data.digits[3]-65;
		packed->count=data.count;
	};

	unpack();	// sneaky stuff to win compo :-)

	std::string guess;

	// The next 3 lambda functions are simply boilerplate std::next_permutation
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

	        while (true) {
	                char *j = i;
	                --i;

	                if (*i < *j) {
	                        char *k = end;

	                        while (!(*i < *--k));

	                        swap(i, k);
	                        reverse(j, end);
	                        return true;
	                }

	                if (i == begin) {
	                        reverse(begin, end);
	                        return false;
	                }
	        }
	};

	if (nCorrectDigits < gCodeLength) {
		if (nCorrectDigits != gCodeLength) {
			if (nCorrectDigits) {  // permuting for >0 is worth .1 better than permuting >1
				/*
				 * These next 2 lines form the core of the optimizations.  Each optimization is worth 1 round of guessing (total 2 less guesses per round).
				 * First, we check if the last guess had All of the correctly guessed digits in their correct positions.
				 * It's unlikely that the first N digits will exactly match the guessed code, so we shuffle them in the hopes
				 * that the next guess(es) will benefit.  This nets us about 1 average permutation round per code solve.
				 *
				 * Checking if nCorrectDigits-1 is > nCorrectPositions nets us about .5 less guesses per round.  This is
				 * because nCorrectDigits is based on the last round guess, which we haven't integrated into this round.
				 *
				 * In essence, we'd be permuting the next round data before we integrated last round's data.  We want the permutation
				 * to lag by 1 integration, to only affect changes that were not experimental.
				 *
				 * Here is an example of the permute holdoff:
				 *
				 * 1: AAAA - ADCF Digits: 1 Positions: 1
				 * 2: ABBB - ADCF Digits: 1 Positions: 1
				 * 3: ACCC - ADCF Digits: 2 Positions: 2
				 * 4: ACDD - ADCF Digits: 3 Positions: 1
				 * 5: ADCE - ADCF Digits: 3 Positions: 3 << 3 digits in 3 positions
				 * 6: ADCF - ADCF Digits: 4 Positions: 4 << no permute before last test
				 * SUCCESSFULLY CRACKED CODE in 6 attempts using 5 bytes of memory
				 * SCORE: 30
				 *
				 * The second optimization builds the guess based on known digits.
				 *
				 * Let's say the code is CADA, the guesses look like this:
				 *
				 * AAAA result 2
				 * AABB result 2
				 * AACC result 3
				 * ACAD result 4  <-- we gained a permute here
				 * ... until we guess CADA (9 rounds)
				 *
				 * Using a more naive approach, you would iterate AAAA..FFFF to guess the digits, requiring 6 rounds for
				 * code with F in them.  By assembling guess from known digits, we get 1 free guess.
				 *
				 * Instead of FFFF being the 6th guess, it would be something like BAFF, which is the same as the first full guess,
				 * so we are getting 1 full permutation for free.
				 *
				 */
				if (nCorrectPositions < nCorrectDigits-1) next_permutation(data.digits, data.digits+nCorrectDigits);
			}
			if (data.count) for (int i=nCorrectDigits;i<gCodeLength;i++) data.digits[i]++;

			guess.append(data.digits,gCodeLength);
			data.count++;
		}

	}


	if (nCorrectDigits == gCodeLength) {
		next_permutation(data.digits, data.digits+gCodeLength);
		guess.append((char *)data.digits,gCodeLength);
	}

	pack();
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
//    int retval=0;

    for (int i=0; i<gMemoryRequired;i++) {
    	gMemory[i]=0;
    }

    for (int i = 0; i < gCodeLength; i++)
        sCode.append(1, (rand() % gCodeLetters) + 65);

//    sCode="BCBE";
    // Guess loop
    do{
        sGuess = codebreak(nCorrectDigits, nCorrectPositions);
/*        sGuess1 = codebreak1(nCorrectDigits, nCorrectPositions);
        if (sGuess == sCode && sGuess1 != sCode && !retval) {
        	std::cout<<"OG won!"<<std::endl;
        	retval=1;
        } else if (sGuess1 == sCode && sGuess != sCode && !retval) {
        	std::cout<<"std won"<<std::endl;
        	retval=2;
        } else if (sGuess == sCode && sGuess1 == sCode && !retval) {
        	std::cout<<"TIE!!!"<<std::endl;
        	retval=3;
        }*/

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

    return nGuessCount;
}

int main()
{
//	int who[5]={0};
	double sumGuessCount=0;
	int playCount=10000;

    srand((unsigned int)std::time(0));
//	return compo();

    // Generate Code
	for (int i=0; i<playCount; i++)
//		who[compo()]++;
		sumGuessCount+=compo();

//	for (int i=0;i<4;i++)
//		std::cout << "who ["<<i<<"] = "<<who[i]<<std::endl;

	std::cout << "Average guesses "<<(float)sumGuessCount/playCount<<std::endl;
	std::cout << "Score "<<(float)sumGuessCount/playCount*gMemoryRequired<<std::endl;

	return 0;
}
