#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define ll long long

// -----------------------------------------------------------------------------------------------------
// SE O PROJETO ESTIVER SENDO ABERTO UTILIZANDO O MinGW É PRECISO ADICIONAR
// #define srandom srand
// #define random rand
// NA stdlib.h DO SEU MinGW
// -----------------------------------------------------------------------------------------------------

struct ascii_number {
    int number;
    char letter;
};

/// Preenche uma lista de structs ascii_number com uma string fornecida
/// \param out A lista a ser preenchida
/// \param size O tamanho da array, deve ser passado como referência
/// \param string A string a fornecer de fonte de preenchimento
void generate_ascii_number_array(struct ascii_number out[], unsigned ll *size, char *string) {
    unsigned ll i;
    struct ascii_number temp;
    for (i = 0; i < strlen(string); i++) {
        temp.letter = string[i];
        temp.number = string[i];
        out[i] = temp;
    }
    *size = i;
}

/// Zera e preenche uma string com valores numéricos de uma lista de structs ascii_number
/// \param in A lista a preencher a string
/// \param size O tamanho da lista
/// \param string A string a ser preenchida
void ascii_array_to_number_string(struct ascii_number in[], unsigned ll size, char *string) {
    unsigned ll i;
    string[0] = '\0';
    for (i = 0; i < size; i++)
        sprintf(string, "%s%d", string, in[i].number);
}

/// Zera e preenche uma string com os caracteres de uma lista de structs ascii_number
/// \param in A lsita a preencher a string
/// \param size O tamanho da lista
/// \param string A string a ser preenchida
void ascii_array_to_char_string(struct ascii_number in[], unsigned ll size, char *string) {
    unsigned ll i;
    string[0] = '\0';
    for (i = 0; i < size; i++)
        strcat(string, &in[i].letter);
}

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
    struct ascii_number t[40];
    unsigned ll t_s;
    fgets(in, 3, stdin);
    generate_ascii_number_array(t, &t_s, in);
    ascii_array_to_number_string(t, t_s, out);
    puts(out);
    ascii_array_to_char_string(t, t_s, out);
    puts(out);
    return 0;
}