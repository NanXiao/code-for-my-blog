#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string FILE_NAME = "number.txt";
const std::string RESULT_FILE_NAME = "result.txt";

char chunk[1048576];

int main(void)
{
    std::ifstream ifs(FILE_NAME, std::ios_base::binary);
    if (!ifs) {
        std::cerr << "Error opeing " << FILE_NAME << std::endl;
        exit(1);
    }
    std::ofstream ofs(RESULT_FILE_NAME, std::ios_base::binary);
    if (!ofs) {
        std::cerr << "Error opeing " << RESULT_FILE_NAME << std::endl;
        exit(1);
    }

    std::vector<std::chrono::milliseconds> duration_vec(5);
    for (std::vector<std::chrono::milliseconds>::size_type i = 0; i < duration_vec.size(); i++) {
        unsigned long long res = 0;
        ifs.seekg(0);
        auto begin = std::chrono::system_clock::now();

        for (size_t j = 0; j < 4096; j++) {
            ifs.read(chunk, sizeof(chunk));
            for (size_t k = 0; k < sizeof(chunk); k++) {
                res += chunk[k];
            }
        }
        ofs << res;

        duration_vec[i] = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin);
        std::cout<< duration_vec[i].count() << std::endl;
    }

    std::chrono::milliseconds total_time{0};
    for (auto const& v : duration_vec) {
        total_time += v;
    }
    std::cout << "Average exec time: " << total_time.count() / duration_vec.size() << std::endl;
    return 0;
}
