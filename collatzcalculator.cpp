#include "collatzcalculator.h"
#include <chrono>
#include <thread>
#include <vector>

CollatzCalculator::CollatzCalculator(int limitOfSet, int threadNumber, QObject *parent): QObject(parent),
                                                                        limitOfSet(limitOfSet),
                                                                        threadNumber(threadNumber),
                                                                        overFlow(false),
                                                                        stopReguest(false),
                                                                        stopReguestCalled(false),
                                                                        setOfNumber(limitOfSet),
                                                                        threadLongestChain(threadNumber, 0),
                                                                        threadNumberWithLongestChain(threadNumber, 0)
{}

void CollatzCalculator::vectorFill(std::vector<DULL>& vector, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        vector[i] = i + 1;
    }
}


int CollatzCalculator::Collatz(DULL n) {
    int chainLength = 1;

    while (n != 1) {
        if(stopReguest.load()){
            if (!stopReguestCalled.exchange(true)) {
                emit stopRegustCalled();
            }
            return 0;
        }
        if (n > maxSystemInt) {
            overFlow.store(true);
            return -1;
        }
        if (overFlow.load()) {
            return 0;
        }

        if (n % 2 == 0)
            n /= 2;
        else
            n = 3 * n + 1;

        chainLength++;
    }
    return chainLength;
}


void CollatzCalculator::CollatzComputations(const std::vector<DULL>& values, size_t start, size_t end, int& localLongestChain, int& localNumberWithLongestChain) {


    localLongestChain = 0;
    localNumberWithLongestChain = 0;

    for (size_t i = start; i < end; ++i) {
        int chainLength = Collatz(values[i]);
        if (chainLength == -1) {
            overFlow.store(true);
            return;
        }

        if (chainLength > localLongestChain) {
            localLongestChain = chainLength;
            localNumberWithLongestChain = values[i];
        }
    }
}

void CollatzCalculator::start(){
    auto startTime = std::chrono::high_resolution_clock::now();
    stopReguest.store(false);
    stopReguestCalled.store(false);

    size_t partOfSet = setOfNumber.size() / threadNumber;
    for (size_t i = 0; i < threadNumber; ++i) {
        int leftLimit = i * partOfSet;
        int rightLimit = 0;
        if (i != threadNumber - 1) {
            rightLimit = (i + 1) * partOfSet;
        }
        else {
            rightLimit = limitOfSet;
        }

        threads.emplace_back([this, leftLimit, rightLimit]() {this->vectorFill(this->setOfNumber, leftLimit, rightLimit); });
    }

    for (auto& t : threads) {
        t.join();
    }

    threads.clear();

    for (size_t i = 0; i < threadNumber; ++i) {
        int leftLimit = i * partOfSet;
        int rightLimit = 0;
        if (i != threadNumber - 1) {
            rightLimit = (i + 1) * partOfSet;
        }
        else {
            rightLimit = limitOfSet;
        }

        threads.emplace_back([this, leftLimit, rightLimit, i]() {this->CollatzComputations(this->setOfNumber, leftLimit, rightLimit, this->threadLongestChain[i], this->threadNumberWithLongestChain[i]); });
    }

    for (auto& t : threads) {
        t.join();
    }

    if (overFlow.load()) {
        emit overFlowDetected();
        return;
    }

    if(!stopReguest.load() && !overFlow.load()){
        longestChain = 0;
        numberWithLongestChain = 0;
        for (size_t i = 0; i < threadNumber; ++i) {
            if (threadLongestChain[i] > longestChain) {
                longestChain = threadLongestChain[i];
                numberWithLongestChain = threadNumberWithLongestChain[i];
            }
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        emit resultsComputed(numberWithLongestChain, longestChain, duration.count());
    }
    emit stopComputations();
}


void CollatzCalculator::stop(){
    stopReguest.store(true);
}




