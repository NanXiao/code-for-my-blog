#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: show_thread_ids pid\n";
        return 1;
    }

    try
    {
        std::filesystem::path p{"/proc"};
        p /= argv[1];
        p /= "task";

        if (!std::filesystem::is_directory(p))
        {
            std::cerr << "Process ID (" << argv[1] << ") doesn't exist\n";
            return 1;
        }

        uint64_t thread_num{};
        std::vector<std::string> thread_id;

        std::filesystem::directory_iterator d_it(p);
        for (const auto& it : d_it)
        {
            thread_num++;
            thread_id.push_back(it.path().filename().string());
        }

        std::cout << "Process ID (" << argv[1] << ") has " << thread_num << " threads, and ids are:\n";
        for (const auto& v : thread_id)
        {
            std::cout << v << '\n';
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}