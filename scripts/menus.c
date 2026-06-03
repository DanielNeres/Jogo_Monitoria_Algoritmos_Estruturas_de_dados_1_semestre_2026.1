#include "menus.h"
#include <string.h>

#define ESCALA_TITULO_MENU 3.0
#define ESCALA_TEXTO_MENU 1.8
#define ESCALA_VALOR_MENU 3.0
#define ESCALA_BOTAO_MENU 2.0
#define ESCALA_TEXTO_AUXILIAR_MENU 1.6

static void desenhar_texto_menu_escalado(ALLEGRO_FONT *font, ALLEGRO_COLOR cor, float x, float y, int alinhamento, const char *texto,float escala) {
    ALLEGRO_TRANSFORM transform_antigo;
    ALLEGRO_TRANSFORM transform_texto;

    al_copy_transform(&transform_antigo, al_get_current_transform());
    al_identity_transform(&transform_texto);
    al_scale_transform(&transform_texto, escala, escala);
    al_use_transform(&transform_texto);

    al_draw_text(font, cor, x / escala, y / escala, alinhamento, texto);

    al_use_transform(&transform_antigo);
}

static float calcular_y_texto_centralizado(ALLEGRO_FONT *font, float y1, float y2, float escala) {
    return ((y1 + y2) / 2.0) - ((al_get_font_line_height(font) * escala) / 2.0);
}

static int custo_upgrade(int nivel_atual, int custo_base) {
    if (nivel_atual >= NIVEL_UPGRADE_MAX) {
        return 0;
    }

    return custo_base + nivel_atual * custo_base;
}

static const char *nome_isca(short tipo) {
    switch (tipo) {
        case VELHA:
            return "Velha";
        case BOIA:
            return "Boia";
        case ESVERDEADA:
            return "Esverdeada";
        case REALISTA:
            return "Realista";
        case PEQUENO_PEIXE:
            return "Peixe pequeno";
        default:
            return "Isca";
    }
}

static int custo_isca(short tipo) {
    switch (tipo) {
        case VELHA:
            return 0;
        case BOIA:
            return 120;
        case ESVERDEADA:
            return 240;
        case REALISTA:
            return 420;
        case PEQUENO_PEIXE:
            return 650;
        default:
            return 0;
    }
}

static const char *nome_peixe(short tipo) {
    switch (tipo) {
        case BAGRE_CANAL:
            return "Bagre canal";
        case BAGRE_CABECA_CHATA:
            return "Bagre cabeca chata";
        case MUSKALONGE:
            return "Muskalonge";
        case WALLEYE:
            return "Walleye";
        case PERCA_AMARELA:
            return "Perca amarela";
        case RUTIO:
            return "Rutilo";
        case ESTURJAO:
            return "Esturjao";
        case LUCIO_PERCA:
            return "Lucio perca";
        case SALMAO_VERMELHO:
            return "Salmao vermelho";
        case LINGUADO:
            return "Linguado";
        case PEIXE_GATO:
            return "Peixe gato";
        case ACHIGA:
            return "Achiga";
        case CAVALINHA:
            return "Cavalinha";
        default:
            return "Peixe";
    }
}

static int preco_base_peixe(short tipo) {
    switch (tipo) {
        case RUTIO:
            return 14;
        case CAVALINHA:
            return 24;
        case PERCA_AMARELA:
            return 28;
        case PEIXE_GATO:
            return 34;
        case LINGUADO:
            return 40;
        case WALLEYE:
            return 46;
        case BAGRE_CANAL:
            return 52;
        case BAGRE_CABECA_CHATA:
            return 64;
        case ACHIGA:
            return 66;
        case LUCIO_PERCA:
            return 72;
        case SALMAO_VERMELHO:
            return 86;
        case ESTURJAO:
            return 140;
        case MUSKALONGE:
            return 180;
        default:
            return 20;
    }
}

static int comparar_diario(Menus *menu, short a, short b) {
    if (menu->ordem_diario == ORDEM_DIARIO_PRECO) {
        return preco_base_peixe(a) > preco_base_peixe(b);
    }

    if (menu->ordem_diario == ORDEM_DIARIO_QUANTIDADE) {
        return menu->quantidade_capturada[a] > menu->quantidade_capturada[b];
    }

    return strcmp(nome_peixe(a), nome_peixe(b)) > 0;
}

static void ordenar_diario_insertion_sort(Menus *menu, short ordem[], int total) {
    for (int i = 1; i < total; i++) {
        short chave = ordem[i];
        int j = i - 1;

        while (j >= 0 && comparar_diario(menu, ordem[j], chave)) {
            ordem[j + 1] = ordem[j];
            j--;
        }

        ordem[j + 1] = chave;
    }
}

static void desenhar_botao_texto(float x1, float y1, float x2, float y2, const char *texto, ALLEGRO_FONT *font, bool ativo) {
    ALLEGRO_COLOR cor_botao = ativo ? al_map_rgb(238, 202, 120) : al_map_rgb(172, 150, 118);
    ALLEGRO_COLOR cor_borda = al_map_rgb(120, 85, 40);
    ALLEGRO_COLOR cor_texto = ativo ? al_map_rgb(70, 45, 20) : al_map_rgb(92, 78, 62);

    al_draw_filled_rounded_rectangle(x1, y1, x2, y2, 10.0, 10.0, cor_botao);
    al_draw_rounded_rectangle(x1, y1, x2, y2, 10.0, 10.0, cor_borda, 2.0);

    float texto_y = calcular_y_texto_centralizado(font, y1, y2, ESCALA_BOTAO_MENU);
    desenhar_texto_menu_escalado(font, cor_texto, (x1 + x2) / 2.0, texto_y, ALLEGRO_ALIGN_CENTER, texto, ESCALA_BOTAO_MENU);
}

void inicializar_menus(Menus *menu) {
    menu->aberto = false;
    menu->loja_aberta = false;
    menu->diario_aberto = false;

    menu->dinheiro = 0;
    menu->nivel_atracao = 0;
    menu->nivel_forca = 0;
    menu->nivel_linha = 0;
    menu->isca_selecionada = VELHA;
    menu->ordem_diario = ORDEM_DIARIO_NOME;
    menu->encomenda_tipo = PERCA_AMARELA;
    menu->encomenda_recompensa = 90;
    menu->mouse_x = 0.0;
    menu->mouse_y = 0.0;

    for (int i = 0; i < TOTAL_TIPOS_ISCAS; i++) {
        menu->isca_comprada[i] = false;
    }

    menu->isca_comprada[VELHA] = true;

    for (int i = 0; i < TOTAL_TIPOS_PEIXES; i++) {
        menu->peixe_descoberto[i] = false;
        menu->quantidade_capturada[i] = 0;
        menu->maior_tamanho[i] = 0.0;
        menu->melhor_venda[i] = 0;
    }

    menu->icone_tamanho = 48.0;
    menu->icone_x = TELA_W - 60.0;
    menu->icone_y = 50.0;

    menu->icone_loja_tamanho = 48.0;
    menu->icone_loja_x = menu->icone_x;
    menu->icone_loja_y = menu->icone_y + 70.0;

    menu->icone_livro_tamanho = 112.0;
    menu->icone_livro_x = 92.0;
    menu->icone_livro_y = TELA_H - 92.0;

    menu->painel_x1 = TELA_W / 2.0 - 560.0;
    menu->painel_y1 = TELA_H / 2.0 - 385.0;
    menu->painel_x2 = TELA_W / 2.0 + 560.0;
    menu->painel_y2 = TELA_H / 2.0 + 385.0;

    menu->fechar_x1 = menu->painel_x2 - 55.0;
    menu->fechar_y1 = menu->painel_y1 + 20.0;
    menu->fechar_x2 = menu->painel_x2 - 20.0;
    menu->fechar_y2 = menu->painel_y1 + 55.0;

    float centro_y = menu->painel_y1 + 230.0;

    menu->botao_menos_x1 = TELA_W / 2.0 - 150.0;
    menu->botao_menos_y1 = centro_y - 35.0;
    menu->botao_menos_x2 = TELA_W / 2.0 - 80.0;
    menu->botao_menos_y2 = centro_y + 35.0;

    menu->botao_mais_x1 = TELA_W / 2.0 + 80.0;
    menu->botao_mais_y1 = centro_y - 35.0;
    menu->botao_mais_x2 = TELA_W / 2.0 + 150.0;
    menu->botao_mais_y2 = centro_y + 35.0;

    float coluna_esq_x1 = menu->painel_x1 + 70.0;
    float coluna_esq_x2 = menu->painel_x1 + 505.0;
    float coluna_dir_x1 = menu->painel_x1 + 615.0;
    float coluna_dir_x2 = menu->painel_x2 - 70.0;

    menu->botao_vender_x1 = coluna_esq_x1;
    menu->botao_vender_y1 = menu->painel_y1 + 150.0;
    menu->botao_vender_x2 = coluna_esq_x2;
    menu->botao_vender_y2 = menu->painel_y1 + 205.0;

    menu->botao_upgrade_atracao_x1 = coluna_esq_x1;
    menu->botao_upgrade_atracao_y1 = menu->painel_y1 + 285.0;
    menu->botao_upgrade_atracao_x2 = coluna_esq_x2;
    menu->botao_upgrade_atracao_y2 = menu->painel_y1 + 335.0;

    menu->botao_upgrade_forca_x1 = coluna_esq_x1;
    menu->botao_upgrade_forca_y1 = menu->painel_y1 + 350.0;
    menu->botao_upgrade_forca_x2 = coluna_esq_x2;
    menu->botao_upgrade_forca_y2 = menu->painel_y1 + 400.0;

    menu->botao_upgrade_linha_x1 = coluna_esq_x1;
    menu->botao_upgrade_linha_y1 = menu->painel_y1 + 415.0;
    menu->botao_upgrade_linha_x2 = coluna_esq_x2;
    menu->botao_upgrade_linha_y2 = menu->painel_y1 + 465.0;

    for (int i = 0; i < TOTAL_TIPOS_ISCAS; i++) {
        menu->botao_isca_x1[i] = coluna_dir_x1;
        menu->botao_isca_y1[i] = menu->painel_y1 + 150.0 + i * 62.0;
        menu->botao_isca_x2[i] = coluna_dir_x2;
        menu->botao_isca_y2[i] = menu->botao_isca_y1[i] + 48.0;
    }

    menu->botao_entregar_x1 = coluna_esq_x1;
    menu->botao_entregar_y1 = menu->painel_y1 + 555.0;
    menu->botao_entregar_x2 = coluna_esq_x2;
    menu->botao_entregar_y2 = menu->painel_y1 + 615.0;

    menu->botao_ordem_nome_x1 = menu->painel_x1 + 70.0;
    menu->botao_ordem_nome_y1 = menu->painel_y1 + 105.0;
    menu->botao_ordem_nome_x2 = menu->painel_x1 + 260.0;
    menu->botao_ordem_nome_y2 = menu->painel_y1 + 150.0;

    menu->botao_ordem_preco_x1 = menu->painel_x1 + 285.0;
    menu->botao_ordem_preco_y1 = menu->painel_y1 + 105.0;
    menu->botao_ordem_preco_x2 = menu->painel_x1 + 475.0;
    menu->botao_ordem_preco_y2 = menu->painel_y1 + 150.0;

    menu->botao_ordem_quantidade_x1 = menu->painel_x1 + 500.0;
    menu->botao_ordem_quantidade_y1 = menu->painel_y1 + 105.0;
    menu->botao_ordem_quantidade_x2 = menu->painel_x1 + 760.0;
    menu->botao_ordem_quantidade_y2 = menu->painel_y1 + 150.0;
}

static void desenhar_icone(float x, float y, float tamanho, ALLEGRO_BITMAP *sprite) {
    if (sprite != NULL) {
        float centro_x = al_get_bitmap_width(sprite) / 2.0;
        float centro_y = al_get_bitmap_height(sprite) / 2.0;
        float escala_x = tamanho / al_get_bitmap_width(sprite);
        float escala_y = tamanho / al_get_bitmap_height(sprite);

        al_draw_scaled_rotated_bitmap(sprite, centro_x, centro_y, x, y, escala_x, escala_y, 0.0, 0);
        return;
    }

    al_draw_filled_rectangle(x - tamanho / 2.0, y - tamanho / 2.0, x + tamanho / 2.0, y + tamanho / 2.0, al_map_rgb(230, 205, 135));
    al_draw_rectangle(x - tamanho / 2.0, y - tamanho / 2.0, x + tamanho / 2.0, y + tamanho / 2.0, al_map_rgb(90, 65, 35), 2.0);
}

void desenhar_icone_menu(Menus *menu, ALLEGRO_BITMAP *spr_icones_menu[]) {
    desenhar_icone(menu->icone_x, menu->icone_y, menu->icone_tamanho, spr_icones_menu[ICONE_ENGRENAGEM]);
    desenhar_icone(menu->icone_loja_x, menu->icone_loja_y, menu->icone_loja_tamanho, spr_icones_menu[ICONE_LOJA]);

    if (mouse_sobre_icone_livro(menu, menu->mouse_x, menu->mouse_y)) {
        desenhar_icone(menu->icone_livro_x, menu->icone_livro_y, menu->icone_livro_tamanho, spr_icones_menu[ICONE_LIVRO_ABERTO]);
    } else {
        desenhar_icone(menu->icone_livro_x, menu->icone_livro_y, menu->icone_livro_tamanho, spr_icones_menu[ICONE_LIVRO_FECHADO]);
    }
}

static void desenhar_fundo_painel(Menus *menu, const char *titulo, ALLEGRO_FONT *font) {
    ALLEGRO_COLOR cor_sombra = al_map_rgba_f(0.0, 0.0, 0.0, 0.35);
    ALLEGRO_COLOR cor_painel = al_map_rgb(244, 222, 164);
    ALLEGRO_COLOR cor_borda = al_map_rgb(115, 82, 43);
    ALLEGRO_COLOR cor_texto = al_map_rgb(65, 43, 20);
    ALLEGRO_COLOR cor_x = al_map_rgb(130, 55, 35);

    al_draw_filled_rectangle(0, 0, TELA_W, TELA_H, cor_sombra);
    al_draw_filled_rounded_rectangle(menu->painel_x1, menu->painel_y1, menu->painel_x2, menu->painel_y2, 20.0, 20.0, cor_painel);
    al_draw_rounded_rectangle(menu->painel_x1, menu->painel_y1, menu->painel_x2, menu->painel_y2, 20.0, 20.0, cor_borda, 4.0);

    desenhar_texto_menu_escalado(font, cor_texto, TELA_W / 2.0, menu->painel_y1 + 28.0, ALLEGRO_ALIGN_CENTER, titulo, ESCALA_TITULO_MENU);

    al_draw_filled_rounded_rectangle(menu->fechar_x1, menu->fechar_y1, menu->fechar_x2, menu->fechar_y2, 8.0, 8.0, al_map_rgb(247, 204, 145));
    al_draw_rounded_rectangle(menu->fechar_x1, menu->fechar_y1, menu->fechar_x2, menu->fechar_y2, 8.0, 8.0, cor_borda, 2.0);

    float x_centro = (menu->fechar_x1 + menu->fechar_x2) / 2.0;
    float y_centro = (menu->fechar_y1 + menu->fechar_y2) / 2.0;
    al_draw_line(x_centro - 8.0, y_centro - 8.0, x_centro + 8.0, y_centro + 8.0, cor_x, 3.0);
    al_draw_line(x_centro + 8.0, y_centro - 8.0, x_centro - 8.0, y_centro + 8.0, cor_x, 3.0);
}

void desenhar_menus(Menus *menu, ALLEGRO_FONT *font, short dificuldade) {
    if (!menu->aberto) {
        return;
    }

    ALLEGRO_COLOR cor_texto = al_map_rgb(65, 43, 20);

    desenhar_fundo_painel(menu, "Menu", font);

    desenhar_texto_menu_escalado(font, cor_texto, TELA_W / 2.0, menu->painel_y1 + 135.0, ALLEGRO_ALIGN_CENTER, "Dificuldade da pesca", ESCALA_TEXTO_MENU);

    char texto_dificuldade[32];
    sprintf(texto_dificuldade, "%d", dificuldade);

    al_draw_filled_circle(TELA_W / 2.0, menu->painel_y1 + 230.0, 36.0, al_map_rgb(250, 226, 150));
    al_draw_circle(TELA_W / 2.0, menu->painel_y1 + 230.0, 36.0, al_map_rgb(115, 82, 43), 3.0);

    float dificuldade_y = calcular_y_texto_centralizado(font, menu->painel_y1 + 194.0, menu->painel_y1 + 266.0, ESCALA_VALOR_MENU);
    desenhar_texto_menu_escalado(font, cor_texto, TELA_W / 2.0, dificuldade_y, ALLEGRO_ALIGN_CENTER, texto_dificuldade, ESCALA_VALOR_MENU);

    desenhar_botao_texto(menu->botao_menos_x1, menu->botao_menos_y1, menu->botao_menos_x2, menu->botao_menos_y2, "-", font, true);
    desenhar_botao_texto(menu->botao_mais_x1, menu->botao_mais_y1, menu->botao_mais_x2, menu->botao_mais_y2, "+", font, true);
    desenhar_texto_menu_escalado(font, al_map_rgb(100, 70, 35), TELA_W / 2.0, menu->painel_y2 - 80.0, ALLEGRO_ALIGN_CENTER, "1 = facil     10 = dificil", ESCALA_TEXTO_AUXILIAR_MENU);
}

int calcular_valor_peixes_capturados(PeixeCapturado capturados[], int capacidade) {
    int total = 0;

    for (int i = 0; i < capacidade; i++) {
        if (capturados[i].ocupado) {
            total += capturados[i].preco_venda;
        }
    }

    return total;
}

static void desenhar_linha_upgrade(
    Menus *menu,
    ALLEGRO_FONT *font,
    const char *nome,
    int nivel,
    int custo,
    float x1,
    float y1,
    float x2,
    float y2
) {
    char texto[128];
    bool maximo = nivel >= NIVEL_UPGRADE_MAX;
    bool pode_comprar = !maximo && menu->dinheiro >= custo;

    if (maximo) {
        sprintf(texto, "%s  Nv %d/%d  MAX", nome, nivel, NIVEL_UPGRADE_MAX);
    } else {
        sprintf(texto, "%s  Nv %d/%d  $%d", nome, nivel, NIVEL_UPGRADE_MAX, custo);
    }

    desenhar_botao_texto(x1, y1, x2, y2, texto, font, maximo || pode_comprar);
}

static void desenhar_linha_isca(Menus *menu, ALLEGRO_FONT *font, short tipo) {
    char texto[128];
    bool comprada = menu->isca_comprada[tipo];
    bool selecionada = menu->isca_selecionada == tipo;
    int custo = custo_isca(tipo);
    bool ativo = comprada || menu->dinheiro >= custo;

    if (selecionada) {
        sprintf(texto, "%s  Selecionada", nome_isca(tipo));
    } else if (comprada) {
        sprintf(texto, "%s  Usar", nome_isca(tipo));
    } else {
        sprintf(texto, "%s  Comprar $%d", nome_isca(tipo), custo);
    }

    desenhar_botao_texto(
        menu->botao_isca_x1[tipo],
        menu->botao_isca_y1[tipo],
        menu->botao_isca_x2[tipo],
        menu->botao_isca_y2[tipo],
        texto,
        font,
        ativo
    );
}

void desenhar_painel_loja(Menus *menu, ALLEGRO_FONT *font, PeixeCapturado capturados[], int capacidade) {
    if (!menu->loja_aberta) {
        return;
    }

    ALLEGRO_COLOR cor_texto = al_map_rgb(65, 43, 20);
    ALLEGRO_COLOR cor_linha = al_map_rgb(145, 103, 52);
    int valor_peixes = calcular_valor_peixes_capturados(capturados, capacidade);
    char texto[128];

    desenhar_fundo_painel(menu, "Loja", font);

    sprintf(texto, "Dinheiro: $%d", menu->dinheiro);
    desenhar_texto_menu_escalado(font, cor_texto, menu->painel_x1 + 70.0, menu->painel_y1 + 92.0, ALLEGRO_ALIGN_LEFT, texto, ESCALA_TEXTO_MENU);
    desenhar_texto_menu_escalado(font, cor_texto, menu->painel_x1 + 615.0, menu->painel_y1 + 92.0, ALLEGRO_ALIGN_LEFT, "Iscas", ESCALA_TEXTO_MENU);
    al_draw_line(menu->painel_x1 + 560.0, menu->painel_y1 + 105.0, menu->painel_x1 + 560.0, menu->painel_y2 - 70.0, cor_linha, 2.0);

    sprintf(texto, "Vender peixes: $%d", valor_peixes);
    desenhar_botao_texto(menu->botao_vender_x1, menu->botao_vender_y1, menu->botao_vender_x2, menu->botao_vender_y2, texto, font, valor_peixes > 0);

    desenhar_texto_menu_escalado(font, cor_texto, menu->painel_x1 + 70.0, menu->painel_y1 + 250.0, ALLEGRO_ALIGN_LEFT, "Melhorias", ESCALA_TEXTO_AUXILIAR_MENU);
    desenhar_linha_upgrade(menu, font, "Atracao", menu->nivel_atracao, custo_upgrade(menu->nivel_atracao, 80), menu->botao_upgrade_atracao_x1, menu->botao_upgrade_atracao_y1, menu->botao_upgrade_atracao_x2, menu->botao_upgrade_atracao_y2);
    desenhar_linha_upgrade(menu, font, "Recolher", menu->nivel_forca, custo_upgrade(menu->nivel_forca, 110), menu->botao_upgrade_forca_x1, menu->botao_upgrade_forca_y1, menu->botao_upgrade_forca_x2, menu->botao_upgrade_forca_y2);
    desenhar_linha_upgrade(menu, font, "Linha", menu->nivel_linha, custo_upgrade(menu->nivel_linha, 95), menu->botao_upgrade_linha_x1, menu->botao_upgrade_linha_y1, menu->botao_upgrade_linha_x2, menu->botao_upgrade_linha_y2);

    for (short i = 0; i < TOTAL_TIPOS_ISCAS; i++) {
        desenhar_linha_isca(menu, font, i);
    }

    desenhar_texto_menu_escalado(font, cor_texto, menu->painel_x1 + 70.0, menu->painel_y1 + 520.0, ALLEGRO_ALIGN_LEFT, "Encomenda especial", ESCALA_TEXTO_AUXILIAR_MENU);
    sprintf(texto, "Encomenda: %s  $%d", nome_peixe(menu->encomenda_tipo), menu->encomenda_recompensa);
    desenhar_botao_texto(menu->botao_entregar_x1, menu->botao_entregar_y1, menu->botao_entregar_x2, menu->botao_entregar_y2, texto, font, true);
}

void desenhar_painel_diario(Menus *menu, ALLEGRO_FONT *font) {
    if (!menu->diario_aberto) {
        return;
    }

    short ordem[TOTAL_TIPOS_PEIXES];
    char texto[64];
    ALLEGRO_COLOR cor_texto = al_map_rgb(65, 43, 20);
    ALLEGRO_COLOR cor_apagado = al_map_rgb(115, 95, 70);
    ALLEGRO_COLOR cor_linha = al_map_rgb(145, 103, 52);
    float x_nome = menu->painel_x1 + 70.0;
    float x_base = menu->painel_x1 + 430.0;
    float x_qtd = menu->painel_x1 + 585.0;
    float x_maior = menu->painel_x1 + 710.0;
    float x_melhor = menu->painel_x1 + 900.0;
    float y_cabecalho = menu->painel_y1 + 180.0;
    float y_primeira_linha = menu->painel_y1 + 220.0;
    float escala_linha = 1.3;

    for (short i = 0; i < TOTAL_TIPOS_PEIXES; i++) {
        ordem[i] = i;
    }

    ordenar_diario_insertion_sort(menu, ordem, TOTAL_TIPOS_PEIXES);
    desenhar_fundo_painel(menu, "Diario", font);

    desenhar_botao_texto(menu->botao_ordem_nome_x1, menu->botao_ordem_nome_y1, menu->botao_ordem_nome_x2, menu->botao_ordem_nome_y2, "Nome", font, menu->ordem_diario == ORDEM_DIARIO_NOME);
    desenhar_botao_texto(menu->botao_ordem_preco_x1, menu->botao_ordem_preco_y1, menu->botao_ordem_preco_x2, menu->botao_ordem_preco_y2, "Preco", font, menu->ordem_diario == ORDEM_DIARIO_PRECO);
    desenhar_botao_texto(menu->botao_ordem_quantidade_x1, menu->botao_ordem_quantidade_y1, menu->botao_ordem_quantidade_x2, menu->botao_ordem_quantidade_y2, "Quantidade", font, menu->ordem_diario == ORDEM_DIARIO_QUANTIDADE);

    desenhar_texto_menu_escalado(font, cor_texto, x_nome, y_cabecalho, ALLEGRO_ALIGN_LEFT, "Peixe", ESCALA_TEXTO_AUXILIAR_MENU);
    desenhar_texto_menu_escalado(font, cor_texto, x_base, y_cabecalho, ALLEGRO_ALIGN_LEFT, "Base", ESCALA_TEXTO_AUXILIAR_MENU);
    desenhar_texto_menu_escalado(font, cor_texto, x_qtd, y_cabecalho, ALLEGRO_ALIGN_LEFT, "Qtd", ESCALA_TEXTO_AUXILIAR_MENU);
    desenhar_texto_menu_escalado(font, cor_texto, x_maior, y_cabecalho, ALLEGRO_ALIGN_LEFT, "Maior", ESCALA_TEXTO_AUXILIAR_MENU);
    desenhar_texto_menu_escalado(font, cor_texto, x_melhor, y_cabecalho, ALLEGRO_ALIGN_LEFT, "Melhor", ESCALA_TEXTO_AUXILIAR_MENU);
    al_draw_line(x_nome, y_cabecalho + 34.0, menu->painel_x2 - 70.0, y_cabecalho + 34.0, cor_linha, 2.0);

    for (int i = 0; i < TOTAL_TIPOS_PEIXES; i++) {
        short tipo = ordem[i];
        float y = y_primeira_linha + i * 35.0;
        ALLEGRO_COLOR cor_linha_texto = menu->peixe_descoberto[tipo] ? cor_texto : cor_apagado;

        if (menu->peixe_descoberto[tipo]) {
            desenhar_texto_menu_escalado(font, cor_linha_texto, x_nome, y, ALLEGRO_ALIGN_LEFT, nome_peixe(tipo), escala_linha);
        } else {
            desenhar_texto_menu_escalado(font, cor_linha_texto, x_nome, y, ALLEGRO_ALIGN_LEFT, "?????", escala_linha);
        }

        sprintf(texto, "$%d", preco_base_peixe(tipo));
        desenhar_texto_menu_escalado(font, cor_linha_texto, x_base, y, ALLEGRO_ALIGN_LEFT, texto, escala_linha);

        sprintf(texto, "%d", menu->peixe_descoberto[tipo] ? menu->quantidade_capturada[tipo] : 0);
        desenhar_texto_menu_escalado(font, cor_linha_texto, x_qtd, y, ALLEGRO_ALIGN_LEFT, texto, escala_linha);

        if (menu->peixe_descoberto[tipo]) {
            sprintf(texto, "%.2f", menu->maior_tamanho[tipo]);
        } else {
            sprintf(texto, "-");
        }
        desenhar_texto_menu_escalado(font, cor_linha_texto, x_maior, y, ALLEGRO_ALIGN_LEFT, texto, escala_linha);

        if (menu->peixe_descoberto[tipo]) {
            sprintf(texto, "$%d", menu->melhor_venda[tipo]);
        } else {
            sprintf(texto, "-");
        }
        desenhar_texto_menu_escalado(font, cor_linha_texto, x_melhor, y, ALLEGRO_ALIGN_LEFT, texto, escala_linha);
    }
}

bool mouse_em_retangulo(float mouse_x, float mouse_y, float x1, float y1, float x2, float y2) {
    return mouse_x >= x1 && mouse_x <= x2 && mouse_y >= y1 && mouse_y <= y2;
}

bool mouse_sobre_icone_menu(Menus *menu, float mouse_x, float mouse_y) {
    float metade = menu->icone_tamanho / 2.0;
    return mouse_em_retangulo(mouse_x, mouse_y, menu->icone_x - metade, menu->icone_y - metade, menu->icone_x + metade, menu->icone_y + metade);
}

bool mouse_sobre_icone_loja(Menus *menu, float mouse_x, float mouse_y) {
    float metade = menu->icone_loja_tamanho / 2.0;
    return mouse_em_retangulo(mouse_x, mouse_y, menu->icone_loja_x - metade, menu->icone_loja_y - metade, menu->icone_loja_x + metade, menu->icone_loja_y + metade);
}

bool mouse_sobre_icone_livro(Menus *menu, float mouse_x, float mouse_y) {
    float metade = menu->icone_livro_tamanho / 2.0;
    return mouse_em_retangulo(mouse_x, mouse_y, menu->icone_livro_x - metade, menu->icone_livro_y - metade, menu->icone_livro_x + metade, menu->icone_livro_y + metade);
}

bool mouse_sobre_x_menu(Menus *menu, float mouse_x, float mouse_y) {
    if (!menu->aberto && !menu->loja_aberta && !menu->diario_aberto) {
        return false;
    }

    return mouse_em_retangulo(mouse_x, mouse_y, menu->fechar_x1, menu->fechar_y1, menu->fechar_x2, menu->fechar_y2);
}

void processar_clique_menus(Menus *menu, float mouse_x, float mouse_y, short *dificuldade) {
    if (menu == NULL || dificuldade == NULL) {
        return;
    }

    if (menu->aberto) {
        if (mouse_sobre_x_menu(menu, mouse_x, mouse_y)) {
            menu->aberto = false;
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_menos_x1, menu->botao_menos_y1, menu->botao_menos_x2, menu->botao_menos_y2)) {
            if (*dificuldade > DIFICULDADE_MIN) {
                (*dificuldade)--;
            }
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_mais_x1, menu->botao_mais_y1, menu->botao_mais_x2, menu->botao_mais_y2)) {
            if (*dificuldade < DIFICULDADE_MAX) {
                (*dificuldade)++;
            }
            return;
        }

        return;
    }

    if (!menu->loja_aberta && !menu->diario_aberto && mouse_sobre_icone_menu(menu, mouse_x, mouse_y)) {
        menu->aberto = true;
    }
}

static void ordenar_capturados_por_tipo_insertion_sort(PeixeCapturado capturados[], int capacidade) {
    for (int i = 1; i < capacidade; i++) {
        PeixeCapturado chave = capturados[i];
        int j = i - 1;

        while (j >= 0 && chave.ocupado && (!capturados[j].ocupado || capturados[j].tipo > chave.tipo)
        ) {
            capturados[j + 1] = capturados[j];
            j--;
        }

        capturados[j + 1] = chave;
    }
}

static int buscar_peixe_capturado_binario(PeixeCapturado capturados[], int capacidade, short tipo) {
    int inicio = 0;
    int fim = capacidade - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        if (!capturados[meio].ocupado || capturados[meio].tipo > tipo) {
            fim = meio - 1;
        } else if (capturados[meio].tipo < tipo) {
            inicio = meio + 1;
        } else {
            return meio;
        }
    }

    return -1;
}

static void gerar_nova_encomenda(Menus *menu) {
    short opcoes[] = {
        PERCA_AMARELA,
        PEIXE_GATO,
        CAVALINHA,
        WALLEYE,
        ACHIGA,
        LUCIO_PERCA,
        BAGRE_CANAL,
        SALMAO_VERMELHO,
        ESTURJAO,
        MUSKALONGE
    };
    int total_opcoes = sizeof(opcoes) / sizeof(opcoes[0]);
    short tipo = opcoes[rand() % total_opcoes];

    menu->encomenda_tipo = tipo;
    menu->encomenda_recompensa = (int)(preco_base_peixe(tipo) * 1.85) + 30;
}

static void entregar_encomenda(Menus *menu, PeixeCapturado capturados[], int capacidade) {
    ordenar_capturados_por_tipo_insertion_sort(capturados, capacidade);

    int indice = buscar_peixe_capturado_binario(capturados, capacidade, menu->encomenda_tipo);

    if (indice < 0) {
        return;
    }

    capturados[indice].tipo = -1;
    capturados[indice].tamanho = 1.0;
    capturados[indice].preco_venda = 0;
    capturados[indice].ocupado = false;

    menu->dinheiro += menu->encomenda_recompensa;
    gerar_nova_encomenda(menu);
}

static void vender_peixes(Menus *menu, PeixeCapturado capturados[], int capacidade) {
    int valor = calcular_valor_peixes_capturados(capturados, capacidade);

    if (valor <= 0) {
        return;
    }

    menu->dinheiro += valor;

    for (int i = 0; i < capacidade; i++) {
        capturados[i].tipo = -1;
        capturados[i].tamanho = 1.0;
        capturados[i].preco_venda = 0;
        capturados[i].ocupado = false;
    }
}

static void comprar_upgrade(int *nivel, int custo_base, int *dinheiro) {
    int custo = custo_upgrade(*nivel, custo_base);

    if (*nivel >= NIVEL_UPGRADE_MAX || *dinheiro < custo) {
        return;
    }

    *dinheiro -= custo;
    (*nivel)++;
}

static void comprar_ou_selecionar_isca(Menus *menu, short tipo) {
    int custo = custo_isca(tipo);

    if (tipo < 0 || tipo >= TOTAL_TIPOS_ISCAS) {
        return;
    }

    if (!menu->isca_comprada[tipo]) {
        if (menu->dinheiro < custo) {
            return;
        }

        menu->dinheiro -= custo;
        menu->isca_comprada[tipo] = true;
    }

    menu->isca_selecionada = tipo;
}

void processar_clique_painel_loja(Menus *menu, float mouse_x, float mouse_y, PeixeCapturado capturados[], int capacidade) {
    if (menu == NULL) {
        return;
    }

    if (menu->loja_aberta) {
        if (mouse_sobre_x_menu(menu, mouse_x, mouse_y)) {
            menu->loja_aberta = false;
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_vender_x1, menu->botao_vender_y1, menu->botao_vender_x2, menu->botao_vender_y2)) {
            vender_peixes(menu, capturados, capacidade);
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_entregar_x1, menu->botao_entregar_y1, menu->botao_entregar_x2, menu->botao_entregar_y2)) {
            entregar_encomenda(menu, capturados, capacidade);
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_upgrade_atracao_x1, menu->botao_upgrade_atracao_y1, menu->botao_upgrade_atracao_x2, menu->botao_upgrade_atracao_y2)) {
            comprar_upgrade(&menu->nivel_atracao, 80, &menu->dinheiro);
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_upgrade_forca_x1, menu->botao_upgrade_forca_y1, menu->botao_upgrade_forca_x2, menu->botao_upgrade_forca_y2)) {
            comprar_upgrade(&menu->nivel_forca, 110, &menu->dinheiro);
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_upgrade_linha_x1, menu->botao_upgrade_linha_y1, menu->botao_upgrade_linha_x2, menu->botao_upgrade_linha_y2)) {
            comprar_upgrade(&menu->nivel_linha, 95, &menu->dinheiro);
            return;
        }

        for (short i = 0; i < TOTAL_TIPOS_ISCAS; i++) {
            if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_isca_x1[i], menu->botao_isca_y1[i], menu->botao_isca_x2[i], menu->botao_isca_y2[i])) {
                comprar_ou_selecionar_isca(menu, i);
                return;
            }
        }

        return;
    }

    if (!menu->aberto && !menu->diario_aberto && mouse_sobre_icone_loja(menu, mouse_x, mouse_y)) {
        menu->loja_aberta = true;
    }
}

void processar_clique_painel_diario(Menus *menu, float mouse_x, float mouse_y) {
    if (menu == NULL) {
        return;
    }

    if (menu->diario_aberto) {
        if (mouse_sobre_x_menu(menu, mouse_x, mouse_y)) {
            menu->diario_aberto = false;
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_ordem_nome_x1, menu->botao_ordem_nome_y1, menu->botao_ordem_nome_x2, menu->botao_ordem_nome_y2)) {
            menu->ordem_diario = ORDEM_DIARIO_NOME;
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_ordem_preco_x1, menu->botao_ordem_preco_y1, menu->botao_ordem_preco_x2, menu->botao_ordem_preco_y2)) {
            menu->ordem_diario = ORDEM_DIARIO_PRECO;
            return;
        }

        if (mouse_em_retangulo(mouse_x, mouse_y, menu->botao_ordem_quantidade_x1, menu->botao_ordem_quantidade_y1, menu->botao_ordem_quantidade_x2, menu->botao_ordem_quantidade_y2)) {
            menu->ordem_diario = ORDEM_DIARIO_QUANTIDADE;
            return;
        }

        return;
    }

    if (!menu->aberto && !menu->loja_aberta && mouse_sobre_icone_livro(menu, mouse_x, mouse_y)) {
        menu->diario_aberto = true;
    }
}

void aplicar_upgrades_loja_na_isca(Menus *menu, Isca *isca) {
    if (menu == NULL || isca == NULL) {
        return;
    }

    isca->upgrade_atracao = 1.0 + menu->nivel_atracao * 0.18;
    isca->forca_barco = 3.0 + menu->nivel_forca * 0.32;
    isca->comprimento_linha = 500.0 + menu->nivel_linha * 70.0;
    isca->tipo = menu->isca_selecionada;
}

void atualizar_mouse_menus(Menus *menu, float mouse_x, float mouse_y) {
    if (menu == NULL) {
        return;
    }

    menu->mouse_x = mouse_x;
    menu->mouse_y = mouse_y;
}

void registrar_peixe_no_diario(Menus *menu, Peixe *peixe) {
    if (menu == NULL || peixe == NULL || peixe->tipo < 0 || peixe->tipo >= TOTAL_TIPOS_PEIXES) {
        return;
    }

    menu->peixe_descoberto[peixe->tipo] = true;
    menu->quantidade_capturada[peixe->tipo]++;

    if (peixe->tamanho > menu->maior_tamanho[peixe->tipo]) {
        menu->maior_tamanho[peixe->tipo] = peixe->tamanho;
    }

    int venda = (int)(peixe->preco * (0.75 + peixe->tamanho * 0.25));

    if (venda > menu->melhor_venda[peixe->tipo]) {
        menu->melhor_venda[peixe->tipo] = venda;
    }
}
