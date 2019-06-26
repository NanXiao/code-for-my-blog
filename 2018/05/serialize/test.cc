#include <fstream>

int main()
{
        int c = 3;
        std::ofstream f("a.txt");
        f.write(reinterpret_cast<char*>(&c), sizeof(c));

        return 0;
}
