#include "cenario.h"


void desenhar_quadrado_escuro(float y_inicial, float y_final, float largura, double R, double G, double B) {
    ALLEGRO_VERTEX v[4];
    
    // Cor cinza no topo (totalmente transparente)
    ALLEGRO_COLOR cor_topo = al_map_rgba_f(R, G, B, 0.0);
    
    // Cor cinza no fundo (parcialmente transparente)
    ALLEGRO_COLOR cor_fundo = al_map_rgba_f(R, G, B, 0.85);

    // Vértice 0: Superior Esquerdo
    v[0].x = -largura; 
    v[0].y = y_inicial; 
    v[0].z = 0;
    v[0].color = cor_topo;

    // Vértice 1: Superior Direito
    v[1].x = largura; 
    v[1].y = y_inicial; 
    v[1].z = 0;
    v[1].color = cor_topo;

    // Vértice 2: Inferior Direito
    v[2].x = largura; 
    v[2].y = y_final; 
    v[2].z = 0;
    v[2].color = cor_fundo;

    // Vértice 3: Inferior Esquerdo
    v[3].x = -largura; 
    v[3].y = y_final; 
    v[3].z = 0;
    v[3].color = cor_fundo;

    // Desenha o retângulo com o gradiente usando primitivas
    al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);
}

void calcular_oscilacao_barco(float base_y, float tempo, float amp_y, float amp_rotacao, float velocidade, float *out_y, float *out_angulo) {
    
    // Calcula a posição Y (sobe e desce)
    *out_y = base_y + (sin(tempo * velocidade) * amp_y);
    
    // Calcula o ângulo (inclinação da frente). 
    // uso cos() para que a inclinação acompanhe a curvatura da onda.
    *out_angulo = cos(tempo * velocidade) * amp_rotacao;
}


void resetar_camera(Camera *camera) {
    camera->alvo.x = TELA_W / 2.0;
    camera->alvo.y = TELA_H / 2.0;

    camera->atual.x = TELA_W / 2.0;
    camera->atual.y = TELA_H / 2.0;

    camera->zoom_alvo = 1.0;
    camera->zoom_atual = 1.0;
}


