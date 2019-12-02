#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define ll long long int

// Liga ou desliga as mensagens de log
#define DEBUG 0


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

struct rsa_key {
    ll ed, n;
};

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
    p = random();
    if (!(p & 1))
        p++;
    while (!primo(p, 2))
        p += 2;
    log("Gerando q");
    q = random();
    if (!(q & 1))
        q++;
    while (!primo(q, 2) || p == q)
        q += 2;
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

void read_key(char *nome, void *p, size_t size) {
    FILE *f;
    f = fopen(nome, "rb");
    if (f == NULL) {
        printf("Impossível abrir arquivo de chave\n");
        exit(2);
    }
    fread(p, size, 1, f);
    fclose(f);
}

void generate_keyfiles() {
    FILE *f;
    ll n, phi, e, d;
    struct rsa_key encrypt, decrypt;

    generate_keys(&e, &d, &phi, &n);

    encrypt.ed = e;
    encrypt.n = n;

    decrypt.ed = d;
    decrypt.n = n;

    f = fopen("encrypt.rsakey", "wb");
    if (f == NULL) {
        printf("Impossível criar arquivo\n");
        scanf("%*c");
        exit(3);
    }
    fwrite(&encrypt, sizeof(struct rsa_key), 1, f);
    fclose(f);

    f = fopen("decrypt.rsakey", "wb");
    if (f == NULL) {
        printf("Impossível criar arquivo\n");
        scanf("%*c");
        exit(3);
    }
    fwrite(&decrypt, sizeof(struct rsa_key), 1, f);
    fclose(f);
}

void encrypt_file(char *file) {
    FILE *f;
    struct rsa_key encrypt;
    char text[5000], c;
    ll raw[5000];
    int raw_pos = 0;

    log("Abrindo arquivo de chave");

    f = fopen("encrypt.rsakey", "rb");
    if (f == NULL) {
        printf("Impossível criar arquivo de chave ou o mesmo nao existe\n");
        scanf("%*c");
        exit(3);
    }

    log("Lendo chave");
    fread(&encrypt, sizeof(struct rsa_key), 1, f);
    fclose(f);

    log("Abrindo arquivo para encriptar");
    f = fopen(file, "r");
    if (f == NULL) {
        printf("Impossível criar arquivo ou o mesmo nao existe\n");
        scanf("%*c");
        exit(4);
    }

    log("Lendo arquivo");
    c = fgetc(f);
    while (c != EOF) {
        text[raw_pos++] = c;
        c = fgetc(f);
    }
    fclose(f);

    log("Convertendo para numeros");
    ascii_to_number_array(text, raw_pos, raw);
    log("Encriptando");
    cypher_blocks(raw, raw_pos, encrypt.ed, encrypt.n);

    log("Abrindo arquivo de saida");

    f = fopen(strcat(file, ".crypt"), "wb");
    if (f == NULL) {
        printf("Impossível criar arquivo de saida\n");
        scanf("%*c");
        exit(4);
    }

    log("Escrevendo arquivo de chave");
    fwrite(raw, sizeof(ll), raw_pos, f);
    fclose(f);
    log("Encriptado");

}

void decrypt_file(char *file) {
    FILE *f;
    struct rsa_key decrypt;
    char text[5000];
    ll raw[5000], temp;
    int raw_pos = 0, i;

    log("Abrindo arquivo de chave");

    f = fopen("decrypt.rsakey", "rb");
    if (f == NULL) {
        printf("Impossível criar arquivo de chave ou o mesmo nao existe\n");
        scanf("%*c");
        exit(3);
    }

    log("Lendo chave");
    fread(&decrypt, sizeof(struct rsa_key), 1, f);
    fclose(f);

    log("Abrindo arquivo para decriptar");
    f = fopen(file, "rb");
    if (f == NULL) {
        printf("Impossível criar arquivo ou o mesmo nao existe\n");
        scanf("%*c");
        exit(4);
    }

    log("Lendo arquivo");
    fread(&temp, sizeof(ll), 1, f);
    raw[raw_pos++] = temp;
    while (!feof(f)) {
        fread(&temp, sizeof(ll), 1, f);
        raw[raw_pos++] = temp;
    }
    fclose(f);

    log("Decriptando");
    cypher_blocks(raw, raw_pos, decrypt.ed, decrypt.n);

    log("Convertendo para texto");
    number_array_to_ascii(raw, raw_pos, text);

    log("Abrindo arquivo de saida");

    // remover .crypt
    file[strlen(file) - 6] = '\0';

    f = fopen(file, "w");
    if (f == NULL) {
        printf("Impossível criar arquivo de saida\n");
        scanf("%*c");
        exit(4);
    }

    log("Escrevendo saida");
    for (i = 0; i < raw_pos - 1; i++)
        fputc(text[i], f);
    fclose(f);
    log("Decriptado");

}

void delete_keys() {
    remove("encrypt.rsakey");
    remove("decrypt.rsakey");
    printf("Chaves removidas com sucesso!\n");
}

int main(int argc, char *argv[]) {

    int op;
    char file_name[200];

    do {
        printf("----------------------------= RSA =----------------------------\n");
        printf("Operacoes: \n");
        printf("    1 - Gerar Arquivos de chave\n");
        printf("    2 - Criptografar Arquivo\n");
        printf("    3 - Descriptografar Arquivo\n");
        printf("    4 - Apagar arquivos de chave\n");
        printf("    0 - SAIR\n");
        printf("\n..: ");

        scanf("%d%*c", &op);

        switch (op) {
            case 1:
                generate_keyfiles();
                break;
            case 2:
                printf("Insira o nome do arquivo a ser criptografado:\n..: ");
                gets(file_name);
                encrypt_file(file_name);
                break;
            case 3:
                printf("Insira o nome do arquivo a ser descriptografado:\n..: ");
                gets(file_name);
                decrypt_file(file_name);
                break;
            case 4:
                delete_keys();
                break;
            default:
                break;
        }
    } while (op != 0);

    return 0;
}
