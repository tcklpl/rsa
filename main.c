#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define ll long long

/// Método de MDC por algorítmo de Euclides
/// \param a primeiro valor
/// \param b segundo valor
/// \return MDC
ll mdc(ll a, ll b) {
    if (a < b)
        return mdc(b, a);
    else if (a % b == 0)
        return b;
    else return mdc(b, a % b);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
ll mod(ll a, unsigned ll n, ll p) {
    ll res = 1;
    a %= p;

    while (n > 0) {
        // se n é impar, multiplicar 'a' com o resultado
        if (n % 2 == 1)
            res = (res * a) % p;

        // n deve ser par agora
        n = n >> 1; // n/2
        a = (a * a) % p;
    }

    return res;
}
#pragma clang diagnostic pop

int primo(ll n, int iterations) {
    ll teste;
    // casos básicos
    if (n <= 1 || n == 4) return 0;
    if (n <= 3) return 1;
    if ((n % 2) == 0) return 0;

    // srandom e random em vez de srand e rand
    srandom(time(NULL));

    while (iterations > 0) {
        teste = 2 + random()%(n-4);

        // checa se os números são coprimos
        if (mdc(n, teste) != 1)
            return 0;

        // teorema de Fermat
        if (mod(teste, n-1, n) != 1)
            return 0;

        iterations--;
    }
    return 1;
}

int main() {

    long long teste;
    srandom(time(NULL));
    do {
        teste = random();
    } while (!primo(teste, 5));
    printf("%lld\n", teste);
    return 0;
}