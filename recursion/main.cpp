#include <iostream>

int nzd(int x, int y) {
    if( !y) return x;
    return nzd(y, x % y);
}

int fib2_0(int n, int pretprosli = 0, int prosli = 1) {
    if( n == 0) return pretprosli;
    if( n == 1) return prosli;
    return fib2_0(n-1, prosli, pretprosli + prosli);
}

//testovi

void testNZD1() {
    std:: cout << nzd(13, 37) << " " << nzd(15, 20) << " " << nzd(80, 160) << std::endl;
}
void testFib2_0() {
    for(int i = 0; i <= 30; i++)
        std:: cout << fib2_0(i) << " ";
}


int main() {

    testNZD1();
    testFib2_0();
    return 0;
}
