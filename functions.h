//By: i180529

//to keep all functions that are being used in one place also so that the main.cpp file is alot neater and easier to understand

#pragma once
#include<iostream>
#include"nb_cells.h"
#include"live_cells.h"
using namespace std;

//print grid function
void printGrid(int g[][35]) {
	for (int i = 0; i < 35; i++) {
		for (int j = 0; j < 35; j++) {
			cout << g[i][j] << "   ";
		}
		cout << endl;
	}
}

//Update Grid Function, according to UnInit Array 
void UpdateGrid(int grid[][35], live_cells arr[], int total_Lcells) {
	for (int i = 0; i < total_Lcells; i++) {
		grid[arr[i].getX()][arr[i].getY()] = 1; //i.e cell is alive
	}
}


//Stores Data of Neighbours
void StoreNeighboursData(live_cells UnInit[], nb_cells aux[], int count_live_cells) {
	for (int i = 0; i < count_live_cells; i++) {
		aux[i].setData(UnInit[i], 35);
	}
}

//checks for survival or death(by loneliness or overpopulation
//it is important to note here that due to the way we stored values in aux array i.e using UnInit indexes of live cells and their corresponding neighbours in aux array are same
void liveCellsCheck(live_cells l_arr[], nb_cells nb_arr[], int count) {
	for (int i = 0; i < count; i++) {

		int live_ncount = 0;									//counter for number of live neighbours

		for (int j = 0; j < nb_arr[i].getNum(); j++) {			//each neighbour compared with live cells to see how many live neighbours present
			for (int k = 0; k < count; k++) {
				if (nb_arr[i].getNeighbour(j).getX() == l_arr[k].getX() && nb_arr[i].getNeighbour(j).getY() == l_arr[k].getY()) live_ncount += 1;
			}
		}

		if (live_ncount == 0 || live_ncount == 1 || live_ncount > 3) {
			l_arr[i].setStatus(0);		//condition which will be used to later remove this cell since it is to die
		}
		else l_arr[i].setStatus(1);		//just a precaution

	}
}


//Updates the UnInit array according to death/survival of cells based on neighbours for next generation
void UpdateUnInit(live_cells l_arr[], int& count, int grid[][35]) {

	int num_deaths = 0;		//to keep track of how many deaths have occured to decrease array size later 
	int n = 1;
	live_cells temp;

	for (int i = 0; i < count; i++) {
		if (l_arr[i].getStatus() == 0) {
			num_deaths += 1;
			grid[l_arr[i].getX()][l_arr[i].getY()] = 0;
			for (int j = count - n; j >= i; j--) {
				if (l_arr[j].getStatus() == 1) {
					temp = l_arr[i];
					l_arr[i] = l_arr[j];		//index i overwritten by index j
					l_arr[j] = temp;			//swapped so it can be deleted
					n++;
					break;
				}
			}
		}
	}
	n--;
	count = count - num_deaths + n;
}

//birth of new cell function 
live_cells* Birth(live_cells arr[], nb_cells nb_arr[], int& count) {

	//first we will make a temp array to store all dead cells among the neighbouring cells
	live_cells* dead_cells = new live_cells[count * 8]; //count is # of live cells and 8 is maximum possible neighbor of each thus this is maximum possible dead cells assuming every neighbour is a dead cell
	int d_i = 0;							 //to store index of dead cells array

	int not_same;
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < nb_arr[i].getNum(); j++) {			//each neighbour compared with live cells to see which are dead
			not_same = 1;
			for (int k = 0; k < count; k++) {
				if (nb_arr[i].getNeighbour(j).getX() == arr[k].getX() && nb_arr[i].getNeighbour(j).getY() == arr[k].getY()) {		//if neighbour not alive 
					not_same = 0;							//if any neighbour same as any cell in live cell array then  prevent it from being added in dead cells array
				}
			}
			if (not_same) {
				int check = 1;
				for (int a = 0; a < d_i; a++) {				//prevent addition of same coordinates in the dead cells array so compare with already present dead cells
					int tempx = nb_arr[i].getNeighbour(j).getX();
					int tempy = nb_arr[i].getNeighbour(j).getY();
					if ((dead_cells[a].getX() == tempx) && (dead_cells[a].getY() == tempy))
						check = 0;			//if already present check zero so not added in array
				}
				if (check) {
					dead_cells[d_i].setX(nb_arr[i].getNeighbour(j).getX());
					dead_cells[d_i].setY(nb_arr[i].getNeighbour(j).getY());
					dead_cells[d_i].setStatus(0);
					dead_cells[d_i].setNeighbours(35);
					d_i++;
				}

			}
		}
	}



	//make another array for all the neighours of the dead cells
	nb_cells* temparr = new nb_cells[d_i];
	StoreNeighboursData(dead_cells, temparr, d_i);

	//check which of the dead_cells need to come to life

	for (int i = 0; i < d_i; i++) {

		int live_ncount = 0;									//counter for number of live neighbours

		for (int j = 0; j < temparr[i].getNum(); j++) {			//each neighbour compared with live cells to see how many live neighbours present
			for (int k = 0; k < count; k++) {
				if (temparr[i].getNeighbour(j).getX() == arr[k].getX() && temparr[i].getNeighbour(j).getY() == arr[k].getY()) live_ncount += 1;
			}
		}

		if (live_ncount < 3 || live_ncount > 3) {
			dead_cells[i].setStatus(0);		//just a precaution
		}
		else if (live_ncount == 3) dead_cells[i].setStatus(1);		//set status to alive this will be used to later add it into the UninIt array
	}

	//func to update UnInit array since dead cells is a local array so we can just update it in same function 
	int cells_toBring_toLife = 0;
	for (int i = 0; i < d_i; i++) {
		if (dead_cells[i].getStatus() == 1) {
			cells_toBring_toLife++;					//to keep a count on number of new cells that will need to be added so size of UnInit can be increased accordingly
		}
	}

	live_cells* tempUninit = new live_cells[cells_toBring_toLife + count];
	int index = 0;
	for (int i = 0; i < count; i++) {
		tempUninit[index] = arr[i];
		index++;
	}
	for (int i = 0; i < d_i; i++) {
		if (dead_cells[i].getStatus() == 1) {
			tempUninit[index] = dead_cells[i];
			index++;
		}
	}

	count = count + cells_toBring_toLife;
	return tempUninit;

	delete[] dead_cells; //deleting dynamic array from memory
	delete[] temparr;
}

//Gtesting Functions-------------------------------------------------------------------------------------------
//function to compare two live cells/cells/neighbour cells arrays which will be used for gtesting
template<class T, class U>
int compare_(T arr1[], U arr2[], int c) {
	int count = 0;
	//double loop so that while testing even if order of cells is not same in expected/actual array function still gives true if cells are same so testing is more efficient and exact
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < c; j++)
			if (arr1[i].getX() == arr2[j].getX() && arr1[i].getY() == arr2[j].getY()) count++;
	}
	if (count == c) return 1;
	else return 0;
}
//function to compare expected and actual grid
bool cmpGrids(int g1[][35], int g2[][35], int r, int c) {
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			if (g1[i][j] != g2[i][j]) return false;
		}
	}
	return true;
}
//check status of cell - for live cells check testing
bool statusCheck(live_cells a[], int b[], int n) {
	for (int i = 0; i < n; i++) {
		if (a[i].getStatus() != b[i]) return false;
	}
	return true;
}