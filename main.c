#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ALLOCATION_ERROR 1
#define SYNTAX_ERROR 2
#define ALLOCATION_SIZE 50

// ////////////////////////////////////////////////////////////////////////////
// TYPEDEFS
// ////////////////////////////////////////////////////////////////////////////

typedef enum operator {
    RIGHT = 62,
    LEFT = 60,
    PLUS = 43,
    MINUS = 45,
    PRINT = 46,
    READ = 44,
    BEGIN_WHILE = 91,
    END_WHILE = 93
} Operator;

// ////////////////////////////////////////////////////////////////////////////
// ALLOCATION
// ////////////////////////////////////////////////////////////////////////////

void checkAllocation(void* pointer) {
    if (pointer == NULL) {
        printf("[ERROR] Speicher konnte nicht zugewiesen werden.\n");
        exit(ALLOCATION_ERROR);
    }
}

// ////////////////////////////////////////////////////////////////////////////
// READ IN FILE
// ////////////////////////////////////////////////////////////////////////////

/*
 * Prüft, ob der übergebene char ein valider Operator ist.
 */
int isValidOperator(char toValidate) {
    return toValidate == RIGHT || toValidate == LEFT || toValidate == PLUS || toValidate == MINUS ||
            toValidate == PRINT || toValidate == READ || toValidate == BEGIN_WHILE || toValidate == END_WHILE;
}

/*
 * Liest eine Datei anhand des übergebenen Dateinamens ein und gibt
 * den enthaltenen Code ohne Nicht-Brainfuck-Zeichen zurück.
 * @param fileName der Dateiname
 */
char* readFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    int i = 0;
    char c;
    char* code = malloc(ALLOCATION_SIZE);
    checkAllocation(code);

    while ((c = fgetc(file)) != EOF) {
        // wenn i die Grenze des allokierten Speichers erreicht hat,
        // muss mehr Speicher allokiert werden
        if (i % ALLOCATION_SIZE == ALLOCATION_SIZE - 1) {
            // z.B.                 49 + 1 +              50
            //                 =   100
            code = realloc(code, i + 1 + ALLOCATION_SIZE);
            checkAllocation(code);
        }
        if (isValidOperator(c)) {
            code[i++] = c;
        }
    }
    code[i] = '\0';
    return code;
}

// ////////////////////////////////////////////////////////////////////////////
// AUSFÜHREN
// ////////////////////////////////////////////////////////////////////////////

/*
 * Sucht im Code nach der nächsten geschlossenen Klammer ab start.
 * @param code der Code, in welchem nach der Klammer gesucht wird.
 * @param start der inklusive Index, ab dem nach der Klammer gesucht wird.
 * @returns den Index der Klammer oder -1, falls keien Klammer gefunden werden konnte.
 */
int findEndWhile(char* code, int start) {
    while (code[start] != '\0') {
        if (code[start] == END_WHILE) {
            return start;
        }
        start++;
    }
    return -1;
}

/*
 * Führt ein while aus.
 * @param code der Code, welcher im while ausgeführt wird.
 * @param cells die globalen Zellen
 * @param pointer der globale Pointer
 * @param i globaler Zähler
 */
void executeWhile(char* code, int* cells, int* pointer, int* i);

/*
 * Führt ein Snippet Code aus.
 * @param code der Code
 * @param cells die globalen Zellen
 * @param pointer der globale Pointer
 * @param start Inklusiver Index, ab welchem der Code ausgeführt wird.
 * @param end Exklusiver Index, bis zu welchem der Code ausgefürt wird.
 */
void executeCodeSnippet(char* code, int* cells, int* pointer, int start, int end) {
    for (int i = start; i < end; i++) {
        if (code[i] == RIGHT) {
            if (*pointer % ALLOCATION_SIZE == ALLOCATION_SIZE - 1) {
                cells = realloc(cells, *pointer + 1 + ALLOCATION_SIZE);
                checkAllocation(code);
            }
            (*pointer)++;
        } else if (code[i] == LEFT) {
            (*pointer)--;
        } else if (code[i] == PLUS) {
            cells[*pointer]++;
        } else if (code[i] == MINUS) {
            cells[*pointer]--;
        } else if (code[i] == PRINT) {
            printf("%c", (char) cells[*pointer]);
        } else if (code[i] == READ) {
            scanf(" %c", &cells[*pointer]);
        } else if (code[i] == BEGIN_WHILE) {
            executeWhile(code, cells, pointer, &i);
        } else if (code[i] == END_WHILE) {
            printf("\n[WARN] Encountered closing bracket. This should not happen.\n");
        } else {
            printf("\n[WARN] Encountered unrecognized operator: %c. Will skip it.\n", code[i]);
        }
    }
}

void executeWhile(char* code, int* cells, int* pointer, int* i) {
    // nächste schließende Klammer
    int endWhile = findEndWhile(code, *i);
    if (!endWhile) {
        printf("\n[SYNTAX ERROR] Failed to find closing bracket for bracking at position: %d.\n", *i);
        exit(SYNTAX_ERROR);
    }

    while (cells[*pointer] != 0) {
        executeCodeSnippet(code, cells, pointer, *i + 1, endWhile);
    }
    // i um die Differenz der öffnenden und schließenden Klammer erhöhen,
    *i += endWhile - *i;
}

/*
 * Führt Brainfuck Code aus.
 * @param code der auszuführende Code
 */
void executeCode(char* code) {
    int pointer = 0;
    int* cells = calloc(ALLOCATION_SIZE, sizeof(int));
    checkAllocation(cells);
    executeCodeSnippet(code, cells, &pointer, 0, strlen(code));
    free(cells);
}

// ////////////////////////////////////////////////////////////////////////////
// MAIN
// ////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[]) {
    printf("[INFO] Reading file...\n");
    clock_t readingTime = clock();
    char* code = readFile(argv[1]);
    readingTime = clock() - readingTime;
    printf("[INFO] File has been read.\n");
    printf("[INFO] Executing read code...\n");
    clock_t executionTime = clock();
    executeCode(code);
    executionTime = clock() - executionTime;

    float time = ((double)executionTime)/CLOCKS_PER_SEC;

    printf("\n[INFO] Code has been executed. It took %fs to execute.", time);
    return 0;
}
