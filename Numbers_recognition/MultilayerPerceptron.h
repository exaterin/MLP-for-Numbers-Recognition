#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>


enum class ActivationFunction {
    Sigmoid,
    ReLU
};

class MultilayerPerceptron {
    public:
        // Define the structure of the multi-layer perceptron
        int num_layers;
        std::vector<int> num_neurons;
        
        ActivationFunction activation_function;

        std::vector<double> input;
        // std::vector<double> output;

        // Store the values of the neurons
        std::vector<std::vector<double> > neurons;

        // Store the errors of the neurons
        std::vector<std::vector<double> > errors;

        /*
        The first dimension represents the layers in the network.
        The second dimension represents the neurons in each layer. 
        the third dimension represents the connections between the neurons.
        */
        std::vector<std::vector<std::vector<double> > > weights;

        /*
        The first dimension represents the layers in the network.
        The second dimension represents the neurons in each layer. 
        */
        std::vector<std::vector<double> > biases;

        // Constructor 
        MultilayerPerceptron(int& num_layers, std::vector<int>& num_neurons, std::string activation_function);


        /*
        Forward propagation

        Input: input layer, output: vector of probabilities for each number

        Compute the weighted sum of the inputs at each layer
        Apply the activation function
        Pass the result to the next layer
        */
        void forward_propagation(const std::vector<double>& input);

        // Make a prediction: return the index of the maximum value in the output layer
        int prediction();

        // Print the predictions
        void print_predictions();

        // Normalize the input
        std::vector<double> normalize_input(std::vector<int>& input);

        // Compute the derivative of the activation function
        double compute_derivative(double neuron_val);

        // Backpropagation algorithm
        void backpropagation(int expected);
        // Compute the gradient of the loss function with respect to the weights and biases
        // Update the weights and biases using a gradient descent algorithm

        // Update the weights and biases
        void update_network(double learning_rate);

        // Read the weights and biases from a file
        void read_weights(const std::string &filename);

        // Save the weights and biases to a file
        void save_weights(const std::string &filename);


        // // Train the multi-layer perceptron
        // void train(std::vector<std::vector<double>> input, std::vector<std::vector<double>> expected, double learning_rate) {
        // // Iterate over the training set
        // // Use the forward propagation and backpropagation algorithms to make predictions and update the weights and biases
        // }

        void print_weights();

        void print_biases();

        void print_neurons();
};