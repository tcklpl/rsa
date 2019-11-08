#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
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
/// Divisão modular
/// \param a Valor base
/// \param n Expoente
/// \param p Divisor
/// \return A^n % p
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
/// Checa a primalidade de um número
/// \param n O número a ser checado
/// \param iterations O número de iterações a serem checadas
/// \return 1 caso for primo e 0 caso não for
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

void precodificar(char *in, char *out) {
    unsigned ll i, c;
    unsigned ll tamanho = strlen(in);
    out[0] = '\0';
    for (i = 0; i < tamanho; i++) {
        c = in[i];
        sprintf(out, "%lld", c);
    }
    sprintf(out, "\0");
    puts(out);
}

int main() {

    // 'p' e 'q' são números primos escoolhidos aleatoriamente
    // 'n' é pq
    // 'phi' é (p - 1)(q - 1)
    // 'e' é um número tal que (2 < e < phi) e 'e' e 'phi' são co-primos
    ll p, q, n, phi, e;
    srandom(time(NULL));
    printf("Gerando p...\n");
    do {
        p = random();
    } while (!primo(p, 2));
    printf("Gerando q...\n");
    do {
        q = random();
    } while (!primo(q, 2) || p == q);
    printf("Gerando n e phi...\n");
    n = p * q;
    phi = (p - 1) * (q - 1);
    printf("Gerando e...\n");
    // 'e' deve ser menor que 'phi'
    for (e = 2; e < phi; e++) {
        // 'e' deve ser co-primo com 'phi'
        if (mdc(e, phi) == 1)
            break;
    }

    printf("Chaves:\n\t%lld\n\t%lld\n\nn = %lld\nphi = %lld\ne = %lld\n", p, q, n, phi, e);

    char in[3], out[40];


    return 0;
}