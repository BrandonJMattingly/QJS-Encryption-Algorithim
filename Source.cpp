//******************************************************************\\
// Quarternion Encryption Program                                    \\
// Purpose: To encrypt an image using a Mandelbrot Summation        \\
// Creator: Dr. Jeff Lee P.H.D                                      \\
// Designer: Mr. Brandon Mattingly                                  \\
// Outline: This program will read in an image. It will then loop   \\
// through each pixel in the image to encrypt them.                 \\
//                                                                  \\
// First, it will convert the pixel into its RGB numeric values. The\\
// RGB values exist as integers between (0,255). Next, it will      \\
// encrypt the RGB values using a quarternion and then perform      \\
// Mandelbrot summations using the function mandelbrot. The function\\
// will run until the partial sum has an integer value of key. The  \\
// key is determined by an RNG generator, and will be communicated  \\
// by Alice and Bob prior to encryption/decryption.                 \\
//                                                                  \\
// Next, these images need to be decrypted. The program can use the \\
// function decrypt to do so. Ideally, you would have the encryption\\
// and decryption functions in seperate programs, but as a proof of \\
// concept this is fine. You will read in the two encrypted pictures\\ 
// pixel by pixel.  Be sure to keep note which picture gives the    \\
// integer parts and which gives the fractional parts. You will pull\\
// the RGB values of each pixel and feed them into decrypt as two   \\
// pointers. decrypt will then convert those two pointers into a key\\
// pointer.  Note that do to round off error the key will be        \\
// slightly less than the matching mandelbrot summation from the    \\
// original picture. The function will then go through each possible\\
// pixel value using a brute force algorithim ending the partial    \\
// with the predetermined key value. This will take less than a     \\
//second for most modern day computers.                             \\
//******************************************************************\\
 
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <time.h>
using namespace std;

/* This function computes three separate mandelbrot summations. It takes
two pointers to two arrays of four elements a piece. The pointer, pxl,
should point to the key (K, R, G, B) values to be summed. The second pointer, m, should
point to the array where you will store the summations. m can hold two
separate KRGB elements which will act as (K_{i-1}, R_{i-1}, G_{i-1}, B_{i-1}, K_i, R_i, G_i, B_i)
where R_i means the ith element in the mandelbrot summation for R.
The formulas for summing is:
K_i=(K_{i-1})^2-[(R_{i-1})^2+(G_{i-1})^2+(B_{i-1})^2]+K
R_i=2*(K_{i-1})*(R_{i-1})+R
G_i=2*(K_{i-1})*(G_{i-1})+G
B_i=2*(K_{i-1})*(B_{i-1})+B
The function will run until the summation continues until the value coeff is reached.
The function returns the pointer m.*/
double* mandelbrot(double* pxl, double* m, int coeff)
{
	//The while loop will repeat summing m using the formulas above to compute the mandelbrot sums.
	//It will continue until either i =10 or j=500.
	for (int i = 0; i < coeff; i++)
	{
		//Computes the three mandelbrot sums using the formulas for quarternion multiplication.
		*(m + 4) = *m * *m - *(m + 1) * *(m + 1) - *(m + 2) * *(m + 2) - *(m + 3) * *(m + 3) + *pxl;
		*(m + 5) = 2 * *m * *(m + 1) + *(pxl + 1);
		*(m + 6) = 2 * *m * *(m + 2) + *(pxl + 2);
		*(m + 7) = 2 * *m * *(m + 3) + *(pxl + 3);
		
		//Copies (F_i,R_i,G_i,B_i) into (F_i-1,R_i-1,G_i-1,B_i-1) in preparation for the next loop.
		for (int k = 0; k < 4; k++)
		{
			*(m + k) = *(m + k + 4);
			std::cout << *(m + k) << " ";
		}
		//std::cout << endl;
	}

	//Returns the array containing the converged sums. Note that every third element should be the same.
	return m;
}

/* This function decrypts two different pixels into the single original pixel.
It takes pointers to two different arrays of 3 elements and returns a pointer
to a 3 element array. First, it converts the two function arguments and
converts them into a single array which will serve as the key. Next it
creates 3 different arrays, one to be the text pixel, the second to hold the
mandelstrom summations, and the third to be the decrypted pixel. It then
creates several ints to hold the R G B values and a check for when the
correct values have been found. Then, it loops twice checking the R and G
values. The loop scales the pixel to be between 0 and 1, computes the
mandelbrot summations, and then compares with the key. If it equals the key
it sets the decrypted pixel's R and G values to the current R and G values
and sets c=1 to end the loop.  Then the R and G values are set to the
decrypted values and c is reset to 0. Finally, the function loops once
more to find the B values based off the known R and G values. The total
number of loops should be 256^2+256=65792 at max, which means the pixel
should be decrypted in less than a second. Finally, the program returns
the found R G B pixels in an array of 3 elements.                            */
int decrypt(double* ip, double* fp, double* dp)
{
	//Creates an array of 3 elements to hold the key for comaprision. k serves 
	//as the pointer to the array.
	double key[3] = { 0,0,0 };
	double *k;
	k = key;

	//Takes the two pointers from the function arguments and converts them
	//into the key. We will compare the mandelstrom sums to this key to see
	//if they came from the same RGB value.
	for (int i = 0; i < 3; i++)
		*(k + i) = *(ip + i) + *(fp + i) / 255;

	//Creates an array of 3 elements to contain the test pixel. This is the 
	//set of RGB values which we mandelstrom sum and compare to the key. 
	//tp will serve as the pointer to this array.
	double testpixel[3] = { 0,0,0 };
	double *tp;
	tp = testpixel;
	//Also, creates an aray of 6 elements to hold (R_i-1, G_i-1, B_i-1, R_i, G_i, B_i)
	//for the mandelstrom sums of the test pixel. tm will serve as the pointer to this
	//array.
	double testmandel[6] = { 0,0,0,0,0,0 };
	double *tm;
	tm = testmandel;

	//Modified Brute Force Algorithim for finding the RGB values
	//Initializes, RGB and c all at 0.
	double R = 0;
	double G = 0;
	double B = 0;
	int c = 0;

	//The mandelstrom summation for the R and G values are intermixed, so the
	//program needs sum through all possible elements (R,G) -> (0,0) to (255,255)
	//to find the R and G mandelstrom summations that match the key. The while loops
	//will repeat until either the key R and G values are found or (R,G) = (256,256).
	while (c == 0 && R < 256)
	{
		while (c == 0 && G < 256)
		{
			for (int coeff = 1; coeff < 51; coeff++)
			{//Initializes the test pixel to the RGB values.
				*tp = R;
				*(tp + 1) = G;
				*(tp + 2) = B;

				//Scales the test pixel to be between 0 and 1.
				for (int l = 0; l < 3; l++)
				{
					*(tp + l) = log10(log10((*(tp + l) + 1000) / 10));
				}

				//Computes the mandelstrom sum.
				tm = mandelbrot(tp, tm, coeff);

				//Checks to see if the R and G summations are within .005 of the key's
				//values. If, so the current R and G values are stored in the decrypted
				//pixel and c is set to 1 to end the loops.
				if (abs(*k - *tm * 255) < .005 && abs(*(k + 1) - *(tm + 1) * 255) < .005)
				{
					/*cout << "R=" << R << " G=" << G << " B=" << B << endl;
					cout << "Key = " << *k << " " << *(k + 1) << " " << *(k + 2) << endl;
					cout << "Test Pixel = " << *tp << " " << *(tp + 1) << " " << *(tp + 2) << endl;
					cout << "Mandelstrom Sum= " << *tm << " " << *(tm + 1) << " " << *(tm + 2) << endl;
					cout << "Check Key: " << abs(*k - *tm * 255) << " " << abs(*(k + 1) - *(tm + 1) * 255) << " " << abs(*(k + 2) - *(tm + 2) * 255) << endl << endl;*/
					*dp = R;
					*(dp + 1) = G;
					c = 1;
				}
			}
			//Increments the G value by 1.
			G++;
		}
		//Resets the G values to 0 and increments the R values by 1.
		G = 0;
		R++;
	}

	//The R and G values have to be reset to the current values stored in the
	//decrypted values and c needs to be reset to 0.
	R = *dp;
	G = *(dp + 1);
	c = 0;

	//Loops through just the B values now to find the mandelstrom sums 
	//that match the key.
	while (c == 0 && B < 256)
	{
		for (int coeff = 1; coeff < 51; coeff++)
		{//Initializes the test pixel to the RGB values.
			*tp = R;
			*(tp + 1) = G;
			*(tp + 2) = B;

			//Scales the test pixel to be between 0 and 1.
			for (int l = 0; l < 3; l++)
			{
				*(tp + l) = log10(log10((*(tp + l) + 1000) / 10));
			}

			//Computes the mandelstrom sum.
			tm = mandelbrot(tp, tm, coeff);

			//Checks to see if B summation are within .005 of the key's
			//values. If, so the current B value is stored in the decrypted
			//pixel and c is set to 1 to end the loop.
			if (abs(*(k + 2) - *(tm + 2) * 255) < .005)
			{
				c = 1;
				*(dp + 2) = B;
				/*cout << "R=" << R << " G=" << G << " B=" << B << endl;
				cout << "Key = " << *k << " " << *(k + 1) << " " << *(k + 2) << endl;
				cout << "Test Pixel = " << *tp << " " << *(tp + 1) << " " << *(tp + 2) << endl;
				cout << "Mandelstrom Sum= " << *tm << " " << *(tm + 1) << " " << *(tm + 2) << endl;
				cout << "Check Key: " << abs(*k - *tm * 255) << " " << abs(*(k + 1) - *(tm + 1) * 255) << " " << abs(*(k + 2) - *(tm + 2) * 255) << endl << endl;
				*/
			}
		}
		B++;
	}

	return 0;
}

int main()
{
	//Creates the array to hold the RGB values of the inputted pixel.
	//p serves as a pointer to the start of the array.
	//Testing shows that .175 is a useful number for the free integer as it creates a create deal of chaos for our quarternion.  
	//This is hard coded for Proof of Concept, but it should be chosen freely as part of your key for added security.
	//The suggested range is from (0.1,.183) Anything above that range will converge rapidly making the
	//encryption worthless. If you get to far below 0.1, the pattern becomes too simple and becomes easy to break.
	//Ultimately, this is an inherent wekaness in the Mandelbulb encryption method.
	double pixel[4] = { .175, 0, 0, 0 };
	double *p;
	p = pixel;
	//This is a double to hold the amount of time taken to run.
	double elapsedtime;

	//First checks to see if the user wants to encrypt a file or decrypt one.
	int ed;
	std::cout << "Enter 1 if you wish to encrypt a file, or Enter 0 if you wish to decrypt a file" << endl;
	std::cin >> ed;

	if (ed == 1)
	{
		//creates a time_t to store the time teh program starts and double to hold the number of seconds.
		time_t beginningtime;
		double seconds;

		//Creates a string to read in the filename of a .csv file containing the 
		//RGB values of an image to be encrypted/decrypted.
		string filename;
		std::cout << "Enter the File Name: " << endl;
		std::cin >> filename;
		//cout << endl << filename << endl;

		//Creates an ifstream to pull the needed values from the file and place them into
		//our array.
		ifstream input(filename);
		//ifstream input("MandelTestSheet.csv");

		//Finally we need to create the files that will hold the encrypted parts of
		//our mandelstrom sums.  EncryptedInts.txt will hold the integer parts.
		//EncryptedFracts.txt will create the encrypted Fractional Parts.
		//EMA.txt hold all of them in an interwoven fashion.
		string EM = "EMA.txt";
		ofstream outputEM(EM);

		char comma;
		int length;
		int width;
		input >> length >> comma >> width >> comma;
		outputEM << length << " " << width << "\n";
		//std::cout << length << " " << width << "\n";
		int area = length*width;

		std::cout << area << endl;

		beginningtime = time(NULL);
		//Repeats the encryption for each line of the .csv file to encode each pixel.
		//The while loop will repeat as long as the file has not hit a eof flag.
		int i = 1;
		while (i <= area)
		{
			//Pulls the integer values from the input three at a time.
			//.csv files put RGB values into the format "#,#,#\n" for each line
			//Consequently, we need a char called comma to absorb the commas
			//and allow only the integer values to be placed into out double array.
			input >> *(p + 1) >> comma >> *(p + 2) >> comma >> *(p + 3);
			//std::cout << *(p + 0) << " " << *(p + 1) << " " << *(p + 2) << " " << *(p + 3) << " " << endl;
			//Note sometimes if the B pixel is greater than 26,
			//it will be truncated by 1 due to Excell's mystery characters

			//This is the key for the Summation. If Eve knows how you choose the 
			//coefficients, she becomes Bob and the message will be intercepted.
			//We have chosen a very simple one of increasing up to 512 and then
			//repeating. In practice, you should pick a string of random numbers.
 			int coeff = 1 + i % 511;
			//std::cout << 1 + i % 511 << "\n";

			//Scales the elements of the pixel to be between (0,1) using the 
			//formula: log10(log10(p+1000)/10).  This formula can be changed as 
			//necessary for a trememndous amount of variation.  The only
			//restriction on possible formulas is that it has to map the
			//RGB values to all be between 0 and 1.
			for (int j = 1; j < 4; j++)
			{
				*(p + j) = log10(log10((*(p + j) + 1000) / 10));
			}

			//Creates an array of 8 elements to hold the (K_i-1, R_i-1, G_i-1, B_i-1, K_i, R_i, G_i, B_i)
			//of the mandelstrom summations. m serves as a pointer to the array.
			double mandel[8] = { 0,0,0,0, 0,0,0,0 };
			double *m;
			m = mandel;

			//Calls the function mandelbrot to execute the mandelstrom sums.
			m = mandelbrot(p, m, coeff);

			//Creates an array of 4 elements to hold the integer parts of the 
			//mandelstrom summations. EP serves as a pointer to the array.
			double EncryptedPixel[4] = { 0,0,0,0 };
			double *EP;
			EP = EncryptedPixel;

			//Multiplies each element of the sums by 255 and then takes the integer
			//part and stores it in EP.
			//The For loop repeats 4 times for each sum.
			for (int j = 0; j < 4; j++)
			{
				*(EP + j) = modf(*(m + j) * 255, EP + j);
				modf(*(EP + j) * 255, EP + j);
			}

			//Finally, we send the Encrypted Pixel to an encrypted files, EMA.txt.
			outputEM << *EP << " " << *(EP + 1) << " " << *(EP + 2) << " " << *(EP + 3) << endl;
			
			//outputEM << *IP << " " << *(IP + 1) << " " << *(IP + 2) << " " << *FP << " " << *(FP + 1) << " " << *(FP + 2) << "\n";
			//std::cout << d6 << endl;

			//This is for BugTesting only
			//std::cout << *IP << " " << *(IP+1) << " " << *(IP+2) << " " << endl;
			//std::cout << *FP << " " << *(FP + 1) << " " << *(FP + 2) << " " << endl << endl;
			i++;
		}

		//creates a second time_t to store endtime.
		time_t endtime; /* might be unnecessary, but it puts the time in seconds (it's at the top as well) */
		time(&endtime); /* this is the same as endtime = time(NULL) */
		elapsedtime = difftime(endtime, beginningtime);
	}

	else if (ed == 0)
	{
		//creates a time_t to store the time teh program starts and double to hold the number of seconds.
		time_t beginningtime;
		double seconds;

		//Creates an array of 3 elements to hold the decrypted RGB values.
		//When the program finds the RGB values that match the key it will be stored
		//in this array. dp will serve as a pointer to this array.
		double decryptedpixel[3] = { 0,0,0 };
		double *dp;
		dp = decryptedpixel;

		//Creates an array of 3 elements to hold the integer parts of the 
		//mandelstrom summations. IP serves as a pointer to the array.
		double IntPixel[3] = { 0,0,0 };
		double *IP;
		IP = IntPixel;
		//Creates an array of 3 elements to hold the fractional parts of the
		//mandelstrom summation. FP serves as a pointer to the array.
		double FractPixel[3] = { 0,0,0 };
		double *FP;
		FP = FractPixel;

		//Next it asks the user for the file to be decrypted.
		//The file is assumed to have been generated by this program,
		//so it should be a .txt file with a format of "# # # # # #\n" for each line.
		string filename;
		string outputname = "DecryptedMandel.txt";
		std::cout << "Please Enter the name of the file to be decrypted" << endl;
		std::cin >> filename;
		//Creates an ifstream to pull the needed values from the file and place them into
		//our array.
		ifstream input(filename);
		//Creates an ostream to store the decrypted RGB values.
		ofstream outputDP(outputname);

		int length;
		int width;
		input >> length >> width;
		outputDP << length << " " << width << "\n";
		int area = length*width;

		beginningtime = time(NULL);
		for (int i = 1; i <= area; i++)
		{
			//Pulls the 2 arrays needed to be run.
			//input >> *IP >> *(IP + 1) >> *(IP + 2) >> *FP >> *(FP + 1) >> *(FP + 2);
			int d6 = i % 6;
			if (d6 == 0)
			{
				input >> *IP >> *(IP + 1) >> *(IP + 2) >> *FP >> *(FP + 1) >> *(FP + 2);
			}
			else if (d6 == 1)
			{
				input >> *(IP + 1) >> *(IP + 2) >> *FP >> *(FP + 1) >> *(FP + 2) >> *IP;
			}
			else if (d6 == 2)
			{
				input >> *(IP + 2) >> *FP >> *(FP + 1) >> *(FP + 2) >> *IP >> *(IP + 1);
			}
			else if (d6 == 3)
			{
				input >> *FP >> *(FP + 1) >> *(FP + 2) >> *IP >> *(IP + 1) >> *(IP + 2);
			}
			else if (d6 == 4)
			{
				input >> *(FP + 1) >> *(FP + 2) >> *IP >> *(IP + 1) >> *(IP + 2) >> *FP;
			}
			else if (d6 == 5)
			{
				input >> *(FP + 2) >> *IP >> *(IP + 1) >> *(IP + 2) >> *FP >> *(FP + 1);
			}
			//std::cout << d6 << endl;
			//std::cout << *IP << " " << *(IP + 1) << " " << *(IP + 2) << " " << *FP << " " << *(FP + 1) << " " << *(FP + 2) << "\n";
			//Takes the Fractional and Integer arrays and decrypts them.
			decrypt(IP, FP, dp);

			//outputs them to an output file.
			outputDP << *dp << " " << *(dp + 1) << " " << *(dp + 2) << "\n";
		}
		//cout << "Decrypted Pixel: " << *dp << " " << *(dp + 1) << " " << *(dp + 2) << endl << endl;

		//creates a second time_t to store endtime.
		time_t endtime; /* might be unnecessary, but it puts the time in seconds (it's at the top as well) */
		time(&endtime); /* this is the same as endtime = time(NULL) */
		elapsedtime = difftime(endtime, beginningtime);
	}

	else
	{
		std::cout << "Invalid entry. Please reboot." << endl;
	}

	std::cout << "The total time is " << elapsedtime << "s" << endl;

	return 0;
}