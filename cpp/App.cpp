#include "../header/App.h"
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <fstream>
#include <sstream>
#include <queue>
#include <stdexcept>
#include <set>
#include <iostream>

App::App(QWidget *parent) : QWidget(parent), logger("../logs/app_log.txt") {
    setWindowTitle("Graph Layer Numbering");
    resize(200, 200);

    QGridLayout *gridLayout = new QGridLayout(this);

    plainTextEdit = new QPlainTextEdit(this);
    plainTextEdit->setReadOnly(true);
    QPushButton *button = new QPushButton("Select Graph File", this);

    gridLayout->addWidget(plainTextEdit, 0, 0, 1, 2);
    gridLayout->addWidget(button, 1, 0, 1, 2);

    connect(button, &QPushButton::clicked, this, &App::processFiles);
}

void App::readGraph(const std::string &filename, std::map<int, std::vector<int>> &graph) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open graph file: " << filename << std::endl;
        throw std::runtime_error("Unable to open graph file");
    }

    int vertex;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream lineStream(line);
        lineStream >> vertex;
        int adjacent;
        while (lineStream >> adjacent) {
            graph[vertex].push_back(adjacent);
            graph[adjacent].push_back(vertex);
        }
    }
}

void App::numberLayers(int startVertex, const std::map<int, std::vector<int>> &graph, std::map<int, int> &layers) {
    if (graph.find(startVertex) == graph.end()) {
        throw std::runtime_error("Start vertex not found in graph");
    }

    std::queue<int> q;
    std::set<int> visited;

    q.push(startVertex);
    layers[startVertex] = 0;
    visited.insert(startVertex);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int neighbor : graph.at(current)) {
            if (visited.find(neighbor) == visited.end()) {
                layers[neighbor] = layers[current] + 1;
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
}

void App::saveLayers(const std::string &filename, const std::map<int, int> &layers) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open output file: " << filename << std::endl;
        throw std::runtime_error("Unable to open output file");
    }

    std::map<int, std::vector<int>> layersByDepth;
    for (const auto &entry : layers) {
        layersByDepth[entry.second].push_back(entry.first);
    }

    for (const auto &entry : layersByDepth) {
        outputFile << "Layer " << entry.first << ": ";
        for (int vertex : entry.second) {
            outputFile << vertex << " ";
        }
        outputFile << std::endl;
    }
}

void App::processFiles() {
    try {
        QString graphFileName = QFileDialog::getOpenFileName(this, "Select Graph File", "../files/",
                                                             "Text Files (*.txt);");
        if (graphFileName.isEmpty()) {
            logger.logMessage("No graph file selected", LogLevel::WARNING);
            return;
        }

        std::cout << "Selected graph file: " << graphFileName.toStdString() << std::endl;

        bool ok;
        int startVertex = QInputDialog::getInt(this, "Input Start Vertex", "Start Vertex:", 0, 0, 10000, 1, &ok);
        if (!ok) {
            logger.logMessage("No start vertex provided", LogLevel::WARNING);
            return;
        }

        std::cout << "Start vertex: " << startVertex << std::endl;

        std::map<int, std::vector<int>> graph;
        readGraph(graphFileName.toStdString(), graph);

        std::map<int, int> layers;
        numberLayers(startVertex, graph, layers);

        QString outputFileName = QFileDialog::getSaveFileName(this, "Save Output File", "../files/",
                                                              "Text Files (*.txt);");
        if (outputFileName.isEmpty()) {
            logger.logMessage("No output file selected", LogLevel::WARNING);
            return;
        }

        std::cout << "Output file: " << outputFileName.toStdString() << std::endl;

        saveLayers(outputFileName.toStdString(), layers);

        std::ostringstream outputContent;
        std::map<int, std::vector<int>> layersByDepth;
        for (const auto &entry : layers) {
            layersByDepth[entry.second].push_back(entry.first);
        }

        for (const auto &entry : layersByDepth) {
            outputContent << "Layer " << entry.first << ": ";
            for (int vertex : entry.second) {
                outputContent << vertex << " ";
            }
            outputContent << std::endl;
        }

        plainTextEdit->setPlainText(QString::fromStdString(outputContent.str()));
        logger.logMessage("Processing completed successfully", LogLevel::INFO);
        QMessageBox::information(this, "Success", "Processing completed successfully");
    } catch (const std::exception &e) {
        logger.logMessage(e.what(), LogLevel::ERROR);
        QMessageBox::critical(this, "Error", e.what());
    }
}
