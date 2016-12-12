#include <stdio.h>
#include <SDL.h>
#include "Racket.h"
#include <time.h>
#include <stdlib.h>
#include "Constantes.h"

void initDirectBalle(int* aleatoire){
    srand(time(NULL));
    *aleatoire = rand()%(360-0) + 0;
}

void getDirection(int direction, SDL_Rect *position, int vitesse){
    if (direction > 0 && direction < 90){
        if(direction < 45){
            position->y -= vitesse;
            position->x += vitesse / 2;
        }
        if (direction > 45){
            position->y -= vitesse / 2;
            position->x += vitesse;
        }
    }

    if (direction > 90 && direction < 180){
        if (direction < 135){
            position->x += vitesse;
            position->y += vitesse / 2;
        }
        if (direction > 135){
            position->x += vitesse / 2;
            position->y += vitesse;
        }
    }

    if (direction > 180 && direction < 270){
        if (direction < 225){
            position->x -= vitesse / 2;
            position->y += vitesse;
        }
        if (direction > 225){
            position->x -= vitesse;
            position->y += vitesse / 2;
        }
    }

    if (direction > 270 && direction < 360){
        if (direction < 315){
            position->x -= vitesse;
            position->y -= vitesse / 2;
        }
        if (direction > 315){
            position->x -= vitesse / 2;
            position->y -= vitesse;
        }
    }

    if (direction == 45) {
        position->y -= vitesse;
        position->x += vitesse;
    }
    if (direction == 135){
        position->x += vitesse;
        position->y += vitesse;
    }
    if (direction == 225){
        position->x -= vitesse;
        position->y += vitesse;
    }
    if (direction == 315){
        position->x -= vitesse;
        position->y -= vitesse;
    }

    if (direction == 0 || direction == 360)position->y -= vitesse;
    if (direction == 90)position->x += vitesse;
    if (direction == 180) position->y += vitesse;
    if (direction == 270) position->x -= vitesse;

}

void getRacketPos(SDL_Rect* tab, int Speed, SDL_Event* event){
    switch (event->type){
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym){
        case SDLK_DOWN://Rackette droite bas
            if ((tab[JOUEUR_2].y + Speed + HAUTEUR_RACKET) > HAUTEUR_FENETRE){
                break;
            }else{
                tab[JOUEUR_2].y += Speed;
            }
            break;
        case SDLK_a://rackette gauche bas
            if ((tab[JOUEUR_1].y + Speed + HAUTEUR_RACKET) > HAUTEUR_FENETRE){
            }else{
                tab[JOUEUR_1].y += Speed;

            }
            break;

        case SDLK_UP:
            tab[JOUEUR_2].y -= Speed;
            break;
        case SDLK_q:
            tab[JOUEUR_1].y -= Speed;
            break;
        default:
            ;
        }
    }
}

int getBallePos(int direction, SDL_Rect posBalle, SDL_Rect* posRacket, SDL_Rect posLimitGauche, SDL_Rect posLimitDroite, int mode){
    int Speed = 2;
    getDirection(direction, &posBalle, Speed);
    if (mode == 1){
        if (posBalle.x <= MODE_1_MUR_POS){
            return -3;
        }
    }
    if (posBalle.y + HAUTEUR_BALLE >= posRacket[JOUEUR_1].y && posBalle.y <= posRacket[JOUEUR_1].y + HAUTEUR_RACKET){//si la balle est sur la rackette gauche
        if (posBalle.x <= posRacketteJOUEUR_1_x + LARGUEUR_RACKET && posBalle.x > posRacket[JOUEUR_1].x){//si la balle touche la rackette gauche
            return 0;
        }
    }
    if (posBalle.y + HAUTEUR_BALLE >= posRacket[JOUEUR_2].y && posBalle.y <= posRacket[JOUEUR_2].y + HAUTEUR_RACKET){//si la balle est sur la rackette droite
        posBalle.x += LARGEUR_BALLE;
        if (posBalle.x >= posRacketteJOUEUR_2_x && posBalle.x < posRacket[JOUEUR_2].x + LARGUEUR_RACKET){//si la balle touche la rackette droite
            return 0;
        }
    }

    if (posBalle.y <= 0){
        return -1;
    }
    if (posBalle.y + HAUTEUR_BALLE >= HAUTEUR_FENETRE){
        return -1;
    }

    if (posBalle.x >= posLimitDroite.x){
        return -2;
    }
    if (posBalle.x <= posLimitGauche.x + LARGEUR_LIMIT){
        return -2;
    }

    return 1;
}

int getRebondRaquette(int *directionBalle, SDL_Rect *posRackette, SDL_Rect posBalle){
    int milieuRaquette = 0, diff = 0, i = 0;
    posBalle.y += HAUTEUR_BALLE / 2;
     if (*directionBalle < 360 && *directionBalle > 180){// Balle en direction de la raquette du joueur 1(gauche)
        *directionBalle = 90;
        milieuRaquette = posRackette[JOUEUR_1].y + (HAUTEUR_RACKET / 2);
        if (posBalle.y > milieuRaquette - reBondMilieu && posBalle.y < milieuRaquette + reBondMilieu);//si la balle arrive quasiment au mileu
        else{
            if (posBalle.y > milieuRaquette){
                i = milieuRaquette;
                while (i < posBalle.y){
                    diff += 3;
                    i += 2;
                }

                *directionBalle += diff;
            }
            if (posBalle.y < milieuRaquette){
                i = milieuRaquette;
                while (i > posBalle.y){
                    diff += 3;
                    i -= 2;
                }
                *directionBalle -= diff;
            }
            if (posBalle.y == milieuRaquette){
                *directionBalle = 90;
            }
            if (posBalle.y == milieuRaquette){
                *directionBalle = 270;
            }
            if (*directionBalle > 360){
                *directionBalle = 140;
            }
            if (*directionBalle < 0){
                *directionBalle = 30;
                return 0;
            }
        }
        return 0;
     }

     if (*directionBalle < 180 && *directionBalle > 0){// Balle en direction de la raquette du joueur 2(droite)
        *directionBalle = 270;
        milieuRaquette = posRackette[JOUEUR_2].y + (HAUTEUR_RACKET / 2);
        if (posBalle.y > milieuRaquette - reBondMilieu && posBalle.y < milieuRaquette + reBondMilieu);//si la balle arrive quasiment au mileu
        else{
            if (posBalle.y > milieuRaquette){
                i = milieuRaquette;
                while (i < posBalle.y){
                    diff += 3;
                    i += 2;
                }
                *directionBalle -= diff;
                return 0;
            }
            if (posBalle.y < milieuRaquette){
                i = milieuRaquette;
                while (i > posBalle.y){
                    diff += 3;
                    i -= 2;
                }
                *directionBalle += diff;
                return 0;
            }
            if (posBalle.y == milieuRaquette){
                *directionBalle = 270;
                return 0;
            }
            if (*directionBalle > 360){
                *directionBalle = 320;
                return 0;
            }
            if (*directionBalle < 0){
                *directionBalle = 210;
                return 0;
            }
        }
        return 0;
     }


    return 0;
}

int getRebondBalle(int* directionBalle){
        if (*directionBalle < 90 && *directionBalle > 0){
            *directionBalle += 90;
            return 0;
        }
        if (*directionBalle < 180 && *directionBalle > 90){
            *directionBalle -= 90;
            return 0;
        }
        if (*directionBalle < 270 && *directionBalle > 180){
            *directionBalle += 90;
            return 0;
        }
        if (*directionBalle < 360 && *directionBalle > 270){
            *directionBalle -= 90;
            return 0;
        }
     if (*directionBalle == 0 || *directionBalle == 360){
        *directionBalle = 180;
        return 0;
     }
     if (*directionBalle == 180){
        *directionBalle = 0;
        return 0;
     }
     return 0;
}

void getRebondMode_1(int* directionBalle){
    if (*directionBalle > 180 && *directionBalle < 270){
        *directionBalle -= 90;
    }
    if (*directionBalle > 270 && *directionBalle < 360){
        *directionBalle += 90;
        if (*directionBalle >= 360){
            *directionBalle -= 360;
        }
    }

    if (*directionBalle == 270){
        *directionBalle = 90;
    }
}

void getMode_1direction(int* directionBalle){
    srand(time(NULL));
    *directionBalle = rand() % (179 - 1) + 1;
}
