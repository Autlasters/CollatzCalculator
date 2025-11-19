#pragma once
#include<QObject>
#include<vector>
#include<atomic>
#include<thread>
#include<climits>

typedef unsigned long long DULL;

class CollatzCalculator: public QObject {
    Q_OBJECT
private:
    const DULL maxSystemInt = (ULLONG_MAX - 1) / 3;
    int limitOfSet;
    int threadNumber;
    int maxThereadsNumber = std::thread::hardware_concurrency();
    int numberWithLongestChain = 0;
    int longestChain = 0;
    std::atomic<bool> overFlow;
    std::atomic<bool> stopReguest;
    std::atomic<bool> stopReguestCalled;
    std::vector<DULL> setOfNumber;
    std::vector<std::thread> threads;
    std::vector<int> threadLongestChain;
    std::vector<int> threadNumberWithLongestChain;
public:
    explicit CollatzCalculator(int limitOfSet, int threadNumber, QObject* parent = nullptr);
    int Collatz(DULL n);
    void vectorFill(std::vector<DULL>& vector, size_t start, size_t end);
    void CollatzComputations(const std::vector<DULL>& values, size_t start, size_t end, int& localLongestChain, int& localNumberWithLongestChain);
public slots:
    void start();
    void stop();
signals:
    void resultsComputed(int bestNumber, int bestChainLength, long long duration);
    void overFlowDetected();
    void stopComputations();
    void stopReguestCalledCheck();
};
