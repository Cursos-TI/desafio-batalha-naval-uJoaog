#include <stdio.h>
#include <stdlib.h> // Para abs()

// Constantes do jogo
#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5 // Novo valor para marcar a área de efeito da habilidade

// Tamanho fixo para as matrizes de habilidade (ex: 7x7)
#define TAMANHO_HABILIDADE 7
#define CENTRO_HABILIDADE (TAMANHO_HABILIDADE / 2)

// Estrutura para definir um navio
typedef struct {
    int linha_inicial;
    int coluna_inicial;
    // 'H' (Horizontal), 'V' (Vertical), '1' (Diagonal Principal), '2' (Diagonal Secundária)
    char direcao; 
} Navio;

// Estrutura para definir uma habilidade
typedef struct {
    int linha_origem;
    int coluna_origem;
    char tipo; // 'C' (Cone), 'X' (Cruz), 'O' (Octaedro)
} Habilidade;

// --- Funções do Jogo Base (Mantidas) ---

// Função para inicializar o tabuleiro 10x10 com o valor AGUA (0).
void inicializar_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Função para verificar se um navio pode ser posicionado sem sair dos limites ou sobrepor.
// Retorna 1 (verdadeiro) se o posicionamento for válido, e 0 (falso) caso contrário.
int validar_posicionamento(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], Navio navio) {
    int r = navio.linha_inicial;
    int c = navio.coluna_inicial;

    // Loop para verificar cada parte do navio (tamanho fixo 3)
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int linha_atual = r;
        int coluna_atual = c;

        // Calcula a próxima posição com base na direção
        if (navio.direcao == 'H') {
            coluna_atual += i;
        } else if (navio.direcao == 'V') {
            linha_atual += i;
        } else if (navio.direcao == '1') { // Diagonal Principal (ambos aumentam)
            linha_atual += i;
            coluna_atual += i;
        } else if (navio.direcao == '2') { // Diagonal Secundária (linha aumenta, coluna diminui)
            linha_atual += i;
            coluna_atual -= i;
        } else {
            return 0; // Direção inválida (Falso)
        }

        // 1. Validação de Limites (deve estar entre 0 e 9)
        if (linha_atual < 0 || linha_atual >= TAMANHO_TABULEIRO ||
            coluna_atual < 0 || coluna_atual >= TAMANHO_TABULEIRO) {
            return 0; // Fora dos limites (Falso)
        }

        // 2. Validação de Sobreposição (a posição não pode já ter um navio)
        if (tabuleiro[linha_atual][coluna_atual] == NAVIO) {
            return 0; // Sobreposição (Falso)
        }
    }

    return 1; // Posicionamento é válido (Verdadeiro)
}

// Função para posicionar um navio no tabuleiro.
void posicionar_navio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], Navio navio) {
    int r = navio.linha_inicial;
    int c = navio.coluna_inicial;

    // Loop para marcar cada parte do navio com o valor NAVIO (3)
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        int linha_atual = r;
        int coluna_atual = c;

        // Determina a posição final da parte do navio
        if (navio.direcao == 'H') {
            coluna_atual += i;
        } else if (navio.direcao == 'V') {
            linha_atual += i;
        } else if (navio.direcao == '1') {
            linha_atual += i;
            coluna_atual += i;
        } else if (navio.direcao == '2') {
            linha_atual += i;
            coluna_atual -= i;
        }
        
        tabuleiro[linha_atual][coluna_atual] = NAVIO;
    }
}

// --- Funções de Habilidade Especial ---

/*
 * Função para criar a matriz de área de efeito (AoE) para a habilidade Cone.
 * O cone aponta para baixo, com o topo no centro (CENTRO_HABILIDADE, CENTRO_HABILIDADE).
 * A área afetada (1) se expande à medida que a linha (i) aumenta.
 */
void criar_matriz_cone(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Distância horizontal do centro
            int dist_coluna = abs(j - CENTRO_HABILIDADE);
            
            // O cone se expande: a largura afetada é 2*i + 1, centrada em CENTRO_HABILIDADE.
            // A condição é que a distância da coluna seja menor ou igual à linha atual (i).
            if (i >= dist_coluna) {
                matriz_habilidade[i][j] = 1;
            } else {
                matriz_habilidade[i][j] = 0;
            }
        }
    }
}

/*
 * Função para criar a matriz de área de efeito (AoE) para a habilidade Cruz.
 * A cruz é centrada em (CENTRO_HABILIDADE, CENTRO_HABILIDADE).
 * A área afetada (1) está na linha central OU na coluna central.
 */
void criar_matriz_cruz(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Condicional para marcar a linha central OU a coluna central
            if (i == CENTRO_HABILIDADE || j == CENTRO_HABILIDADE) {
                matriz_habilidade[i][j] = 1;
            } else {
                matriz_habilidade[i][j] = 0;
            }
        }
    }
}

/*
 * Função para criar a matriz de área de efeito (AoE) para a habilidade Octaedro (Losango/Diamante).
 * O losango é centrado em (CENTRO_HABILIDADE, CENTRO_HABILIDADE).
 * A área afetada (1) é definida pela distância de Manhattan (abs(i-c) + abs(j-c)).
 */
void criar_matriz_octaedro(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Distância de Manhattan do centro
            int dist_manhattan = abs(i - CENTRO_HABILIDADE) + abs(j - CENTRO_HABILIDADE);
            
            // O losango é formado por todos os pontos cuja distância de Manhattan é menor ou igual ao raio.
            // O raio máximo para um losango completo em uma matriz 7x7 é 3 (CENTRO_HABILIDADE).
            if (dist_manhattan <= CENTRO_HABILIDADE) {
                matriz_habilidade[i][j] = 1;
            } else {
                matriz_habilidade[i][j] = 0;
            }
        }
    }
}

/*
 * Função para sobrepor a matriz de habilidade no tabuleiro principal.
 * A matriz de habilidade é centrada no ponto de origem (habilidade.linha_origem, habilidade.coluna_origem).
 * A sobreposição só ocorre se a posição do tabuleiro for AGUA (0) ou NAVIO (3),
 * e se a posição da matriz de habilidade for afetada (1).
 */
void aplicar_habilidade_no_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], Habilidade habilidade) {
    // Matriz temporária para a habilidade atual
    int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];

    // 1. Criar a matriz de AoE baseada no tipo de habilidade
    if (habilidade.tipo == 'C') {
        criar_matriz_cone(matriz_habilidade);
    } else if (habilidade.tipo == 'X') {
        criar_matriz_cruz(matriz_habilidade);
    } else if (habilidade.tipo == 'O') {
        criar_matriz_octaedro(matriz_habilidade);
    } else {
        printf("ERRO: Tipo de habilidade inválido: %c\n", habilidade.tipo);
        return;
    }

    // 2. Sobrepor a matriz de AoE no tabuleiro principal
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Se a posição na matriz de habilidade for afetada (1)
            if (matriz_habilidade[i][j] == 1) {
                // Calcular as coordenadas no tabuleiro principal
                // O centro da matriz de habilidade (CENTRO_HABILIDADE) deve coincidir com a origem da habilidade.
                int linha_tabuleiro = habilidade.linha_origem + (i - CENTRO_HABILIDADE);
                int coluna_tabuleiro = habilidade.coluna_origem + (j - CENTRO_HABILIDADE);

                // Verificar se a posição calculada está dentro dos limites do tabuleiro
                if (linha_tabuleiro >= 0 && linha_tabuleiro < TAMANHO_TABULEIRO &&
                    coluna_tabuleiro >= 0 && coluna_tabuleiro < TAMANHO_TABULEIRO) {
                    
                    // Marcar a posição no tabuleiro com o valor HABILIDADE (5)
                    // Nota: O requisito pede para marcar visualmente, mas não proíbe sobrepor navios.
                    // Para simplificar, vamos marcar, mas o valor do navio (3) tem prioridade visual na exibição.
                    if (tabuleiro[linha_tabuleiro][coluna_tabuleiro] != NAVIO) {
                        tabuleiro[linha_tabuleiro][coluna_tabuleiro] = HABILIDADE;
                    }
                }
            }
        }
    }
}

// --- Função de Exibição Atualizada ---

// Função para exibir o tabuleiro no console de forma formatada, usando caracteres.
void exibir_tabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    // Imprime o cabeçalho das colunas (0 a 9)
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    // Linha separadora
    printf("  +");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("---");
    }
    printf("\n");

    // Imprime as linhas do tabuleiro
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d|", i); // Imprime o índice da linha (0 a 9)
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            char simbolo;
            
            // Utiliza caracteres diferentes para cada elemento
            if (tabuleiro[i][j] == NAVIO) {
                simbolo = 'N'; // Navio (3)
            } else if (tabuleiro[i][j] == HABILIDADE) {
                simbolo = 'X'; // Área afetada pela habilidade (5)
            } else { // AGUA (0)
                simbolo = '~'; // Água (0)
            }
            
            printf(" %c ", simbolo);
        }
        printf("\n");
    }
}

// --- Função Principal (main) ---

int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    
    // 1. Inicializa o tabuleiro com água (0)
    inicializar_tabuleiro(tabuleiro);

    // 2. Definição e Posicionamento dos Navios (Mantido do código original)
    
    // Navios Ortogonais (Horizontal e Vertical)
    Navio navio1 = {1, 1, 'H'}; // Horizontal: (1,1), (1,2), (1,3)
    Navio navio2 = {5, 8, 'V'}; // Vertical: (5,8), (6,8), (7,8)

    // Navios Diagonais
    Navio navio3 = {3, 3, '1'}; // Diagonal Principal: (3,3), (4,4), (5,5)
    Navio navio4 = {7, 2, '2'}; // Diagonal Secundária: (7,2), (8,1), (9,0)

    // Array de navios para facilitar o posicionamento
    Navio navios[] = {navio1, navio2, navio3, navio4};
    int num_navios = sizeof(navios) / sizeof(navios[0]);

    printf("--- Batalha Naval: Nível Habilidades Especiais ---\n");
    printf("Posicionando %d navios de tamanho %d...\n\n", num_navios, TAMANHO_NAVIO);

    // Posicionamento e Validação
    for (int k = 0; k < num_navios; k++) {
        if (validar_posicionamento(tabuleiro, navios[k])) {
            posicionar_navio(tabuleiro, navios[k]);
            printf("Navio %d posicionado com sucesso. Direção: %c, Início: (%d, %d)\n", 
                   k + 1, navios[k].direcao, navios[k].linha_inicial, navios[k].coluna_inicial);
        } else {
            printf("ERRO: Navio %d (Direção: %c, Início: (%d, %d)) não pôde ser posicionado.\n", 
                   k + 1, navios[k].direcao, navios[k].linha_inicial, navios[k].coluna_inicial);
        }
    }
    
    printf("\n--- Aplicando Habilidades Especiais (Tamanho %dx%d) ---\n", TAMANHO_HABILIDADE, TAMANHO_HABILIDADE);

    // 3. Definição e Aplicação das Habilidades (Novo Requisito)
    
    // Habilidade 1: Cone (Origem: 0, 5)
    Habilidade hab_cone = {0, 5, 'C'}; 
    printf("Aplicando Habilidade Cone (Origem: %d, %d)\n", hab_cone.linha_origem, hab_cone.coluna_origem);
    aplicar_habilidade_no_tabuleiro(tabuleiro, hab_cone);

    // Habilidade 2: Cruz (Origem: 5, 5)
    Habilidade hab_cruz = {5, 5, 'X'}; 
    printf("Aplicando Habilidade Cruz (Origem: %d, %d)\n", hab_cruz.linha_origem, hab_cruz.coluna_origem);
    aplicar_habilidade_no_tabuleiro(tabuleiro, hab_cruz);

    // Habilidade 3: Octaedro (Origem: 8, 2)
    Habilidade hab_octaedro = {8, 2, 'O'}; 
    printf("Aplicando Habilidade Octaedro (Origem: %d, %d)\n", hab_octaedro.linha_origem, hab_octaedro.coluna_origem);
    aplicar_habilidade_no_tabuleiro(tabuleiro, hab_octaedro);


    printf("\n--- Tabuleiro Final (N: Navio, X: Habilidade, ~: Água) ---\n");
    // 4. Exibição do Tabuleiro Atualizado
    exibir_tabuleiro(tabuleiro);

    return 0;
} 