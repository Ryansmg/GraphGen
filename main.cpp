#include "graphGen.hpp"

namespace settings {
    bool file_output = false;
}
void registerSettings() {
    using namespace settings;
    file_output = opt<bool>("fo");
    if(file_output) freopen(R"(C:\Users\ryans\OneDrive\Desktop\Projects\GraphGen\out.txt)", "w", stdout);
}

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);


    Tree(Graph::cactus(15, 6, 3)).ps_style_print(true);
}
