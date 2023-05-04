#include <stdio.h>
#include <stdlib.h>

//Representação do processo PCB
typedef struct process
{
    int id;
    int prioridade;
    int tempo_restante;
} Process;


//Representação do nó
typedef struct node
{
    Process *proc;
    struct node *next;
} Node;


//Representação da fila circular
typedef struct queue
{
    Node *first;
    Node *last;
    int quantum;
    int id;
} Queue;


//Função para adicionar um processo na fila de acordo com sua prioridade
void enqueue(Queue *q, Process *p){
    Node *new_node = malloc(sizeof(Node));
    new_node->proc = malloc(sizeof(Process));
    *new_node->proc = *p;
    if(q->last == NULL){
        new_node->next = new_node;
        q->first = new_node;
    }
    else{
        new_node->next = q->first;
        q->last->next = new_node;
    }
    q->last = new_node;
}

void dequeue(Queue *q){
    if(q->first == q->last){
        free(q->first);
        q->first = NULL;
        q->last = NULL;
    }
    else{
        Node *temp = q->first;
        q->first = q->first->next;
        q->last->next = q->first;
        free(temp);
    }
}

Process *get_next_process(Queue *queues){
    if(queues->first != NULL){
        return queues->first->proc;
    }
    return NULL;
}


//Função principal
int main() {
    int num_queues = 4;
    Queue *queues[num_queues];

    for(int i = 0; i<num_queues; i++){
        queues[i] = malloc(sizeof(Queue));
        queues[i]->first = NULL;
        queues[i]->last = NULL;
        queues[i]->quantum = i+1;
        queues[i]->id = i+1;
    }

    // Adiciona alguns processos de teste
    Process p1 = {1, 0, 10};
    Process p2 = {2, 1, 5};
    Process p3 = {3, 2, 7};
    Process p4 = {4, 3, 12};

    for(int i=0; i<4; i++){
        enqueue(queues[i], &p1);
        enqueue(queues[i], &p2);
        enqueue(queues[i], &p3);
        enqueue(queues[i], &p4);
    }

    // Simula o escalonamento
    int tempo_total = 0;
    for(int y=0; y<num_queues; y++){
        while(1) {
            Process *p = get_next_process(queues[y]);
            if(p == NULL){
                break;
            }
            if(p->tempo_restante <= queues[y]->quantum){
                tempo_total += p->tempo_restante;
                printf("Processo %d da fila %d executou por %d segundos\n", p->id, queues[y]->id, p->tempo_restante);
                dequeue(queues[y]);
            }
            else{
                tempo_total += queues[y]->quantum;
                printf("Processo %d da fila %d executou por %d segundos\n", p->id, queues[y]->id, queues[y]->quantum);
                p->tempo_restante -= queues[y]->quantum;
                enqueue(queues[y], p);
                dequeue(queues[y]);
            }
        }
        printf("Tempo total de executando: %d segundos\n", tempo_total);
        tempo_total=0;
    }

    return 0;
}
