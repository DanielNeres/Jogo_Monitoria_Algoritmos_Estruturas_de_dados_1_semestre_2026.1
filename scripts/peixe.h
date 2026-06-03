#ifndef PEIXE_H
#define PEIXE_H
#include "utils.h"
#include "isca.h"

#define MAX_PEIXES_CAPTURADOS 20

// enum que guarda cada tipo de peixe
enum TIPOS_PEIXES {
    BAGRE_CANAL, BAGRE_CABECA_CHATA, MUSKALONGE, WALLEYE, PERCA_AMARELA,
    RUTIO, ESTURJAO, LUCIO_PERCA, SALMAO_VERMELHO, LINGUADO, PEIXE_GATO,
    ACHIGA, CAVALINHA, TOTAL_TIPOS_PEIXES
};

enum STATUS_PEIXE {
    NORMAL, INTERESSADO, FISGADO
};

enum ESTADO_LUTA_PEIXE {
    LUTA_CANSADO,
    LUTA_NORMAL,
    LUTA_PIQUE
};

enum RESULTADO_LUTA {
    LUTA_CONTINUA,
    LUTA_ROMPEU_LINHA,
    LUTA_CAPTUROU
};

// estrutura que guarda os dados de um peixe
typedef struct {
    // Posição e visual
    Ponto posicao;
    float tamanho;
    float angulo;
    float angulo_alvo;
    short flip;

    // Nado normal fora da luta
    Ponto dir;
    Ponto passo;
    float velocidade;

    // Dados gerais do peixe
    float fator_fisgada;
    int preco;
    short tipo;
    short status;
    bool ativo;

    // Controle da luta
    bool luta_iniciada;

    float tempo_luta;
    float tempo_estado_luta;
    float duracao_estado_luta;

    float velocidade_luta;
    float velocidade_luta_alvo;
    float velocidade_luta_min;
    float velocidade_luta_max;

    float angulo_desejado_luta;

    float centro_luta_x;
    float centro_luta_y;
    float largura_zona_luta;
    float altura_zona_luta;

    short estado_luta;

} Peixe;

typedef struct {
    short tipo;
    float tamanho;
    int preco_venda;
    bool ocupado;
} PeixeCapturado;

// estrutura de um Nó de uma lista de peixes
typedef struct NoPeixe {
    Peixe data_peixe;
    struct NoPeixe* Prox_No;
} NoPeixe;

void inicializar_lista_capturados(PeixeCapturado capturados[], int capacidade);

bool adicionar_peixe_capturado(PeixeCapturado capturados[], int capacidade,Peixe *peixe);

void desenhar_peixes_capturados(PeixeCapturado capturados[], int capacidade, ALLEGRO_BITMAP *spr_peixes[]);

/**
 * @brief Calcula um novo ponto aleatório de destino baseado na zona de profundidade do peixe.
 * * @param tipo Identificador numérico do tipo de peixe.
 * @return Estrutura Ponto contendo as coordenadas X e Y geradas.
 */
Ponto calcular_ponto_aletorio_peixe(short tipo);

void apontar_peixe_para_direcao(Peixe *p, float dx, float dy);

/**
 * @brief Sorteia o próximo destino do peixe e calcula a variação de direção e ângulo.
 * * @param p Ponteiro para a estrutura do peixe.
 */
void definir_novo_destino_peixe(Peixe *p);

/**
 * @brief Atualiza as coordenadas do peixe aproximando-o do seu ponto de destino ou da isca.
 * @param p Ponteiro para a estrutura do peixe.
 * @param isca Ponteiro para a estrutura da isca na água.
 */
void atualizar_movimento_peixe(Peixe *p, Isca *isca);

/**
 * @brief Gira o sprite do peixe gradativamente até alcançar o ângulo desejado de deslocamento.
 * * @param p Ponteiro para a estrutura do peixe.
 */
void atualizar_rotacao_peixe(Peixe *p);

/**
 * @brief Configura as características essenciais (tamanho, velocidade e posição inicial) de um novo peixe.
 * * @param p Ponteiro para a estrutura do peixe a ser inicializada.
 * @param tipo_peixe Identificador numérico da espécie de peixe a ser instanciada.
 */
void inicializar_peixe(Peixe* p, short tipo_peixe);

/**
 * @brief Instancia e adiciona vários peixes de uma mesma espécie na lista encadeada.
 * * @param topo_pilha Ponteiro duplo para o nó cabeça da lista de peixes.
 * @param tipo_peixe Identificador numérico da espécie que será criada.
 * @param quantidade O número de instâncias desse peixe que serão inseridas na lista.
 */
void empilhar_peixes(NoPeixe** topo_pilha, short tipo_peixe, int quantidade);

/**
 * @brief Percorre a lista encadeada e renderiza na tela todos os peixes sinalizados como ativos.
 * * @param topo_pilha Ponteiro para o nó cabeça da lista de peixes.
 * @param spr_peixes Array contendo os sprites dos peixes em memória.
 */
void desenhar_peixes(NoPeixe* topo_pilha, ALLEGRO_BITMAP* spr_peixes[]);

/**
 * @brief Percorre toda a lista encadeada destruindo os nós e liberando a memória ocupada.
 * * @param topo_pilha Ponteiro duplo para o nó cabeça da lista de peixes.
 */
void limpar_pilha_peixes(NoPeixe** topo_pilha);

/**
 * @brief Verifica as condições de atração e probabilidade para determinar se o peixe morde a isca.
 * @param peixe Ponteiro para a estrutura do peixe a ser verificado.
 * @param isca Ponteiro para a estrutura da isca na água.
 * @return Retorna true se o peixe fisgar a isca, caso contrário, false.
 */
bool verificar_fisgada(Peixe *peixe, Isca *isca);

/**
 * @brief Gerencia a física, rotação e a verificação periódica de fisgadas para todos os peixes da lista.
 * @param topo Ponteiro para o nó cabeça da lista encadeada de peixes.
 * @param isca Ponteiro para a estrutura da isca.
 * @param flag_isca Booleano indicando se a isca está atualmente lançada na água.
 */
Peixe *atualizar_status_peixes(NoPeixe *topo, Isca *isca, bool flag_isca);

short processar_luta_pesca(Peixe *peixe, Isca *isca, bool puxando, short dificuldade);

void mover_peixe_para_frente_luta(Peixe *peixe, Isca *isca);

void escolher_angulo_luta(Peixe *peixe, Isca *isca);

void configurar_luta_por_tipo(Peixe *peixe, short dificuldade);

void iniciar_luta_peixe(Peixe *peixe, Isca *isca, short dificuldade);

void atualizar_velocidade_luta_peixe(Peixe *peixe, Isca *isca, bool puxando, short dificuldade);

float calcular_velocidade_giro_luta(Peixe *peixe);

void aplicar_movimento_luta(Peixe *peixe, Isca *isca, bool puxando, short dificuldade);

void aplicar_puxada_barco(Peixe *peixe, Isca *isca, short dificuldade);

void aplicar_resistencia_peixe(Peixe *peixe, Isca *isca, bool puxando, short dificuldade);

void limitar_zona_luta(Peixe *peixe, Isca *isca, float altura_captura);

void escolher_estado_luta(Peixe *peixe);

void atualizar_tensao_linha(Peixe *peixe, Isca *isca, bool puxando, short dificuldade);

bool atualizar_estado_luta_peixe(Peixe *peixe);

void virar_peixe_para_barco_se_fraco(Peixe *peixe, Isca *isca);

short verificar_fim_luta(Peixe *peixe, Isca *isca, float altura_captura);

#endif
