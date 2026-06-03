#include "peixe.h"

#define SUPERFICIE_AGUA_LUTA (TELA_H - 245.0)
#define ALTURA_CAPTURA_LUTA (SUPERFICIE_AGUA_LUTA + 70)
#define LIMITE_SUPERIOR_LUTA SUPERFICIE_AGUA_LUTA
#define FUNDO_LUTA (TELA_H * 2.0 - 80.0)
#define MARGEM_LATERAL_LUTA 320.0


void inicializar_lista_capturados(PeixeCapturado capturados[], int capacidade) {
    for (int i = 0; i < capacidade; i++) {
        capturados[i].tipo = -1;
        capturados[i].tamanho = 1.0;
        capturados[i].preco_venda = 0;
        capturados[i].ocupado = false;
    }
}

bool adicionar_peixe_capturado(PeixeCapturado capturados[], int capacidade, Peixe *peixe) {
    if (peixe == NULL) {
        return false;
    }

    for (int i = 0; i < capacidade; i++) {
        if (!capturados[i].ocupado) {
            capturados[i].tipo = peixe->tipo;
            capturados[i].tamanho = peixe->tamanho;
            capturados[i].preco_venda = (int)(peixe->preco * (0.75 + peixe->tamanho * 0.25));
            capturados[i].ocupado = true;

            return true;
        }
    }

    return false;
}

void desenhar_peixes_capturados(PeixeCapturado capturados[], int capacidade, ALLEGRO_BITMAP *spr_peixes[]) {
    float inicio_x = 45.0;
    float inicio_y = 55.0;

    float espacamento = 75.0;
    float raio_slot = 24.0;

    for (int i = 0; i < capacidade; i++) {
        float x = inicio_x + i * espacamento;
        float y = inicio_y;

        

        if (capturados[i].ocupado) {
            short tipo = capturados[i].tipo;

            if (tipo >= 0 && tipo < TOTAL_TIPOS_PEIXES && spr_peixes[tipo] != NULL) {
                ALLEGRO_BITMAP *sprite = spr_peixes[tipo];

                float centro_x = al_get_bitmap_width(sprite) / 2.0;
                float centro_y = al_get_bitmap_height(sprite) / 2.0;

                float escala = 1.5*capturados[i].tamanho;

                al_draw_scaled_rotated_bitmap(sprite, centro_x, centro_y, x, y, escala, escala, PI/2, 0);
            }
        } else{
            al_draw_filled_circle(x, y, raio_slot, al_map_rgb(0, 0, 0));
        }
    }
}

Ponto calcular_ponto_aletorio_peixe(short tipo) {
    Ponto novo_ponto;

    // Limites de movimentação no eixo X (deixando uma margem nas bordas)
    float min_x = 50.0;
    float max_x = TELA_W - 50.0;

    // --- DEFINIÇÃO DAS ZONAS DO MAR ---
    float y_superficie  = TELA_H + 50.0;
    float y_raso        = TELA_H + (TELA_H * 0.15); // 15% de profundidade 
    float y_medio_alto  = TELA_H + (TELA_H * 0.30); // 30% de profundidade
    float y_medio_baixo = TELA_H + (TELA_H * 0.55); // 55% de profundidade 
    float y_fundo       = TELA_H + (TELA_H * 0.70); // 70% de profundidade
    float y_abissal     = (TELA_H * 2.0) - 50;      // 100% 

    switch (tipo) {
        case CAVALINHA: 
        case SALMAO_VERMELHO:
            novo_ponto.x = min_x + ((float)rand() / RAND_MAX) * (max_x - min_x);
            novo_ponto.y = y_superficie + ((float)rand() / RAND_MAX) * (y_medio_alto - y_superficie);
            break;

        case MUSKALONGE:
            novo_ponto.x = ((float)rand() / RAND_MAX) * TELA_W; 
            novo_ponto.y = y_superficie + ((float)rand() / RAND_MAX) * (y_medio_baixo - y_superficie);
            break;

        case ACHIGA:
            novo_ponto.x = min_x + ((float)rand() / RAND_MAX) * (max_x - min_x);
            novo_ponto.y = y_raso + ((float)rand() / RAND_MAX) * (y_medio_baixo - y_raso);
            break;

        case PERCA_AMARELA:
        case RUTIO:
            novo_ponto.x = min_x + ((float)rand() / RAND_MAX) * (max_x - min_x);
            novo_ponto.y = y_raso + ((float)rand() / RAND_MAX) * (y_medio_alto - y_raso);
            break;

        case WALLEYE:
        case LUCIO_PERCA:
            novo_ponto.x = min_x + ((float)rand() / RAND_MAX) * (max_x - min_x);
            novo_ponto.y = y_medio_alto + ((float)rand() / RAND_MAX) * (y_fundo - y_medio_alto);
            break;

        case ESTURJAO:
            novo_ponto.x = min_x + 150.0 + ((float)rand() / RAND_MAX) * (max_x - min_x - 300.0);
            novo_ponto.y = y_medio_baixo + ((float)rand() / RAND_MAX) * (y_abissal - y_medio_baixo);
            break;

        case PEIXE_GATO:
        case BAGRE_CANAL:
        case BAGRE_CABECA_CHATA:
            novo_ponto.x = min_x + 250.0 + ((float)rand() / RAND_MAX) * (max_x - min_x - 500.0);
            novo_ponto.y = y_fundo + ((float)rand() / RAND_MAX) * (y_abissal - y_fundo);
            break;

        case LINGUADO:
            novo_ponto.x = min_x + 100.0 + ((float)rand() / RAND_MAX) * (max_x - min_x - 200.0);
            novo_ponto.y = y_abissal - 20.0 + ((float)rand() / RAND_MAX) * 20.0;
            break;

        default:
            novo_ponto.x = min_x + ((float)rand() / RAND_MAX) * (max_x - min_x);
            novo_ponto.y = y_superficie + ((float)rand() / RAND_MAX) * (y_abissal - y_superficie);
            break;
    }

    return novo_ponto;
}

void apontar_peixe_para_direcao(Peixe *p, float dx, float dy) {
    if (dx > 0) {
        p->flip = 1;
        p->angulo_alvo = atan2(dy, dx);
    } else {
        p->flip = -1;
        p->angulo_alvo = -atan2(dy, -dx);
    }
}

void definir_novo_destino_peixe(Peixe *p) {
    p->dir = calcular_ponto_aletorio_peixe(p->tipo);

    float dx = p->dir.x - p->posicao.x;
    float dy = p->dir.y - p->posicao.y;

    float distancia = sqrt(dx * dx + dy * dy);

    if (distancia > 0.0) {
        p->passo.x = (dx / distancia) * p->velocidade;
        p->passo.y = (dy / distancia) * p->velocidade;
    } else {
        p->passo.x = 0.0;
        p->passo.y = 0.0;
    }

    apontar_peixe_para_direcao(p, dx, dy);
}

void atualizar_movimento_peixe(Peixe *p, Isca *isca) {
    
    // Variáveis auxiliares para os cálculos
    float dx, dy, dist_quadrada, vel_quadrada;

    switch (p->status) {
        
        case NORMAL:
            // Lógica original de nado livre
            dx = p->dir.x - p->posicao.x;
            dy = p->dir.y - p->posicao.y;
            dist_quadrada = (dx * dx) + (dy * dy);
            vel_quadrada = p->velocidade * p->velocidade;

            if (dist_quadrada <= vel_quadrada) {
                p->posicao.x = p->dir.x;
                p->posicao.y = p->dir.y;
                definir_novo_destino_peixe(p); 
            } else {
                p->posicao.x += p->passo.x;
                p->posicao.y += p->passo.y;
            }
            break;

        case INTERESSADO:
            // O alvo agora é a posição atual da isca
            dx = isca->posicao.x - p->posicao.x;
            dy = isca->posicao.y - p->posicao.y;
            dist_quadrada = (dx * dx) + (dy * dy);
            vel_quadrada = p->velocidade * p->velocidade;

            // Se encostou na isca (chegou no alvo)
            if (dist_quadrada <= vel_quadrada) {
                p->posicao.x = isca->posicao.x;
                p->posicao.y = isca->posicao.y;
                
                // Muda para o próximo estado!
                p->status = FISGADO; 
            } else {
                // Calcula o passo em direção à isca (que pode estar se movendo)
                float distancia_real = sqrt(dist_quadrada);
                p->passo.x = (dx / distancia_real) * p->velocidade;
                p->passo.y = (dy / distancia_real) * p->velocidade;
                
                p->posicao.x += p->passo.x;
                p->posicao.y += p->passo.y;

                // Atualiza a rotação visual para mirar na isca constantemente
                apontar_peixe_para_direcao(p, dx, dy);
            }
            break;

        case FISGADO:
            /*  Quando o peixe está fisgado, quem controla a luta é
                processar_luta_pesca().*/
            break;
    }
}

void atualizar_rotacao_peixe(Peixe *p) {
    float velocidade_giro = 0.035;

    if (p->status == INTERESSADO) {
        velocidade_giro = 0.055;
    }

    p->angulo = aproximar_angulo(p->angulo, p->angulo_alvo, velocidade_giro);
}

void mover_peixe_para_frente_luta(Peixe *peixe, Isca *isca) {
    float dir_x;
    float dir_y;

    /*
        No seu projeto, o sprite original parece olhar para a esquerda.
        Quando flip == 1, você usa ALLEGRO_FLIP_HORIZONTAL,
        então ele passa a olhar para a direita.
    */
    if (peixe->flip == 1) {
        dir_x = cos(peixe->angulo);
        dir_y = sin(peixe->angulo);
    } else {
        dir_x = -cos(peixe->angulo);
        dir_y = -sin(peixe->angulo);
    }

    isca->posicao.x += dir_x * peixe->velocidade_luta;
    isca->posicao.y += dir_y * peixe->velocidade_luta;

    peixe->posicao.x = isca->posicao.x;
    peixe->posicao.y = isca->posicao.y;
}

void inicializar_peixe(Peixe* p, short tipo_peixe) {
    p->tipo = tipo_peixe;
    p->ativo = true; 
    p->flip = (rand() % 2 == 0) ? 1 : -1; 
    
    // Define o estado inicial para a máquina de estados
    p->status = NORMAL;

    p->luta_iniciada = false;

    p->tempo_luta = 0.0;
    p->tempo_estado_luta = 0.0;
    p->duracao_estado_luta = 1.0;

    p->velocidade_luta = 0.0;
    p->velocidade_luta_alvo = 0.0;
    p->velocidade_luta_min = 0.6;
    p->velocidade_luta_max = 4.0;

    p->angulo_desejado_luta = 0.0;
    p->angulo = 0.0;
    p->angulo_alvo = 0.0;

    p->centro_luta_x = 0.0;
    p->centro_luta_y = 0.0;
    p->largura_zona_luta = 350.0;
    p->altura_zona_luta = 180.0;

    p->estado_luta = LUTA_NORMAL;

    // Define a velocidade, tamanho e fator de fisgada de acordo com a espécie
    switch (tipo_peixe) {
        case PERCA_AMARELA:
            p->velocidade = sortear_float(2.0, 2.5); 
            p->tamanho = sortear_float(1.65, 2.0);
            p->fator_fisgada = 0.8; // Peixe fácil
            break;
        case WALLEYE:
            p->velocidade = sortear_float(1.4, 1.8); 
            p->tamanho = sortear_float(1.8, 2.3);
            p->fator_fisgada = 0.5; // Peixe médio
            break;
        case MUSKALONGE:
            p->velocidade = sortear_float(3.0, 3.5);
            p->tamanho = sortear_float(2.3, 2.65);
            p->fator_fisgada = 0.1; // Peixe chefão (Difícil)
            break;
        case BAGRE_CANAL:
            p->velocidade = sortear_float(1.0, 1.3); 
            p->tamanho = sortear_float(1.9, 2.25);
            p->fator_fisgada = 0.6;
            break;
        case BAGRE_CABECA_CHATA:
            p->velocidade = sortear_float(0.6, 0.9); 
            p->tamanho = sortear_float(2.0, 2.35);
            p->fator_fisgada = 0.6;
            break;
        case RUTIO:
            p->velocidade = sortear_float(1.8, 2.2); 
            p->tamanho = sortear_float(0.4, 0.5);
            p->fator_fisgada = 0.9; // Peixe muito fácil
            break;
        case ESTURJAO:
            p->velocidade = sortear_float(0.5, 0.8);
            p->tamanho = sortear_float(0.6, 0.9); 
            p->fator_fisgada = 0.2; // Peixe difícil
            break;
        case LUCIO_PERCA:
            p->velocidade = sortear_float(1.5, 2.0);
            p->tamanho = sortear_float(0.55, 0.65);
            p->fator_fisgada = 0.4;
            break;
        case SALMAO_VERMELHO:
            p->velocidade = sortear_float(2.5, 3.0);
            p->tamanho = sortear_float(0.65, 0.75);
            p->fator_fisgada = 0.5;
            break;
        case LINGUADO:
            p->velocidade = sortear_float(1.8, 2.2);
            p->tamanho = sortear_float(0.7, 0.8);
            p->fator_fisgada = 0.6;
            break;
        case PEIXE_GATO:
            p->velocidade = sortear_float(0.8, 1.2);
            p->tamanho = sortear_float(0.55, 0.7);
            p->fator_fisgada = 0.7;
            break;
        case ACHIGA:
            p->velocidade = sortear_float(2.0, 2.8);
            p->tamanho = sortear_float(0.5, 0.7);
            p->fator_fisgada = 0.4;
            break;
        case CAVALINHA:
            p->velocidade = sortear_float(2.8, 3.4);
            p->tamanho = sortear_float(0.4, 0.6);
            p->fator_fisgada = 0.8;
            break;
        default:
            p->velocidade = sortear_float(1.7, 2.3);
            p->tamanho = sortear_float(0.05, 0.10); 
            p->fator_fisgada = 0.3;
            break;
    }
    
    switch (tipo_peixe) {
        case RUTIO:
            p->preco = 14;
            break;
        case CAVALINHA:
            p->preco = 24;
            break;
        case PERCA_AMARELA:
            p->preco = 28;
            break;
        case PEIXE_GATO:
            p->preco = 34;
            break;
        case LINGUADO:
            p->preco = 40;
            break;
        case WALLEYE:
            p->preco = 46;
            break;
        case BAGRE_CANAL:
            p->preco = 52;
            break;
        case BAGRE_CABECA_CHATA:
            p->preco = 64;
            break;
        case ACHIGA:
            p->preco = 66;
            break;
        case LUCIO_PERCA:
            p->preco = 72;
            break;
        case SALMAO_VERMELHO:
            p->preco = 86;
            break;
        case ESTURJAO:
            p->preco = 140;
            break;
        case MUSKALONGE:
            p->preco = 180;
            break;
        default:
            p->preco = 20;
            break;
    }

    // Define a posição inicial de Nascimento no eixo X
    if (p->flip == 1) {
        p->posicao.x = -200.0; // Nasce na esquerda e vai pra direita
    } else {
        p->posicao.x = TELA_W + 200.0; // Nasce na direita e vai pra esquerda
    }

    // Sorteia a profundidade do nascimento
    float limite_y_min = TELA_H;
    float limite_y_max = TELA_H + 500.0;
    p->posicao.y = sortear_float(limite_y_min, limite_y_max);

    // Zera as variáveis de direção por precaução antes de calcular
    p->dir.x = 0; p->dir.y = 0;
    p->passo.x = 0; p->passo.y = 0;

    // Define o primeiro ponto que ele vai seguir logo ao nascer
    definir_novo_destino_peixe(p);

}

void empilhar_peixes(NoPeixe** topo_pilha, short tipo_peixe, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        // Aloca memória para o novo peixe
        NoPeixe* novo_no = (NoPeixe*)malloc(sizeof(NoPeixe));
        if (novo_no == NULL) {
            printf("Erro: Memória insuficiente para criar novo peixe.\n");
            return; 
        }

        // inicializa as variaveis desse peixe
        inicializar_peixe(&novo_no->data_peixe, tipo_peixe);

        // Comportamento de Pilha: O novo nó aponta para o antigo topo
        novo_no->Prox_No = *topo_pilha;
        
        // O topo da pilha passa a ser este novo nó
        *topo_pilha = novo_no;
    }
}

void desenhar_peixes(NoPeixe* topo_pilha, ALLEGRO_BITMAP* spr_peixes[]) {
    NoPeixe* atual = topo_pilha; // Começa pelo topo da pilha

    // Percorre todos os peixes da lista até chegar no final (NULL)
    while (atual != NULL) {
        
        // Só desenhamos se ele estiver marcado como ativo
        if (atual->data_peixe.ativo) {
            
            // Se o peixe estiver nadando para a esquerda (-1), espelha a imagem
            int flag_espelhamento = (atual->data_peixe.flip == -1) ? 0 : ALLEGRO_FLIP_HORIZONTAL;

            // Pegamos o sprite do peixe atual
            ALLEGRO_BITMAP* sprite_atual = spr_peixes[atual->data_peixe.tipo];
            
            // Lemos as dimensões reais da imagem e dividimos por 2 para achar o centro exato
            float centro_x = al_get_bitmap_width(sprite_atual) / 2.0;
            float centro_y = al_get_bitmap_height(sprite_atual) / 2.0;

            al_draw_scaled_rotated_bitmap(
                sprite_atual,                                               // A imagem do peixe
                centro_x, centro_y,                                         // Eixo de rotação
                atual->data_peixe.posicao.x, atual->data_peixe.posicao.y,   // Posição (X, Y) na tela
                atual->data_peixe.tamanho, atual->data_peixe.tamanho,                          
                atual->data_peixe.angulo,                                   // Ângulo de rotação 
                flag_espelhamento                                           // Vira ou não o peixe
            );
            
            // Verifica se o peixe saiu da área limite do eixo X
            if (atual->data_peixe.posicao.x > TELA_W + 400.0 || atual->data_peixe.posicao.x < -400.0) {
                atual->data_peixe.ativo = false;
            }
        }
        
        // Pula para o próximo peixe da lista
        atual = atual->Prox_No;
    }
}

void limpar_pilha_peixes(NoPeixe** topo_pilha) {
    NoPeixe* atual = *topo_pilha;
    NoPeixe* proximo;

    // Percorre a lista até o final
    while (atual != NULL) {
        proximo = atual->Prox_No; // Salva o endereço do próximo nó
        free(atual);              // Libera a memória do nó atual
        atual = proximo;          // Avança para o próximo nó
    }

    // Após apagar tudo, garante que o ponteiro principal não aponte para lixo de memória
    *topo_pilha = NULL; 
}

bool verificar_fisgada(Peixe *peixe, Isca *isca) {
    // Se o peixe já está fisgado, não faz sentido checar de novo
    if (peixe->status != NORMAL) return false;

    // Constantes do filtro de fisgada
    float raio_atracao = 150.0;
    float raio_quadrado = raio_atracao * raio_atracao; // Evita calcular sqrt toda hora

    bool atrai_peixe = false;

    switch (isca->tipo) {
        case VELHA:
            if (
                peixe->tipo == RUTIO ||
                peixe->tipo == PERCA_AMARELA ||
                peixe->tipo == PEIXE_GATO
            ) {
                atrai_peixe = true;
            }
            break;

        case BOIA:
            if (
                peixe->tipo == RUTIO ||
                peixe->tipo == CAVALINHA ||
                peixe->tipo == SALMAO_VERMELHO ||
                peixe->tipo == PERCA_AMARELA
            ) {
                atrai_peixe = true;
            }
            break;

        case ESVERDEADA:
            if (
                peixe->tipo == ACHIGA ||
                peixe->tipo == LUCIO_PERCA ||
                peixe->tipo == WALLEYE
            ) {
                atrai_peixe = true;
            }
            break;

        case REALISTA:
            if (
                peixe->tipo == WALLEYE ||
                peixe->tipo == ACHIGA ||
                peixe->tipo == LUCIO_PERCA ||
                peixe->tipo == LINGUADO ||
                peixe->tipo == SALMAO_VERMELHO ||
                peixe->tipo == BAGRE_CANAL
            ) {
                atrai_peixe = true;
            }
            break;

        case PEQUENO_PEIXE:
            if (
                peixe->tipo == MUSKALONGE ||
                peixe->tipo == BAGRE_CABECA_CHATA ||
                peixe->tipo == BAGRE_CANAL ||
                peixe->tipo == ESTURJAO ||
                peixe->tipo == PEIXE_GATO
            ) {
                atrai_peixe = true;
            }
            break;

        default:
            atrai_peixe = true; 
            break;
    }

    // Se a isca não atrai esse tipo de peixe, já aborta a matemática e retorna falso
    if (!atrai_peixe) return false;


    float dx = isca->posicao.x - peixe->posicao.x;
    float dy = isca->posicao.y - peixe->posicao.y;
    float dist_quadrada = (dx * dx) + (dy * dy);

    // Se o peixe estiver fora do raio de atração, retorna falso
    if (dist_quadrada > raio_quadrado) return false;

    float distancia_real = sqrt(dist_quadrada);
    // Proximidade: varia de 0.0 (na borda do raio) a 1.0
    float proximidade = (raio_atracao - distancia_real) / raio_atracao;
    
    // Multiplica pela raridade do peixe e pelo multiplicador da loja
    float chance_base = proximidade * peixe->fator_fisgada * isca->upgrade_atracao;
    
    // Suavização para o FPS: Como isso roda 60x por segundo, a chance por frame deve ser pequena (ex: 2%)
    float chance_por_frame = chance_base * 0.5;

    // Rola os dados de 0.0 a 1.0. Se bater na chance, o peixe morde
    if (sortear_float(0.0, 1.0) <= chance_por_frame) {
        return true; 
    }

    return false;
}

Peixe *atualizar_status_peixes(NoPeixe *topo, Isca *isca, bool flag_isca) {
    
    Peixe *peixe_fisgado = NULL;

    static float cronometro_fisgada = 0.0;
    cronometro_fisgada += (1.0 / FPS);

    bool checar_agora = false;
    if (cronometro_fisgada >= 1.0) {
        checar_agora = true;
    }

    NoPeixe *atual = topo;

    while (atual != NULL) {

        if (atual->data_peixe.ativo) {

            atualizar_movimento_peixe(&atual->data_peixe, isca);

            /*
                Só gira normalmente se NÃO estiver fisgado.
                Quando estiver fisgado, a rotação é controlada pela fuga.
            */
            if (atual->data_peixe.status != FISGADO) {
                atualizar_rotacao_peixe(&atual->data_peixe);
            }

            /*
                Agora sim: só retorna peixe para a luta
                quando ele realmente estiver FISGADO.
            */
            if (atual->data_peixe.status == FISGADO) {
                peixe_fisgado = &atual->data_peixe;
            }

            /*
                Só tenta atrair peixe se:
                - a isca está lançada;
                - não tem peixe interessado/fisgado;
                - passou o tempo da checagem.
            */
            if (checar_agora && flag_isca && !isca->esta_com_peixe) {

                if (verificar_fisgada(&atual->data_peixe, isca)) {
                    atual->data_peixe.status = INTERESSADO;
                    isca->esta_com_peixe = true;
                }
            }
        }

        atual = atual->Prox_No;
    }

    if (checar_agora) {
        cronometro_fisgada = 0.0;
    }

    return peixe_fisgado;
}

short processar_luta_pesca(Peixe *peixe, Isca *isca, bool puxando, short dificuldade) {
    if (peixe == NULL || isca == NULL) {
        return LUTA_CONTINUA;
    }

    if (peixe->status != FISGADO) {
        return LUTA_CONTINUA;
    }

    float altura_captura = ALTURA_CAPTURA_LUTA;

    if (!peixe->luta_iniciada) {
        iniciar_luta_peixe(peixe, isca, dificuldade);
    }

    bool mudou_estado = atualizar_estado_luta_peixe(peixe);

    if (mudou_estado) {
        escolher_angulo_luta(peixe, isca);
    }

    atualizar_velocidade_luta_peixe(peixe, isca, puxando, dificuldade);
    aplicar_movimento_luta(peixe, isca, puxando, dificuldade);
    limitar_zona_luta(peixe, isca, altura_captura);

    peixe->posicao.x = isca->posicao.x;
    peixe->posicao.y = isca->posicao.y;

    atualizar_tensao_linha(peixe, isca, puxando, dificuldade);

    return verificar_fim_luta(peixe, isca, altura_captura);
}

void configurar_luta_por_tipo(Peixe *peixe, short dificuldade) {
    peixe->largura_zona_luta = 330.0;
    peixe->altura_zona_luta = 170.0;

    peixe->velocidade_luta_min = 0.45 + peixe->velocidade * 0.12;
    peixe->velocidade_luta_max = 1.6 + peixe->velocidade * 0.65;

    switch (peixe->tipo) {
        case RUTIO:
            peixe->largura_zona_luta = 210.0;
            peixe->altura_zona_luta = 120.0;
            peixe->velocidade_luta_min = 0.28;
            peixe->velocidade_luta_max = 1.05;
            break;

        case PERCA_AMARELA:
            peixe->largura_zona_luta = 250.0;
            peixe->altura_zona_luta = 145.0;
            peixe->velocidade_luta_min = 0.35;
            peixe->velocidade_luta_max = 1.35;
            break;

        case WALLEYE:
            peixe->largura_zona_luta = 330.0;
            peixe->altura_zona_luta = 185.0;
            peixe->velocidade_luta_min = 0.48;
            peixe->velocidade_luta_max = 1.85;
            break;

        case LUCIO_PERCA:
            peixe->largura_zona_luta = 370.0;
            peixe->altura_zona_luta = 200.0;
            peixe->velocidade_luta_min = 0.62;
            peixe->velocidade_luta_max = 2.45;
            break;

        case ACHIGA:
            peixe->largura_zona_luta = 360.0;
            peixe->altura_zona_luta = 190.0;
            peixe->velocidade_luta_min = 0.58;
            peixe->velocidade_luta_max = 2.35;
            break;

        case CAVALINHA:
            peixe->largura_zona_luta = 420.0;
            peixe->altura_zona_luta = 170.0;
            peixe->velocidade_luta_min = 0.50;
            peixe->velocidade_luta_max = 2.00;
            break;

        case SALMAO_VERMELHO:
            peixe->largura_zona_luta = 470.0;
            peixe->altura_zona_luta = 220.0;
            peixe->velocidade_luta_min = 0.65;
            peixe->velocidade_luta_max = 2.80;
            break;

        case BAGRE_CANAL:
            peixe->largura_zona_luta = 310.0;
            peixe->altura_zona_luta = 250.0;
            peixe->velocidade_luta_min = 0.55;
            peixe->velocidade_luta_max = 2.20;
            break;

        case BAGRE_CABECA_CHATA:
            peixe->largura_zona_luta = 350.0;
            peixe->altura_zona_luta = 280.0;
            peixe->velocidade_luta_min = 0.62;
            peixe->velocidade_luta_max = 2.55;
            break;

        case PEIXE_GATO:
            peixe->largura_zona_luta = 260.0;
            peixe->altura_zona_luta = 210.0;
            peixe->velocidade_luta_min = 0.38;
            peixe->velocidade_luta_max = 1.45;
            break;

        case ESTURJAO:
            peixe->largura_zona_luta = 420.0;
            peixe->altura_zona_luta = 330.0;
            peixe->velocidade_luta_min = 0.70;
            peixe->velocidade_luta_max = 3.00;
            break;

        case MUSKALONGE:
            peixe->largura_zona_luta = 540.0;
            peixe->altura_zona_luta = 280.0;
            peixe->velocidade_luta_min = 0.85;
            peixe->velocidade_luta_max = 3.80;
            break;

        case LINGUADO:
            peixe->largura_zona_luta = 260.0;
            peixe->altura_zona_luta = 260.0;
            peixe->velocidade_luta_min = 0.42;
            peixe->velocidade_luta_max = 1.55;
            break;

        default:
            break;
    }

    /*
        Dificuldade global.
    */
    float d = normalizar_dificuldade(dificuldade);

    /*
        Aumenta zona e velocidade conforme dificuldade.
        dificuldade 1  -> praticamente base
        dificuldade 10 -> bem mais agressivo
    */
    float mult_zona = 1.0 + d * 0.35;
    float mult_velocidade = 1.0 + d * 0.60;

    peixe->largura_zona_luta *= mult_zona;
    peixe->altura_zona_luta *= mult_zona;

    peixe->velocidade_luta_min *= mult_velocidade;
    peixe->velocidade_luta_max *= mult_velocidade;
}

void iniciar_luta_peixe(Peixe *peixe, Isca *isca, short dificuldade) {
    peixe->luta_iniciada = true;

    peixe->tempo_luta = 0.0;
    peixe->tempo_estado_luta = 0.0;

    peixe->centro_luta_x = isca->posicao.x;
    peixe->centro_luta_y = isca->posicao.y;

    configurar_luta_por_tipo(peixe, dificuldade);

    peixe->velocidade_luta = peixe->velocidade_luta_min + 0.4;
    peixe->passo.x = 0.0;
    peixe->passo.y = 0.0;

    escolher_estado_luta(peixe);
    escolher_angulo_luta(peixe, isca);
}

void atualizar_velocidade_luta_peixe(Peixe *peixe, Isca *isca, bool puxando, short dificuldade) {
    float d = normalizar_dificuldade(dificuldade);

    float aceleracao = 0.035;

    if (peixe->estado_luta == LUTA_PIQUE) {
        aceleracao = 0.08 + d * 0.025;
    } else if (peixe->estado_luta == LUTA_CANSADO) {
        aceleracao = 0.025 - d * 0.008;

        if (aceleracao < 0.012) {
            aceleracao = 0.012;
        }
    }

    peixe->velocidade_luta += 
        (peixe->velocidade_luta_alvo - peixe->velocidade_luta) * aceleracao;

    if (puxando) {
        /*
            Quanto maior a dificuldade, menos o jogador freia o peixe.
        */
        float eficiencia_puxada = 0.75 - d * 0.35;

        if (eficiencia_puxada < 0.25) {
            eficiencia_puxada = 0.25;
        }

        peixe->velocidade_luta -= isca->forca_barco * 0.014 * eficiencia_puxada;

        if (peixe->velocidade_luta < peixe->velocidade_luta_min * 0.35) {
            peixe->velocidade_luta = peixe->velocidade_luta_min * 0.35;
        }
    }
}

float calcular_velocidade_giro_luta(Peixe *peixe) {
    if (peixe->estado_luta == LUTA_PIQUE) {
        return 0.028;
    }

    if (peixe->estado_luta == LUTA_CANSADO) {
        return 0.020;
    }

    return 0.024;
}

void aplicar_movimento_luta(Peixe *peixe, Isca *isca, bool puxando, short dificuldade) {
    float d = normalizar_dificuldade(dificuldade);

    float barco_x = TELA_W / 2.0;
    float barco_y = TELA_H / 1.78;

    float peixe_dir_x;
    float peixe_dir_y;

    if (peixe->flip == 1) {
        peixe_dir_x = cos(peixe->angulo_desejado_luta);
        peixe_dir_y = sin(peixe->angulo_desejado_luta);
    } else {
        peixe_dir_x = -cos(peixe->angulo_desejado_luta);
        peixe_dir_y = -sin(peixe->angulo_desejado_luta);
    }

    float dx_barco = barco_x - isca->posicao.x;
    float dy_barco = barco_y - isca->posicao.y;
    float dist_barco = sqrt(dx_barco * dx_barco + dy_barco * dy_barco);

    if (dist_barco < 1.0) {
        dist_barco = 1.0;
    }

    float dir_barco_x = dx_barco / dist_barco;
    float dir_barco_y = dy_barco / dist_barco;

    float forca_peixe = peixe->velocidade_luta * (0.030 + d * 0.012);
    float forca_recolher = 0.0;

    if (peixe->estado_luta == LUTA_PIQUE) {
        forca_peixe *= 1.85;
    } else if (peixe->estado_luta == LUTA_CANSADO) {
        forca_peixe *= 0.42;
    }

    if (puxando) {
        forca_recolher = isca->forca_barco * (0.055 - d * 0.018);

        if (peixe->estado_luta == LUTA_PIQUE) {
            forca_recolher *= 0.18;
        } else if (peixe->estado_luta == LUTA_CANSADO) {
            forca_recolher *= 1.70;
        }
    } else {
        forca_peixe *= 1.55;
    }

    if (isca->posicao.y < SUPERFICIE_AGUA_LUTA + 130.0 && peixe->estado_luta != LUTA_CANSADO) {
        forca_peixe *= 1.45;

        if (peixe_dir_y < 0.35) {
            peixe_dir_y = 0.35;
        }
    }

    float aceleracao_x = peixe_dir_x * forca_peixe + dir_barco_x * forca_recolher;
    float aceleracao_y = peixe_dir_y * forca_peixe + dir_barco_y * forca_recolher;

    if (isca->posicao.y < SUPERFICIE_AGUA_LUTA) {
        aceleracao_y += (SUPERFICIE_AGUA_LUTA - isca->posicao.y) * 0.008;
    }

    if (isca->posicao.y > FUNDO_LUTA) {
        aceleracao_y -= (isca->posicao.y - FUNDO_LUTA) * 0.006;
    }

    if (isca->posicao.x < -MARGEM_LATERAL_LUTA) {
        aceleracao_x += (-MARGEM_LATERAL_LUTA - isca->posicao.x) * 0.004;
    }

    if (isca->posicao.x > TELA_W + MARGEM_LATERAL_LUTA) {
        aceleracao_x -= (isca->posicao.x - (TELA_W + MARGEM_LATERAL_LUTA)) * 0.004;
    }

    peixe->passo.x = (peixe->passo.x + aceleracao_x) * 0.94;
    peixe->passo.y = (peixe->passo.y + aceleracao_y) * 0.94;

    float velocidade_maxima = 7.0 + d * 3.0;
    float velocidade_atual = sqrt(peixe->passo.x * peixe->passo.x + peixe->passo.y * peixe->passo.y);

    if (velocidade_atual > velocidade_maxima) {
        peixe->passo.x = (peixe->passo.x / velocidade_atual) * velocidade_maxima;
        peixe->passo.y = (peixe->passo.y / velocidade_atual) * velocidade_maxima;
    }

    isca->posicao.x += peixe->passo.x;
    isca->posicao.y += peixe->passo.y;

    if (fabs(peixe->passo.x) > 0.08 || fabs(peixe->passo.y) > 0.08) {
        apontar_peixe_para_direcao(peixe, peixe->passo.x, peixe->passo.y);
    }

    peixe->angulo = aproximar_angulo(peixe->angulo, peixe->angulo_alvo, calcular_velocidade_giro_luta(peixe));
}

void virar_peixe_para_barco_se_fraco(Peixe *peixe, Isca *isca) {
    if (peixe->velocidade_luta >= peixe->velocidade_luta_min * 0.75) {
        return;
    }

    float barco_x = TELA_W / 2.0;
    float barco_y = TELA_H / 1.78;

    float dx = barco_x - isca->posicao.x;
    float dy = barco_y - isca->posicao.y;

    if (dx >= 0) {
        peixe->flip = 1;
        peixe->angulo_desejado_luta = atan2(dy, dx);
    } else {
        peixe->flip = -1;
        peixe->angulo_desejado_luta = -atan2(dy, -dx);
    }
}

void aplicar_puxada_barco(Peixe *peixe, Isca *isca, short dificuldade) {
    float d = normalizar_dificuldade(dificuldade);

    float barco_x = TELA_W / 2.0;
    float barco_y = TELA_H / 1.78;

    float dx_barco = barco_x - isca->posicao.x;
    float dy_barco = barco_y - isca->posicao.y;

    float dist_barco = sqrt(dx_barco * dx_barco + dy_barco * dy_barco);

    if (dist_barco <= 1.0) {
        return;
    }

    float forca_puxada = isca->forca_barco * (0.58 - d * 0.22);

    if (peixe->estado_luta == LUTA_CANSADO) {
        forca_puxada *= 1.35;
    }

    if (peixe->estado_luta == LUTA_PIQUE) {
        forca_puxada *= 0.12;
    }

    if (peixe->velocidade_luta < peixe->velocidade_luta_min * 0.75) {
        forca_puxada *= 1.25;
    }

    isca->posicao.x += (dx_barco / dist_barco) * forca_puxada;
    isca->posicao.y += (dy_barco / dist_barco) * forca_puxada;
}

void aplicar_resistencia_peixe(Peixe *peixe, Isca *isca, bool puxando, short dificuldade) {
    float d = normalizar_dificuldade(dificuldade);

    float barco_x = TELA_W / 2.0;
    float barco_y = TELA_H / 1.78;

    float dx = isca->posicao.x - barco_x;
    float dy = isca->posicao.y - barco_y;
    float distancia = sqrt(dx * dx + dy * dy);

    if (distancia <= 1.0) {
        dx = 0.0;
        dy = 1.0;
        distancia = 1.0;
    }

    float dir_x = dx / distancia;
    float dir_y = dy / distancia;

    if (dir_y < 0.25) {
        dir_y = 0.25;
    }

    float tensao = isca->tensao_linha / 100.0;
    float forca = peixe->velocidade_luta * (0.08 + d * 0.05);

    if (puxando) {
        if (peixe->estado_luta == LUTA_PIQUE) {
            forca += 1.30 + d * 1.10 + tensao * 1.20;
        } else if (peixe->estado_luta == LUTA_NORMAL) {
            forca += 0.18 + d * 0.28;
        } else {
            forca += 0.05 + d * 0.08;
        }
    } else {
        if (peixe->estado_luta == LUTA_PIQUE) {
            forca += 2.10 + d * 1.35 + tensao * 1.30;
        } else if (peixe->estado_luta == LUTA_NORMAL) {
            forca += 0.85 + d * 0.55 + tensao * 0.40;
        } else {
            forca += 0.35 + d * 0.25 + tensao * 0.20;
        }
    }

    if (isca->posicao.y < ALTURA_CAPTURA_LUTA + 150.0) {
        forca *= 1.35;
    }

    isca->posicao.x += dir_x * forca * 0.55;
    isca->posicao.y += dir_y * forca;
}

void limitar_zona_luta(Peixe *peixe, Isca *isca, float altura_captura) {
    (void)altura_captura;

    float limite_esq = -MARGEM_LATERAL_LUTA;
    float limite_dir = TELA_W + MARGEM_LATERAL_LUTA;
    float limite_cima = SUPERFICIE_AGUA_LUTA;
    float limite_baixo = FUNDO_LUTA;

    if (isca->posicao.x < limite_esq) {
        isca->posicao.x = limite_esq;
        peixe->passo.x *= -0.25;
    }

    if (isca->posicao.x > limite_dir) {
        isca->posicao.x = limite_dir;
        peixe->passo.x *= -0.25;
    }

    if (isca->posicao.y < limite_cima) {
        isca->posicao.y = limite_cima;
        if (peixe->passo.y < 0.0) {
            peixe->passo.y *= -0.20;
        }
    }

    if (isca->posicao.y > limite_baixo) {
        isca->posicao.y = limite_baixo;
        if (peixe->passo.y > 0.0) {
            peixe->passo.y *= -0.20;
        }
    }
}

void atualizar_tensao_linha(Peixe *peixe, Isca *isca, bool puxando, short dificuldade) {
    float d = normalizar_dificuldade(dificuldade);

    float mult_tensao = 1.0 + d * 0.80;
    float mult_alivio = 1.0 - d * 0.45;
    float bonus_linha = (isca->comprimento_linha - 500.0) / 350.0;

    bonus_linha = limitar_float(bonus_linha, 0.0, 1.0);
    mult_tensao *= 1.0 - bonus_linha * 0.25;
    mult_alivio *= 1.0 + bonus_linha * 0.20;

    if (mult_alivio < 0.55) {
        mult_alivio = 0.55;
    }

    if (puxando) {
        if (peixe->estado_luta == LUTA_PIQUE) {
            isca->tensao_linha += 1.30 * mult_tensao;
        } else if (peixe->estado_luta == LUTA_NORMAL) {
            isca->tensao_linha += 0.42 * mult_tensao;
        } else {
            isca->tensao_linha += 0.18 * mult_tensao;
        }
    } else {
        if (peixe->estado_luta == LUTA_PIQUE) {
            isca->tensao_linha -= 0.95 * mult_alivio;
        } else {
            isca->tensao_linha -= 0.75 * mult_alivio;
        }
    }

    float barco_x = TELA_W / 2.0;
    float barco_y = TELA_H / 1.78;
    float dx_linha = isca->posicao.x - barco_x;
    float dy_linha = isca->posicao.y - barco_y;
    float distancia_linha = sqrt(dx_linha * dx_linha + dy_linha * dy_linha);
    float excesso_linha = distancia_linha - isca->comprimento_linha;

    if (excesso_linha > 0.0) {
        isca->tensao_linha += (excesso_linha / 220.0) * (0.15 + d * 0.15);
    }

    isca->tensao_linha = limitar_float(isca->tensao_linha, 0.0, 100.0);
}

void escolher_estado_luta(Peixe *peixe) {
    /*
        Quanto maior o tempo de luta, mais cansado o peixe fica.
        Vai de 0.0 até 1.0.
    */
    float cansaco = peixe->tempo_luta / 55.0;

    if (cansaco > 1.0) {
        cansaco = 1.0;
    }

    /*
        Chance de pique cai com o tempo.
        No começo: até 42%.
        Depois: mínimo de 18%.
    */
    float chance_pique = 0.42 - (cansaco * 0.22);

    if (chance_pique < 0.18) {
        chance_pique = 0.18;
    }

    /*
        Chance de ficar cansado aumenta com o tempo.
        No começo: 10%.
        Depois: até 40%.
    */
    float chance_cansado = 0.10 + (cansaco * 0.30);

    float sorteio = sortear_float(0.0, 1.0);

    if (sorteio < chance_pique) {
        peixe->estado_luta = LUTA_PIQUE;

        peixe->velocidade_luta_alvo = sortear_float(
            peixe->velocidade_luta_max * 0.75,
            peixe->velocidade_luta_max
        );

        /*
            Piques ficam mais curtos com o tempo.
        */
        peixe->duracao_estado_luta = sortear_float(
            1.2,
            2.3 - cansaco * 0.4
        );

        if (peixe->duracao_estado_luta < 0.9) {
            peixe->duracao_estado_luta = 0.9;
        }

    } else if (sorteio < chance_pique + chance_cansado) {
        peixe->estado_luta = LUTA_CANSADO;

        peixe->velocidade_luta_alvo = sortear_float(
            peixe->velocidade_luta_min * 0.70,
            peixe->velocidade_luta_min + 0.35
        );

        /*
            Cansaço dura mais conforme a luta passa.
        */
        peixe->duracao_estado_luta = sortear_float(
            1.4 + cansaco * 0.7,
            2.6 + cansaco * 1.2
        );

    } else {
        peixe->estado_luta = LUTA_NORMAL;

        float velocidade_normal_min = peixe->velocidade_luta_min + 0.5;
        float velocidade_normal_max = peixe->velocidade_luta_max * 0.75;

        if (velocidade_normal_max < velocidade_normal_min) {
            velocidade_normal_max = velocidade_normal_min + 0.25;
        }

        peixe->velocidade_luta_alvo = sortear_float(velocidade_normal_min, velocidade_normal_max);

        peixe->duracao_estado_luta = sortear_float(1.4, 2.7);
    }

    peixe->tempo_estado_luta = 0.0;
}

bool atualizar_estado_luta_peixe(Peixe *peixe) {
    peixe->tempo_luta += 1.0 / FPS;
    peixe->tempo_estado_luta += 1.0 / FPS;

    if (peixe->tempo_estado_luta >= peixe->duracao_estado_luta) {
        escolher_estado_luta(peixe);
        return true;
    }

    return false;
}

void escolher_angulo_luta(Peixe *peixe, Isca *isca) {
    float barco_x = TELA_W / 2.0;
    float sorteio_direcao = sortear_float(0.0, 1.0);

    if (isca->posicao.y < SUPERFICIE_AGUA_LUTA + 170.0 && peixe->estado_luta != LUTA_CANSADO) {
        if (isca->posicao.x < barco_x) {
            peixe->flip = -1;
            peixe->angulo_desejado_luta = sortear_float(-1.20, -0.45);
        } else {
            peixe->flip = 1;
            peixe->angulo_desejado_luta = sortear_float(0.45, 1.20);
        }

        return;
    }

    if (isca->posicao.y > FUNDO_LUTA - 220.0) {
        if (isca->posicao.x < barco_x) {
            peixe->flip = -1;
            peixe->angulo_desejado_luta = sortear_float(0.20, 0.75);
        } else {
            peixe->flip = 1;
            peixe->angulo_desejado_luta = sortear_float(-0.75, -0.20);
        }

        return;
    }

    if (peixe->estado_luta == LUTA_PIQUE) {
        if (isca->posicao.x < barco_x) {
            peixe->flip = -1;
            peixe->angulo_desejado_luta = sortear_float(-1.05, -0.05);
        } else {
            peixe->flip = 1;
            peixe->angulo_desejado_luta = sortear_float(0.05, 1.05);
        }

        return;
    }

    if (peixe->estado_luta == LUTA_CANSADO) {
        if (sorteio_direcao < 0.5) {
            peixe->flip = 1;
            peixe->angulo_desejado_luta = sortear_float(-0.25, 0.35);
        } else {
            peixe->flip = -1;
            peixe->angulo_desejado_luta = sortear_float(-0.35, 0.25);
        }

        return;
    }

    if (sorteio_direcao < 0.25) {
        peixe->flip = 1;
        peixe->angulo_desejado_luta = sortear_float(0.15, 0.85);
    } else if (sorteio_direcao < 0.50) {
        peixe->flip = -1;
        peixe->angulo_desejado_luta = sortear_float(-0.85, -0.15);
    } else if (sorteio_direcao < 0.75) {
        peixe->flip = 1;
        peixe->angulo_desejado_luta = sortear_float(-0.55, 0.20);
    } else {
        peixe->flip = -1;
        peixe->angulo_desejado_luta = sortear_float(-0.20, 0.55);
    }

    return;

    float limite_esq = peixe->centro_luta_x - peixe->largura_zona_luta;
    float limite_dir = peixe->centro_luta_x + peixe->largura_zona_luta;

    float limite_cima = ALTURA_CAPTURA_LUTA;
    float limite_baixo = peixe->centro_luta_y + peixe->altura_zona_luta;

    /*
        Não deixa a zona sair muito da área útil do mar.
    */
    if (limite_cima < LIMITE_SUPERIOR_LUTA) {
        limite_cima = LIMITE_SUPERIOR_LUTA;
    }

    if (limite_baixo > TELA_H + 650.0) {
        limite_baixo = TELA_H + 650.0;
    }

    /*
        Se chegou perto demais do fundo da zona,
        força um ângulo mais para cima.
    */
    if (isca->posicao.y > limite_baixo - 40.0) {
        if (isca->posicao.x < peixe->centro_luta_x) {
            peixe->flip = 1;
            peixe->angulo_desejado_luta = sortear_float(-0.9, -0.25);
        } else {
            peixe->flip = -1;
            peixe->angulo_desejado_luta = sortear_float(0.25, 0.9);
        }

        return;
    }

    /*
        Se subiu demais, ele tenta descer um pouco.
    */
    if (isca->posicao.y < limite_cima + 180.0) {
        if (isca->posicao.x < peixe->centro_luta_x) {
            peixe->flip = 1;
            peixe->angulo_desejado_luta = sortear_float(0.65, 1.20);
        } else {
            peixe->flip = -1;
            peixe->angulo_desejado_luta = sortear_float(-1.20, -0.65);
        }

        return;
    }

    /*
        Se foi muito para a esquerda, vira para a direita.
    */
    if (isca->posicao.x < limite_esq + 50.0) {
        peixe->flip = 1;
        peixe->angulo_desejado_luta = sortear_float(-0.5, 0.6);
        return;
    }

    /*
        Se foi muito para a direita, vira para a esquerda.
    */
    if (isca->posicao.x > limite_dir - 50.0) {
        peixe->flip = -1;
        peixe->angulo_desejado_luta = sortear_float(-0.6, 0.5);
        return;
    }

    /*
        Movimento livre dentro da zona.
        Aqui ele pode querer subir, descer ou ir mais reto.
    */
    if (sortear_float(0.0, 1.0) < 0.5) {
        peixe->flip = 1;
        peixe->angulo_desejado_luta = sortear_float(-0.65, 0.65);
    } else {
        peixe->flip = -1;
        peixe->angulo_desejado_luta = sortear_float(-0.65, 0.65);
    }
}

short verificar_fim_luta(Peixe *peixe, Isca *isca, float altura_captura) {
    if (isca->tensao_linha >= 100.0) {
        peixe->status = NORMAL;
        peixe->luta_iniciada = false;

        definir_novo_destino_peixe(peixe);

        isca->esta_com_peixe = false;
        isca->tensao_linha = 0.0;

        return LUTA_ROMPEU_LINHA;
    }

    float barco_x = TELA_W / 2.0;
    float distancia_horizontal_barco = fabs(barco_x - isca->posicao.x);
    float velocidade_real = sqrt(peixe->passo.x * peixe->passo.x + peixe->passo.y * peixe->passo.y);

    if (
        isca->posicao.y <= altura_captura &&
        distancia_horizontal_barco <= 230.0 &&
        peixe->estado_luta == LUTA_CANSADO &&
        peixe->tempo_luta >= 4.0 &&
        velocidade_real <= 2.8 &&
        isca->tensao_linha < 70.0
    ) {
        /*
            Não desativa o peixe aqui ainda.
            O main precisa ter chance de salvar esse peixe
            na lista de capturados antes.
        */
        peixe->status = NORMAL;
        peixe->luta_iniciada = false;

        isca->esta_com_peixe = false;
        isca->tensao_linha = 0.0;

        return LUTA_CAPTUROU;
    }

    return LUTA_CONTINUA;
}








