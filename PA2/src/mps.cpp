#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void help_message() {
    cout << "usage: ./mps <input_file> <output_file>" << endl;
}
//int findSubset(int**, int);			//bottom-up
int findSubset(int**, int, int);	//top-down
void writeChord(int**, int, int);

vector<int> Chords(180010);
fstream fout;

int main(int argc, char* argv[]){
	if (argc != 3){
		help_message();
		return 0;
	}

	//read input file
	fstream fin(argv[1]);
	int _2N;
	fin >> _2N;
	int i = 0;
	while (i<_2N && !fin.eof()){
		int start_vertice, end_vertice;
		fin >> start_vertice >> end_vertice;
		Chords[start_vertice] = end_vertice;
		Chords[end_vertice] = start_vertice;
		i += 2;
	}
	fin.close();

	//initialize M table
	int** MPS = new int*[_2N];
	for (int i=0; i<_2N; i++){
		MPS[i] = new int[_2N]();
		// for (int j=0; j<_2N; j++){
		// 	MPS[i][j] = 0;
		// }
	}

	//bottom-up algorithm
	//int max_chords_number = findSubset(MPS, _2N);

	//top-down algorithm
	int max_chords_number = findSubset(MPS, 0, _2N-1);

	//write result in output file
	fout.open(argv[2], ios::out);
	fout << max_chords_number << endl;
	writeChord(MPS, 0, _2N-1);
	fout.close();
}

//bottom-up
// int findSubset(int** MPS, int _2N){
// 	for (int i=0; i<_2N; i++){
// 		MPS[i][i] = 0;
// 	}

// 	int i,j,k;
// 	for (int l=1; l<_2N; l++){
// 		for (i=0; i<_2N-l; i++){
// 			j = i + l;
// 			k = Chords[j];
// 			if (k < i || k > j){
// 				MPS[i][j] = MPS[i][j-1];
// 			}
// 			else if (k > i && k < j){
// 				if (MPS[i][j-1] >= 1 + MPS[i][k-1] + MPS[k+1][j-1]){
// 					MPS[i][j] = MPS[i][j-1];
// 				}else{
// 					MPS[i][j] = 1 + MPS[i][k-1] + MPS[k+1][j-1];
// 				}
// 			}
// 			else if (k == i){
// 				if (l == 1){
// 					MPS[i][j] = 1;
// 				}else{
// 					MPS[i][j] = 1 + MPS[i+1][j-1];
// 				}
// 			}
// 		}
// 	}
// 	return MPS[0][_2N-1];
// }

//top-down
int findSubset(int** MPS, int i, int j){
	if (i >= j){
		MPS[i][j] = 0;
		return MPS[i][j];
	}
	if (MPS[i][j] > 0){
		return MPS[i][j];
	}

	int k = Chords[j];
	if (k < i || k > j){
		MPS[i][j] = findSubset(MPS, i, j-1);
	}
	else if (k > i && k < j){
		MPS[i][j] = findSubset(MPS, i, j-1);
		int temp = 1 + findSubset(MPS, i, k-1) + findSubset(MPS, k+1, j-1);
		if (MPS[i][j] < temp){
			MPS[i][j] = temp;
		}
	}
	else if (k == i){
		if (j == i + 1){
			MPS[i][j] = 1;
		}else{
			MPS[i][j] = 1 + findSubset(MPS, i+1, j-1);
		}
	}
	return MPS[i][j];
}

//bottom-up
// void writeChord(int** MPS, int start, int end){
// 	if (end == start){
// 		return;
// 	}
// 	int k = Chords[end];
// 	if (k == start){
// 		fout << k << " " << end << endl;
// 		writeChord(MPS, start+1, end-1);
// 	}
// 	else if (k > start && k < end && MPS[start][k-1] + MPS[k][end] == MPS[start][end]){
// 		writeChord(MPS, start, k-1);
// 		writeChord(MPS, k, end);
// 	}
// 	else{
// 		writeChord(MPS, start, end-1);
// 	}
// }

//top-down
void writeChord(int** MPS, int start, int end){
	if (end == start){
		return;
	}
	int k = Chords[end];
	if (k == start){
		fout << k << " " << end << endl;
		writeChord(MPS, start+1, end-1);
	}
	else if (k > start && k < end){
		if (1 + findSubset(MPS, start, k-1) + findSubset(MPS, k+1, end-1) > findSubset(MPS, start, end-1)){
			writeChord(MPS, start, k-1);
			fout << k << " " << end << endl;
			writeChord(MPS, k+1, end-1);
		}else{
			writeChord(MPS, start, end-1);
		}
	}
	else{
		writeChord(MPS, start, end-1);
	}
}