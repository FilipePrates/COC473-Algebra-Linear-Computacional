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

double relError(vector<double> X, vector<double> XX) {
    double erro = 0;
    for (int i = 0; i < X.size(); i++) {
        double erro_i = abs((X[i] - XX[i]) / X[i]);
        if (erro_i > erro) {
            erro = erro_i;
        }
    }
    return erro;
}

vector<double> jacobi(matriz A, vector<double> B, int maxIter, double tol) {
    vector<double> X(NUM_LINES, 0);
    vector<double> XX(NUM_LINES, 0);
    int iter = 0;
    double erro = 1;
    while (iter < maxIter && erro > tol) {
        XX = X;
        for (int i = 0; i < NUM_LINES; i++) {
            double soma = 0;
            for (int j = 0; j < NUM_COLS; j++) {
                if (j != i) {
                    soma += A.elements[i][j] * XX[j];
                }
            }
            X[i] = (B[i] - soma) / A.elements[i][i];
        }
        erro = relError(X, XX);
        iter++;
    }
    if (iter == maxIter) {
        cout << "Saiu por Max Iter" << endl;
    } else {
        cout << iter << " iterações" << endl;
    }
    return X;
}

vector<double> gaussSeidel(matriz A, vector<double>
 B, int maxIter, double tol) {
    vector<double> X(NUM_LINES, 0);
    vector<double> XX(NUM_LINES, 0);
    int iter = 0;
    double erro = 1;
    while (iter < maxIter && erro > tol) {
        XX = X;
        for (int i = 0; i < NUM_LINES; i++) {
            double soma1 = 0;
            for (int j = 0; j < i; j++) {
                soma1 += A.elements[i][j] * X[j];
            }
            double soma2 = 0;
            for (int j = i + 1; j < NUM_COLS; j++) {
                soma2 += A.elements[i][j] * XX[j];
            }
            X[i] = (B[i] - soma1 - soma2) / A.elements[i][i];
        }
        erro = relError(X, XX);
        iter++;
    }
    if (iter == maxIter) {
        cout << "Saiu por Max Iter" << endl;
    } else {
        cout << iter << " iterações" << endl;
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
    cout << "1 - Jacobi" << endl;
    cout << "2 - Gauss-Seidel" << endl;
    cin >> function;

    vector<double> X;
    if (function == 1) {
        int maxIter;
        double tol;
        cout << "Max Iterações (1000): ";
        cin >> maxIter;
        cout << "Tolerância (0.0005): ";
        cin >> tol;
        X = jacobi(A, B, maxIter, tol);
    } else if (function == 2) {
        int maxIter;
        double tol;
        cout << "Max Iterações (1000): ";
        cin >> maxIter;
        cout << "Tolerância (0.0005): ";
        cin >> tol;
        X = gaussSeidel(A, B, maxIter, tol);
    } else {
        cout << "Por favor respeite as opções disponíveis." << endl;
        return -1;
    }
    for (int i = 0; i < X.size(); i++) {
        cout << "x_" << i+1 << " = " << X[i] << endl;
    }
    return 0;
}

