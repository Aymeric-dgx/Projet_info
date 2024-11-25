// Note Aymeric : si on clique dans la zone du quitter mais sur une autre fenetre, le programme considère quand meme qu'on a cliqué sur quitter et ferme le programme
// Faire la vérification des fenetre montrées/cachées

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Obtenir les dimensions de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    int screen_width = screen.w;
    int screen_height = screen.h;

    // Créer la fenêtre menu et le renderer
    SDL_Window* window_menu = SDL_CreateWindow("Dooble",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen_width, screen_height,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer_menu = SDL_CreateRenderer(window_menu, -1, SDL_RENDERER_ACCELERATED);

    // Charger la police
    TTF_Font* bigFont = TTF_OpenFont("../police/arial.ttf", 128); // Police grande taille
    TTF_Font* smallFont = TTF_OpenFont("../police/arial.ttf", 96); // Police petite taille

    // Texte "Dooble"
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* titleSurface = TTF_RenderText_Solid(bigFont, "Dooble", textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer_menu, titleSurface);
    SDL_Rect titleRect = {screen_width / 2 - titleSurface->w / 2, screen_height / 8 - titleSurface->h / 2, titleSurface->w, titleSurface->h};
    SDL_FreeSurface(titleSurface);

    // Boutons (trois boutons alignés verticalement)
    SDL_Color buttonTextColor = {0, 0, 0, 255}; // Noir pour le texte des boutons
    const char* buttonLabels[3] = {"Play", "Score", "Quit"};

    SDL_Rect buttons[3];
    SDL_Rect buttonTextRects[3]; // Pour stocker les positions du texte
    SDL_Texture* buttonTextures[3];
    for (int i = 0; i < 3; i++) {
        SDL_Surface* buttonSurface = TTF_RenderText_Solid(smallFont, buttonLabels[i], buttonTextColor);
        buttonTextures[i] = SDL_CreateTextureFromSurface(renderer_menu, buttonSurface);

        buttons[i].w = 700; // Largeur du bouton (texte + padding)
        buttons[i].h = 175; // Hauteur du bouton (texte + padding)
        buttons[i].x = screen_width / 2 - buttons[i].w / 2;
        buttons[i].y = screen_height / 2 + buttons[i].h * i - buttons[i].h / 1.5; // Espacement vertical entre les boutons

        // Calcul de la position du texte
        buttonTextRects[i].w = buttonSurface->w;
        buttonTextRects[i].h = buttonSurface->h;
        buttonTextRects[i].x = buttons[i].x + (buttons[i].w - buttonTextRects[i].w) / 2;
        buttonTextRects[i].y = buttons[i].y + (buttons[i].h - buttonTextRects[i].h) / 2;

        SDL_FreeSurface(buttonSurface);
    }
    // Créer la fenêtre play et le renderer
    SDL_Window* window_play = SDL_CreateWindow("Dooble",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen_width, screen_height,
                                          SDL_WINDOW_HIDDEN);
    SDL_Renderer* renderer_play = SDL_CreateRenderer(window_play , -1, SDL_RENDERER_ACCELERATED);



    int running = 1;
    SDL_Event event;

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x, mouseY = event.button.y;
                for (int i = 0; i < 3; i++) {
                    if (mouseX >= buttons[i].x && mouseX <= buttons[i].x + buttons[i].w &&
                        mouseY >= buttons[i].y && mouseY <= buttons[i].y + buttons[i].h) {
                        if (i == 0) {
                            SDL_Log("Bouton 'Play' cliquer !");
                            SDL_ShowWindow(window_play);
                            SDL_HideWindow(window_menu);
                        } else if (i == 1) {
                            SDL_Log("Bouton 'Score' cliquer !");
                        } else if (i == 2) {
                            SDL_Log("Bouton 'Quit' cliquer !");
                            running = 0;
                        }
                    }
                }
            }
        }

        // Dessiner l'arrière-plan
        SDL_SetRenderDrawColor(renderer_menu, 0, 0, 50, 255); // Bleu foncé
        SDL_RenderClear(renderer_menu);

        // Dessiner le texte principal "Dooble"
        SDL_RenderCopy(renderer_menu, titleTexture, NULL, &titleRect);

        // Affichage fenetre Menu
        for (int i = 0; i < 3; i++) {
            SDL_SetRenderDrawColor(renderer_menu, 255, 255, 255, 255);                        // Blanc
            SDL_RenderFillRect(renderer_menu, &buttons[i]);                                           // Remplir le bouton
            SDL_SetRenderDrawColor(renderer_menu, 0, 0, 0, 255);                              // Noir
            SDL_RenderDrawRect(renderer_menu, &buttons[i]);                                           // Contour du bouton
            SDL_RenderCopy(renderer_menu, buttonTextures[i], NULL, &buttonTextRects[i]); // Texte du bouton
        }

        SDL_RenderPresent(renderer_menu);

        //affichage fenetre Play

    }

    // Libération des ressources
    SDL_DestroyTexture(titleTexture);
    for (int i = 0; i < 3; i++) SDL_DestroyTexture(buttonTextures[i]);
    TTF_CloseFont(bigFont);
    TTF_CloseFont(smallFont);
    SDL_DestroyRenderer(renderer_menu);
    SDL_DestroyWindow(window_menu);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
