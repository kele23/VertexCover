#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <set>
#include <unordered_set>
#include <iterator>
#include <cstring>

/*------------------------------
STRUTTURE UTILI
------------------------------*/

class Vertex{

public:
	
	Vertex(int weight,std::unordered_set<int> *edges = NULL){
		this->weight = weight;
		this->edges = edges;
	}

	void addEdge(int edge){
		if(this->edges == NULL)
			this->edges = new std::unordered_set<int>();
		this->edges->insert(edge);
	}

	void removeEdge(int edge){
		if(this->edges != NULL)
			this->edges->erase(edge);
	}

	std::unordered_set<int>* getEdges(){
		return this->edges;
	}

	int getWeight(){
		return this->weight;
	}

	void setWeight(int weight){
		this->weight = weight;
	}

	int getDegree() {
		if(this->edges == NULL)
			return 0;
		return this->edges->size();
	}

private:
	int weight;
	std::unordered_set<int> *edges;
};

class Edge{

public:

	Edge(Vertex* vertexA,Vertex* vertexB){
		this->vertexA = vertexA;
		this->vertexB = vertexB;
	}

	void setVertexA(Vertex* vertexA){
		this->vertexA = vertexA;
	}

	void setVertexB(Vertex* vertexB){
		this->vertexB = vertexB;
	}

	Vertex* getVertexA(){
		return this->vertexA;
	}

	Vertex* getVertexB(){
		return this->vertexB;
	}

	int getTotalDegree(){
		return vertexA->getDegree() + vertexB->getDegree();
	}

private:
	Vertex* vertexA;
	Vertex* vertexB;
};

/*------------------------------
END STRUTTURE UTILI
------------------------------*/





int main(int argc,char* argv[]){

	int V,E;
	Vertex** vertices;
	Edge** edges;





	/*------------------------------
	LETTURA DA FILE
	------------------------------*/
	FILE *input = fopen("input.txt","r");

	fscanf(input,"%d %d\n",&V,&E);
	vertices = new Vertex*[V];
	edges = new Edge*[E];

	for(int i = 0; i < E; i ++){
		int vA,vB;
		fscanf(input,"%d %d\n",&vA,&vB);
		
		if(vertices[vA] == NULL)
			vertices[vA] = new Vertex(E);
		vertices[vA]->setWeight(vertices[vA]->getWeight()-1);
		vertices[vA]->addEdge(i);

		//MODIFY VERTEX B
		if(vertices[vB] == NULL)
			vertices[vB] = new Vertex(E);
		vertices[vB]->setWeight(vertices[vB]->getWeight()-1);
		vertices[vB]->addEdge(i);

		Edge* edge = new Edge(vertices[vA],vertices[vB]);
		edges[i] = edge;
	}

	fclose(input);

	/*------------------------------
	END LETTURA DA FILE
	------------------------------*/







	/*------------------------------
	COMPUTAZIONE RISULTATO
	------------------------------*/

	for(int i = 0; i < E; i++){
		
		Vertex* vA = edges[i]->getVertexA();
		Vertex* vB = edges[i]->getVertexB();
		
		Vertex* min = vA->getWeight() < vB->getWeight() ? vA : vB;
		Vertex* max = min == vA ? vB : vA;

		min->setWeight(0);
		if(max->getWeight() != 0)
			max->setWeight(max->getWeight()+1);

	}

	/*------------------------------
	END COMPUTAZIONE RISULTATO
	------------------------------*/









	/*------------------------------
	SCRITTURA RISULTATO
	------------------------------*/
	FILE *output = fopen("output.txt","w");

	std::stringstream soluzione;

	int cardinality = 0;
	for(int i = 0; i < V; i++){
		if(vertices[i]->getWeight() == 0){
			cardinality++;
			soluzione << i << " ";
		}	
	}

	fprintf(output,"%d %s\n",cardinality,soluzione.str().c_str());

	fclose(output);
	/*------------------------------
	END SCRITTURA RISULTATO
	------------------------------*/


}