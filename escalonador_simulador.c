#include <stdio.h>
#include <stdlib.h>

int menu(){
    int esc;
 
    system("clear");
    printf("\n\n                              ESCALONADOR DE CPU");
    printf("\n-------------------------------------------------------------------------------");
    printf("\n     Escolha o algoritimo que pretente utilizar para simular o escalonador. \n");
    printf("\n                  < 1 > Firts-Come, First-Serve (FCFS)");
    printf("\n                  < 2 > Shortest-Job-First- (SRTF) ");
    printf("\n                  < 3 > Escalonador com prioridade ");
    printf("\n                  < 4 > Round Robin (RR) ");
    printf("\n                  < 5 > Exit \n");
    printf("\n     Sua escolha: ");
    scanf("%i", &esc);

    return esc;
}

struct processos {
    int id;
    int dur;
    int prio;
    int execu;
    int esp;
    struct processos *prox;
    /*
        id - identificação do processo
        dur - tempo de duração do processo
        exe - tempo de execução do processo
        esp - tempo de espera do processo
    */
};

struct processos *entra_processos(int id, int dur, int prio){
    struct processos *loc;
    loc = (struct processos*)malloc(sizeof(struct processos));
    if(loc == NULL){
        printf("Erro de alocação.\nFim de execução\n");
        exit(1);
    };
    loc->id = id;
    loc->dur = dur;
    loc->prio = prio;
    loc->execu = 0;
    loc->esp = 0;
    loc->prox = NULL;
}

void listar_processos(struct processos *loc){
    struct processos *tmp = loc;
    printf("\n\n\tLista dos processos\n\n");
    while(tmp != NULL){
        printf("\tProcesso: %d\tPrioridade: %d\tDuração: %d\n", tmp->id, tmp->prio, tmp->dur);
        tmp = tmp->prox;
    }
    printf("\n");
}

void fcfs(struct processos *loc) {
    int tempo = 0, inicio, fim;
    struct processos *tmp = loc;
    printf("\n\t\tEscalonamento Firts-Come, First-Serve (FCFS)\n\n");
    while (tmp != NULL){
        inicio = tempo;
        tempo += tmp->dur;
        fim = tempo;
        printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", tmp->id, tempo, inicio, fim);
        tmp = tmp->prox;
    }
}

void sjf(struct processos *loc){
    int tempo_exe, shortest, inicio, fim;
    struct processos *copia, *src_tmp, *tmp, *antes_shortest;
    printf("\n\t\tShortest-Job-First- (SRTF)\n\n ");
    src_tmp = loc;
    copia = tmp = NULL;
    while (src_tmp != NULL){/*fazendo copia dos processos*/
        if(copia == NULL){
            copia = entra_processos(src_tmp->id, src_tmp->dur, src_tmp->prio);
            tmp = copia;
        }
        else{
            tmp->prox = entra_processos(src_tmp->id, src_tmp->dur, src_tmp->prio);
            tmp = tmp->prox;
        }
        src_tmp = src_tmp->prox;
    }
    tempo_exe = 0;
    while(copia != NULL){/*procura do novo processo*/
        antes_shortest = NULL;
        shortest = copia->dur;
        tmp = copia->prox;
        src_tmp = copia;
        while(tmp != NULL){
            if(tmp->dur < shortest){
                shortest = tmp->dur;
                antes_shortest = src_tmp;
            }
            src_tmp = tmp;
            tmp = tmp->prox;
        }
        if(antes_shortest == NULL){/*processo é executado e copia e eliminado e aloca o 1º processo s for menor*/
            inicio = tempo_exe;
            tempo_exe += copia->dur;
            fim = tempo_exe;
            printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", copia->id, tempo_exe, inicio, fim);
            src_tmp = copia;
            copia = copia->prox;
            free(src_tmp);
        }
        else{ /*aloca o 1º processo se n~ao houver outro menor*/
            tmp = antes_shortest->prox;
            inicio = tempo_exe;
            tempo_exe += tmp->dur;
            fim = tempo_exe;
            printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", tmp->id, tempo_exe, inicio, fim);
            antes_shortest->prox = tmp->prox;
            free(tmp);
        }
    }
} 

int escalonador_prioridade(struct processos *loc){
    int tempo_exe, inicio, maior, fim;
    struct processos *copia, * src_tmp, *tmp, *maior_prio;
    printf("\n\t\tEscalonador com prioridade \n\n");
    src_tmp = loc;
    copia = tmp = NULL;
    while (src_tmp != NULL){/*fazendo copia dos processos*/
        if(copia == NULL){
            copia = entra_processos(src_tmp->id, src_tmp->dur, src_tmp->prio);
            tmp = copia;
        }
        else{
            tmp->prox = entra_processos(src_tmp->id, src_tmp->dur, src_tmp->prio);
            tmp = tmp->prox;
        }
        src_tmp = src_tmp->prox;
    }
    tempo_exe = 0;
    while(copia != NULL){/*verifica proximo processo*/
        maior_prio = NULL;
        maior = copia->prio;
        tmp = copia->prox;
        src_tmp = copia;
        while(tmp != NULL){
            if(tmp->prio < maior){
                maior = tmp->prio;
                maior_prio = src_tmp;
            }
            src_tmp = tmp;
            tmp = tmp->prox;
        }
        if(maior_prio == NULL){/*ver se 1º tem prioridade maior*/
            inicio = tempo_exe;
            tempo_exe += copia->dur;
            fim = tempo_exe;
            printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", copia->id, tempo_exe, inicio, fim);
            src_tmp = copia->prox;
            free(copia);
            copia = src_tmp;
        }
        else {/* se 1º nao tem maior prioridade*/
            tmp = maior_prio->prox;
            inicio = tempo_exe;
            tempo_exe += tmp->dur;
            fim = tempo_exe;
            printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", tmp->id, tempo_exe, inicio, fim);
            maior_prio->prox = tmp->prox;
            free(tmp);
        }
    }
}

int robbin_round(struct processos *loc, int quantum){
    int jobs_remain, passe;
    struct processos *copia, *src_tmp, *tmp, *slot;
    printf("\n\t\tRound Robin (RR) - Quantum: %d\n", quantum);
    src_tmp = loc;
    copia = tmp = NULL;
    while (src_tmp != NULL){/*fazendo copia dos processos*/
        if(copia == NULL){
            copia = entra_processos(src_tmp->id, src_tmp->dur, src_tmp->prio);
            tmp = copia;
        }
        else{
            tmp->prox = entra_processos(src_tmp->id, src_tmp->dur, src_tmp->prio);
            tmp = tmp->prox;
        }
        src_tmp = src_tmp->prox;
    }
    jobs_remain = quantum;
    slot = NULL;
    while(jobs_remain <= passe){/*apanhando proximo processo, fazendo a alocação*/
        jobs_remain = 0;
        if (slot == NULL){
            slot = copia;
            jobs_remain = 2;
        }
        else{
            passe = 0;
            do{
                if(slot->prox == NULL){
                    passe++;
                    slot = copia;
                }
                else{
                    slot = slot->prox;
                }
            }while(passe <= 3 && slot->dur == slot->execu);
            if(passe <= 3){
                jobs_remain = 2;
            }
        }
        tmp = copia;
        while(tmp != NULL){
            if(tmp->dur > tmp->execu){
                if(tmp == slot){
                    tmp->execu += quantum;
                }
                else{
                    tmp->esp += quantum;
                }
            }
            tmp = tmp->prox;
        }
    }
    tmp = copia;
    while(tmp){/*monstra resultados do escalonamento, deselacando as copia*/
        printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", tmp->id, tmp->dur, tmp->esp, tmp->execu + tmp->esp);
        src_tmp = tmp;
        tmp = tmp->prox;
        free(src_tmp);
    }
}

int main(){
    int esc;
    do{
    esc = menu();
    struct processos *list_proc, *tmp_proc;
    list_proc = entra_processos(1, 53, 3);
    list_proc->prox = entra_processos(2, 17, 1); tmp_proc = list_proc->prox;
    tmp_proc->prox  = entra_processos(3,  68, 3); tmp_proc = tmp_proc->prox;
    tmp_proc->prox  = entra_processos(4,  24, 2);

    if(esc == 1){
        system("clear");
        listar_processos(list_proc);
        fcfs(list_proc);
        printf("\n\n\t< 1 > Para voltar\nEscolha: ");
        int i;
        scanf("%d", &i);
    }
    if ( esc == 2){
        system("clear");
        listar_processos(list_proc);
        sjf(list_proc);
        printf("\n\n\t< 1 > Para voltar\nEscolha: ");
        int i;
        scanf("%d", &i);
    }
    if (esc == 3){
        system("clear");
        listar_processos(list_proc);
        escalonador_prioridade(list_proc);
        printf("\n\n\t< 1 > Para voltar\nEscolha: ");
        int i;
        scanf("%d", &i);
    }
    if( esc == 4){
        system("clear");
        int quantum;
        listar_processos(list_proc);
        printf("\nDigite o quantum de tempo desejado: ");
        scanf("%d", &quantum);
        robbin_round(list_proc, quantum);
        printf("\n\n\t< 1 > Para voltar\nEscolha: ");
        int i;
        scanf("%d", &i);
    }
    if( esc == 5){
        printf("\n\nTermino do programa.\n\n");
        exit(2);
    }
    }while(esc <= 6);
    return 0;
}