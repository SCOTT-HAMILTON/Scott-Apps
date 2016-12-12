#include <stdio.h>
#include <SDL_image.h>
#include <SDL.h>
#include "Racket.h"
#include "Racket.c"
#include "Constantes.h"
#include <stdlib.h>

int main(int argc, char *argv[]){
    SDL_Surface *fenetre = NULL, *Racket[NBR_JOUEURS] = {NULL};
    SDL_Event event, tempEvent2, tempEvent1;SDL_Rect posRacket[NBR_JOUEURS];
    SDL_Surface *LimiteGauche = NULL, *LimiteDroite = NULL;
    SDL_Rect posLimitGauche, posLimitDroite, posMilieuTerrain;
    SDL_Surface *Balle = NULL;SDL_Rect posBalle;
    SDL_Surface *milieuTerrain, *Perdu, *icon, *menu;SDL_Rect posPerdu, posMenu;

    int continuer = 1;int SpeedRacket = 1;
    int directionBalle;int VitesseBalle = 3;
    int tempsActuel, tempsPrecedent;int ticks = 0;
    int typeRebond = 0;int joueur1Appuyer = 0;
    int joueur2Appuyer = 0;

    int timeFirstKeydown1 = 0;int Menu = 1;
    int timeFirstKeydown2 = 0;int mode = 0;

    SDL_Init(SDL_INIT_VIDEO);

    icon = IMG_Load("balle.bmp");
    SDL_SetColorKey(icon, SDL_SRCCOLORKEY, SDL_MapRGB(icon->format, 255, 255, 255));
    SDL_WM_SetIcon(icon, NULL);

    fenetre = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Pong game!",NULL);
    SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255));

    //init Surface
    Racket[JOUEUR_1] = SDL_LoadBMP("Racket.bmp");
    Racket[JOUEUR_2] = SDL_LoadBMP("Racket.bmp");
    Balle = IMG_Load("balle.bmp");
    SDL_SetColorKey(Racket[JOUEUR_1], SDL_SRCCOLORKEY, SDL_MapRGB(Racket[JOUEUR_1]->format, 255, 0, 0));
    SDL_SetColorKey(Racket[JOUEUR_2], SDL_SRCCOLORKEY, SDL_MapRGB(Racket[JOUEUR_2]->format, 255, 0, 0));
    SDL_SetColorKey(Balle, SDL_SRCCOLORKEY, SDL_MapRGB(Balle->format, 255, 255, 255));
    LimiteGauche = SDL_CreateRGBSurface(SDL_HWSURFACE, ECART_RACKET - ECART_LIMITE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
    LimiteDroite = SDL_CreateRGBSurface(SDL_HWSURFACE, ECART_RACKET - ECART_LIMITE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
    milieuTerrain = SDL_CreateRGBSurface(SDL_HWSURFACE, 6, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
    Perdu = SDL_LoadBMP("Perdu.bmp");
    menu = SDL_LoadBMP("imgMenu.bmp");

    //init direction
    initDirectBalle(&directionBalle);

    //init posSurface
    posRacket[JOUEUR_1].x = ECART_RACKET;
    posRacket[JOUEUR_1].y = (fenetre->h / 2) - (Racket[JOUEUR_1]->h / 2);

    posRacket[JOUEUR_2].x = (fenetre->w - ECART_RACKET) - Racket[JOUEUR_2]->w;
    posRacket[JOUEUR_2].y = (fenetre->h / 2) - (Racket[JOUEUR_2]->h / 2);

    posLimitGauche.x = 0;
    posLimitGauche.y = 0;

    posLimitDroite.x = (fenetre->w - LimiteDroite->w);
    posLimitDroite.y = 0;

    posBalle.x = (fenetre->w / 2) - (Balle->w / 2);
    posBalle.y = (fenetre->h / 2) - (Balle->h / 2);

    posMilieuTerrain.x = (fenetre->w / 2) - (milieuTerrain->w / 2);
    posMilieuTerrain.y = 0;

    posPerdu.x = 0;
    posPerdu.y = 0;

    posMenu.x = 0;
    posMenu.y = 0;

    //init couleur surface
    SDL_FillRect(LimiteGauche, NULL, SDL_MapRGB(LimiteGauche->format, 219, 20, 32));
    SDL_FillRect(LimiteDroite, NULL, SDL_MapRGB(LimiteDroite->format, 219, 20, 32));

    //Blit init
    SDL_BlitSurface(menu, NULL, fenetre, &posMenu);

    while (continuer){
        if (Menu){
            SDL_BlitSurface(menu, NULL, fenetre, &posMenu);
            SDL_WaitEvent(&event);
            switch(event.type){
            case SDL_QUIT:
                continuer = 0;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym){
                case SDLK_KP1:
                    getMode_1direction(&directionBalle);
                    mode = 1;
                    Menu = 0;
                    break;
                case SDLK_KP2:
                    mode = 2;
                    Menu = 0;
                    break;
                case SDLK_1:
                    mode = 1;
                    Menu = 0;
                    break;
                case SDLK_2:
                    mode = 2;
                    Menu = 0;
                    break;
                case SDLK_ESCAPE:
                    continuer = 0;

                default:
                    ;
                }
            }
        }
        else{
            if (directionBalle == 0 || directionBalle == 180){
                directionBalle += 1;
            }
            SDL_PollEvent(&event);
            switch (event.type){
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_a){
                    getRacketPos(posRacket, SpeedRacket, &event);
                    joueur1Appuyer = 0;
                    timeFirstKeydown1 = SDL_GetTicks();
                }

                if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_UP){
                    getRacketPos(posRacket, SpeedRacket, &event);
                    joueur2Appuyer = 0;
                    timeFirstKeydown2 = SDL_GetTicks();
                }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q){
                    getRacketPos(posRacket, SpeedRacket, &event);
                    joueur1Appuyer = 1;
                    tempEvent1 = event;
                }

                 if ( event.key.keysym.sym == SDLK_a){
                    getRacketPos(posRacket, SpeedRacket, &event);
                    joueur1Appuyer = 1;
                    tempEvent1 = event;
                 }

                if (event.key.keysym.sym == SDLK_DOWN){
                    getRacketPos(posRacket, SpeedRacket, &event);
                    joueur2Appuyer = 1;
                    tempEvent2 = event;

                }

                if (event.key.keysym.sym == SDLK_UP){
                    getRacketPos(posRacket, SpeedRacket, &event);
                    joueur2Appuyer = 1;
                    tempEvent2 = event;
                }
                if (event.key.keysym.sym == SDLK_SPACE){
                    Menu = 1;
                }
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    continuer = 0;
                }
                break;
            }
            if (joueur1Appuyer && SDL_GetTicks() - timeFirstKeydown1 >= 20){
                getRacketPos(posRacket, SpeedRacket, &tempEvent1);
            }
            if (joueur2Appuyer && SDL_GetTicks() - timeFirstKeydown2 >= 20){
                getRacketPos(posRacket, SpeedRacket, &tempEvent2);
            }

            ticks = SDL_GetTicks();
            tempsActuel = SDL_GetTicks();
            if ((tempsActuel - 10) >= tempsPrecedent){
                tempsPrecedent = tempsActuel;
            }else{
                SDL_Delay(10);
            }
            if (ticks % 5 == 0){
                getDirection(directionBalle, &posBalle, VitesseBalle);
                typeRebond = getBallePos(directionBalle, posBalle, posRacket, posLimitGauche, posLimitDroite, mode);
                if (typeRebond == 1);
                else{
                    if (typeRebond == 0){
                        getRebondRaquette(&directionBalle, posRacket, posBalle);
                    }
                    if (typeRebond == -1){
                        getRebondBalle(&directionBalle);
                    }
                    if (typeRebond == -2){
                        posBalle.x = (fenetre->w / 2) - (Balle->w / 2);
                        posBalle.y = (fenetre->h / 2) - (Balle->h / 2);
                        SDL_BlitSurface(Perdu, NULL, fenetre, &posPerdu);
                        SDL_Flip(fenetre);
                        SDL_Delay(2000);
                        initDirectBalle(&directionBalle);
                    }
                    if (typeRebond == -3){
                        getRebondMode_1(&directionBalle);
                    }
                }
            SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255));
            SDL_BlitSurface(milieuTerrain, NULL, fenetre, &posMilieuTerrain);
            SDL_BlitSurface(Racket[JOUEUR_1], NULL, fenetre, &posRacket[JOUEUR_1]);
            SDL_BlitSurface(Racket[JOUEUR_2], NULL, fenetre, &posRacket[JOUEUR_2]);
            SDL_BlitSurface(LimiteGauche,NULL, fenetre, &posLimitGauche);
            SDL_BlitSurface(LimiteDroite, NULL, fenetre, &posLimitDroite);
            SDL_BlitSurface(Balle, NULL, fenetre, &posBalle);

            }

        }
        SDL_Flip(fenetre);
    }

    SDL_FreeSurface(Racket[JOUEUR_1]);
    SDL_FreeSurface(Racket[JOUEUR_2]);
    SDL_FreeSurface(LimiteDroite);
    SDL_FreeSurface(LimiteGauche);
    SDL_FreeSurface(Balle);
    SDL_FreeSurface(milieuTerrain);
    SDL_FreeSurface(Perdu);
    SDL_FreeSurface(menu);
    SDL_FreeSurface(icon);

    SDL_Quit();
    return EXIT_SUCCESS;
}
