#ifndef APP_H
#define APP_H

#include <QWidget>
#include <QPlainTextEdit>
#include <vector>
#include <map>
#include "Logger.h"

class App : public QWidget {
Q_OBJECT

public:
    explicit App(QWidget *parent = nullptr);

private slots:
    void processFiles();

private:
    QPlainTextEdit *plainTextEdit;
    Logger logger;

    void readGraph(const std::string &filename, std::map<int, std::vector<int>> &graph);
    void numberLayers(int startVertex, const std::map<int, std::vector<int>> &graph, std::map<int, int> &layers);
    void saveLayers(const std::string &filename, const std::map<int, int> &layers);
};

#endif // APP_H
