#include <string>

bool isPalindrome(
    std::string& s,
    std::string::size_type start,
    std::string::size_type end) {

        auto count = (end - start + 1) / 2;
        for (std::string::size_type i = 0; i < count; i++) {
            if (s.at(start) != s.at(end)) {
                return false;
            }
            start++;
            end--;
        }

        return true;
}

int main() {
        std::string s(1'000'000'000, 'a');

        isPalindrome(s, 0, s.size() - 1);
        return 0;
}
