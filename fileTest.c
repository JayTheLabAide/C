/*
	This is a short demo on generating a random dynamic array,
	writing it to a binary file, counting the number
	of elements in the file, and reading it back into
	a dynamic array based on the tallied number of elements.
	----------------------------------------------------------------------------
	Code by Jason Lillard, 2018
*/
#include <stdio.h> //for console & file i/o
#include <stdlib.h> //for rand & srand and exit
#include <time.h> //for ctime


//byte size of unsigned short
const unsigned short US_BYTESZ = sizeof(unsigned short);

//helper func to make the .dat file for this test
FILE * makeBinaryFile(const char * fname);

//helper func to print the array
void printArr( const unsigned short * arr, unsigned size);

////////////////////////////////////////////////////////////////////////////////
int main(){
	
	//seed RNG
	srand( time(0) );
	
	//file name used for this test
	const char * FNAME = "cTestData.dat";
	
	//Pointer to hold array
	unsigned short * usArr=0; 
	
	//size of array
	unsigned short size=0;
	
	//prompt for size of random array
	printf("Enter an array size: ");
	
	//read in array size, store at size
	scanf("%d", &size);
	
	puts("Allocating space for array...");
	
	//allocate a short array of size "size", store at usArr
	unsigned arrByteSize = US_BYTESZ*size;
	
	//allocate array for unsigned shorts
	usArr = (unsigned short *)malloc( arrByteSize );
	
	puts("Filling array with random numbers...");
	
	//fill array with random numbers
	unsigned i = 0;
	while( i < size ){
		//assign random unsigned short
		*(usArr+i)= (unsigned short) (rand()%(1<<15));
		//inc loop counter
		++i;
	}
	
	//Print generated array
	printArr(usArr, size);
	
	//make .dat file, store in fptr
	puts("\nMaking file...");
	FILE* fptr = makeBinaryFile(FNAME);
	
	puts("Saving array to file...");
	//fill it with data
	fwrite( usArr, US_BYTESZ, size, fptr);
	fflush(fptr);
	
	puts("Closing file...");
	//close file
	fclose(fptr);
	fptr = 0;
	puts("Deleting array...");
	//delete array
	free( usArr );
	usArr = 0;
	size = 0;
	
	puts("Re-opening file...");
	//reopen file
	fptr = fopen( FNAME, "rb");


	//determine number of elements
	puts("Counting elements...");
	//put internal file position pointer to the end
	fseek( fptr, 0, SEEK_END );
	//divide current position by short byte size to get number of shorts
	size = ftell(fptr)/US_BYTESZ;
	//reset internal position pointer of fptr
	rewind(fptr);
	printf("%d elements counted.\n", size);
	
	
	//allocate space for array from file 
	usArr = (unsigned short*)malloc( US_BYTESZ * size );
	//copy block of data into local array
	fread( usArr, US_BYTESZ, size, fptr);
	
	//output read-in array
	printf("Read in %d elements:\n", size );
	printArr(usArr, size);
	
	//free memory
	puts("\nDeleting array...");
	free( usArr );
	
	//close and delete files
	puts("Removing file...");
	fclose(fptr);
	remove(FNAME);
	
	puts("\nTest has concluded. Goodbye.");
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
FILE * makeBinaryFile(const char * fname){
	//create/overwrite file for writing in binary mode
	FILE * fptr = fopen( fname, "wb");
	if (!fptr) exit(1);
	else return fptr;
}

void printArr( const unsigned short * arr, unsigned size){
	unsigned i = 0;
	while( i < size ){
		//print array to screen
		printf("%7u%s", *(arr+i), (i==size-1)?" ":", " );
		const short COLS_PER_ROW = 5;
		//newline if at end of row
		if ( !((1+i)%COLS_PER_ROW) ) putchar('\n');
		//inc loop counter
		++i;
	}
}
