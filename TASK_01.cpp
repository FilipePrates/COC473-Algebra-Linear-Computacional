#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

const int NUM_LINES = 10;
const int NUM_COLS = 10;

struct matriz {
    vector<vector<double>> elements;
};

matriz readMatrixFile(string nomefile) {
    matriz A;
    A.elements.resize(NUM_LINES);
    for (int i = 0; i < NUM_LINES; i++) {
        A.elements[i].resize(NUM_COLS);
    }
    ifstream file(nomefile.c_str());
    if (file.is_open()) {
        for (int i = 0; i < NUM_LINES; i++) {
            for (int j = 0; j < NUM_COLS; j++) {
                file >> A.elements[i][j];
            }
        }
        file.close();
    } else {
        cout << "Erro ao abrir " << nomefile << endl;
    }
    return A;
}
vector<double> readVectorFile(string nomefile) {
    vector<double> B;
    ifstream file(nomefile.c_str());
    if (file.is_open()) {
        B.resize(NUM_LINES);
        for (int i = 0; i < NUM_LINES; i++) {
            file >> B[i];
        }
        file.close();
    } else {
        cout << "Erro ao abrir " << nomefile << endl;
    }
    return B;
}

matriz LUdecomp(matriz A) {
    int n = NUM_LINES;
    matriz LU = A;
    for (int j = 0; j < n-1; j++) {
        for (int i = j+1; i < n; i++) {
            LU.elements[i][j] = LU.elements[i][j] / LU.elements[j][j];
            for (int k = j+1; k < n; k++) {
                LU.elements[i][k] = LU.elements[i][k] - LU.elements[i][j] * LU.elements[j][k];
            }
        }
    }
    return LU;
}

vector<double> solveLU(matriz LU, vector<double> B) {
    int n = NUM_LINES;
    vector<double> X(n);
    vector<double> Y(n);
    //LY = B
    Y[0] = B[0];
    for (int i = 1; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++) {
            sum += LU.elements[i][j] * Y[j];
        }
        Y[i] = B[i] - sum;
    }
    //UX = Y
    X[n-1] = Y[n-1] / LU.elements[n-1][n-1];
    for (int i = n-2; i >= 0; i--) {
        double sum = 0;
        for (int j = i+1; j < n; j++) {
            sum += LU.elements[i][j] * X[j];
        }
        X[i] = (Y[i] - sum) / LU.elements[i][i];
    }
    return X;
}

matriz cholesky(matriz A) {
    int n = NUM_LINES;
    matriz L = A;
    for (int j = 0; j < n; j++) {
        double sum = 0;
        for (int k = 0; k < j; k++) {
            sum += pow(L.elements[j][k], 2);
        }
        L.elements[j][j] = sqrt(L.elements[j][j] - sum);
        for (int i = j+1; i < n; i++) {
            sum = 0;
            for (int k = 0; k < j; k++) {
                sum += L.elements[i][k] * L.elements[j][k];
            }
            L.elements[i][j] = (L.elements[i][j] - sum) / L.elements[j][j];
        }
    }
    return L;
}

vector<double> solveCholesky(matriz L, vector<double> B) {
    int n = NUM_LINES;
    vector<double> X(n);
    vector<double> Y(n);
    //LY = B
    Y[0] = B[0] / L.elements[0][0];
    for (int i = 1; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++) {
            sum += L.elements[i][j] * Y[j];
        }
        Y[i] = (B[i] - sum) / L.elements[i][i];
    }
    //L^TX = Y
    X[n-1] = Y[n-1] / L.elements[n-1][n-1];
    for (int i = n-2; i >= 0; i--) {
        double sum = 0;
        for (int j = i+1; j < n; j++) {
            sum += L.elements[j][i] * X[j];
        }
        X[i] = (Y[i] - sum) / L.elements[i][i];
    }
    return X;
}

int main() {
    int function;
    int vectorNum;
    ostringstream oss;
    matriz A = readMatrixFile("Matriz_A.txt");
    cout << "Qual o Vetor B a ser usado? (1/2/3)" << endl;
    cin >> vectorNum;
    oss << "Vetor_B_0" << vectorNum << ".txt";
    vector<double> B = readVectorFile(oss.str());
    cout << "Digite o código do método desejado: " << endl;
    cout << "1 - Decomposição LU" << endl;
    cout << "2 - Decomposição de cholesky" << endl;
    cin >> function;
    
    vector<double> X;
    if (function == 1) {
        matriz LU = LUdecomp(A);
        X = solveLU(LU, B);
    } else if (function == 2) {
        matriz L = cholesky(A);
        X = resolverSistemacholesky(L, B);
    } else {
        cout << "Por favor respeite as opções disponíveis." << endl;
        return -1;
    }
    for (int i = 0; i < X.size(); i++) {
        cout << "x_" << i+1 << " = " << X[i] << endl;
    }
    return 0;
}

