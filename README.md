# QJS-Encryption-Algorithim ver 1.0 11/17/20
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
//second for most modern day computers. 
