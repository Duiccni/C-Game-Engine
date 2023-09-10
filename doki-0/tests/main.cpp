#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

const int N = 4;  // number of input samples
const int D = 2;  // number of input dimensions
const int H = 3;  // number of hidden units
const int O = 1;  // number of output units

// Activation function (sigmoid)
double sigmoid(double x) {
  return 1.0 / (1.0 + exp(-x));
}

// Derivative of activation function (sigmoid derivative)
double sigmoid_deriv(double x) {
  return sigmoid(x) * (1 - sigmoid(x));
}

int main() {
  // Input samples
  vector<vector<double>> X = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  // Target output values
  vector<double> y = {0, 1, 1, 0};

  // Weights and biases for hidden layer
  vector<vector<double>> W1(D, vector<double>(H));
  vector<double> b1(H);

  // Weights and biases for output layer
  vector<vector<double>> W2(H, vector<double>(O));
  vector<double> b2(O);

  // Initialize weights and biases randomly
  for (int i = 0; i < D; i++) {
    for (int j = 0; j < H; j++) {
      W1[i][j] = rand() / double(RAND_MAX);
    }
  }
  for (int i = 0; i < H; i++) {
    b1[i] = rand() / double(RAND_MAX);
  }
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < O; j++) {
      W2[i][j] = rand() / double(RAND_MAX);
    }
  }
  for (int i = 0; i < O; i++) {
    b2[i] = rand() / double(RAND_MAX);
  }

  // Training loop
  for (int epoch = 0; epoch < 1000; epoch++) {
    // Forward pass
    vector<vector<double>> h(N, vector<double>(H));
    vector<double> y_pred(N);
    for (int i = 0; i < N; i++) {
      // Hidden layer
      double z = 0;
      for (int j = 0; j < H; j++) {
        z += W2[j][0] * h[i][j];
      }
      z += b2[0];
      y_pred[i] = sigmoid(z);

      // Compute loss
      double loss = (y[i] - y_pred[i]) * (y[i] - y_pred[i]);
      cout << "Loss: " << loss << endl;

      // Backpropagation
      // Output layer
      vector<double> grad_b2(O);
      vector<vector<double>> grad_W2(H, vector<double>(O));
      double grad_z = 2 * (y[i] - y_pred[i]) * sigmoid_deriv(z);
      for (int j = 0; j < H; j++) {
        grad_W2[j][0] = grad_z * h[i][j];
      }
      grad_b2[0] = grad_z;

      // Hidden layer
      vector<double> grad_b1(H);
      vector<vector<double>> grad_W1(D, vector<double>(H));
      for (int j = 0; j < H; j++) {
        double grad_h = grad_z * W2[j][0] * sigmoid_deriv(h[i][j]);
        for (int k = 0; k < D; k++) {
          grad_W1[k][j] = grad_h * X[i][k];
        }
        grad_b1[j] = grad_h;
      }

      // Update weights and biases
      for (int j = 0; j < H; j++) {
        b1[j] += grad_b1[j];
        for (int k = 0; k < D; k++) {
          W1[k][j] += grad_W1[k][j];
        }
      }
      b2[0] += grad_b2[0];
      for (int j = 0; j < H; j++) {
        W2[j][0] += grad_W2[j][0];
      }
    }
  }

  return 0;
}
