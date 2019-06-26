#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

void inner(int i, ZZX& t, Vec<ZZX>& phi)
{
        for (long j = 1; j <= i-1; j++)
         if (i % j == 0)
            t *= phi(j);
}

void outer(int i, Vec<ZZX>& phi)
{
        ZZX t;
        t = 1;
        inner(i, t, phi);
        phi(i) = (ZZX(INIT_MONO, i) - 1)/t;
        cout << phi(i) << "\n";
}

int main()
{
   Vec<ZZX> phi(INIT_SIZE, 100);

   for (long i = 1; i <= phi.length(); i++) {
      outer(i, phi);
   }
}
