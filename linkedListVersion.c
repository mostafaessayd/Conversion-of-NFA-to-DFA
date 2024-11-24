#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct transition
{
    char symbol;
    int rightState;
    struct transition *next;
} transition;

typedef struct mainNodes
{
    int leftState;
    int initialState;
    int finaleState;
    struct transition *listOfRights;
    struct mainNodes *next;
} mainNodes;

mainNodes *automate = NULL;
int nbrOfState;
int nbrOfInitialState;
int nbrOfFinaleState;
int nbrOfTransition;

mainNodes *generateAutomate()
{
    mainNodes *head = NULL;
    mainNodes *last = NULL;
    for (int i = 0; i < nbrOfState; i++)
    {
        mainNodes *newNode = malloc(sizeof(mainNodes));
        newNode->leftState = i;
        newNode->listOfRights = NULL;
        newNode->next = NULL;
        newNode->initialState = 0;
        newNode->finaleState = 0;
        if (last == NULL)
        {
            head = newNode;
            last = head;
        }
        else
        {
            last->next = newNode;
            last = last->next;
        }
    }

    return head;
}

void setStatusOfState(mainNodes *head, int state, int status)
{
    while (head->leftState != state)
        head = head->next;
    if (status == 0)
    {
        head->initialState = 1;
    }
    else
    {
        head->finaleState = 1;
    }
}

void addTransition(int left, char symbol, int right)
{
    mainNodes *head = automate;
    while (head->leftState != left)
        head = head->next;
    transition *newTransition = malloc(sizeof(transition));
    newTransition->rightState = right;
    newTransition->symbol = symbol;
    newTransition->next = head->listOfRights;
    head->listOfRights = newTransition;
}

void deleteEpsolin()
{
    mainNodes *head = automate;
    while (head)
    {
        transition *ptr = head->listOfRights;
        while (ptr)
        {
            if (ptr->symbol == '$')
            {
                int targetLeft = ptr->rightState;
                mainNodes *h = automate;
                while (h->leftState != targetLeft)
                    h = h->next;
                transition *allRightsOfTargetLeft = h->listOfRights;
                while (allRightsOfTargetLeft)
                {
                    addTransition(head->leftState, allRightsOfTargetLeft->symbol, allRightsOfTargetLeft->rightState);
                    allRightsOfTargetLeft = allRightsOfTargetLeft->next;
                }
            }
            ptr = ptr->next;
        }
        head = head->next;
    }
}

///////////////////////////////////////////////// convert ///////////////////////////////////////
typedef struct normalList
{
    int state;
    struct normalList *next;
} normalList;

typedef struct transitionList
{
    char symbol;
    struct normalList *ListOfNodes;
    struct transitionList *next;
} transitionList;

typedef struct ListOfAlgorithmNodes
{
    struct normalList *ListOfNodes;
    struct transitionList *ListOfTransition;
    struct ListOfAlgorithmNodes *next;
} ListOfAlgorithmNodes;

ListOfAlgorithmNodes *headOfAlgorithm = NULL;
ListOfAlgorithmNodes *lastOfListOfNodesInAlgorithm = NULL;

void addListInAlgorithm(normalList *List)
{
    ListOfAlgorithmNodes *newNode = malloc(sizeof(ListOfAlgorithmNodes));
    newNode->ListOfNodes = List;
    newNode->next = NULL;

    char validLetters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
                           'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '$'};
    newNode->ListOfTransition = NULL;
    for (int i = 26; i >= 0; i--)
    {
        transitionList *newTransition = malloc(sizeof(transitionList));
        newTransition->ListOfNodes = NULL;
        newTransition->symbol = validLetters[i];
        newTransition->next = newNode->ListOfTransition;
        newNode->ListOfTransition = newTransition;
    }

    if (lastOfListOfNodesInAlgorithm == NULL)
    {
        headOfAlgorithm = newNode;
        lastOfListOfNodesInAlgorithm = newNode;
    }
    else
    {
        lastOfListOfNodesInAlgorithm->next = newNode;
        lastOfListOfNodesInAlgorithm = lastOfListOfNodesInAlgorithm->next;
    }
}

void init()
{
    mainNodes *head = automate;
    normalList *listOfAllInitialState = NULL;
    normalList *last = NULL;
    while (head)
    {
        if (head->initialState == 1)
        {
            normalList *newNode = malloc(sizeof(normalList));
            newNode->next = NULL;
            newNode->state = head->leftState;
            if (last == NULL)
            {
                listOfAllInitialState = newNode;
                last = newNode;
            }
            else
            {
                last->next = newNode;
                last = last->next;
            }
        }
        head = head->next;
    }

    addListInAlgorithm(listOfAllInitialState);
}

int sameList(normalList *L1, normalList *L2)
{
    while (L1 && L2)
    {
        if (L1->state != L2->state)
        {
            return 0;
        }
        L1 = L1->next;
        L2 = L2->next;
    }

    return L1 == NULL && L2 == NULL;
}

int isNewState(normalList *L)
{
    ListOfAlgorithmNodes *head = headOfAlgorithm;
    while (head)
    {
        if (sameList(head->ListOfNodes, L) == 1)
        {
            return 0;
        }
        head = head->next;
    }
    return 1;
}

void insertNewStateInSpeceficeSymbolOfSpeceficeState(normalList *targetL, char targetSymbol, int newState)
{
    ListOfAlgorithmNodes *head = headOfAlgorithm;
    while (sameList(head->ListOfNodes, targetL) == 0)
        head = head->next;
    transitionList *headOfTransition = head->ListOfTransition;
    while (headOfTransition->symbol != targetSymbol)
        headOfTransition = headOfTransition->next;
    normalList *ptr = headOfTransition->ListOfNodes;
    normalList *last = NULL;
    while (ptr)
    {
        if (ptr->state == newState)
        {
            return;
        }
        if (ptr->state > newState)
        {
            break;
        }
        last = ptr;
        ptr = ptr->next;
    }
    normalList *newNode = malloc(sizeof(normalList));
    newNode->state = newState;
    if (last == NULL)
    {
        newNode->next = headOfTransition->ListOfNodes;
        headOfTransition->ListOfNodes = newNode;
    }
    else
    {
        if (ptr == NULL)
        {
            last->next = newNode;
            newNode->next = NULL;
        }
        else
        {
            newNode->next = ptr;
            last->next = newNode;
        }
    }
}

normalList *getListOfRights(int left, char symbol)
{
    mainNodes *head = automate;
    while (head->leftState != left)
        head = head->next;
    transition *ptr = head->listOfRights;
    normalList *headOfList = NULL;
    while (ptr)
    {
        if (ptr->symbol == symbol)
        {
            normalList *newNode = malloc(sizeof(normalList));
            newNode->state = ptr->rightState;
            newNode->next = headOfList;
            headOfList = newNode;
        }
        ptr = ptr->next;
    }

    return headOfList;
}

void ALGORITHM()
{
    ListOfAlgorithmNodes *head = headOfAlgorithm;
    while (head)
    {
        normalList *headOfNormalList = head->ListOfNodes;
        while (headOfNormalList)
        {
            transitionList *headOfTransitionList = head->ListOfTransition;
            while (headOfTransitionList)
            {
                normalList *ListOfRights = getListOfRights(headOfNormalList->state, headOfTransitionList->symbol);
                while (ListOfRights)
                {
                    insertNewStateInSpeceficeSymbolOfSpeceficeState(head->ListOfNodes, headOfTransitionList->symbol, ListOfRights->state);
                    ListOfRights = ListOfRights->next;
                }
                headOfTransitionList = headOfTransitionList->next;
            }
            headOfNormalList = headOfNormalList->next;
        }
        transitionList *headOfTransitionList = head->ListOfTransition;
        while (headOfTransitionList)
        {
            if (headOfTransitionList->ListOfNodes)
            {
                if (isNewState(headOfTransitionList->ListOfNodes) == 1)
                {
                    addListInAlgorithm(headOfTransitionList->ListOfNodes);
                }
            }
            headOfTransitionList = headOfTransitionList->next;
        }
        head = head->next;
    }
}

int sizeOfGivenNumber(int x)
{
    if (x == 0)
        return 1;
    int length = 0;
    while (x > 0)
    {
        length++;
        x /= 10;
    }
    return length;
}

void showAlgorithm()
{
    ListOfAlgorithmNodes *head = headOfAlgorithm;
    int maxWidthInFirstColumn = 0;
    int lengthOf[100][256];
    int maxWidth[100];
    int k = 0;
    while (head)
    {
        normalList *L = head->ListOfNodes;
        int length = 0;
        int comp = 1;
        while (L)
        {
            comp++;
            length += sizeOfGivenNumber(L->state);
            L = L->next;
        }

        transitionList *ListOfTransition = head->ListOfTransition;
        while (ListOfTransition)
        {
            normalList *list = ListOfTransition->ListOfNodes;
            int lght = 0;
            int cmp = 1;
            while (list)
            {
                cmp++;
                lght += sizeOfGivenNumber(list->state);
                list = list->next;
            }
            lengthOf[k][ListOfTransition->symbol - 'a' + 1] = lght + cmp;
            if (lght == 0)
            {
                lengthOf[k][ListOfTransition->symbol - 'a' + 1] = 0;
            }
            if (lght > 0)
                if (lght + cmp > maxWidth[ListOfTransition->symbol - 'a'])
                {
                    maxWidth[ListOfTransition->symbol - 'a'] = lght + cmp;
                }
            ListOfTransition = ListOfTransition->next;
        }

        lengthOf[k++][0] = length + comp;
        if (length + comp > maxWidthInFirstColumn)
        {
            maxWidthInFirstColumn = length + comp;
        }
        head = head->next;
    }

    for (int i = 0; i < maxWidthInFirstColumn; i++)
    {
        printf(" ");
    }
    printf(" ");
    for (int i = 0; i < 26; i++)
    {
        if (maxWidth[i] == 0)
            continue;
        printf("+");
        for (int j = 0; j < maxWidth[i]; j++)
        {
            printf("-");
        }
    }
    printf("+\n");

    for (int i = 0; i < maxWidthInFirstColumn; i++)
    {
        printf(" ");
    }
    printf(" ");
    for (int i = 0; i < 26; i++)
    {
        if (maxWidth[i] == 0)
            continue;
        printf("|");
        for (int j = 0; j < maxWidth[i] / 2; j++)
        {
            printf(" ");
        }
        printf("%c", i + 'a');
        for (int j = 0; j < maxWidth[i] / 2; j++)
        {
            printf(" ");
        }
    }
    printf("|\n");

    printf("+");
    for (int i = 0; i < maxWidthInFirstColumn; i++)
    {
        printf("-");
    }
    for (int i = 0; i < 26; i++)
    {
        if (maxWidth[i] == 0)
            continue;
        printf("+");
        for (int j = 0; j < maxWidth[i]; j++)
        {
            printf("-");
        }
    }
    printf("+\n");

    head = headOfAlgorithm;
    k = 0;
    while (head)
    {
        normalList *L = head->ListOfNodes;
        printf("|{%d", L->state);
        L = L->next;
        while (L)
        {
            printf(",%d", L->state);
            L = L->next;
        }
        printf("}");
        for (int i = 0; i < maxWidthInFirstColumn - lengthOf[k][0]; i++)
        {
            printf(" ", lengthOf[k][0]);
        }
        printf("|");
        transitionList *ListOfTransition = head->ListOfTransition;
        while (ListOfTransition)
        {
            if (maxWidth[ListOfTransition->symbol - 'a'] != 0)
            {
                if (ListOfTransition->ListOfNodes)
                {
                    normalList *list = ListOfTransition->ListOfNodes;
                    printf("{%d", list->state);
                    list = list->next;
                    while (list)
                    {
                        printf(",%d", list->state);
                        list = list->next;
                    }
                    printf("}");
                    for (int i = 0; i < maxWidth[ListOfTransition->symbol - 'a'] - lengthOf[k][ListOfTransition->symbol - 'a' + 1]; i++)
                    {
                        printf(" ");
                    }
                    printf("|");
                }
                else if (ListOfTransition->symbol != '$')
                {
                    for (int i = 0; i < maxWidth[ListOfTransition->symbol - 'a']; i++)
                    {
                        printf(" ");
                    }
                    printf("|");
                }
            }
            ListOfTransition = ListOfTransition->next;
        }
        printf("\n+");
        k++;
        for (int i = 0; i < maxWidthInFirstColumn; i++)
        {
            printf("-");
        }
        for (int i = 0; i < 26; i++)
        {
            if (maxWidth[i] == 0)
                continue;
            printf("+");
            for (int j = 0; j < maxWidth[i]; j++)
            {
                printf("-");
            }
        }
        printf("+\n");
        head = head->next;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////

void enterAutomate()
{
    // printf("- number of state : ");
    scanf("%d", &nbrOfState);
    automate = generateAutomate();

    // printf("- number of initial state : ");
    scanf("%d", &nbrOfInitialState);
    // printf("- enter initial state : \n");
    while (nbrOfInitialState--)
    {
        int x;
        scanf("%d", &x);
        setStatusOfState(automate, x, 0);
    }
    // printf("- number of finale state : ");
    scanf("%d", &nbrOfFinaleState);
    // printf("- enter finale state : \n");
    while (nbrOfFinaleState--)
    {
        int x;
        scanf("%d", &x);
        setStatusOfState(automate, x, 1);
    }

    // printf("- number of transition : ");
    scanf("%d", &nbrOfTransition);
    while (nbrOfTransition--)
    {
        int left, right;
        char symbol;
        scanf("%d %c %d", &left, &symbol, &right);
        addTransition(left, symbol, right);
    }
}

int main()
{
    enterAutomate();
    init();
    ALGORITHM();
    showAlgorithm();
    return 0;
}
