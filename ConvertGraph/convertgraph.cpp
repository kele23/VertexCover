#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

int main(int argc, char* argv[]){

	if(argc < 3){
		std::cerr << std::endl;
		std::cerr << "Necessario il nome del file da leggere e il nome del file di output" << std::endl;
		std::cerr << "Es: ./convertgraph.x Graphs/The_Kele_Graph.txt ../input.txt" << std::endl;
		std::cerr << std::endl;
		exit(1);
	}

	int V = 0;
	int E = 0;

	std::stringstream edges;

	FILE *input = fopen(argv[1],"r");

	fscanf(input,"%d\n",&V);

	for(int i = 0; i < V; i++){
		for(int j = 0; j < V; j++){
			int to = 0;
			fscanf(input,"%d",&to);
			if(j >= i && to == 1){
				edges << i << " " << j << "\n";
				E++;
			}
		}
		fscanf(input,"\n");
	}

	fclose(input);

	FILE* output = fopen(argv[2],"w");

	fprintf(output, "%d %d\n",V,E);
	fprintf(output, "%s\n", edges.str().c_str());

	fclose(output);


	return 0;
}