#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MultilayerPerceptron.h"
#include "drawingboard.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void onClearButtonClicked();
        void onPredictButtonClicked();
        void onModelChanged(int index);
        void onTrainButtonClicked();


    private:
        std::vector<std::shared_ptr<MultilayerPerceptron>> mlp_models;
        int current_model_index;
        void trainModel(const QString& modelName, const QString& hiddenLayers, const QString& activFunc);

        DrawingBoard *drawingBoard;
        QVBoxLayout *centralLayout;
        QVBoxLayout *leftLayout;

        QPushButton *clearButton;
        QPushButton *predictButton;
        QLabel *resultLabel;
        QComboBox *modelSelector;

        QVBoxLayout *trainModelLayout;
        QLineEdit *modelNameLineEdit;
        QLineEdit *hiddenLayersLineEdit;
        QLineEdit *activFuncLineEdit;
        QPushButton *trainButton;
        QLabel *inputValidationLabel;
        QLabel *trainingMessageLabel;
};
#endif // MAINWINDOW_H
