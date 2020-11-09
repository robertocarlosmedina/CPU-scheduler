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

int painel(){
    int esc;
    system("clear");
    printf("\n\n                              ESCALONADOR DE CPU");
    printf("\n-------------------------------------------------------------------------------\n\n");
    printf("                           Definição dos processos.\n\n");
    printf("\n                  < 1 > Usar processos estabelecidos pelo programa.");
    printf("\n                  < 2 > Introduzir novos procesos.");
    printf("\n\n    Sua escolha: ");
    scanf("%i", &esc);
    if (esc>2)
        painel();
    else
        return esc;
}

void top(){
    system("clear");
    printf("\n\n                              ESCALONADOR DE CPU");
    printf("\n-------------------------------------------------------------------------------\n\n");
}

struct processos{
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
    printf("\n\n\t\t\tLista dos processos\n\n");
    while(tmp != NULL){
        printf("\tProcesso: %d\tPrioridade: %d\tDuração: %d\n", tmp->id, tmp->prio, tmp->dur);
        tmp = tmp->prox;
    }
    printf("\n");
}

void fcfs(struct processos *loc, int n_proc) {
    int tempo = 0, inicio, fim, tmp_esp=0, turnaround_time=0;
    int p[n_proc], i=0;
    struct processos *tmp = loc;
    printf("\n\t\tEscalonamento Firts-Come, First-Serve (FCFS)\n\n");
    while (tmp != NULL){
        inicio = tempo;
        tempo += tmp->dur;
        p[i] = tempo;
        fim = tempo;
        printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", tmp->id, tmp->dur, inicio, fim);
        tmp_esp += inicio;
        tmp = tmp->prox;
        i++;
    }
    i=0;
    printf("\n\t\tAverage Waiting Time= %f\n\n\n",tmp_esp*1.0/n_proc);
    printf("\t    Simulando o escalonamento visualmente.");
    printf("\n\n\t\t    ");
    for (i=0; i<n_proc; i++){
            printf("| P%d ", i+1);
    }
    printf("|");
    printf("\n\t\t    0  ");
    for (i=0; i<n_proc; i++)
        printf("  %d ", p[i]);
}

void sjf(struct processos *loc, int n_proc){
    int tempo_exe, shortest, inicio, fim, tmp_esp, durc;
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
            durc = copia->dur;
            fim = tempo_exe;
            printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", copia->id, durc, inicio, fim);
            tmp_esp += fim;
            src_tmp = copia;
            copia = copia->prox;
            free(src_tmp);
        }
        else{ /*aloca o 1º processo se n~ao houver outro menor*/
            tmp = antes_shortest->prox;
            inicio = tempo_exe;
            tempo_exe += tmp->dur;
            durc = tmp->dur;
            fim = tempo_exe;
            printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", tmp->id, durc, inicio, fim);
            antes_shortest->prox = tmp->prox;
            free(tmp);
        }
    }
    printf("\n\t\tAverage Waiting Time= %f\n",tmp_esp*1.0/n_proc);
} 

void sjf_simulador(struct processos *loc, int n_proc){
    int tempo_exe, shortest, inicio, fim, durc, i=0, p[n_proc];
    struct processos *copia, *src_tmp, *tmp, *antes_shortest;
    printf("\n\t    Simulando o escalonamento visualmente\n\n ");
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
    printf("\t\t  ");
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
            durc = copia->dur;
            fim = tempo_exe;
            p[i]=tempo_exe;
            printf("| P%d ", copia->id);
            src_tmp = copia;
            copia = copia->prox;
            free(src_tmp);
        }
        else{ /*aloca o 1º processo se n~ao houver outro menor*/
            tmp = antes_shortest->prox;
            inicio = tempo_exe;
            tempo_exe += tmp->dur;
            durc = tmp->dur;
            p[i]=tempo_exe;
            fim = tempo_exe;
            printf("| P%d ", tmp->id);
            antes_shortest->prox = tmp->prox;
            free(tmp);
        }
        i++;
    }
    printf("|");
    printf("\n\t\t  0  ");
    for (i=0; i<n_proc; i++)
        printf("  %d ", p[i]);
}

void escalonador_prioridade(struct processos *loc, int n_proc){
    int tempo_exe, inicio, maior, fim, tmp_esp, durc;
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
            durc = copia->dur;
            fim = tempo_exe;
            printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", copia->id, durc, inicio, fim);
            tmp_esp += fim;
            src_tmp = copia->prox;
            free(copia);
            copia = src_tmp;
        }
        else {/* se 1º nao tem maior prioridade*/
            tmp = maior_prio->prox;
            inicio = tempo_exe;
            tempo_exe += tmp->dur;
            durc = tmp->dur;
            fim = tempo_exe;
            printf("\tProcesso: %d\t Duração: %d\tEspera: %d\tTermino: %d\n", tmp->id, durc, inicio, fim);
            maior_prio->prox = tmp->prox;
            free(tmp);
        }
    }
    printf("\n\t\tAverage Waiting Time= %f\n",tmp_esp*1.0/n_proc);
}

void escalonador_prioridade_simulador(struct processos *loc, int n_proc){
    int tempo_exe, inicio, maior, fim, durc, i=0, p[n_proc];
    struct processos *copia, * src_tmp, *tmp, *maior_prio;
    printf("\n\t    Simulando o escalonamento visualmente\n\n ");
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
    printf("\t\t  ");
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
            p[i]=tempo_exe;
            durc = copia->dur;
            fim = tempo_exe;
            printf("| P%d ", copia->id);
            src_tmp = copia->prox;
            free(copia);
            copia = src_tmp;
        }
        else {/* se 1º nao tem maior prioridade*/
            tmp = maior_prio->prox;
            inicio = tempo_exe;
            tempo_exe += tmp->dur;
            p[i]=tempo_exe;
            durc = tmp->dur;
            fim = tempo_exe;
            printf("| P%d ", tmp->id);
            maior_prio->prox = tmp->prox;
            free(tmp);
        }
        i++;
    }
    printf("|");
    printf("\n\t\t  0  ");
    for (i=0; i<n_proc; i++)
        printf("  %d ", p[i]);
}

void robbin_round(struct processos *loc, int quantum, int n_proc){
    int count, j, time, remain, flag=0;
    int tmp_esp=0, turnaround_time=0, at[10], bt[10], rt[10];
    remain=n_proc;
    struct processos *copia, *src_tmp, *tmp, *slot;
    printf("\n\t\tRound Robin (RR) - Quantum: %d\n", quantum);
    src_tmp = loc;
    for(count=0; count < n_proc; count++){
            bt[count] = src_tmp->dur;
            rt[count] = bt[count];
            src_tmp = src_tmp->prox;
    }
    for(time = 0, count = 0; remain !=0;){
        at[count]=time;
        if(rt[count] <= quantum && rt[count] > 0){
            time+=rt[count];
            rt[count]=0;
            flag=1;
        }
        else if(rt[count]>0){
            rt[count]-=quantum;
            time+=quantum;
        }
        if(rt[count]==0 && flag==1){
            remain--;
            printf("\tProcesso: %d\tEspera: %d\tTermino: %d\n", count+1, time-bt[count], time);
            tmp_esp+=time-bt[count];
            turnaround_time+=time;
            flag=0;
        }
        if(count == n_proc-1)
            count=0;
        else if(at[count]<=time)
            count++;
        else
            count=0;
    }
    printf("\n\t      Average Waiting Time= %f\n",tmp_esp*1.0/n_proc); 
    printf("\t   Avgerage Turnaround Time = %f",turnaround_time*1.0/n_proc);
    free(src_tmp);
}

int main(){
    int esc, es_proc, n_proc, i, id, dur, prio, posi=0;
    do{
    if (posi==0){
        es_proc=painel();
    }
    else
        esc = menu();
    struct processos *list_proc, *tmp_proc;
    if(es_proc == 1){
        n_proc = 4;
        list_proc = entra_processos(1, 12, 2);
        list_proc->prox = entra_processos(2, 2, 1); tmp_proc = list_proc->prox;
        tmp_proc->prox  = entra_processos(3,  8, 4); tmp_proc = tmp_proc->prox;
        tmp_proc->prox  = entra_processos(4,  10, 3);
        esc = menu();
    }
    else if(es_proc==2){
        top();
        printf("\n\n  OBS: Escoher de preferência para mais de 3 procesos.");
        printf("\n\n   Intoduza o numero de processos: ");
        scanf("%d", &n_proc);
        for(i=0; i<n_proc;i++){
            system("clear");
            top();
            printf("\n             Processo %d: ", i+1);
            printf("\n\nIntroduza o seu tempo de duração (Burst time) P[%d]: ", i+1);
            scanf("%d", &dur);
            printf("Introduza a sua prioridade de execução P[%d]: ", i+1);
            scanf("%d", &prio);
            if(i==0)
                list_proc = entra_processos(i+1, dur, prio);
            else if(i==1){
                list_proc->prox = entra_processos(i+1, dur, prio); tmp_proc = list_proc->prox;
            }
            else if(i==n_proc-1){
                tmp_proc->prox  = entra_processos(i+1,  dur, prio);
            }
            else{
                tmp_proc->prox  = entra_processos(i+1,  dur, prio); tmp_proc = tmp_proc->prox;
            }
        }
        posi=1;
        esc = menu();
    }

    if(esc == 1){
        system("clear");
        listar_processos(list_proc);
        fcfs(list_proc, n_proc);
        printf("\n\n\t< 1 > Para voltar\nEscolha: ");
        int i;
        scanf("%d", &i);
    }
    else if ( esc == 2){
        system("clear");
        listar_processos(list_proc);
        sjf(list_proc, n_proc);
        sjf_simulador(list_proc, n_proc);
        printf("\n\n\t< 1 > Para voltar\nEscolha: ");
        int i;
        scanf("%d", &i);
    }
    else if (esc == 3){
        system("clear");
        listar_processos(list_proc);
        escalonador_prioridade(list_proc, n_proc);
        escalonador_prioridade_simulador(list_proc, n_proc);
        printf("\n\n\t< 1 > Para voltar\nEscolha: ");
        int i;
        scanf("%d", &i);
    }
    else if( esc == 4){
        system("clear");
        int quantum;
        listar_processos(list_proc);
        printf("\nDigite o quantum de tempo desejado: ");
        scanf("%d", &quantum);
        robbin_round(list_proc, quantum, n_proc);
        printf("\n\n\t< 1 > Para voltar\nEscolha: ");
        int i;
        scanf("%d", &i);
    }
    else if( esc == 5){
        printf("\n\nTermino do programa.\n\n");
        exit(2);
    }
    }while(esc <= 6);
    return 0;
}