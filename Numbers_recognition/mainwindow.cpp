#include "mainwindow.h"
#include "Helper.h"

#include <QCoreApplication>
#include <QDir>


#include <fstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    // Create the left layout
    leftLayout = new QVBoxLayout();

    // Choose model components
    QHBoxLayout *chooseModelLayout = new QHBoxLayout();
    QLabel *modelLabel = new QLabel("Choose the model:", this);
    chooseModelLayout->addWidget(modelLabel);

    modelSelector = new QComboBox(this);
    chooseModelLayout->addWidget(modelSelector);
    connect(modelSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onModelChanged(int)));

    leftLayout->addLayout(chooseModelLayout);

    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    leftLayout->addWidget(line1);
    vector<string> mlp_files = {"Configuration.txt", "Configuration.txt"};
    vector<string> weights_files = {"weights.txt", "weights.txt"};
    vector <string> models_names = {"Model1", "Model2"};

    for(int i=0; i < mlp_files.size(); ++i){
        // Create an instance of MLP
        int num_layers;
        vector<int> num_neurons;
        string activation_function;

        Helper::read_mlp_configuration(mlp_files[i], num_layers, num_neurons, activation_function);
        std::shared_ptr<MultilayerPerceptron> mlp = std::make_shared<MultilayerPerceptron>(num_layers, num_neurons, activation_function);

        mlp->read_weights(weights_files[i]);
        mlp_models.push_back(mlp);

        modelSelector->addItem(QString::fromStdString(models_names[i]));

    }


    // Create the right layout
    centralLayout = new QVBoxLayout();
    drawingBoard = new DrawingBoard(this);
    centralLayout->addWidget(drawingBoard);

    // Clear board button
    clearButton = new QPushButton("Clear", this);
    centralLayout->addWidget(clearButton);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);

    predictButton = new QPushButton("Predict", this);
    centralLayout->addWidget(predictButton);
    connect(predictButton, &QPushButton::clicked, this, &MainWindow::onPredictButtonClicked);

    resultLabel = new QLabel("Prediction: ", this);
    centralLayout->addWidget(resultLabel);

    // Add a new model components
    QLabel *addModelLabel = new QLabel("Add a new model", this);
    leftLayout->addWidget(addModelLabel);

    QHBoxLayout *modelNameLayout = new QHBoxLayout();
    QLabel *modelNameLabel = new QLabel("Model name:", this);
    modelNameLayout->addWidget(modelNameLabel);
    modelNameLineEdit = new QLineEdit(this);
    modelNameLayout->addWidget(modelNameLineEdit);
    leftLayout->addLayout(modelNameLayout);

    QHBoxLayout *hiddenLayersLayout = new QHBoxLayout();
    QLabel *hiddenLayersLabel = new QLabel("Hidden layers:", this);
    hiddenLayersLayout->addWidget(hiddenLayersLabel);
    hiddenLayersLineEdit = new QLineEdit(this);
    hiddenLayersLayout->addWidget(hiddenLayersLineEdit);
    leftLayout->addLayout(hiddenLayersLayout);

    QHBoxLayout *activFuncLayout = new QHBoxLayout();
    QLabel *activFuncLabel = new QLabel("Activation function:", this);
    activFuncLayout->addWidget(activFuncLabel);
    activFuncLineEdit = new QLineEdit(this);
    activFuncLayout->addWidget(activFuncLineEdit);
    leftLayout->addLayout(activFuncLayout);

    // Message for incorrect input
    inputValidationLabel = new QLabel(this);
    inputValidationLabel->setStyleSheet("QLabel { color : red; }");
    leftLayout->addWidget(inputValidationLabel);

    // Train button
    trainButton = new QPushButton("Train", this);
    leftLayout->addWidget(trainButton);

    connect(trainButton, &QPushButton::clicked, this, &MainWindow::onTrainButtonClicked);

    // Message for training process
    trainingMessageLabel = new QLabel(this);
    trainingMessageLabel->setStyleSheet("QLabel { color : green; }");
    leftLayout->addWidget(trainingMessageLabel);


    // Add the two layouts to the main layout
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(centralLayout);

}

MainWindow::~MainWindow(){
}

void MainWindow::onClearButtonClicked(){
    drawingBoard->clearBoard();
}

void MainWindow::onPredictButtonClicked(){

    QImage scaledImage = drawingBoard->getScaledImage();
    vector<int> input(784);

    // Convert the QImage to vector<int> with values in range 0-255
    for (int i = 0; i < 28; ++i){
        for (int j = 0; j < 28; ++j){
            int pixelValue = qGray(scaledImage.pixel(j, i));
            if (pixelValue == 255)
                pixelValue = 0;
            input[i * 28 + j] = pixelValue;
        }
    }

    vector <double> normalized_input = mlp_models[current_model_index]->normalize_input(input);

//    vector<double> centered = Helper::centerDigit(normalized_input, 28, 28, 0.5);

//    Helper::print_pixels(centered);

    mlp_models[current_model_index]->forward_propagation(normalized_input);
    int prediction = mlp_models[current_model_index]->prediction();

    resultLabel->setText(QString("Prediction: %1").arg(prediction));
}

void MainWindow::onModelChanged(int index) {
    current_model_index = index;
}

void MainWindow::onTrainButtonClicked() {
    QString modelName = modelNameLineEdit->text();
    QString hiddenLayers = hiddenLayersLineEdit->text();
    QString activFunc = activFuncLineEdit->text();

    if (modelName.isEmpty() || hiddenLayers.isEmpty() || activFunc.isEmpty()) {
        inputValidationLabel->setText("All fields must be filled out!");
        return;
    }

    QStringList hiddenLayerValues = hiddenLayers.split(" ");

    // Check if each hidden layer value is in range (0, 1000)
    for (const QString &value : hiddenLayerValues) {
        bool ok;
        int num = value.toInt(&ok);
        if (!ok || num <= 0 || num > 1000) {
            inputValidationLabel->setText("Incorrect input");
            return;
        }
    }

    // Check if activation function is correct
    if (activFunc != "relu" && activFunc != "sigmoid"){
        inputValidationLabel->setText("Activation function must be relu or sigmoid");
        return;
    }

    inputValidationLabel->setStyleSheet("QLabel { color : green; }");
    inputValidationLabel->setText("OK");
    QCoreApplication::processEvents();

    trainModel(modelName, hiddenLayers, activFunc);
}

void MainWindow::trainModel(const QString& modelName, const QString& hiddenLayers, const QString& activFunc) {

    // Define the input and label vectors for training and testing sets
    vector<vector<int> > training_set_inputs;
    vector<int> training_set_labels;

    // Load the MNIST dataset from the input file and fill the input and label vectors for training and test sets
    trainingMessageLabel->setText("Loading MNIST dataset...");
    QCoreApplication::processEvents();
    Helper::read_mnist_file("MNIST_train.txt", training_set_inputs, training_set_labels);

    // Configuration of MLP
    // Convert inputs to the correct types
    QStringList hiddenLayersList = hiddenLayers.split(" ");
    // Add input and ouput layer
    hiddenLayersList.prepend("784");
    hiddenLayersList.append("10");
    int num_layers = hiddenLayersList.size();
    vector<int> num_neurons;


    for (const QString& layer : hiddenLayersList) {
        bool ok;
        int num = layer.toInt(&ok);
        num_neurons.push_back(num);
    }

    string activation_function = activFunc.toStdString();

    int number_of_epochs = 20;
    int number_of_examples = training_set_inputs.size();
    double initial_learning_rate = 0.1;
    double learning_rate = initial_learning_rate;
    int step_size = 5; // Number of epochs after which the learning rate will be reduced
    double decay_rate = 0.9; // Factor by which the learning rate will be reduced

    // Create an instance of the multi-layer perceptron
    MultilayerPerceptron mlp = MultilayerPerceptron(num_layers, num_neurons, activation_function);

    vector<double> input = mlp.normalize_input(training_set_inputs[0]);

    trainingMessageLabel->setText("Training...");
    QCoreApplication::processEvents();

    // Train the MLP
    for (int epoch = 0; epoch < number_of_epochs; epoch++){
        int correct = 0;

        // Update learning rate based on step decay
        if ((epoch + 1) % step_size == 0)
            learning_rate *= decay_rate;

        for (int i = 0; i < number_of_examples; i++){

            vector<double> input = mlp.normalize_input(training_set_inputs[i]);
            mlp.forward_propagation(input);

            int predict = mlp.prediction();

            if (predict != training_set_labels[i]){

                mlp.backpropagation(training_set_labels[i]);
                mlp.update_network(learning_rate);
            }

            else
                correct++;

        }

        cout << "Epoch: " << epoch + 1 << "\n" << "Accuracy: " << (correct / static_cast<double>(number_of_examples) * 100) << "%" << endl;
        QString message = QString("Epoch: %1\n Accuracy: %2%").arg(epoch + 1).arg((correct / static_cast<double>(number_of_examples) * 100));
        trainingMessageLabel->setText(message);
        QCoreApplication::processEvents();
    }

    trainingMessageLabel->setText("Training complete");

    // Save the weights and biases
    string weights_file_name = "weights_" + modelName.toStdString() + ".txt";
    mlp.save_weights(weights_file_name);

    trainingMessageLabel->setText(QString("Model is saved into\n%1").arg(QString::fromStdString(weights_file_name)));

    // Add the new model to the modelSelector
    modelSelector->addItem(modelName);

    std::shared_ptr<MultilayerPerceptron> mlp_ptr = std::make_shared<MultilayerPerceptron>(mlp);
    mlp_models.push_back(mlp_ptr);

}

