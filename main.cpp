#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Tree node
struct Node{
	string ch;
	double prob;
	Node *left, *right;
};

Node* newNode(string ch, double prob, Node* left, Node* right){
	Node* node = new Node();

	node->ch = ch;
	node->prob = prob;
	node->left = left;
	node->right = right;

	return node;
}

vector<Node*> nodos;

void order(){
	// Descending Order
	for (int i = 0; i < nodos.size() ; i++){
		for (int j = 0; j < nodos.size() - 1 ; j++){
			if (nodos[j]->prob < nodos[j+1]->prob){ 
					Node* temp = nodos[j]; 
					nodos[j] = nodos[j+1]; 
					nodos[j+1] = temp;
			}
		}
	}
}

ofstream file2;

void write(Node *nodo, string code){

    if(!nodo) return;

    if(!nodo->left  && !nodo->right){
        file2 << nodo->ch + "\t" + code + "\n";
        return;
    }
    
    if(nodo->left){
        code.push_back('1');
        recorrer(nodo->left, code);
    }

    code.pop_back();
    
    if(nodo->right){
        code.push_back('0');
        recorrer(nodo->right, code);
    }
}

char* huffman(char *argv) {
    
    FILE *file;
    file = fopen(argv, "r");

    vector<string> sym;
    vector<double> prob;

    if (file == NULL){
        printf("Error opening the file in huffman.\n");
        fclose(file);
        return 0;
    }

    string s = "";
    char z = fgetc(file);
    // new line
    while(z != 10){
    	s.push_back(z);
		z = fgetc(file);	
	}
    
    int size = stoi(s);
    s = "";
    int id = 0;

    int flagSym = 0, flagP = 0;

    while(z != EOF){
        // null
        if(z == 0) break;

        // tab
        if(z == 9){ 
            flagP = 1; 
            flagSym = 0;
        }else
        // new line
        if(z == 10 && !flagP){ flagSym = 1; }
        else 
        if(z == 10){ 
            prob.push_back(stod(s));
            s.clear();
            flagSym = 1;
            flagP = 0;
        }else
        if(flagSym){ 
            s.push_back(z); 
            sym.push_back(s); 
            s.clear();
        }else
        if(flagP){ s.push_back(z); }
        
        z = fgetc(file);
    }
 
    fclose(file);

    // Descending order
	for (int i = 0; i < size ; i++){
		for (int j = 0; j < size - 1 ; j++){
			if (prob[j] < prob[j+1]){ 
					double temp = prob[j]; 
					prob[j] = prob[j+1]; 
					prob[j+1] = temp;
					string tmp = sym[j]; 
					sym[j] = sym[j+1]; 
					sym[j+1] = tmp;
			}
		}
	}

    for (int i = 0; i < prob.size() ; i++){
		// New tree
		nodos.push_back(newNode(sym.at(i),prob.at(i),NULL,NULL));
    }

	// Algorithm
	while (nodos.size() != 1){
		Node *min = nodos.back(); nodos.pop_back();
		Node *max = nodos.back(); nodos.pop_back();

		nodos.push_back(newNode("",min->prob + max->prob, min, max));

		order();
	}

    //Recorrer el arbol resultante.
    char* result = "file_huffman.txt";
    file2.open(result);
    file2 << "\n";

    write(nodos.back(), "");

    file2.close();
	
    return result;
}
