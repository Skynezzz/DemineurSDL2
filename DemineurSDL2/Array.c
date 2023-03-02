#include "Array.h"
#include <stdlib.h>

stArray createTab(int size)
{
	stArray tab = { NULL, 0 };
	tab.point = (int*)malloc(size * sizeof(int));
	if (tab.point == NULL)
		return tab;
	tab.size = size;

	for (int i = 0; i < size; i++)
	{
		tab.point[i] = i;
	}
	return tab;
}

void removeAt(stArray* tab, int indexValue)
{
	if (indexValue >= tab->size)
		return;
	int* newPoint = (int*)malloc((tab->size - 1) * sizeof(int));
	int i = 0;

	for (i = 0; i < indexValue; i++)
	{
		newPoint[i] = tab->point[i];
	}

	for (i = indexValue + 1; i < tab->size; i++)
	{
		newPoint[i - 1] = tab->point[i];
	}

	free(tab->point);
	tab->point = newPoint;
}