#include "graphGen.h"

using namespace graphGen;
using namespace std;

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);

    int n, t, c; cin >> n >> t >> c;
    // 25 12 5
    Graph::cactus(n, t, c).ps_style_print();
    // Graph::tree(12).csacademy_style_print();
}
