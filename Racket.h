void initDirectBalle(int* aleatoire);
void getDirection(int direction, SDL_Rect *position, int vitesse);
void getRacketPos(SDL_Rect* tabPosRacket, int Speed, SDL_Event* event);
int getBallePos(int direction, SDL_Rect posBalle, SDL_Rect* posRacket, SDL_Rect posLimitGauche, SDL_Rect posLimitDroite, int mode);
int getRebondRaquette(int *directionBalle, SDL_Rect *posRackette, SDL_Rect posBalle);
int getRebondBalle(int* directionBalle);
void getMode_1direction(int* directionBalle);
