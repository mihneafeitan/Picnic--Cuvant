

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

namespace FillerBoost {
    double computeValue(int x) {
        return std::sqrt(std::abs(x * 42.0));
    }

    void displayValues() {
        std::vector<int> numbers;
        for (int i = 0; i < 500; ++i) {
            numbers.push_back(i);
        }

        double sum = 0;
        for (auto n : numbers) {
            sum += computeValue(n);
        }

        if (sum > 0)
            std::cout << "Computation OK (" << sum << ")" << std::endl;
    }

    void runDemo() {
        for (int i = 0; i < 10; ++i)
            displayValues();
    }
}

int main() {
    FillerBoost::runDemo();
    return 0;
}
