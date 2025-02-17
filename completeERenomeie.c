/*
  EP: Árvore Binária de Busca (BST)
  Professor Anderson Ávila - UEL
  Descrição: Completar as funções solicitadas, sem alterar suas assinaturas.
  
  A main() contém testes que exibem resultados esperados como comentário.
  Quando as funções estiverem corretamente implementadas, esses testes
  devem produzir a saída indicada.
*/

#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó
typedef struct no {
    int chave;
    int contador;           // número de cópias (frequência) dessa chave
    struct no* esq;         // ponteiro para subárvore esquerda
    struct no* dir;         // ponteiro para subárvore direita
} NO, *PONT;

// Protótipos das funções
void inicializar(PONT* raiz);
PONT criarNo(int valor);
PONT inserir(PONT raiz, int valor);
PONT removerUmaOcorrencia(PONT raiz, int valor);
PONT removerTodasOcorrencias(PONT raiz, int valor);
PONT buscar(PONT raiz, int valor);
void exibirInOrder(PONT raiz);
int contarNos(PONT raiz);
int contarTotalElementos(PONT raiz);
int kEsimoMenor(PONT raiz, int k);
void imprimirIntervalo(PONT raiz, int min, int max);
PONT lowestCommonAncestor(PONT raiz, int val1, int val2);

//------------------------------------------------------------------------------
// 1) Inicializar
void inicializar(PONT* raiz) {
    *raiz = NULL;
}

//------------------------------------------------------------------------------
// 2) Criar nó
PONT criarNo(int valor) {
    PONT novo = (PONT) malloc(sizeof(NO));
    if (novo) {
        novo->chave = valor;
        novo->contador = 1;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

//------------------------------------------------------------------------------
// 3) Buscar
PONT buscar(PONT raiz, int valor) {
    if (raiz == NULL) return (NULL);                            // Verificação inicial se a raíz é nula.
    if (raiz->chave == valor) return (raiz);                    // Verificação se a chave da raíz é igual ao valor que recebemos como parâmetro, retornando o ponteiro pra raíz.
    if (valor < raiz->chave) return (buscar(raiz->esq, valor)); // Verificação da BST de esquerda e direita, retornando raiz->dir se não cair nessa verificação.
    return buscar(raiz->dir, valor);
}

//------------------------------------------------------------------------------
// 4) Inserir
PONT inserir(PONT raiz, int valor) {
    if (raiz == NULL) return criarNo(valor);                    // Se a raiz não existir, retorna a criação de um novo nó com o valor inserido
    if (valor < raiz->chave) {                                  // Se valor for menor que a chave da raiz, vai pro lado esquerdo recursivamente.
        raiz->esq = inserir(raiz->esq, valor);
    } else if (valor > raiz->chave) {                           // Se valor for maior que a chave da raiz, vai pro lado direito recursivamente.
        raiz->dir = inserir(raiz->dir, valor);
    } else {
        raiz->contador++;                                       // Agora, se valor == raiz->chave, acrescenta 1 no contador do elemento e retorna a raiz.
    }
    return raiz;
}

//------------------------------------------------------------------------------
// 5) Remover UMA ocorrência
PONT removerUmaOcorrencia(PONT raiz, int valor) {
    if (raiz == NULL) return NULL;                              // Se a raiz não existir, retorna null (não dá pra remover algo se não tiver nada)
    if (valor < raiz->chave) {                                  // Se valor for menor que a chave da raiz, vai pro lado esquerdo recursivamente
        raiz->esq = removerUmaOcorrencia(raiz->esq, valor);
    } else if (valor > raiz->chave) {                           // Se valor for maior que a chave da raiz, vai pro lado direito recursivamente
        raiz->dir = removerUmaOcorrencia(raiz->dir, valor);
    } else {                                                    // Se valor == raiz->chave, começamos a remoção:
        if (raiz->contador > 1) {                               // Se o contador do elemento for maior do que um, remove um
            raiz->contador--;
        } else {                                                // Se o contador do elemento for exatamente um, entra aqui:
            if (!raiz->esq) {                                   // - Se não há filho esquerdo, entra nessa condição
                PONT temp = raiz->dir;                          // Criação de um ponteiro temp, que armazena o valor o filho direito do elemento analisado
                free(raiz);                                     // Libera o elemento da memória
                return temp;                                    // Retorna temp, que é o filho direito.
            } else if (!raiz->dir) {                            // - Se não há filho direito, entra na condição
                PONT temp = raiz->esq;                          // Cria um ponteiro temp, mas dessa vez armazena o filho esquerdo
                free(raiz);                                     // Libera o então elemento
                return temp;                                    // Retorna temp, que é o filho esquerdo.
            } else {                                            // - Se há ambos, entramos nessa condição
                PONT temp = raiz->dir;                          // Cria um ponteiro temporário para o filho direito
                while (temp->esq) temp = temp->esq;             // Enquanto o filho esquerdo do filho direito do nó a ser removido não for null, vai até o menor valor (máximo esquerda)
                raiz->chave = temp->chave;                      // Acabando a verificação, a chave da raiz vira a chave do menor valor
                raiz->contador = temp->contador;                // O contador da raiz vira o contador do menor valor
                temp->contador = 1;                             // E então, o contador de temp vira 1.
                raiz->dir = removerUmaOcorrencia(raiz->dir, temp->chave); // Por fim, recursivamente chamamos raiz->dir como parâmetro raiz novamente.
            }
        }
    }
    return raiz;
}

//------------------------------------------------------------------------------
// 6) Remover TODAS ocorrências
PONT removerTodasOcorrencias(PONT raiz, int valor) {            // Com base na função de remover uma ocorrência, é só fazer isso até que seja null.
    while (buscar(raiz, valor) != NULL) {                       // Para isso, é utilizado um laço de repetição
        raiz = removerUmaOcorrencia(raiz, valor);               
    }
    return raiz;
}

//------------------------------------------------------------------------------
// 7) Exibir InOrder
void exibirInOrder(PONT raiz) {                                 // A exibição InOrder percorre da subárvore esquerda até a raíz, e da raíz até a subárvore direita
    if (raiz == NULL) return;                                   // Para tal, inicia-se verificando se a raiz é nula
    exibirInOrder(raiz->esq);                                   // Recursivamente, chama a funçao novamente e percorre até o máximo possível do lado esquerdo, dizendo de forma simples
    for (int i = 0; i < raiz->contador; i++) {                  
        printf("%d ", raiz->chave);                             // Após isso, entra em um laço de repetição, indo da esquerda pra direita com a função recursivamente chamada c o parâmetro raid->dir
    }
    exibirInOrder(raiz->dir);
}

//------------------------------------------------------------------------------
// 8) Contar nós distintos
int contarNos(PONT raiz) {
    if (raiz == NULL) return 0;                                 // Inicialmente, verifica se a raiz é nula 
    return(contarNos(raiz->esq) + 1 + contarNos(raiz->dir));    // Caso contrário, retorna a soma dos nós esquerdos, direitos e + 1 (que é a raíz).
}

//------------------------------------------------------------------------------
// 9) Contar total de elementos (somando contadores)
int contarTotalElementos(PONT raiz) {                           // Diferentemente da função contarNos, focamos na soma dos contadores
    if (raiz == NULL) return 0;                                 // Verifica se a raiz é nula
    return(raiz->contador + contarTotalElementos(raiz->esq) + contarTotalElementos(raiz->dir)); // Soma o contador da raíz, depois chama recursivamente para esquerda e então direita.
}

//------------------------------------------------------------------------------
// 10) k-ésimo menor 
int kEsimoMenor(PONT raiz, int k) {
    if (raiz == NULL) return -1;                                // Inicialmente, verifica se a raiz é nula
    int numAtual = raiz->contador;                              // Inicializa uma variável que conta a quantidade de vezes que a chave do nó atual aparece.
    int totalEsq = contarTotalElementos(raiz->esq);             // Então, contamos o número total de elementos na subárvore esquerda
    if (k <= totalEsq) return kEsimoMenor(raiz->esq, k);        // Se o k-ésimo menor está na subárvore esquerda, recursivamente busca lá
    if (k <= totalEsq + numAtual) return raiz->chave;           // Se o k-ésimo menor está no nó atual, retorna a chave da raíz.
    int kNaDireita = k - (totalEsq + numAtual);                 // Ajusta k para a busca na direita
    return kEsimoMenor(raiz->dir, kNaDireita);                  // Se o k-ésimo menor está na subárvore direita, recursivamente busca na direita
}

//------------------------------------------------------------------------------
// 11) Imprimir Intervalo [min, max]
void imprimirIntervalo(PONT raiz, int min, int max) {
    if (raiz == NULL) return;                                       // Inicialmente, verifica se a raiz é nula
    if (raiz->chave > min) imprimirIntervalo(raiz->esq, min, max);  // Se o valor da raiz é maior que o mínimo, buscamos na subárvore esquerda
    if (raiz->chave >= min && raiz->chave <= max) {                 // Se o valor da raiz está entre o mínimo e o máximo, printamos
        for (int i = 0; i < raiz->contador; i++) {
            printf("%d ", raiz->chave);
        }
    }
    if (raiz->chave < max) imprimirIntervalo(raiz->dir, min, max);  // Se o valor da raiz é menor que o máximo, recursivamente buscamos na subárvore direita
}

PONT lowestCommonAncestor(PONT raiz, int val1, int val2) {
    if (raiz == NULL) return NULL;                                                                    // Por início, verifica se a raiz é nula
    if (val1 > raiz->chave && val2 < raiz->chave) return lowestCommonAncestor(raiz->esq, val1, val2); // Se val1 é maior que a chave do elemento e val2 é MENOR do que a chave do mesmo, chama recursivamente a função para a esquerda
    if (val1 > raiz->chave && val2 > raiz->chave) return lowestCommonAncestor(raiz->dir, val1, val2); // Se val1 é maior que a chave do elemento e val2 é MAIOR do que a chave do mesmo, chama recursivamente a função para a direita
    return raiz;                                                                                      // Caso contrário, a raiz é o LCA.
}

//------------------------------------------------------------------------------
// main() para testes com expectativas de resultado
int main() {
    PONT raiz;                 // ponteiro para a raiz da BST
    inicializar(&raiz);        // deixa a árvore vazia

    // -------------------------------------------------------
    // 1) Inserção com valores repetidos
    //    Esperado que:
    //      - nó 10 tenha contador=2
    //      - nó 5  tenha contador=3
    //      - nó 15 tenha contador=1
    //      - nó 18 tenha contador=1
    //
    // InOrder final esperado (antes de quaisquer remoções):
    //     "5 5 5 10 10 15 18"
    //

    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 5);
    raiz = inserir(raiz, 15);
    raiz = inserir(raiz, 10);  // repetido => contador(10)++
    raiz = inserir(raiz, 5);   // repetido => contador(5)++
    raiz = inserir(raiz, 5);   // repetido => contador(5)++
    raiz = inserir(raiz, 18);

    printf("\n--- APÓS INSERIR (10,5,15,10,5,5,18) ---\n");
    printf("InOrder esperado: 5 5 5 10 10 15 18\n");
    printf("InOrder obtido:   ");
    exibirInOrder(raiz); 
    printf("\n");

    // -------------------------------------------------------
    // 2) Busca por valores
    PONT node5 = buscar(raiz, 5);
    if(node5) {
        printf("\nBuscar(5): encontrado com contador=%d (esperado=3)\n", node5->contador);
    } else {
        printf("\nBuscar(5): não encontrado (inesperado)\n");
    }
    PONT nodeX = buscar(raiz, 999); // valor não existente
    if(!nodeX) {
        printf("Buscar(999): não encontrado (esperado)\n");
    } else {
        printf("Buscar(999): encontrado??? (inesperado)\n");
    }

    // -------------------------------------------------------
    // 3) Remover UMA ocorrência 
    //    removerUmaOcorrencia(5) => contador(5) deve passar de 3 para 2
    removerUmaOcorrencia(raiz, 5);

    printf("\n--- APÓS removerUmaOcorrencia(5) ---\n");
    printf("Esperado InOrder: 5 5 10 10 15 18\n");
    printf("InOrder obtido:   ");
    exibirInOrder(raiz);
    printf("\n");

    node5 = buscar(raiz, 5);
    if(node5) {
        printf("Agora contador(5)=%d (esperado=2)\n", node5->contador);
    }  

    // -------------------------------------------------------
    // 4) Remover TODAS ocorrências
    //    removerTodasOcorrencias(10) => remove nó com chave=10 por completo
    removerTodasOcorrencias(raiz, 10);

    printf("\n--- APÓS removerTodasOcorrencias(10) ---\n");
    printf("Esperado InOrder: 5 5 15 18\n");
    printf("InOrder obtido:   ");
    exibirInOrder(raiz);
    printf("\n");

    // -------------------------------------------------------
    // 5) Contagem de nós e total de elementos
    //    Árvores resultante: {5(cont=2), 15(cont=1), 18(cont=1)}
    //      => contarNos=3   (chaves distintas: 5,15,18)
    //      => contarTotalElementos=4   (5,5,15,18)
    int qtdNos = contarNos(raiz);
    int totalElem = contarTotalElementos(raiz);
    printf("\ncontarNos => %d (esperado=3)\n", qtdNos);
    printf("contarTotalElementos => %d (esperado=4)\n", totalElem);

    // -------------------------------------------------------
    // 6) k-ésimo menor (considerando contadores)
    //    InOrder real: [5,5,15,18]
    //       k=1 => 5
    //       k=2 => 5
    //       k=3 => 15
    //       k=4 => 18
    //       k=5 => -1 (não existe)
    printf("\n--- Teste k-ésimo menor ---\n");
    printf("k=1 => %d (esperado=5)\n", kEsimoMenor(raiz,1));
    printf("k=2 => %d (esperado=5)\n", kEsimoMenor(raiz,2));
    printf("k=3 => %d (esperado=15)\n", kEsimoMenor(raiz,3));
    printf("k=4 => %d (esperado=18)\n", kEsimoMenor(raiz,4));
    printf("k=5 => %d (esperado=-1)\n", kEsimoMenor(raiz,5));

    // -------------------------------------------------------
    // 7) imprimirIntervalo [6..18]
    //    InOrder da árvore: 5,5,15,18
    //    Filtrando [6..18], esperamos: 15 18
    printf("\n--- Teste imprimirIntervalo [6..18] ---\n");
    printf("Esperado: 15 18\nObtido:   ");
    imprimirIntervalo(raiz, 6, 18);
    printf("\n");
    
    // -------------------------------------------------------
    // 8) Testar LCA (lowestCommonAncestor) - não é opcional
    //    Vamos inserir mais alguns valores para teste de LCA
    //    Situação final da árvore atual: 5(cont=2),15(cont=1),18(cont=1)
    //    Inserir(12), Inserir(16), Inserir(3)
    //    Nova BST (com contadores):
    //       5 (cont=2)
    //           /    \
    //         3(1)   15(1)
    //                /  \
    //              12(1) 18(1)
    //                  \
    //                  16(1)

    inserir(raiz, 12);
    inserir(raiz, 16);
    inserir(raiz, 3);

    printf("\n--- Árvore após inserir(12,16,3) ---\n");
    printf("InOrder esperado: 3 5 5 12 15 16 18\n");
    printf("Obtido:          ");
    exibirInOrder(raiz);
    printf("\n");

    // LCA tests:
    //  LCA(3,5) -> 5 ou 3? 
    //     - 3 e 5 estão ambos na subárvore da raiz=5? 
    //     - Se a raiz é 5, e 3<5 => esq 
    //       => LCA(3,5) deve ser 5, mas depende da estrutura exata
    //  LCA(3,12) -> 5
    //  LCA(16,18) -> 16 ou 15? 
    //     => Analisando: 16 < 18 => 16 deve estar na subárvore direita(15)
    //        -> 15 < 16 => goes right => 18
    //        => 16 < 18 => subárvore esquerda de 18
    //        => LCA deve ser 15 ou 18? Precisamos verificar a implementação.
    //  LCA(5,18) -> 5 (pois 5 é raiz, e 18 está à direita)

    PONT nLCA;

    nLCA = lowestCommonAncestor(raiz, 3, 5);
    if(nLCA) {
        printf("\nLCA(3,5) => chave=%d (esperado=5)\n", nLCA->chave);
    }

    nLCA = lowestCommonAncestor(raiz, 3, 12);
    if(nLCA) {
        printf("LCA(3,12) => chave=%d (esperado=5)\n", nLCA->chave);
    }

    nLCA = lowestCommonAncestor(raiz, 16, 18);
    if(nLCA) {
        printf("LCA(16,18) => chave=%d (esperado=18)\n", nLCA->chave);
    }

    nLCA = lowestCommonAncestor(raiz, 5, 18);
    if(nLCA) {
        printf("LCA(5,18) => chave=%d (esperado=15)\n", nLCA->chave);
    }

    // Por fim, buscar um LCA com valor inexistente
    nLCA = lowestCommonAncestor(raiz, 100, 3);
    if(!nLCA) {
        printf("LCA(100,3) => NULL (esperado=chave nao existe)\n");
    }

    printf("\n--- FIM DOS TESTES ---\n");

    return 0;
}
