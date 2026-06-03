// Incluindo os módulos
#include "utils.h"
#include "cenario.h"
#include "isca.h"
#include "peixe.h"
#include "menus.h"
#include "save.h"

#define CAMERA_MIN_X (TELA_W / 2.0)
#define CAMERA_MAX_X (TELA_W * 1.5)
#define CAMERA_MIN_Y (TELA_H / 2.0)
#define CAMERA_MAX_Y (TELA_H * 2)


int main() {

    // Inicializa a semente aleatória com o relógio do computador
    srand(time(NULL));

    // Inicializações básicas
    if (!al_init()) {
        printf("Falha ao inicializar o Allegro!\n");
        return -1;
    }

    if (!al_init_font_addon()) {
        printf("Falha ao inicializar o addon de fonte!\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        printf("Erro ao inicializar add-on de imagem!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        printf("Erro ao inicializar teclado!\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        printf("Erro ao inicializar primitivas!\n");
        return -1;
    }

    if (!al_install_mouse()) {
        printf("Falha ao inicializar o mouse!\n");
        return -1;
    }
    

    // Criação do timer (Configura o jogo para rodar a 60 quadros por segundo)
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        printf("Falha ao criar o timer!\n");
        return -1;
    }

    // cria a janela
    ALLEGRO_DISPLAY* display = al_create_display(TELA_W, TELA_H);
    if (!display) {
        printf("Falha ao criar a janela!\n");
        return -1;
    }
    
    // carrega a fonte, para adição de texto
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) {
        printf("Falha ao criar a fonte!\n");
        return -1;
    }

    // Carregamento de Sprites
    ALLEGRO_BITMAP* spr_back_mar = al_load_bitmap("../sprites/spr_back_mar.png");
    if (!spr_back_mar) {
        printf("Erro ao carregar spr_back_mar!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_barco = al_load_bitmap("../sprites/spr_barco.png");
    if (!spr_barco) {
        printf("Erro ao carregar spr_barco!\n");
        return -1;
    }

    ALLEGRO_BITMAP* spr_mar = al_load_bitmap("../sprites/spr_mar.png");
    if (!spr_mar) {
        printf("Erro ao carregar spr_mar!\n");
        return -1;
    }

    // Carregamento dos sprites dos peixes em um vetor usando o enum
    ALLEGRO_BITMAP* spr_peixes[TOTAL_TIPOS_PEIXES];
    spr_peixes[BAGRE_CANAL] = al_load_bitmap("../sprites/spr_bagre_canal.png");
    spr_peixes[BAGRE_CABECA_CHATA] = al_load_bitmap("../sprites/spr_bagre_cabeca_chata.png");
    spr_peixes[MUSKALONGE] = al_load_bitmap("../sprites/spr_muskie.png");
    spr_peixes[WALLEYE] = al_load_bitmap("../sprites/spr_walleye.png");
    spr_peixes[PERCA_AMARELA] = al_load_bitmap("../sprites/spr_perca_amarela.png");
    spr_peixes[RUTIO] = al_load_bitmap("../sprites/spr_rutilo.png");
    spr_peixes[ESTURJAO] = al_load_bitmap("../sprites/spr_esturjao.png");
    spr_peixes[LUCIO_PERCA] = al_load_bitmap("../sprites/spr_lucio_perca.png");
    spr_peixes[SALMAO_VERMELHO] = al_load_bitmap("../sprites/spr_salmao_vermelho.png");
    spr_peixes[LINGUADO] = al_load_bitmap("../sprites/spr_linguado.png");
    spr_peixes[PEIXE_GATO] = al_load_bitmap("../sprites/spr_peixe_gato.png");
    spr_peixes[ACHIGA] = al_load_bitmap("../sprites/spr_achiga.png");
    spr_peixes[CAVALINHA] = al_load_bitmap("../sprites/spr_cavalinha.png");



    for(int i = 0; i < TOTAL_TIPOS_PEIXES; i++) {
        if (!spr_peixes[i]) {
            printf("Erro ao carregar sprite do peixe %d!\n", i);
            return -1;
        }
    }

    // Carregamento dos sprites dos peixes em um vetor usando o enum
    ALLEGRO_BITMAP* spr_iscas[TOTAL_TIPOS_ISCAS];
    spr_iscas[VELHA] = al_load_bitmap("../sprites/spr_isca_0.png");
    spr_iscas[BOIA] = al_load_bitmap("../sprites/spr_isca_1.png");
    spr_iscas[ESVERDEADA] = al_load_bitmap("../sprites/spr_isca_2.png");
    spr_iscas[REALISTA] = al_load_bitmap("../sprites/spr_isca_3.png");
    spr_iscas[PEQUENO_PEIXE] = al_load_bitmap("../sprites/spr_isca_4.png");

    for(int i = 0; i < TOTAL_TIPOS_ISCAS; i++) {
        if (!spr_iscas[i]) {
            printf("Erro ao carregar sprite do peixe %d!\n", i);
            return -1;
        }
    }

    ALLEGRO_BITMAP* spr_icones_menu[TOTAL_ICONES_MENU];

    spr_icones_menu[ICONE_ENGRENAGEM] = al_load_bitmap("../sprites/spr_icone_engrenagem.png");
    spr_icones_menu[ICONE_LOJA] = al_load_bitmap("../sprites/spr_icon_loja.png");
    spr_icones_menu[ICONE_LIVRO_FECHADO] = al_load_bitmap("../sprites/spr_livro_fechado.png");
    spr_icones_menu[ICONE_LIVRO_ABERTO] = al_load_bitmap("../sprites/spr_livro_aberto.png");

    if (!spr_icones_menu[ICONE_ENGRENAGEM]) {
        printf("Erro ao carregar sprite do icone de menu %d!\n", ICONE_ENGRENAGEM);
        return -1;
    }

    if (!spr_icones_menu[ICONE_LOJA]) {
        printf("Aviso: spr_icon_loja.png nao encontrado. A loja usara fallback visual.\n");
    }

    if (!spr_icones_menu[ICONE_LIVRO_FECHADO] || !spr_icones_menu[ICONE_LIVRO_ABERTO]) {
        printf("Aviso: sprites do livro nao encontrados. O diario usara fallback visual.\n");
    }



    // Fila de Eventos
    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        printf("Falha ao criar a fila de eventos!\n");
        return -1;
    }

    // Registros na fila
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());


    // Inicia o timer antes de entrar no loop
    al_start_timer(timer);

    // Varieaveis
    bool rodando = true;
    bool redesenhar = true;
    bool flag_isca = false;
    float barco_y = 0.0;
    float barco_angulo = 0.0;
    float clk_barco = 0.0;

    short dificuldade_jogo = 5;

    NoPeixe* topo_cardume = NULL;
    Isca isca;
    Camera camera;
    bool teclas[TOTAL_TECLAS] = { false };

    int capacidade_peixes_capturados = 8;
    PeixeCapturado peixes_capturados[MAX_PEIXES_CAPTURADOS];

    inicializar_lista_capturados(peixes_capturados, MAX_PEIXES_CAPTURADOS);

    camera.atual.x = TELA_W / 2.0;
    camera.atual.y = TELA_H / 2.0;
    camera.alvo.x = TELA_W / 2.0;
    camera.alvo.y = TELA_H / 2.0;
    camera.zoom_atual = 1.0;
    camera.zoom_alvo = 1.0;
    camera.vel_suavizacao = 0.05; 

    empilhar_peixes(&topo_cardume, WALLEYE, 15);
    empilhar_peixes(&topo_cardume, BAGRE_CANAL, 3);
    empilhar_peixes(&topo_cardume, BAGRE_CABECA_CHATA, 2);
    empilhar_peixes(&topo_cardume, MUSKALONGE, 2);
    empilhar_peixes(&topo_cardume, PERCA_AMARELA, 10);
    empilhar_peixes(&topo_cardume, LINGUADO, 2);
    empilhar_peixes(&topo_cardume, SALMAO_VERMELHO, 3);
    empilhar_peixes(&topo_cardume, RUTIO, 2);
    empilhar_peixes(&topo_cardume, ESTURJAO, 3);
    empilhar_peixes(&topo_cardume, PEIXE_GATO, 4);
    empilhar_peixes(&topo_cardume, ACHIGA, 2);
    empilhar_peixes(&topo_cardume, CAVALINHA, 4);

    Menus menus;
    inicializar_menus(&menus);

    if (!carregar_save(CAMINHO_SAVE, &menus, peixes_capturados, capacidade_peixes_capturados)) {
        printf("Aviso: nao foi possivel carregar ou criar o arquivo de save.\n");
    }


    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        // Verifica os eventos
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            redesenhar = true;
            clk_barco += 0.05;

            Peixe *peixe_fisgado = atualizar_status_peixes(topo_cardume, &isca, flag_isca);

            if (flag_isca) {
                if (peixe_fisgado != NULL) {
                    short resultado_luta = processar_luta_pesca(peixe_fisgado, &isca, teclas[CIMA], dificuldade_jogo);

                    if (resultado_luta != LUTA_CONTINUA) {
                        if (resultado_luta == LUTA_CAPTUROU) {
                            registrar_peixe_no_diario(&menus, peixe_fisgado);

                            bool salvou = adicionar_peixe_capturado(
                                peixes_capturados,
                                capacidade_peixes_capturados,
                                peixe_fisgado
                            );
                            if (salvou) {
                                peixe_fisgado->ativo = false;
                            } else {
                                peixe_fisgado->ativo = true;
                                peixe_fisgado->status = NORMAL;
                                definir_novo_destino_peixe(peixe_fisgado);
                            }
                        }

                        flag_isca = false;
                        isca.esta_com_peixe = false;
                        isca.tensao_linha = 0.0;

                        resetar_camera(&camera);
                    }
                } else {
                    atualizar_isca(&isca, teclas[ESQUERDA], teclas[DIREITA], teclas[CIMA]);
                }

                if (flag_isca) {
                    camera.alvo.x = isca.posicao.x;
                    camera.alvo.y = isca.posicao.y;
                    camera.zoom_alvo = 2.5;
                }
            } else {
                camera.alvo.x = TELA_W / 2.0;
                camera.alvo.y = TELA_H / 2.0;
                camera.zoom_alvo = 1.0;
            }

            // Aplica a Interpolação para andar um pouquinho por frame
            camera.alvo.x = limitar_float(camera.alvo.x, CAMERA_MIN_X, CAMERA_MAX_X);
            camera.alvo.y = limitar_float(camera.alvo.y, CAMERA_MIN_Y, CAMERA_MAX_Y);

            camera.atual.x += (camera.alvo.x - camera.atual.x) * camera.vel_suavizacao;
            camera.atual.y += (camera.alvo.y - camera.atual.y) * camera.vel_suavizacao;
            camera.zoom_atual += (camera.zoom_alvo - camera.zoom_atual) * camera.vel_suavizacao;

            camera.atual.x = limitar_float(camera.atual.x, CAMERA_MIN_X, CAMERA_MAX_X);
            camera.atual.y = limitar_float(camera.atual.y, CAMERA_MIN_Y, CAMERA_MAX_Y);
                     
        } 
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            rodando = false;
        } 
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (evento.mouse.button == 1) {
                atualizar_mouse_menus(&menus, evento.mouse.x, evento.mouse.y);
                processar_clique_menus(&menus, evento.mouse.x, evento.mouse.y, &dificuldade_jogo);
                processar_clique_painel_loja(&menus, evento.mouse.x, evento.mouse.y, peixes_capturados, capacidade_peixes_capturados);
                processar_clique_painel_diario(&menus, evento.mouse.x, evento.mouse.y);
            }
        }

        if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
            atualizar_mouse_menus(&menus, evento.mouse.x, evento.mouse.y);
        }
        
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                rodando = false;
            }

            if (evento.keyboard.keycode == ALLEGRO_KEY_M) {
                menus.aberto = !menus.aberto;
            }

            // Marca a tecla como VERDADEIRA, pois foi precionada
            if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                if (!flag_isca && !menus.aberto && !menus.loja_aberta && !menus.diario_aberto) {
                    flag_isca = true;
                    inicializar_isca(&isca);
                    aplicar_upgrades_loja_na_isca(&menus, &isca);
                }
            } 
            else if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT) 
                teclas[ESQUERDA] = true;
            else if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
                teclas[DIREITA] = true;
            else if(evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
                teclas[CIMA] = true;
            
        }
        if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            // Marca a tecla como FALSA, pois foi solta
            if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT) 
                teclas[ESQUERDA] = false;
            else if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
                teclas[DIREITA] = false;
            else if(evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
                teclas[CIMA] = false;
        }

        // Só desenha se o clk autorizou
        if (redesenhar) {
            redesenhar = false; // Desmarca após desenhar

            // Configura e aplica a câmera
            ALLEGRO_TRANSFORM allegro_camera;
            al_identity_transform(&allegro_camera);

            // Configura e aplica a câmera unificada e suavizada
            ALLEGRO_TRANSFORM trans_camera;
            al_identity_transform(&trans_camera);

            // move o ponto atual para o centro da origem
            al_translate_transform(&trans_camera, -camera.atual.x, -camera.atual.y);
            // aplica o zoom atual
            al_scale_transform(&trans_camera, camera.zoom_atual, camera.zoom_atual);
            // move o mundo de volta para o centro da tela
            al_translate_transform(&trans_camera, TELA_W / 2.0, TELA_H / 2.0);
            
            al_use_transform(&trans_camera);

            // calcula a rotação e posição y do barco
            calcular_oscilacao_barco(TELA_H / 1.78, clk_barco, 15.0, 0.05, 1.0, &barco_y, &barco_angulo);

            al_clear_to_color(al_map_rgb(55,192,183));

            al_draw_scaled_rotated_bitmap(spr_back_mar, 0, 0, 0, -200, 1.25, 1.25, 0, 0);
            al_draw_scaled_rotated_bitmap(spr_barco, 0, 0, TELA_W/2, barco_y, 0.29, 0.29, barco_angulo, 0);
            
            if (flag_isca) {
                desenhar_isca(&isca, spr_iscas);
                desenhar_linha_pesca(TELA_W / 2.0, barco_y, barco_angulo, &isca);
            }

            desenhar_peixes(topo_cardume, spr_peixes);

            desenhar_quadrado_escuro(TELA_H + 200, 2*TELA_H, 2*TELA_W, 0.01, 0.01, 0.01);

            // Desenha o mar da frente usando a cor com transparência
            ALLEGRO_COLOR cor_transparente = al_map_rgba_f(1.0, 1.0, 1.0, 0.7);
            al_draw_tinted_scaled_rotated_bitmap(spr_mar, cor_transparente, 0, 0, 0, TELA_H/1.21, 2.7, 2.7, 0, 0);
            al_draw_tinted_scaled_rotated_bitmap(spr_mar, cor_transparente, 0, 0, 576*2.7, TELA_H/1.21, 2.7, 2.7, 0, 0);

            ALLEGRO_TRANSFORM sem_camera;
            al_identity_transform(&sem_camera);
            al_use_transform(&sem_camera);

            if (flag_isca && isca.esta_com_peixe) {
                desenhar_barra_tensao(&isca);
            }

            if (!flag_isca) {
                desenhar_peixes_capturados(peixes_capturados, capacidade_peixes_capturados, spr_peixes);
            }

            desenhar_icone_menu(&menus, spr_icones_menu);
            desenhar_menus(&menus, font, dificuldade_jogo);
            desenhar_painel_loja(&menus, font, peixes_capturados, capacidade_peixes_capturados);
            desenhar_painel_diario(&menus, font);

            al_flip_display();
        }
    }

    // Limpeza de memória
    if (!salvar_jogo(CAMINHO_SAVE, &menus, peixes_capturados, capacidade_peixes_capturados)) {
        printf("Aviso: nao foi possivel salvar o progresso.\n");
    }

    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(spr_mar);
    al_destroy_bitmap(spr_barco);
    al_destroy_bitmap(spr_back_mar);
    for(int i = 0; i < TOTAL_TIPOS_PEIXES; i++) {
        if (spr_peixes[i] != NULL) {
            al_destroy_bitmap(spr_peixes[i]);
        }
    }
    for(int i = 0; i < TOTAL_TIPOS_ISCAS; i++) {
        if (spr_iscas[i] != NULL) {
            al_destroy_bitmap(spr_iscas[i]);
        }
    }
    for (int i = 0; i < TOTAL_ICONES_MENU; i++) {
        if (spr_icones_menu[i] != NULL) {
            al_destroy_bitmap(spr_icones_menu[i]);
        }
    }
    al_destroy_font(font);
    al_destroy_display(display);

    limpar_pilha_peixes(&topo_cardume);
    

    return 0;
}
