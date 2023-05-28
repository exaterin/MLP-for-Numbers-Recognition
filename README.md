# Numbers Recognition Application

A Qt-based application that uses a Multi-Layer Perceptron (MLP) model to predict handwritten numbers drawn by the user.

## Model 

Multi-layer perceptron was trained on the MNIST dataset, training dataset contains 60000 examples, test dataset - 10000.
Wights for the MLP are stored in `weights.txt`.

The best achieved accuracy is 90.8%, with the configuration stored in `Configuration.txt`. The neural network has 3 layers (784 x 64 x 10), activation function is Leaky ReLu.


## Features

- Draw a handwritten number using a simple drawing interface.
- Predict the drawn number using a trained MLP model.
- Clear the drawing area to try different numbers.
- High accuracy in recognizing handwritten digits. 
- Accuracy: 90.8%

## Requirements

- Supported Operating Systems: Windows, possibility to release for macOS or Linux.
- Qt 6.5.0 or higher.
- C++ compiler compatible with C++11 or higher.

## Usage

1. Launch the Number Recognition application by double-clicking the executable file (`build-Numbers_recognition-Desktop_Qt_6_5_0_MinGW_64_bit-Release / Numbers_recognition.exe`).
2. Use your mouse or stylus to draw a handwritten number in the drawing area.
3. Click the "Predict" button to let the MLP model predict the drawn number. The predicted number will be displayed on the screen.
4. To draw a new number, click the "Clear" button to erase the drawing area and start again.



