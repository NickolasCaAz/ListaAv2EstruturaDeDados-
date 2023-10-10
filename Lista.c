#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char artist[100];
    char song[100];
    struct Node* next;
    struct Node* prev;
};

struct Playlist {
    struct Node* head;
    struct Node* current;
};

void append(struct Playlist* playlist, const char* artist, const char* song) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    strcpy(new_node->artist, artist);
    strcpy(new_node->song, song);
    
    if (playlist->head == NULL) {
        playlist->head = new_node;
        playlist->head->next = playlist->head;
        playlist->head->prev = playlist->head;
        playlist->current = playlist->head;
    } else {
        new_node->prev = playlist->head->prev;
        new_node->next = playlist->head;
        playlist->head->prev->next = new_node;
        playlist->head->prev = new_node;
    }
}

void display(struct Playlist* playlist) {
    if (playlist->head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    struct Node* current_node = playlist->head;
    do {
        printf("Artista: %s, Música: %s\n", current_node->artist, current_node->song);
        current_node = current_node->next;
    } while (current_node != playlist->head);
}

void sort_by_song(struct Playlist* playlist) {
    if (playlist->head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    
    // Crie um array de ponteiros para nós e copie os nós para o array
    int num_nodes = 0;
    struct Node* current_node = playlist->head;
    do {
        num_nodes++;
        current_node = current_node->next;
    } while (current_node != playlist->head);
    
    struct Node** node_array = (struct Node**)malloc(num_nodes * sizeof(struct Node*));
    
    current_node = playlist->head;
    for (int i = 0; i < num_nodes; i++) {
        node_array[i] = current_node;
        current_node = current_node->next;
    }
    
    // Classifique o array de nós com base no nome da música
    for (int i = 0; i < num_nodes - 1; i++) {
        for (int j = 0; j < num_nodes - i - 1; j++) {
            if (strcmp(node_array[j]->song, node_array[j + 1]->song) > 0) {
                struct Node* temp = node_array[j];
                node_array[j] = node_array[j + 1];
                node_array[j + 1] = temp;
            }
        }
    }
    
    // Reconstrua a lista encadeada a partir do array ordenado
    playlist->head = node_array[0];
    playlist->head->prev = node_array[num_nodes - 1];
    node_array[num_nodes - 1]->next = playlist->head;
    
    for (int i = 1; i < num_nodes; i++) {
        node_array[i]->prev = node_array[i - 1];
        node_array[i - 1]->next = node_array[i];
    }
    
    free(node_array);
}

void insert(struct Playlist* playlist, const char* artist, const char* song) {
    append(playlist, artist, song);
    // Salve a playlist em um arquivo aqui...
}

void remove_song(struct Playlist* playlist, const char* song) {
    if (playlist->head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    struct Node* current_node = playlist->head;
    do {
        if (strcmp(current_node->song, song) == 0) {
            current_node->prev->next = current_node->next;
            current_node->next->prev = current_node->prev;
            if (current_node == playlist->head) {
                playlist->head = current_node->next;
            }
            free(current_node);
            // Salve a playlist em um arquivo aqui...
            return;
        }
        current_node = current_node->next;
    } while (current_node != playlist->head);
    printf("A música '%s' não foi encontrada na playlist.\n", song);
}

void search(struct Playlist* playlist, const char* song) {
    if (playlist->head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    struct Node* current_node = playlist->head;
    do {
        if (strcmp(current_node->song, song) == 0) {
            printf("Artista: %s, Música: %s\n", current_node->artist, current_node->song);
            return;
        }
        current_node = current_node->next;
    } while (current_node != playlist->head);
    printf("A música '%s' não foi encontrada na playlist.\n", song);
}

void next_song(struct Playlist* playlist) {
    if (playlist->head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    playlist->current = playlist->current->next;
    display_current_song(playlist);
}

void prev_song(struct Playlist* playlist) {
    if (playlist->head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    playlist->current = playlist->current->prev;
    display_current_song(playlist);
}

void display_current_song(struct Playlist* playlist) {
    if (playlist->current == NULL) {
        printf("A playlist está vazia.\n");
    } else {
        printf("Artista: %s, Música: %s\n", playlist->current->artist, playlist->current->song);
    }
}

void load_from_file(struct Playlist* playlist) {
    FILE* file = fopen("musicas.txt", "r");
    if (file == NULL) {
        printf("Arquivo 'musicas.txt' não encontrado.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char artist[100];
        char song[100];
        if (sscanf(line, "%99[^;];%99[^\n]", artist, song) == 2) {
            append(playlist, artist, song);
        }
    }

    fclose(file);
}

void save_to_file(struct Playlist* playlist) {
    FILE* file = fopen("musicas.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo 'musicas.txt' para escrita.\n");
        return;
    }

    struct Node* current_node = playlist->head;
    if (current_node == NULL) {
        printf("A playlist está vazia.\n");
        fclose(file);
        return;
    }

    do {
        fprintf(file, "%s;%s\n", current_node->artist, current_node->song);
        current_node = current_node->next;
    } while (current_node != playlist->head);

    fclose(file);
}


// Implemente as funções load_from_file e save_to_file aqui...

int main() {
    struct Playlist playlist;
    playlist.head = NULL;
    playlist.current = NULL;

    load_from_file(&playlist);

    // Carregue os dados do arquivo aqui...

    while (1) {
        printf("\nMenu:\n");
        printf("1. Exibir a playlist pela ordem de cadastro\n");
        printf("2. Exibir a playlist ordenada pelo nome das músicas\n");
        printf("3. Inserir uma nova música\n");
        printf("4. Remover uma música\n");
        printf("5. Buscar por uma música\n");
        printf("6. Avançar para a próxima música\n");
        printf("7. Retornar à música anterior\n");
        printf("8. Sair\n");

        char choice[10];
        printf("Escolha uma opção: ");
        fgets(choice, sizeof(choice), stdin);

        switch (choice[0]) {
            case '1':
                display(&playlist);
                break;
            case '2':
                sort_by_song(&playlist);
                break;
            case '3':
                char artist[100];
                char song[100];
                printf("Digite o nome do artista: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = '\0';  // Remover a quebra de linha
                printf("Digite o nome da música: ");
                fgets(song, sizeof(song), stdin);
                song[strcspn(song, "\n")] = '\0';  // Remover a quebra de linha
                insert(&playlist, artist, song);
                break;
            case '4':
                char remove_song_name[100];
                printf("Digite o nome da música a ser removida: ");
                fgets(remove_song_name, sizeof(remove_song_name), stdin);
                remove_song_name[strcspn(remove_song_name, "\n")] = '\0';  // Remover a quebra de linha
                remove_song(&playlist, remove_song_name);
                break;
            case '5':
                char search_song_name[100];
                printf("Digite o nome da música a ser buscada: ");
                fgets(search_song_name, sizeof(search_song_name), stdin);
                search_song_name[strcspn(search_song_name, "\n")] = '\0';  // Remover a quebra de linha
                search(&playlist, search_song_name);
                break;
            case '6':
                next_song(&playlist);
                break;
            case '7':
                prev_song(&playlist);
                break;
            case '8':
                // Implemente a função para sair e liberar a memória alocada
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
