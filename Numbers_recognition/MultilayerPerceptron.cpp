#include "MultilayerPerceptron.h"
#include "Helper.h"
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <sstream>

using namespace std;

/* Constructor:
    Initialize the neurons and errors vectors with zeros
    Initialize the weights and biases with random numbers
*/

MultilayerPerceptron::MultilayerPerceptron(int& num_layers, vector<int>& num_neurons, string activation_function) {
    this->num_layers = num_layers;
    this->num_neurons = num_neurons;

    // Initialize the activation function
    if (activation_function == "sigmoid") {
        this->activation_function = ActivationFunction::Sigmoid;
    }
    else if (activation_function == "relu") {
        this->activation_function = ActivationFunction::ReLU;
    }
    else {
        throw std::invalid_argument("Invalid activation function: " + activation_function);
    }

    // Initialize the neurons and errors vectors
    for (int i = 0; i < num_layers; i++) {
        vector<double> layer_neurons;
        vector<double> layer_errors;
        for (int j = 0; j < num_neurons[i]; j++) {
            layer_neurons.push_back(0);
            layer_errors.push_back(0);
        }
        neurons.push_back(layer_neurons);
        errors.push_back(layer_errors);
    }

    // Initialize the weights and biases with random numbers
    srand(time(NULL)); // Seed the random number generator

    for (int i = 0; i < num_layers - 1; i++) {

        // Generate a matrix of random weights
        vector<std::vector<double> > layer_weights;
        for (int j = 0; j < num_neurons[i + 1]; j++) {
            vector<double> neuron_weights;
            for (int k = 0; k < num_neurons[i]; k++) {
                double weight = static_cast<double>(std::rand()) / RAND_MAX;
                neuron_weights.push_back(weight);
            }
            layer_weights.push_back(neuron_weights);
        }
        weights.push_back(layer_weights);

        // Generate a vector of random biases
        vector<double> layer_biases;
        for (int j = 0; j < num_neurons[i]; j++) {
            double bias = static_cast<double>(std::rand()) / RAND_MAX;
            layer_biases.push_back(bias);
        }
        biases.push_back(layer_biases);
    }

};

void MultilayerPerceptron::forward_propagation(const vector<double>& input) {

    // Set the input layer to the first layer of the network (neurons[0])
    for (int i = 0; i < num_neurons[0]; i++) {
        neurons[0][i] = input[i];
    }

    // Iterate over the layers in the network
    for (int layer_index = 0; layer_index < num_layers - 1; ++layer_index) {

        Helper::multiply(weights[layer_index], neurons[layer_index], neurons[layer_index + 1]);

        // Add the bias to the neuron
        for (int i = 0; i < num_neurons[layer_index + 1]; i++) {
            neurons[layer_index + 1][i] += biases[layer_index][i];
        }

        // Apply the activation function to the output of the layer
        switch (activation_function) {
            case ActivationFunction::Sigmoid:
                for (int i = 0; i < num_neurons[layer_index + 1]; i++){
                    neurons[layer_index + 1][i] = 1 / (1 + exp(-neurons[layer_index + 1][i]));
                }
                break;
            
            case ActivationFunction::ReLU:
                for (int i = 0; i < num_neurons[layer_index + 1]; i++){
                    if (neurons[layer_index + 1][i] < 0)
                        neurons[layer_index + 1][i] = 0.01 * neurons[layer_index + 1][i];
                    else if (neurons[layer_index + 1][i] > 1)
                        neurons[layer_index + 1][i] = 1.0 + 0.01 * (neurons[layer_index + 1][i] - 1.0);
                }
                break;


        }
    }

};

int MultilayerPerceptron::prediction(){
    // Find the index of the maximum value in the output layer
    int max_index = 0;
    for (int i = 0; i < num_neurons[num_layers - 1]; i++) {
        if (neurons[num_layers - 1][i] > neurons[num_layers - 1][max_index])
            max_index = i;
    }
    return max_index;
};

void MultilayerPerceptron::print_predictions(){
    for (int i = 0; i < num_neurons[num_layers - 1]; i++) {
        cout << neurons[num_layers - 1][i] << " ";
    }
    cout << endl;
};

double MultilayerPerceptron::compute_derivative(double neuron_val){
    double new_value = 0.0;
    switch (activation_function) {
        case ActivationFunction::Sigmoid:
            new_value = neuron_val * (1 - neuron_val);
            break;
        case ActivationFunction::ReLU:
            new_value = neuron_val > 0 ? 1.0 : 0.01;
            break;
        default:
            cerr << "Activation function not supported" << std::endl;
            break;
    }
    return new_value;
};

void MultilayerPerceptron::backpropagation(int expect){

    // Compute the error of the output layer, using derivative of the activation function
    for (int i = 0; i < num_neurons[num_layers - 1]; i++) {

        double der = compute_derivative(neurons[num_layers - 1][i]);
        if (i != expect)
            errors[num_layers - 1][i] = -neurons[num_layers - 1][i] * der;
        else
            errors[num_layers - 1][i] = (1.0 - neurons[num_layers - 1][i]) * der;
    }

    // Compute the error of the hidden layers, using derivative of the activation function
    for (int layer_index = num_layers - 2; layer_index > 0; layer_index--) {

        Helper::multiply_transposed(weights[layer_index], errors[layer_index + 1], errors[layer_index]);
        
        for (int i = 0; i < num_neurons[layer_index]; i++) 
            errors[layer_index][i] *= compute_derivative(neurons[layer_index][i]);
    }

};

void MultilayerPerceptron::update_network(double learning_rate){

    // Update the weights
    for (int i = 0; i < num_layers - 1; i++) {
        for (int j = 0; j < num_neurons[i + 1]; j++) {
            for (int k = 0; k < num_neurons[i]; k++) 
                weights[i][j][k] += learning_rate * errors[i + 1][j] * neurons[i][k];
        }
    }

    // Update the biases
    for (int i = 0; i < num_layers - 1; i++) {
        for (int j = 0; j < num_neurons[i + 1]; j++) 
            biases[i][j] += learning_rate * errors[i + 1][j];
    }

};

vector<double> MultilayerPerceptron::normalize_input(vector<int>& input) {
    // Preprocess the input data by normalizing the values to the range 0-1
    vector<double> normalize_input;
    for (int i = 0; i < input.size(); i++)
        normalize_input.push_back(static_cast<double>(input[i]) / 255.0);

    return normalize_input;
};


void MultilayerPerceptron::print_weights(){
    for (int i = 0; i < num_layers - 1; i++) {
        cout << "Number of layer: " << i << endl;
        for (int j = 0; j < num_neurons[i]; j++) {
            cout << "Current neuron: " << j << endl;
            for (int k = 0; k < num_neurons[i + 1]; k++) 
                cout << weights[i][j][k] << " ";
            cout << endl;
        }
    }  
};

void MultilayerPerceptron::print_biases(){
    for (int i = 0; i < biases.size(); i++) {
        cout << "Layer: " << i << endl;
        for (int j = 0; j < biases[i].size(); j++) 
            cout << "Bias: " << biases[i][j] << endl;
        cout << endl;
    }
};

void MultilayerPerceptron::print_neurons(){
    for (int i = 0; i < neurons.size(); i++) {
        cout << "Layer: " << i << endl;
        for (int j = 0; j < neurons[i].size(); j++) 
            cout << "Neuron " << j << ": " << neurons[i][j] << endl;
        cout << endl;
    }
};

void MultilayerPerceptron::save_weights(const string &filename){
    ofstream file(filename);
    if (file.is_open()) {

        // Save weights
        for (int i = 0; i < num_layers - 1; i++) {
            for (int j = 0; j < num_neurons[i + 1]; j++) {
                for (int k = 0; k < num_neurons[i]; k++) 
                    file << weights[i][j][k] << " ";
                file << endl;
            }
        }

        // Save biases
        for (int i = 0; i < biases.size(); i++) {
            for (int j = 0; j < biases[i].size(); j++) 
                file << biases[i][j] << " ";
            file << endl;
        }
    }
    else {
        cout << "Unable to open file" << endl;
    }
    file.close();
};

void MultilayerPerceptron::read_weights(const string &filename){
    ifstream file(filename);
    if (file.is_open()) {

        // Load weights
        for (int i = 0; i < num_layers - 1; i++) {
            for (int j = 0; j < num_neurons[i + 1]; j++) {
                for (int k = 0; k < num_neurons[i]; k++) 
                    file >> weights[i][j][k];
            }
        }

        // Load biases
        for (int i = 0; i < biases.size(); i++) {
            for (int j = 0; j < biases[i].size(); j++) 
                file >> biases[i][j];
        }
    }
    else {
        cout << "Unable to open file" << endl;
    }
    file.close();
};
