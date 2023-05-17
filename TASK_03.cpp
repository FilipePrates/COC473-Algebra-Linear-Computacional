#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <Eigen/Eigenvalues>

using namespace std;
using namespace Eigen;

const int NUM_LINES = 10;
const int NUM_COLS = 10;


matriz readMatrixFile(string nomefile) {
    MatrixXd matriz(NUM_LINES, NUM_COLS);
    ifstream file(nomefile.c_str());
    if (file.is_open()) {
        for (int i = 0; i < NUM_LINES; i++) {
            for (int j = 0; j < NUM_COLS; j++) {
                file >> matriz(i, j)
            }
        }
        file.close();
    } else {
        cout << "Erro ao abrir " << nomefile << endl;
    }
    return matriz;
}


void powerMethod(const MatrixXd& matriz, int numIter) {
    x = [1,1,1,1,1,1,1,1,1,1];

    for (int i = 0; i < numIter; i++) {
        x = matriz * x;
        x.normalize();
    }
    
    VectorXd eigenvalues = matriz * x;
    double lambda = eigenvalues(0) / x(0);
    
    cout << "Método da Potência:" << endl;
    cout << "Autovalor: " << lambda << endl;
    cout << "Vetor Última Iteração: " << x << endl;
}

void jacobiMethod(const MatrixXd& matriz) {
    SelfAdjointEigenSolver<MatrixXd> eigensolver(matrix);    
    VectorXd eigenvalues = eigensolver.eigenvalues();
    MatrixXd eigenvectors = eigensolver.eigenvectors();

    cout << "Método de Jacobi:" << endl;
    cout << "Autovalores:" << endl;
    cout << eigenvalues << endl;
    cout << "Autovetores:" << endl;
    cout << eigenvectors << endl;
}

double calculateDeterminant(const MatrixXd& matrix) {
    FullPivLU<MatrixXd> lu(matrix);
    double determinant = lu.determinant();
    return determinant;
}

int main() {
    int function;
    int numIter;
    ostringstream oss;
    matriz A = readMatrixFile("Matriz_A.txt");
    cout << "Digite o código do método desejado: " << endl;
    cout << "1 - Jacobi" << endl;
    cout << "2 - Gauss-Seidel" << endl;
    cin >> function;
    char calculateDeterminantChoice;
    cout << "Deseja calcular o determinante da matriz? (S/N): ";
    cin >> calculateDeterminantChoice;

    vector<double> X;
    if (function == 1) {
        cout << "Num Iterações (200): ";
        cin >> numIter;
        jacobiMethod(matrix, numIter);
    } else if (function == 2) {
        cout << "Num Iterações (200): ";
        cin >> numIter;
        powerMethod(matrix, numIter);
    } else {
        cout << "Por favor respeite as opções disponíveis." << endl;
        return -1;
    }
    if (calculateDeterminantChoice == 'S' || calculateDeterminantChoice == 's') {
        double determinant = calculateDeterminant(matrix);
        cout << "Determinante da matriz: " << determinant << endl;
    }
    return 0;
}
