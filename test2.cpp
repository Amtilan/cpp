#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> myVector = {1, 2, 3, 4, 5};

    // Переместить элементы влево на 2 позиции

    std::rotate(myVector.begin(), myVector.begin() + (112 % myVector.size()), myVector.end());

    // Вывести результат
    for (const auto& element : myVector) {
        std::cout << element << " ";
    }

    return 0;
}
