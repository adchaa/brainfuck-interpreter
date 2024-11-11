#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
  int address;
  struct stack* next;
}stack;

void push_stack(stack** s,int address){
  stack * n = malloc(sizeof(stack));
  n->address = address;
  n->next = *s;
  (*s) = n;
}

int pop_stuck(stack** s){
  if (*s == NULL){
    fprintf(stderr,"ERROR: poping from null pointer");
    exit(1);
  }
  stack * d = *s;
  (*s) = (*s)->next;
  int x = d->address;
  free(d);
  return x;
}

int read_head_stack(stack *s){
  return s->address;
}

typedef struct data_pointer {
  char value;
  struct data_pointer * left;
  struct data_pointer * right;
} data_pointer;

void increment(data_pointer* dp) {
  dp->value++;
}

void decrement(data_pointer* dp) {
  dp->value--;
}

void output(data_pointer* dp){
  fprintf(stdout,"%c",dp->value);
}

void input(data_pointer* dp){
  fgetc(stdin);
}

void go_right(data_pointer** dp){
  if ((*dp)->right){
    *dp = (*dp)->right;
  } else {
    data_pointer* r = malloc(sizeof(data_pointer));
    r->value = 0;
    r->left = (*dp);
    r->right = NULL;
    (*dp)->right = r;
    *dp = (*dp)->right;
  }
}

void go_left(data_pointer** dp){
  if ((*dp)->left){
    *dp = (*dp)->left;
  } else {
    data_pointer* l = malloc(sizeof(data_pointer));
    l->value = 0;
    l->left = NULL;
    l->right = (*dp);
    (*dp)->left = l;
    *dp = (*dp)->left;
  }
}

void print_data_pointer(data_pointer *dp){
  while(dp->left) dp = dp->left;
  size_t i = 0 ;
  do {
    fprintf(stdout,"at %lu : %d\n",i++,dp->value);
    dp = dp->right;
  } while(dp);
}

int fgetchar(FILE * f,size_t *i){
  (*i)++;
  return fgetc(f);
}


int main(int argc ,char ** argv){
  if (argc < 2){
    fprintf(stderr, "ERROR: provide the brainfuck file\n");
    return 1;
  }
  FILE * f = fopen(argv[1],"r");
  if (f == NULL){
    fprintf(stderr, "ERROR: cannot open the file\n");
    return 1;
  }
  int cmd;
  data_pointer* dp = malloc(sizeof(data_pointer));
  dp->value = 0;
  dp->left = NULL;
  dp->right = NULL;
  stack * s = NULL;
  size_t current_address = 0;
  do {
    cmd = fgetchar(f,&current_address);
    switch (cmd) {
      case '>' : go_right(&dp); break;
      case '<' : go_left(&dp); break;
      case '+' : increment(dp); break;
      case '-' : decrement(dp); break;
      case '.' : output(dp); break;
      case ',' : input(dp); break;
      case '[' : {
        if(dp->value == 0){
          do { cmd = fgetchar(f,&current_address); } while(cmd !=EOF && cmd != ']');
        }else {
          push_stack(&s,current_address);
        }
        break;
      }
      case ']': {
        size_t address;
        if(dp->value == 0){
          address = pop_stuck(&s);
          continue;
        }else {
          address = read_head_stack(s);
          fseek(f,address,SEEK_SET);
          current_address = address;
        }
        break;
      }
    }
  } while(cmd != EOF);
  //print_data_pointer(dp);
  return 0;
}
