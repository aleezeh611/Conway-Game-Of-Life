//By: i180529

//NOTE --->> no templates used since no user input etc taken, it is an independant game working automatically, once started, with whole integers that are the indexes of grid

#include<fstream>
#include"functions.h"
using namespace std;


//Main Game Of Life Function where everything is happening!!
live_cells* GameOfLife() {

	//basic variables being used 
	int grid[35][35] = { 0 };
	int count_live_cells;
	int tempx, tempy;
	live_cells* UnInit;
	nb_cells* aux;
	int gen_count;
	//variables needed for file handling
	fstream data;
	data.open("info.txt", ios::in);

	if (!data) {
		cout << "unable to open file\n";
		exit(1);
	}
	else {
		data >> gen_count;
		data >> count_live_cells;
		UnInit = new live_cells[count_live_cells];
		aux = new nb_cells[count_live_cells];

		for (int i = 0; i < count_live_cells; i++) {
			data >> tempx;
			data >> tempy;
			UnInit[i].setX(tempx);
			UnInit[i].setY(tempy);
			UnInit[i].setNeighbours(35);
		}
	}
	data.close();  //close file

	cout << "Number of Iterations:" << gen_count << endl;

	//Update grid by using UnInit array as initiallized with all dead cells
	UpdateGrid(grid, UnInit, count_live_cells);

	//Store Data of neighbours
	StoreNeighboursData(UnInit, aux, count_live_cells);

	//display grid
	cout << endl;
	cout << "===========================================================================================================================" << endl;
	printGrid(grid);

	//BEGIN LOOP FOR GAME OF LIFE (10 iterations as defined by assignment pdf, can be altered by altering info.txt file)

	int enter = 1;
	while (enter < 10) {
		system("CLS");
		//checking conditions for survival/death/birth of live cells through function
		liveCellsCheck(UnInit, aux, count_live_cells);
		live_cells* temparray = Birth(UnInit, aux, count_live_cells);   //a temorary array which will store the elements in UnInit array + the new cells which should be added to UnInit array


		//Updating grid and UnInit array and aux array
		delete[] UnInit;
		UnInit = new live_cells[count_live_cells];
		for (int i = 0; i < count_live_cells; i++) {
			UnInit[i] = temparray[i];
		}
		UpdateUnInit(UnInit, count_live_cells, grid);
		cout << endl;
		StoreNeighboursData(UnInit, aux, count_live_cells);
		UpdateGrid(grid, UnInit, count_live_cells);

		//display next generation
		cout << endl << "==========================================================<< GAME OF LIFE >>=============================================================\n\n";
		printGrid(grid);
		cout << endl << "==============================================================<< END >>==================================================================\n";

		delete[] temparray;

		//cout << "\npress 1 for next generation or 0 to exit   ";
		//cin >> enter;		//-------->> IF WANT TO MAKE USER CONTROLLED GAME 
		enter++;			//----->>automatic game

	}

	//deleting all memory created on heap by new
	//delete[] UnInit; would only be deleted if this was a sole function, but need to return UnInit due to Gtesting
	delete[] aux;
	return UnInit; //better to return UnInit since checking whole grid(which could even be infinite would take too much time and memory thus more efficient

}



int main() {
	live_cells* testing = GameOfLife();
	cout << "\nFinal living cells:\n";
	for (int i = 0; i < 6; i++) {
		testing[i].showStoredValues();
	}
	return 0;
}