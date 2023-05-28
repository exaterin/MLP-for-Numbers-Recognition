#include "MultilayerPerceptron.h"
#include "Helper.h"

#include <iostream>

using namespace std;

void train() {
  // Define the input and label vectors for training and testing sets
  vector<vector<int> > training_set_inputs;
  vector<int> training_set_labels;

  // Load the MNIST dataset from the input file and fill the input and label vectors for training and test sets
  cout << "Loading MNIST dataset..." << endl;
  Helper::read_mnist_file("MNIST_train.txt", training_set_inputs, training_set_labels);

  // Configuration of MLP
  int num_layers;
  vector<int> num_neurons;
  string activation_function;


  Helper::read_mlp_configuration("Config2.txt", num_layers, num_neurons, activation_function);

  int number_of_epochs = 20;
  int number_of_examples = training_set_inputs.size();
  double initial_learning_rate = 0.1;
  double learning_rate = initial_learning_rate;
  int step_size = 5; // Number of epochs after which the learning rate will be reduced
  double decay_rate = 0.9; // Factor by which the learning rate will be reduced

  // Create an instance of the multi-layer perceptron
  MultilayerPerceptron mlp = MultilayerPerceptron(num_layers, num_neurons, activation_function);

  cout << "Training..." << endl;

  // Train the MLP
  for (int epoch = 0; epoch < number_of_epochs; epoch++){
    int correct = 0;

    // Update learning rate based on step decay
    if ((epoch + 1) % step_size == 0) 
      learning_rate *= decay_rate;

    for (int i = 0; i < number_of_examples; i++){

      vector<double> input = mlp.normalize_input(training_set_inputs[i]);
      vector<double> centered = Helper::centerDigit(input, 28, 28, 0.5);

      mlp.forward_propagation(centered);

      int predict = mlp.prediction();

      if (predict != training_set_labels[i]){

        mlp.backpropagation(training_set_labels[i]);
        mlp.update_network(learning_rate);
      }

      else
        correct++;

    }

    cout << "Epoch: " << epoch + 1 << "\n" << "Accuracy: " << (correct / static_cast<double>(number_of_examples) * 100) << "%" << endl;
  }

  cout << "Training complete." << endl;

  // Save the weights and biases
  cout << "Saving weights..." << endl;
  mlp.save_weights("weights2.txt");
}


void test() {

  vector<vector<int> > testing_set_inputs;
  vector<int> testing_set_labels;

  Helper::read_mnist_file("MNIST_test.txt", testing_set_inputs, testing_set_labels);

  // Configuration of MLP
  int num_layers;
  vector<int> num_neurons;
  string activation_function;

  Helper::read_mlp_configuration("Config2.txt", num_layers, num_neurons, activation_function);

  MultilayerPerceptron mlp = MultilayerPerceptron(num_layers, num_neurons, activation_function);

  mlp.read_weights("weights2.txt");

  cout << "Testing..." << endl;

  // Test the MLP
  int correct = 0;
  for (int i = 0; i < testing_set_inputs.size(); i++){

    vector<double> input = mlp.normalize_input(testing_set_inputs[i]);
    vector<double> centered = Helper::centerDigit(input, 28, 28, 0.5);

    mlp.forward_propagation(centered);

    int predict = mlp.prediction();

    if (predict == testing_set_labels[i])
      correct++;

  }

  cout << "Test set accuracy: " << (correct / static_cast<double>(testing_set_inputs.size()) * 100) << "%" << endl;

}

int main() {


  train();
  test();



  return 0;
}