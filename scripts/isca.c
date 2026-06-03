#include "isca.h"


void inicializar_isca(Isca *isca) {
    // Nasce no centro da tela
    isca->posicao.x = TELA_W / 2.0;
    isca->posicao.y = TELA_H - 250;
    isca->tipo = VELHA;
    isca->tamanho = 0.5;
    isca->angulo = PI/4;
    isca->vel_angulo = 0.05;
    isca->velocidade = 1.5;
    isca->esta_com_peixe = false;
    isca->upgrade_atracao = 1.0;
    isca->forca_barco = 3.0;
    isca->comprimento_linha = 500.0; 
    isca->tensao_linha = 0.0;
}


void desenhar_isca(Isca *isca, ALLEGRO_BITMAP* spr_iscas[]) {
    ALLEGRO_BITMAP* sprite = spr_iscas[isca->tipo];
    
    if (sprite) {
        float centro_x = al_get_bitmap_width(sprite) / 2.0;
        float centro_y = al_get_bitmap_height(sprite) / 2.0;

        al_draw_scaled_rotated_bitmap(
            sprite,
            centro_x, centro_y,
            isca->posicao.x, isca->posicao.y,
            isca->tamanho, isca->tamanho,
            isca->angulo,
            0 
        );
    }
}

void atualizar_isca(Isca *isca, bool virar_esquerda, bool virar_direita, bool cima) {
    float barco_x = (TELA_W / 2.0) + 105;
    float barco_y = (TELA_H / 1.78) + 158;

    if (virar_esquerda) {
        isca->angulo += isca->vel_angulo;
    }
    if (virar_direita) {
        isca->angulo -= isca->vel_angulo;
    } 
    if (cima){
        float dx_barco = barco_x - isca->posicao.x;
        float dy_barco = barco_y - isca->posicao.y;
        float dist_barco = sqrt(dx_barco * dx_barco + dy_barco * dy_barco);

        if (dist_barco > 1.0) {
            isca->posicao.x += (dx_barco / dist_barco) * 2.0 * isca->velocidade;
            isca->posicao.y += (dy_barco / dist_barco) * 2.0 * isca->velocidade;
        }
    }
    

    // Trava a rotação da sprite para que ela não dê uma volta 
    if (isca->angulo < 0) 
        isca->angulo = 0;
    else if (isca->angulo > PI / 2) 
        isca->angulo = PI / 2;

    float angulo_movimento = isca->angulo + (PI / 4.0);

    // Aplica a trigonometria na direção(direita ou esquerda)
    isca->posicao.x += cos(angulo_movimento) * isca->velocidade;
    isca->posicao.y += sin(angulo_movimento) * isca->velocidade;
}



void desenhar_linha_pesca(float barco_x, float barco_y, float barco_angulo, Isca *isca) {
    
    
    float ajuste_barco_x = 105.0; 
    float ajuste_barco_y = 158.0; 

    float ponta_barco_x = barco_x + (ajuste_barco_x * cos(barco_angulo) - ajuste_barco_y * sin(barco_angulo));
    float ponta_barco_y = barco_y + (ajuste_barco_x * sin(barco_angulo) + ajuste_barco_y * cos(barco_angulo));

    
    float base_ajuste_isca_x = -19.5; 
    float base_ajuste_isca_y = -20.0; 
    
    float ajuste_isca_x = base_ajuste_isca_x * isca->tamanho;
    float ajuste_isca_y = base_ajuste_isca_y * isca->tamanho;

    float engate_isca_x = isca->posicao.x + (ajuste_isca_x * cos(isca->angulo) - ajuste_isca_y * sin(isca->angulo));
    float engate_isca_y = isca->posicao.y + (ajuste_isca_x * sin(isca->angulo) + ajuste_isca_y * cos(isca->angulo));


    float nivel_linha = (isca->comprimento_linha - 500.0) / 70.0;
    nivel_linha = limitar_float(nivel_linha, 0.0, 5.0);

    unsigned char r = (unsigned char)(35 + nivel_linha * 18);
    unsigned char g = (unsigned char)(35 + nivel_linha * 28);
    unsigned char b = (unsigned char)(35 + nivel_linha * 36);

    ALLEGRO_COLOR cor_linha = al_map_rgba(r, g, b, 170);
    float espessura = 1.2 + nivel_linha * 0.28;

    al_draw_line(ponta_barco_x, ponta_barco_y, engate_isca_x, engate_isca_y, cor_linha, espessura);
}

void desenhar_barra_tensao(Isca *isca) {
    float x = 40.0;
    float y = 40.0;
    float largura = 300.0;
    float altura = 24.0;

    float porcentagem = isca->tensao_linha / 100.0;

    if (porcentagem < 0.0) porcentagem = 0.0;
    if (porcentagem > 1.0) porcentagem = 1.0;

    ALLEGRO_COLOR fundo = al_map_rgb(40, 40, 40);
    ALLEGRO_COLOR borda = al_map_rgb(255, 255, 255);

    ALLEGRO_COLOR cor;

    if (isca->tensao_linha < 50.0) {
        cor = al_map_rgb(80, 220, 80);
    } else if (isca->tensao_linha < 80.0) {
        cor = al_map_rgb(240, 210, 70);
    } else {
        cor = al_map_rgb(240, 70, 70);
    }

    al_draw_filled_rectangle(x, y, x + largura, y + altura, fundo);
    al_draw_filled_rectangle(x, y, x + largura * porcentagem, y + altura, cor);
    al_draw_rectangle(x, y, x + largura, y + altura, borda, 2.0);
}
