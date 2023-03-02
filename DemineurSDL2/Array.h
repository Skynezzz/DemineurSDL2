#ifndef ARRAY
#define ARRAY

typedef struct stArray
{
	int* point;
	int size;
} stArray;


stArray createTab(int size);
void removeAt(stArray* tab, int indexValue);

#endif