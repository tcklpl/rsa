#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define ll long long int

// Liga ou desliga as mensagens de log
#define DEBUG 1

// Caso o projeto esteja sendo editado no sistema Windows, definir bibliotecas inexistentes
#ifdef _WIN32
#define srandom srand
#define random rand
#endif

// Macro para mensagens de log
#if DEBUG == 1
#define log(x) printf("[LOG] %s\n", x)
#else
#define log(x)
#endif

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
/// Checa a primalidade de um número por meio do teorema de Fermat
/// Teorema de Fermat:
///
///     α é primo, logo ∀x ∈ (2, α), mdc(x, α) = 1 ∧ x^n-1 ≡ 1 (mod α)
///
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
        // de 2 a n
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

/// Gera as chaves e, d, phi e n para a criptografia
/// \param e saída, passada como referência
/// \param d "
/// \param phi "
/// \param n  "
void generate_keys(ll *e, ll *d, ll *phi, ll *n) {

    // 'p' e 'q' são números primos escoolhidos aleatoriamente
    // 'n' é pq
    // 'phi' é (p - 1)(q - 1)
    // 'e' é um número tal que (2 < e < phi) e 'e' e 'phi' são co-primos
    // 'd' é um número tal que de (mod phi) = 1
    ll p, q;
    srandom(time(NULL));
    log("Gerando p");
    do {
        p = random();
    } while (!primo(p, 2));
    log("Gerando q");
    do {
        q = random();
    } while (!primo(q, 2) || p == q);
    log("Gerando n e phi");
    *n = p * q;
    *phi = (p - 1) * (q - 1);
    log("Gerando e e d");
    // 'e' deve ser menor que 'phi'
    for (*e = 2; *e < *phi; (*e)++) {
        // 'e' deve ser co-primo com 'phi' e 'n'
        if (mdc(*e, *phi) == 1 && mdc(*e, *n) == 1)
            break;
    }
    // 'd' deve obedecer a equação
    //      de (mod phi) = 1
    for (*d = *e + 1; ; (*d)++) {
        if (((*d * *e) % *phi) == 1)
            break;
    }
    log("Chaves geradas com sucesso");
}

void ascii_to_number_array(char text[], ll size, ll output[]) {
    ll i;
    for (i = 0; i < size; i++) {
        output[i] = (ll) text[i];
    }
}

void number_array_to_ascii(ll numbers[], ll size, char out[]) {
    ll i;
    for (i = 0; i < size; i++)
        out[i] = (char) numbers[i];
    out[i] = '\0';
}

void cypher_blocks(ll blocks[], ll size, ll key, ll n) {
    ll i;
    for (i = 0; i < size; i++) {
        blocks[i] = mod(blocks[i], key, n);
    }
}

void print_blocks(ll blocks[], ll size) {
    ll i;
    for (i = 0; i < size; i++) {
        printf("%lld ", blocks[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    ll n, phi, e, d;
    generate_keys(&e, &d, &phi, &n);

    printf("e = %lld\nd = %lld\nphi = %lld\nn = %lld\n", e, d, phi, n);
    printf("Insere o texto ae: ");

    char texto[500], texto2[500];
    gets(texto);

    ll size = strlen(texto);

    ll raw[500], cypher[500];
    ascii_to_number_array(texto, size, raw);
    printf("-----------------------------------\n");

    print_blocks(raw, size);

    printf("-----------------------------------\n");

    cypher_blocks(raw, size, e, n);
    print_blocks(raw, size);

    printf("-----------------------------------\n");

    cypher_blocks(raw, size, d, n);
    print_blocks(raw, size);

    printf("-----------------------------------\n");

    number_array_to_ascii(raw, size, texto2);
    puts(texto2);


    return 0;
}
