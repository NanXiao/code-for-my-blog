#include <sys/lwp.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

void output_thread_id(const std::string& prefix)
{
    std::stringstream ss;
    ss <<"Process ID is " << getpid() <<
        ", lwp ID is " << lwp_gettid() <<
        ", " << prefix << std::this_thread::get_id() <<'\n';
    std::cout << ss.str();
}

void thread_fun(int64_t duration)
{
    output_thread_id("sub thread ID is ");
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(duration));
    }
}

int main()
{
    output_thread_id("main thread ID is ");

    std::thread t1{thread_fun, 1};
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::thread t2{thread_fun, 2};

    t1.join();
    t2.join();
    return 0;
}