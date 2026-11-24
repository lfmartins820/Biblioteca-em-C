#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Uso de LLM:
 A ferramenta foi usada como apoio para estruturar e revisar o programa,
 especialmente a organizacao das funcoes e a integracao dos menus.
*/

typedef struct usuario {
    char email[100];
    char nome[100];
    struct usuario *next;
} Usuario;

typedef struct livro {
    char titulo[100];
    char autor[100];
    int ano;
    int codigo;
    int status;                 /* 0 = disponivel; 1 = emprestado */
    char emailUsuario[100];
    struct livro *next;
} Livro;

Usuario *listaUsuarios = NULL;
Livro *listaLivros = NULL;
int proximoCodigo = 1;

/* Busca um usuario pelo email. */
Usuario *buscarUsuarioEmail(char email[]) {
    Usuario *aux = listaUsuarios;
    while (aux != NULL) {
        if (strcmp(aux->email, email) == 0)
            return aux;
        aux = aux->next;
    }
    return NULL;
}

/* Busca um livro pelo codigo. A lista esta ordenada por codigo. */
Livro *buscarLivroCodigo(int codigo) {
    Livro *aux = listaLivros;
    while (aux != NULL && aux->codigo <= codigo) {
        if (aux->codigo == codigo)
            return aux;
        aux = aux->next;
    }
    return NULL;
}

/* ---------- Cadastro ---------- */

void cadastrarUsuario() {
    Usuario *novo;
    char email[100];

    printf("Email: ");
    scanf("%99s", email);

    if (buscarUsuarioEmail(email) != NULL) {
        printf("Ja existe um usuario com esse email.\n");
        return;
    }

    novo = malloc(sizeof(Usuario));
    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    strcpy(novo->email, email);
    printf("Nome: ");
    scanf("%99s", novo->nome);

    novo->next = listaUsuarios;
    listaUsuarios = novo;

    printf("Usuario cadastrado com sucesso.\n");
}

void cadastrarLivro() {
    Livro *novo;
    Livro *aux;
    Livro *anterior = NULL;

    novo = malloc(sizeof(Livro));
    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    printf("Titulo: ");
    scanf("%99s", novo->titulo);
    printf("Autor: ");
    scanf("%99s", novo->autor);
    printf("Ano de publicacao: ");
    scanf("%d", &novo->ano);

    novo->codigo = proximoCodigo++;
    novo->status = 0;
    novo->emailUsuario[0] = '\0';
    novo->next = NULL;

    /* Insere mantendo os livros em ordem crescente de codigo. */
    aux = listaLivros;
    while (aux != NULL && aux->codigo < novo->codigo) {
        anterior = aux;
        aux = aux->next;
    }

    if (anterior == NULL) {
        novo->next = listaLivros;
        listaLivros = novo;
    } else {
        novo->next = aux;
        anterior->next = novo;
    }

    printf("Livro cadastrado. Codigo: %d\n", novo->codigo);
}

/* ---------- Consulta ---------- */

void mostrarLivro(Livro *livro) {
    printf("\nCodigo: %d\n", livro->codigo);
    printf("Titulo: %s\n", livro->titulo);
    printf("Autor: %s\n", livro->autor);
    printf("Ano: %d\n", livro->ano);
    if (livro->status == 0)
        printf("Status: Disponivel\n");
    else
        printf("Status: Emprestado para %s\n", livro->emailUsuario);
}

void consultarLivros() {
    int opcao, codigo, encontrou = 0;
    char autor[100];
    Livro *aux;

    printf("\n1 - Buscar por codigo\n2 - Buscar por autor\nOpcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("Codigo: ");
        scanf("%d", &codigo);
        aux = buscarLivroCodigo(codigo);
        if (aux == NULL)
            printf("Livro nao encontrado.\n");
        else
            mostrarLivro(aux);
    } else if (opcao == 2) {
        printf("Autor: ");
        scanf("%99s", autor);
        aux = listaLivros;
        while (aux != NULL) {
            if (strcmp(aux->autor, autor) == 0) {
                mostrarLivro(aux);
                encontrou = 1;
            }
            aux = aux->next;
        }
        if (!encontrou)
            printf("Livro nao encontrado.\n");
    } else {
        printf("Opcao invalida.\n");
    }
}

void consultarUsuarios() {
    int opcao, encontrou = 0;
    char email[100], nome[100];
    Usuario *aux;

    printf("\n1 - Buscar por email\n2 - Buscar por nome\nOpcao: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("Email: ");
        scanf("%99s", email);
        aux = buscarUsuarioEmail(email);
        if (aux == NULL)
            printf("Usuario nao cadastrado.\n");
        else
            printf("Email: %s | Nome: %s\n", aux->email, aux->nome);
    } else if (opcao == 2) {
        printf("Nome: ");
        scanf("%99s", nome);
        aux = listaUsuarios;
        while (aux != NULL) {
            if (strcmp(aux->nome, nome) == 0) {
                printf("Email: %s | Nome: %s\n", aux->email, aux->nome);
                encontrou = 1;
            }
            aux = aux->next;
        }
        if (!encontrou)
            printf("Usuario nao cadastrado.\n");
    } else {
        printf("Opcao invalida.\n");
    }
}

void consultarEmprestimos() {
    char email[100];
    int encontrou = 0;
    Livro *aux;

    printf("Email do usuario: ");
    scanf("%99s", email);

    if (buscarUsuarioEmail(email) == NULL) {
        printf("Usuario nao cadastrado.\n");
        return;
    }

    aux = listaLivros;
    while (aux != NULL) {
        if (aux->status == 1 && strcmp(aux->emailUsuario, email) == 0) {
            mostrarLivro(aux);
            encontrou = 1;
        }
        aux = aux->next;
    }

    if (!encontrou)
        printf("Esse usuario nao possui livros emprestados.\n");
}

/* ---------- Atualizacao ---------- */

void atualizarLivro() {
    int codigo;
    Livro *aux;

    printf("Codigo do livro: ");
    scanf("%d", &codigo);
    aux = buscarLivroCodigo(codigo);

    if (aux == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    }

    printf("Novo titulo: ");
    scanf("%99s", aux->titulo);
    printf("Novo autor: ");
    scanf("%99s", aux->autor);
    printf("Novo ano: ");
    scanf("%d", &aux->ano);
    printf("Livro atualizado.\n");
}

void atualizarUsuario() {
    char email[100];
    Usuario *aux;

    printf("Email do usuario: ");
    scanf("%99s", email);
    aux = buscarUsuarioEmail(email);

    if (aux == NULL) {
        printf("Usuario nao cadastrado.\n");
        return;
    }

    printf("Novo nome: ");
    scanf("%99s", aux->nome);
    printf("Usuario atualizado.\n");
}

/* ---------- Exclusao ---------- */

void excluirLivro() {
    int codigo;
    Livro *aux = listaLivros;
    Livro *anterior = NULL;

    printf("Codigo do livro: ");
    scanf("%d", &codigo);

    while (aux != NULL && aux->codigo != codigo) {
        anterior = aux;
        aux = aux->next;
    }

    if (aux == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    }
    if (aux->status == 1) {
        printf("Nao e possivel excluir um livro emprestado.\n");
        return;
    }

    if (anterior == NULL)
        listaLivros = aux->next;
    else
        anterior->next = aux->next;

    free(aux);
    printf("Livro excluido.\n");
}

void excluirUsuario() {
    char email[100];
    Usuario *aux = listaUsuarios;
    Usuario *anterior = NULL;
    Livro *livro;

    printf("Email do usuario: ");
    scanf("%99s", email);

    while (aux != NULL && strcmp(aux->email, email) != 0) {
        anterior = aux;
        aux = aux->next;
    }

    if (aux == NULL) {
        printf("Usuario nao cadastrado.\n");
        return;
    }

    /* Evita remover um usuario que ainda tenha emprestimos ativos. */
    livro = listaLivros;
    while (livro != NULL) {
        if (livro->status == 1 && strcmp(livro->emailUsuario, email) == 0) {
            printf("Nao e possivel excluir: usuario possui livro emprestado.\n");
            return;
        }
        livro = livro->next;
    }

    if (anterior == NULL)
        listaUsuarios = aux->next;
    else
        anterior->next = aux->next;

    free(aux);
    printf("Usuario excluido.\n");
}

/* ---------- Emprestimo e devolucao ---------- */

void emprestarLivro() {
    int codigo;
    char email[100];
    Livro *livro;

    printf("Codigo do livro: ");
    scanf("%d", &codigo);
    livro = buscarLivroCodigo(codigo);

    if (livro == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    }
    if (livro->status == 1) {
        printf("Esse livro ja esta emprestado.\n");
        return;
    }

    printf("Email do usuario: ");
    scanf("%99s", email);
    if (buscarUsuarioEmail(email) == NULL) {
        printf("Usuario nao cadastrado.\n");
        return;
    }

    livro->status = 1;
    strcpy(livro->emailUsuario, email);
    printf("Emprestimo realizado com sucesso.\n");
}

void devolverLivro() {
    int codigo;
    Livro *livro;

    printf("Codigo do livro: ");
    scanf("%d", &codigo);
    livro = buscarLivroCodigo(codigo);

    if (livro == NULL) {
        printf("Livro nao encontrado.\n");
        return;
    }
    if (livro->status == 0) {
        printf("Esse livro nao estava emprestado.\n");
        return;
    }

    livro->status = 0;
    livro->emailUsuario[0] = '\0';
    printf("Devolucao realizada com sucesso.\n");
}

/* ---------- Menus ---------- */


void menuCadastro() {
    /*Aqui precisei de ajuda da IA*/
    int opcao;
    do {
        printf("\n--- CADASTRO ---\n1 - Livros\n2 - Usuarios\n0 - Voltar\nOpcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: cadastrarLivro(); break;
            case 2: cadastrarUsuario(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

void menuConsulta() {
    int opcao;
    do {
        printf("\n--- CONSULTA ---\n1 - Livros\n2 - Usuarios\n3 - Emprestimos\n0 - Voltar\nOpcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: consultarLivros(); break;
            case 2: consultarUsuarios(); break;
            case 3: consultarEmprestimos(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

void menuAtualizacao() {
    int opcao;
    do {
        printf("\n--- ATUALIZACAO ---\n1 - Livros\n2 - Usuarios\n0 - Voltar\nOpcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: atualizarLivro(); break;
            case 2: atualizarUsuario(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

void menuExclusao() {
    int opcao;
    do {
        printf("\n--- EXCLUSAO ---\n1 - Livros\n2 - Usuarios\n0 - Voltar\nOpcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: excluirLivro(); break;
            case 2: excluirUsuario(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

/* Libera todos os nos das duas listas antes de encerrar. */
void liberarMemoria() {
    Usuario *usuario;
    Livro *livro;

    while (listaUsuarios != NULL) {
        usuario = listaUsuarios;
        listaUsuarios = listaUsuarios->next;
        free(usuario);
    }

    while (listaLivros != NULL) {
        livro = listaLivros;
        listaLivros = listaLivros->next;
        free(livro);
    }
}

int main() {
    int opcao;

    do {
        printf("\n========== BIBLIOTECA ==========\n");
        printf("1 - Cadastro\n2 - Consulta\n3 - Atualizacao\n");
        printf("4 - Exclusao\n5 - Emprestimo\n6 - Devolucao\n0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: menuCadastro(); break;
            case 2: menuConsulta(); break;
            case 3: menuAtualizacao(); break;
            case 4: menuExclusao(); break;
            case 5: emprestarLivro(); break;
            case 6: devolverLivro(); break;
            case 0:
                liberarMemoria();
                printf("Memoria liberada. Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
