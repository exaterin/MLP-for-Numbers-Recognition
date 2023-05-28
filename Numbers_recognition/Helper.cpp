#include "Helper.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>


using namespace std;

void Helper::read_mlp_configuration(const string& filename, int &num_layers, vector<int> &num_neurons, string &activation_function) {
  ifstream config_file(filename);

  if (config_file.is_open()) {
    string line;

    // Read the number of layers
    getline(config_file, line);
    num_layers = stoi(line);

    // Read the number of neurons in each layer
    getline(config_file, line);
    istringstream neurons_stream(line);
    num_neurons.clear();
    int neuron_count;
    while (neurons_stream >> neuron_count) {
      num_neurons.push_back(neuron_count);
    }

    // Read the activation function
    getline(config_file, line);
    activation_function = line;

    config_file.close();
    
  } else {
    cout << "Unable to open." << endl;
  }
}

void Helper::read_mnist_file (const string& filename, vector<vector<int> >& inputs, vector<int>& labels) {
    // Open the input file
    ifstream input_file(filename);

    // Read each line in the file
    string line;
    while (getline(input_file, line)) {
        // Split the line into a label value and a string of pixel values
        stringstream line_stream(line);
        string label_str, pixel_str;
        getline(line_stream, label_str, ',');
        getline(line_stream, pixel_str);


        // Convert the label value to an integer and add it to the training set labels vector
        int label = stoi(label_str);
        labels.push_back(label);

        // Split the string of pixel values into individual pixel values
        stringstream pixel_stream(pixel_str);
        string pixel;
        vector<int> input;

        while (getline(pixel_stream, pixel, ',')) {
            // Convert the pixel value to an integer and add it to the training set inputs vector
            int pixel_val = std::stoi(pixel);
            input.push_back(pixel_val);
        }
        inputs.push_back(input);
    }
}

void Helper::print_pixels(vector<double>& input){

  int count = 0;
    cout << "________________________________________________________" << endl;
    // Iterate through pixels
    for (auto& pixel : input){
        ++count;
        if(pixel != 0)
          cout << "%" << " ";
        else
          cout << " " << " ";

        if(count == 28){
          cout << endl;
          count = 0;
        }
    }
    cout << "________________________________________________________" << endl;

}

void Helper::multiply(const vector<vector<double> >& matrix1, const vector<double>& vec2, vector<double>& result) {
    int rows = matrix1.size();
    int cols = matrix1[0].size();

    if (cols != vec2.size()) {
        cout << "Error: Matrix dimensions do not match" << endl;
        cout << "Matrix: " << rows << "x" << cols << endl;
        cout << "Vector: " << vec2.size() << endl;
        return;
    }

    result.resize(rows);

    for (int i = 0; i < rows; i++) {
        double sum = 0;
        for (int j = 0; j < cols; j++) {
            sum += matrix1[i][j] * vec2[j];
        }
        result[i] = sum;
    }
}

void Helper::multiply_transposed(const vector<vector<double> >& matrix1, const vector<double>& vec2, vector<double>& result) {
    int rows = matrix1.size();
    int cols = matrix1[0].size();

    if (rows != vec2.size()) {
        cout << "Error: Matrix dimensions do not match" << endl;
        cout << "Matrix: " << rows << "x" << cols << endl;
        cout << "Vector: " << vec2.size() << endl;
        return;
    }

    result.resize(cols);

    for (int i = 0; i < cols; i++) {
        double sum = 0;
        for (int j = 0; j < rows; j++) {
            sum += matrix1[j][i] * vec2[j];
        }
        result[i] = sum;
    }
}

std::vector<double> Helper::centerDigit(const std::vector<double>& input, int width, int height, double threshold){
    // Reshape the input to a 2D matrix
    std::vector<std::vector<double> > img(height, std::vector<double>(width));
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            img[i][j] = input[i * width + j];

    // Calculate the centroid of the digit
    double sumX = 0, sumY = 0;
    int count = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (img[i][j] > threshold) {
                sumX += j;
                sumY += i;
                ++count;
            }
        }
    }

    int centroidX = static_cast<int>(sumX / count);
    int centroidY = static_cast<int>(sumY / count);

    // Create a new image where the digit is translated such that the centroid is at the center
    std::vector<std::vector<double> > newImg(height, std::vector<double>(width, 0));
    int offsetX = width / 2 - centroidX;
    int offsetY = height / 2 - centroidY;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i - offsetY >= 0 && i - offsetY < height && j - offsetX >= 0 && j - offsetX < width) {
                newImg[i][j] = img[i - offsetY][j - offsetX];
            }
        }
    }

    // Flatten the new image back to a 1D vector
    std::vector<double> output(height * width);
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            output[i * width + j] = newImg[i][j];

    return output;
}
