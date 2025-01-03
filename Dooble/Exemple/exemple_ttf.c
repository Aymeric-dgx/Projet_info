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

    // Créer la fenêtre et le renderer
    SDL_Window* window = SDL_CreateWindow("Dooble",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen_width, screen_height,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger la police
    TTF_Font* bigFont = TTF_OpenFont("../police/arial.ttf", 64); // Police grande taille
    TTF_Font* smallFont = TTF_OpenFont("../police/arial.ttf", 32); // Police petite taille

    // Texte "Bienvenue"
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* titleSurface = TTF_RenderText_Solid(bigFont, "Bienvenue", textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = {screen_width / 2 - titleSurface->w / 2, screen_height / 4 - titleSurface->h / 2, titleSurface->w, titleSurface->h};
    SDL_FreeSurface(titleSurface);

    // Texte 2 - "Cliquez pour commencer"
    SDL_Surface* surface2 = TTF_RenderText_Solid(bigFont, "sur notre jeux", textColor);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Rect rect2 = {screen_width / 2  - titleSurface->w / 2 , screen_height / 4+50 - titleSurface->h / 2+50, titleSurface->w, titleSurface->h};
    SDL_FreeSurface(surface2);

    // Boutons (trois boutons alignés verticalement)
    SDL_Color buttonTextColor = {0, 0, 0, 255}; // Noir pour le texte des boutons
    const char* buttonLabels[3] = {"Jouer", "Options", "Quitter"};

    SDL_Rect buttons[3];
    SDL_Texture* buttonTextures[3];
    for (int i = 0; i < 3; i++) {
        SDL_Surface* buttonSurface = TTF_RenderText_Solid(smallFont, buttonLabels[i], buttonTextColor);
        buttonTextures[i] = SDL_CreateTextureFromSurface(renderer, buttonSurface);

        buttons[i].w = buttonSurface->w + 40; // Largeur du bouton (texte + padding)
        buttons[i].h = buttonSurface->h + 20; // Hauteur du bouton (texte + padding)
        buttons[i].x = screen_width / 2 - buttons[i].w / 2;
        buttons[i].y = screen_height / 2 + i * 100; // Espacement vertical entre les boutons

        SDL_FreeSurface(buttonSurface);
    }

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
                            SDL_Log("Bouton 'Jouer' cliqué !");
                        } else if (i == 1) {
                            SDL_Log("Bouton 'Options' cliqué !");
                        } else if (i == 2) {
                            SDL_Log("Bouton 'Quitter' cliqué !");
                            running = 0;
                        }
                    }
                }
            }
        }

        // Dessiner l'arrière-plan
        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255); // Bleu foncé
        SDL_RenderClear(renderer);

        // Dessiner le texte principal "Bienvenue"
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);

        // Dessiner les boutons
        for (int i = 0; i < 3; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
            SDL_RenderFillRect(renderer, &buttons[i]);           // Remplir le bouton
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);      // Noir
            SDL_RenderDrawRect(renderer, &buttons[i]);           // Contour du bouton
            SDL_RenderCopy(renderer, buttonTextures[i], NULL, &buttons[i]); // Texte du bouton
        }

        SDL_RenderPresent(renderer);
    }

    // Libération des ressources
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(texture2);
    for (int i = 0; i < 3; i++) SDL_DestroyTexture(buttonTextures[i]);
    TTF_CloseFont(bigFont);
    TTF_CloseFont(smallFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
