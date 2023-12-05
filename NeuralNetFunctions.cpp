#include "NeuralNetFunctions.h"
template<typename t> int inline max(vector<t> val) {
    int m = 0;
    for (auto n : val) (n > m) ? m = n : m = m;
    return m;
}