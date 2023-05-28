#include <fstream>
#include <vector>
#include <iostream>


class Helper {
    public:

    // Reads the MLP configuration file and store it into num_layers, num_neurons and activation_function variables
    static void read_mlp_configuration(const std::string& filename, int &num_layers, std::vector<int> &num_neurons, std::string &activation_function);

    // Reads MNIST file and fills arrays of inputs and labels
    static void read_mnist_file(const std::string& filename, std::vector<std::vector<int> >& inputs, std::vector<int>& labels);

    // Print a number for the given array of pixels
    static void print_pixels(std::vector<double>& input);

    // Multiply matrix with vector, used for forward propagation
    static void multiply(const std::vector<std::vector<double> >& matrix1, const std::vector<double>& vec2, std::vector<double>& result);

    // Multiply transposed matrix with vector, used for backpropagation
    static void multiply_transposed(const std::vector<std::vector<double> >& matrix1, const std::vector<double>& vec2, std::vector<double>& result);

    // Center the digit in the image
    static std::vector<double> centerDigit(const std::vector<double>& input, int width, int height, double threshold);

};