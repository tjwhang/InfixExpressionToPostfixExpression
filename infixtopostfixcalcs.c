//전처리문
#include <stdio.h>
#include <stdlib.h>//exit()
#include <string.h>

#define TRUE 1
#define FALSE 0

//연결 리스트 구조체
typedef struct _node
{
	int data;
	struct _node* next;
} Node;

//스택 구조체
typedef struct _Stack
{
	Node* head;
} Stack;

//전방 선언
void StackInit(Stack* pstack);
int SIsEmpty(Stack* pstack);

void SPush(Stack* pstack, int data);
int SPop(Stack* pstack);
int SPeek(Stack* pstack);

void PrintStack(Stack* pstack,char* exp,char* postExp);
void PrintEval(Stack* pstack,char* exp);

int GetPrioOp(char op);
int CompareOP(char op1, char op2);
void InfixToPostfix(char exp[],int sel);

int CalPostExp(char exp[],int sel);

int main()
{
    printf("===Infix Expression to Postfix Expression Converter and Calculator===\n\n");
    while(1)
    {
		printf("Infix Expression: ");
        char exp[100];
        scanf("%s",exp);

        int sel;
        printf("Toggle Conversion and Calculation Progress (0,1):");
        scanf("%d",&sel);
        InfixToPostfix(exp,sel);
        printf("Converted Post Expression:%s\n",exp);

        printf("Calculated Result: %d\n====================================\n\n",CalPostExp(exp,sel));
    }
}

//Stack 초기화
void StackInit(Stack* pstack)
{
    pstack->head = NULL;
}

//비어있는지 확인해주는 함수
int SIsEmpty(Stack* pstack)
{
    if (pstack->head==NULL)
        return TRUE;
    else
        return FALSE;
}

//Push
void SPush(Stack* pstack, int data)
{
	//newNode에 메모리 동적 할당
    Node * newNode = (Node*)malloc(sizeof(Node));

    newNode->data=data;
    newNode->next=pstack->head;

    pstack->head=newNode;
}

//pop
int SPop(Stack* pstack)
{
    int rdata;//head 값 저장하기 위한 변수
    Node* rnode;//head의 포인터

	//비어있는지 체크
    if(SIsEmpty(pstack))
	{
        printf("Error: Stack is empty!");
        exit(-1);
    }

    rdata=pstack->head->data;
    rnode=pstack->head;

    pstack->head=pstack->head->next;
    free(rnode);

    return rdata;
}

//head의 data를 리턴만 해주는 함수
int SPeek(Stack* pstack)
{
    if(SIsEmpty(pstack))
	{
        printf("Error: Stack is empty!");
        exit(-1);
    }

    return pstack->head->data;
}

//과정을 그림으로 표현
void PrintStack(Stack* pstack,char* exp,char* postExp)
{
    getchar();
    if(SIsEmpty(pstack))
    {
        printf("    |     |   \n");
        printf("    -------   \n");

        return;
    }

    Stack tmp;
    StackInit(&tmp);

    printf("Infix Expression: %s\n\n",exp);
    while(!SIsEmpty(pstack))
    {
        int top=SPop(pstack);

		//매개변수로 들어온 char 타입이 10진수 숫자로 변경 가능하면 0이 아닌 숫자, 변경 불가능하면 0 반환하는 함수
        if(isdigit(top))
            printf("    |  %d  |   \n",top);
        else
            printf("    |  %c  |   \n",top);
        SPush(&tmp,top);
    }
    printf("    -------   \n");

    while(!SIsEmpty(&tmp))
    {
        int top=SPop(&tmp);


        SPush(pstack,top);
    }

    printf("Postfix Expression: %s\n",postExp);
}

//과정을 그림으로 표현
void PrintEval(Stack* pstack,char* exp)
{
    getchar();
    if(SIsEmpty(pstack))
    {
        printf("    |     |   \n");
        printf("    -------   \n");

        return;
    }

    Stack tmp;
    StackInit(&tmp);

    printf("Infix Expression: %s\n\n",exp);
    while(!SIsEmpty(pstack))
    {
        int top=SPop(pstack);
        printf("    |  %d  |   \n",top);
        SPush(&tmp,top);
    }
    printf("    -------   \n");

    while(!SIsEmpty(&tmp))
    {
        int top=SPop(&tmp);


        SPush(pstack,top);
    }
}

//연산자 우선순위를 가져오는 함수
int GetPrioOp(char op)//Operator Precedence
{
    switch (op)
    {
    case '*':
    case '/':
        return 3;
    case '+':
    case '-':
        return 2;
    case '(':
        return 1;

    return -1;
    }
}

//연산자 우선순위 비교 함수
int CompareOP(char op1, char op2)
{
    int prio1=GetPrioOp(op1);
    int prio2=GetPrioOp(op2);

    if(prio1>prio2)
        return 1;
    else if (prio1<prio2)
        return -1;
    else
        return 0;
}

//
void InfixToPostfix(char exp[],int sel)
{
    Stack stack;
    int explen=strlen(exp);
    char* postExp=(char*)malloc(explen+1);//'\0' 고려해서 +1
    char* printExp=(char*)malloc(explen+1);

	char a[10];
	int* pa = a;
	pa[4];
    int idx=0;
    char tok,popOp;

    memset(postExp,0,sizeof(char) * explen+1);
    memset(postExp,0,sizeof(char) * explen+1);
    StackInit(&stack);

    if(sel==1)
        printf("\nPress Enter to view next progress\nPrinting Conversion Progress\n");

    for(int i = 0 ; i < explen ; i++)
    {
        tok=exp[i];

        int pidx=0;
        int j;
        for(j=i+1;j<explen;j++)
            printExp[pidx++]=exp[j];

        printExp[pidx]='\0';
        if(isdigit(tok))
		{
            postExp[idx++]=tok;
        }
        else
		{
			//')'연산자가 나오면 '('연산자가 나올때 까지 pop후 저장
            switch(tok)
            {
            case '(':
                SPush(&stack,tok);
                break;
            case ')':
                while(1)
                {
                    popOp=SPop(&stack);

                    if(popOp=='(')
                        break;

                    postExp[idx++]=popOp;
                }
                break;
            case '+': case '-':
            case '*': case '/':
				//비어있는지 확인하고 연산자 우선순위 비교 후 pop해서 배열안에 대입
                while(!SIsEmpty(&stack) && CompareOP(SPeek(&stack),tok)>=0)
                    postExp[idx++]=SPop(&stack);
                SPush(&stack,tok);
                break;

            }
        }
		//과정 출력 여부
        if(sel==1)
		{
            PrintStack(&stack,printExp,postExp);
            printf("\n");
        }
    }

	//잉여 문자 추가
    while(!SIsEmpty(&stack))
	{
        int top=SPop(&stack);
        postExp[idx++]=top;
        if(sel==1)
            PrintStack(&stack,printExp,postExp);
    }

    strcpy(exp,postExp);//postExp -> exp 문자열 복사
    free(postExp);//해제
}

//
int CalPostExp(char exp[],int sel)
{
    Stack stack;
    int explen=strlen(exp);
    char tok,op1,op2;

    char* printExp=(char*)malloc(explen+1);
    StackInit(&stack);

    if(sel==1)
        printf("\nPrinting Calculation Progress\n");
    for (int i=0;i<explen;i++)
    {
        tok=exp[i];

        int pidx=0;
        int j;
        for(j=i+1;j<explen;j++)
            printExp[pidx++]=exp[j];

        printExp[pidx]='\0';
        if(isdigit(tok))//문자열인지 확인
            SPush(&stack,tok-'0');//값 push
        else
		{
			//op2를 먼저 지정하는 이유는 stack이 후입선출이라는 특성을 갖고 있기 때문에
            op2=SPop(&stack);
            op1=SPop(&stack);

            switch(tok)
            {
            case '+':
                SPush(&stack,op1+op2);
                break;

            case '-':
                SPush(&stack,op1-op2);
                break;

            case '*':
                SPush(&stack,op1*op2);
                break;

            case '/':
                SPush(&stack,op1/op2);
                break;
            }
        }
        if(sel==1)
            PrintEval(&stack,printExp);
    }
    return SPop(&stack);
}
